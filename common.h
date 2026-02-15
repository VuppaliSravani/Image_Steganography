#ifndef COMMON_H
#define COMMON_H

// Magic string used to identify encoded data in the image
#define MAGIC_STRING "#*"

// Status values returned by functions
typedef enum
{
    failure,    // Operation failed
    success     // Operation successful
} Status;

// Operation types supported by the application
typedef enum
{
    unsupported,    // Invalid or unsupported operation
    encode,         // Encoding operation
    decode          // Decoding operation
} Opr_type;

#endif
