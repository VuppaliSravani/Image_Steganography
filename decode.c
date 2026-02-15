/*
Name        : VUPPALI SRAVANI
Date        : 23-01-2026
Description : LSB-based image steganography decoding to extract secret data
              from a BMP image without image distortion.
*/

#include <stdio.h>
#include <string.h>
#include "decode.h"

/* Validate decoding arguments and open encoded image file */
Status validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Check whether input file is a BMP file
    char *res = strstr(argv[2], ".bmp");
    if (res == NULL)
    {
        printf("Error: Invalid BMP filename\n");
        return failure;
    }

    // Store encoded image filename
    decInfo->encoded_image_fname = argv[2];

    printf("Opening required files\n");

    // Handle output file name
    if (argv[3] == NULL)
    {
        // Default output filename
        strcpy(decInfo->output_fname, "default_data_file");
    }
    else
    {
        // Copy output filename and remove extension if present
        strcpy(decInfo->output_fname, argv[3]);
        char *dot = strrchr(decInfo->output_fname, '.');
        if (dot != NULL)
            *dot = '\0';
    }

    // Open encoded BMP image in binary mode
    decInfo->encoded_image_fptr = fopen(decInfo->encoded_image_fname, "rb");
    if (decInfo->encoded_image_fptr == NULL)
    {
        perror("fopen");
        return failure;
    }

    printf("Opened %s\n", decInfo->encoded_image_fname);
    printf("It's a bmp file\n");

    return success;
}

/* Perform decoding process */
Status do_decoding(char *argv[], DecodeInfo *decInfo)
{
    printf("\n## Decoding Procedure Started ##\n");

    // Validate arguments and open encoded image
    if (validate_decode_args(argv, decInfo) == failure)
        return failure;

    // Decode and verify magic string
    printf("Decoding Magic String Signature\n");
    if (decode_magic_string(decInfo) == failure)
    {
        printf("Magic string is not decoding successfully\n");
        printf("Decoding FAILED\n");
        fclose(decInfo->encoded_image_fptr);
        return failure;
    }
    printf("Done\n");

    // Decode secret file extension
    decode_secret_file_extn(decInfo);

    // Decode secret file size and data
    decode_secret_file_data(decInfo);

    printf("## Decoding Done Successfully ##\n");
    fclose(decInfo->encoded_image_fptr);
    return success;
}

/* Decode one byte from 8 LSBs */
char decode_1byte_to_lsb(char *buffer_8)
{
    char ch = 0;

    for (int i = 7; i >= 0; i--)
    {
        int bit = buffer_8[7 - i] & 1;
        ch |= (bit << i);
    }
    return ch;
}

/* Decode 4 bytes from 32 LSBs */
int decode_4byte_to_lsb(char *buffer_32)
{
    int value = 0;

    for (int i = 31; i >= 0; i--)
    {
        int bit = buffer_32[31 - i] & 1;
        value |= (bit << i);
    }
    return value;
}

/* Decode and validate magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    // Skip BMP header
    fseek(decInfo->encoded_image_fptr, 54, SEEK_SET);

    char ch[32];
    fread(ch, 32, 1, decInfo->encoded_image_fptr);

    int magic_string_len = decode_4byte_to_lsb(ch);

    // Safety check to avoid segmentation fault
    if (magic_string_len <= 0 || magic_string_len > 10)
        return failure;

    char magic_string[magic_string_len + 1];

    // Decode magic string characters
    for (int i = 0; i < magic_string_len; i++)
    {
        char c[8];
        fread(c, 8, 1, decInfo->encoded_image_fptr);
        magic_string[i] = decode_1byte_to_lsb(c);
    }
    magic_string[magic_string_len] = '\0';

    // Compare with original magic string
    if (strcmp(magic_string, MAGIC_STRING) == 0)
        return success;

    return failure;
}

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    printf("Decoding Output File Extension\n");

    char ch[32];
    fread(ch, 32, 1, decInfo->encoded_image_fptr);
    decInfo->secret_extn_size = decode_4byte_to_lsb(ch);

    // Decode extension characters
    for (int i = 0; i < decInfo->secret_extn_size; i++)
    {
        char c[8];
        fread(c, 8, 1, decInfo->encoded_image_fptr);
        decInfo->secret_extn[i] = decode_1byte_to_lsb(c);
    }
    decInfo->secret_extn[decInfo->secret_extn_size] = '\0';

    printf("Done\n");

    // Append extension to output filename
    strcat(decInfo->output_fname, decInfo->secret_extn);
    printf("Output File not mentioned. Creating %s as default\n",
           decInfo->output_fname);

    // Open output file in write mode
    decInfo->output_fptr = fopen(decInfo->output_fname, "w");
    if (decInfo->output_fptr == NULL)
        return failure;

    printf("Opened %s\n", decInfo->output_fname);
    printf("Done. Opened all required files\n");

    return success;
}

/* Decode secret file size and data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf("Decoding the File Size\n");

    char ch[32];
    fread(ch, 32, 1, decInfo->encoded_image_fptr);
    decInfo->secret_file_size = decode_4byte_to_lsb(ch);
    printf("Done\n");

    printf("Decoding File Data\n");

    // Decode secret data and write to output file
    for (int i = 0; i < decInfo->secret_file_size; i++)
    {
        char c[8];
        fread(c, 8, 1, decInfo->encoded_image_fptr);
        char data = decode_1byte_to_lsb(c);
        fputc(data, decInfo->output_fptr);
    }

    fclose(decInfo->output_fptr);
    return success;
}
