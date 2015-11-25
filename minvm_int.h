#ifndef _included_minvm_int_h
#define _included_minvm_int_h

//
// Internal functions thrown together to support common test code
//

typedef struct buffer_t {
    // Allocated buffer
    byte        *base_ptr;
    size_t      base_size;

    // Usable data
    byte        *data;
    size_t      data_size;
} buffer_t;

typedef struct file_t {
  FILE          *stream;
  size_t        size;
} file_t;


typedef int errno_t;
#define ERR_OK ((errno_t)0)

void        mvm_error (cchar *fmt, ...);
uint32_t    mvm_error_count ();
void        mvm_info (cchar *fmt, ...);
errno_t     mvm_file_open (file_t *file, cchar *filename, cchar *mode);
void        mvm_file_close (file_t *file);
void        mvm_get_error (char *message, size_t size, errno_t err);
int         mvm_ishex (cchar c);
int         mvm_isoneof (cchar c, cchar *str);
bool        mvm_read_buffer (cchar *filename, buffer_t *buffer);
bool        mvm_read_buffer_ram (cchar *filename, buffer_t *buffer);
bool        mvm_validate_buffer (const buffer_t *buffer);
bool        mvm_free_buffer (buffer_t *buffer);
uint32_t    mvm_count_bits (uint32_t n);
int         mvm_print_string (char *dst, uint32_t size, cchar *fmt, ...);
int         mvm_vprint_string (char *dst, uint32_t size, cchar *fmt, va_list ap);
int         mvm_check_bytes (byte *data, uint32_t offset, uint32_t count, byte check);

#endif // _included_minvm_int_h
