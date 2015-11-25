#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "minvm_defs.h"
#include "minvm_int.h"


#define BUFFER_PADDING  256
#define BUFFER_FILL     0xBB

static uint32_t s_errors = 0;

uint32_t mvm_error_count () {
    return s_errors;
}

void mvm_error (cchar *fmt, ...) {
    va_list ap;

    fprintf(stderr, "ERROR: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    fflush(stderr);

    s_errors++;
}

void mvm_info (cchar *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    fprintf(stdout, "\n");
    fflush(stdout);
}

int mvm_file_open (file_t *f, cchar *filename, cchar *mode) {
    struct stat st;

#if BUILD_WINDOWS
    { 
        errno_t e = 0;
        FILE *fp = NULL;

        e = fopen_s(&fp, filename, mode);
        f->stream = fp;
    }
#else
    f->stream = fopen(filename, mode);
#endif

    if (f->stream == NULL) {
        int error = errno;
        char message[MESSAGE_SZ];
        mvm_get_error(message, sizeof(message), error);
        mvm_error("minvm_file_open: couldn't open file: %s: %s", filename, message);
        return error;
    }

    if (0 != stat(filename, &st)) {
        fclose(f->stream);
        f->stream = NULL;
        mvm_error("mvm_file_open: couldn't stat: %s", filename);
        return 0;
    }

    f->size = st.st_size;

    return 0;
}

void mvm_file_close (file_t *file) {
  if (file->stream) {
    fclose(file->stream);
    file->stream = NULL;
    file->size = 0;
  }
}

void mvm_get_error (char *message, size_t size, errno_t err) {
#ifdef BUILD_WINDOWS
    strerror_s(message, size, err);
#else
    cchar *src = strerror(err);
    if (src) {
        strncpy(message, src, size);
    }
#endif
}

int mvm_ishex (cchar c) {
    return (c >= '0' && c <= '9')
        || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}

int mvm_isoneof (cchar c, cchar *str) {
    while(*str) {
        if (c == *str++) return 1;
    }
    return 0;
}

bool mvm_buffer_alloc (buffer_t* buffer, size_t size) {
    // Reserve buffer size and fixed padding
    size_t buffer_size = size + (2 * BUFFER_PADDING);
    byte* allocated = (byte*)malloc(buffer_size);
    if (!allocated) {
        mvm_error("couldn't allocate buffer %u bytes", buffer_size);
        return false;
    }

    // Set guard bytes
    memset(allocated, BUFFER_FILL, BUFFER_PADDING);
    memset(allocated + buffer_size - BUFFER_PADDING, BUFFER_FILL, BUFFER_PADDING);

    // Caller uses 'data', just keep the pointers around instead of calculating offsets
    buffer->data = allocated + BUFFER_PADDING;
    buffer->data_size = size;
    buffer->base_ptr = allocated;
    buffer->base_size  = buffer_size;
  
    return true;
}

bool mvm_read_buffer_internal (file_t* file, buffer_t *buffer, size_t size) {
    size_t read = fread(buffer->data, 1, size, file->stream);
    if (size != read) {
        mvm_error("mvm_read_buffer_internal: read failed %u != %u", read, size);
        return false;
    }

    return true;
}

bool mvm_read_buffer (cchar *filename, buffer_t *buffer) {
    file_t f = { 0, };

    if (ERR_OK != mvm_file_open(&f, filename, "rb")) {
        return false;
    }

    // Allocate a buffer to the size of the file
    if (!mvm_buffer_alloc(buffer, f.size)) {
        return false;
    }

    if (!mvm_read_buffer_internal(&f, buffer, f.size)) {
        mvm_file_close(&f);
        mvm_free_buffer(buffer);
        return false;
    }

    mvm_file_close(&f);

    return true;
}


bool mvm_read_buffer_ram (cchar *filename, buffer_t *buffer) {
    file_t f = { 0, };

    if (ERR_OK != mvm_file_open(&f, filename, "rb")) {
        return false;
    }

    // Check the size of the file
    if (f.size > RAM_SIZE) { 
      mvm_error("mvm_read_buffer_ram: file size exceeds RAM size RAM_SIZE");
      mvm_file_close(&f);
      return false;
    }

    // Allocate the buffer for the RAM
    if (!mvm_buffer_alloc(buffer, RAM_SIZE)) {
        return false;
    }

    // Clear contents to zero
    memset(buffer->data, 0, RAM_SIZE);

    // Read contents or fail
    if (!mvm_read_buffer_internal(&f, buffer, f.size)) {
        mvm_file_close(&f);
        mvm_free_buffer(buffer);
        return false;
    }

    mvm_file_close(&f);

    return true;
}

int mvm_check_bytes (byte *data, uint32_t offset, uint32_t count, byte check) {
    uint32_t end;

    for (end = offset + count; offset < end; ++offset) {
        if (data[offset] != check) {
            mvm_error("mvm_check_bytes: corrupt byte (0x%02x) at offset: %u", data[offset], offset);
            return 0;
        }
    }

    return 1;
}

bool mvm_validate_buffer (const buffer_t *buffer) {
    if (!mvm_check_bytes(buffer->base_ptr, 0, BUFFER_PADDING, BUFFER_FILL)) {
        return false;
    }

    if (buffer->base_size < BUFFER_PADDING) {
        mvm_error("mvm_validate_buffer: base_size (%u) < BUFFER_PADDING (%u)", buffer->base_size, BUFFER_PADDING);
        return false;
    }

    if (!mvm_check_bytes(buffer->base_ptr, buffer->base_size - BUFFER_PADDING, BUFFER_PADDING, BUFFER_FILL)) {
        return false;
    }

    return true;
}

bool mvm_free_buffer (buffer_t *buffer) {
    bool status = true;

    if (!buffer) {
        return false;
    }

    // Check the padding bytes in the buffer
    status = true;
    if (!mvm_check_bytes(buffer->base_ptr, 0, BUFFER_PADDING, BUFFER_FILL)) {
        status = false;
    }

    if (!mvm_check_bytes(buffer->base_ptr, BUFFER_PADDING + buffer->data_size, BUFFER_PADDING, BUFFER_FILL)) {
        status = false;
    }

    if (buffer->base_ptr) {
      free(buffer->base_ptr);
    }

    buffer->base_ptr = NULL;
    buffer->data = NULL;
    buffer->base_size = 0;
    buffer->data_size = 0;

    return status; 
}

uint32_t mvm_count_bits (uint32_t n) {
    uint32_t count = 0;

    while (n) {
        n = n & (n - 1);
        count++;
    }

    return count;
}

int mvm_vprint_string (char *dest, uint32_t size, cchar *fmt, va_list ap) {
    int n;
#ifdef BUILD_WINDOWS
    n = _vsnprintf_s(dest, size, _TRUNCATE, fmt, ap); 
#else
    n = vsnprintf(dest, size, fmt, ap);
#endif
    return n;
}

int mvm_print_string (char *dest, uint32_t size, cchar *fmt, ...) {
    int n;
    va_list ap;
    va_start(ap, fmt);
    n = mvm_vprint_string(dest, size, fmt, ap);
    va_end(ap);
    return n;
}

