#ifndef DECODE_H
#define DECODE_H

#include "common.h"

/* Structure to store decoding related information */
typedef struct DecodeInfo
{
    char *encoded_image_fname;   // Name of the encoded BMP image file
    FILE *encoded_image_fptr;    // File pointer for encoded image

    char output_fname[50];       // Name of the output secret file
    FILE *output_fptr;           // File pointer for output file

    char secret_extn[10];        // Secret file extension
    int secret_extn_size;        // Size of secret file extension
    int secret_file_size;        // Size of secret file data
} DecodeInfo;

/* Validate decoding arguments and open required files */
Status validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform complete decoding operation */
Status do_decoding(char *argv[], DecodeInfo *decInfo);

/* Decode and verify magic string from encoded image */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size and data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode 1 byte from 8 LSBs of image data */
char decode_1byte_to_lsb(char *buffer_8);

/* Decode 4 bytes (int) from 32 LSBs of image data */
int decode_4byte_to_lsb(char *buffer_32);

#endif
