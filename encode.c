/*
Name        : VUPPALI SRAVANI
Date        : 23-01-2026
Description : LSB-based image steganography to securely hide and extract secret data within a BMP image without visible distortion.
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"

/* Get image size from BMP header */
unsigned long int get_image_size_for_bmp(FILE *fptr_image)
{
    unsigned int width, height;

    // Move file pointer to width field in BMP header
    fseek(fptr_image, 18, SEEK_SET);

    // Read image width
    fread(&width, sizeof(int), 1, fptr_image);

    // Read image height
    fread(&height, sizeof(int), 1, fptr_image);

    // Return total image capacity in bytes (RGB image)
    return width * height * 3;
}

/* Open source image, secret file and output image */
Status open_files(EncodeInfo *encInfo)
{
    printf("Opening required files\n");

    // Open source BMP image
    encInfo->src_image_fptr = fopen(encInfo->src_image_fname, "r");
    printf("Opened %s\n", encInfo->src_image_fname);

    if (encInfo->src_image_fptr == NULL)
    {
        perror("fopen");
        return failure;
    }

    // Open secret file
    encInfo->secret_fptr = fopen(encInfo->secret_fname, "r");
    printf("Opened %s\n", encInfo->secret_fname);

    if (encInfo->secret_fptr == NULL)
    {
        perror("fopen");
        return failure;
    }

    // Open output image file
    encInfo->output_image_fptr = fopen(encInfo->output_image_fname, "w");
    printf("Opened %s\n", encInfo->output_image_fname);

    if (encInfo->output_image_fptr == NULL)
    {
        perror("fopen");
        return failure;
    }

    printf("Done\n");
    return success;
}

/* Identify encode or decode operation */
Opr_type check_operation(char *option)
{
    if (!strcmp(option, "-e"))
        return encode;
    else if (!strcmp(option, "-d"))
        return decode;
    else{
        printf("Invalid option.\nPlease pass either -e for encoding or -d for decoding.\n");
        return unsupported;
    }
}

/* Perform encoding process */
Status do_encoding(char *argv[], EncodeInfo *encInfo)
{
    // Validate command-line arguments
    if (validate_encode_args(argv, encInfo) == failure)
        return failure;

    printf("## Encoding Procedure Started ##\n");

    // Reset file pointer to beginning of source image
    rewind(encInfo->src_image_fptr);

    // Copy BMP header to output image
    printf("Copying Image Header\n");
    copy_bmp_header(encInfo);
    printf("Done\n");

    // Encode magic string
    printf("Encoding Magic String Signature\n");
    encode_magic_string(MAGIC_STRING, encInfo);
    printf("Done\n");

    // Encode secret file extension
    printf("Encoding %s File Extension\n", encInfo->secret_fname);
    encode_secret_file_extn(encInfo);
    printf("Done\n");

    // Encode secret file size and data
    printf("Encoding %s File Size\n", encInfo->secret_fname);
    encode_secret_file_data(encInfo);
    printf("Done\n");

    // Copy remaining image data
    printf("Copying Left Over Data\n");
    copy_remaining_img_data(encInfo);
    printf("Done\n");

    printf("## Encoding Done Successfully ##\n");
    return success;
}

/* Validate encoding arguments */
Status validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Check source file extension
    char *res = strstr(argv[2], ".bmp");
    if (res == NULL)
    {
        printf("Error: Invalid Source Filename\n");
        return failure;
    }

    encInfo->src_image_fname = argv[2];
    printf("Src file is .bmp\n");

    // Extract secret file extension
    char *p = strstr(argv[3], ".");
    if (p == NULL)
    {
        printf("Error: Invalid Source Filename\n");
        return failure;
    }

    encInfo->secret_fname = argv[3];

    int i = 0;
    while (p[i] != '\0')
    {
        encInfo->secret_extn[i] = p[i];
        i++;
    }
    encInfo->secret_extn[i] = '\0';
    encInfo->secret_extn_size = strlen(encInfo->secret_extn);

    // Handle output file name
    if (argv[4] == NULL)
    {
        encInfo->output_image_fname = "output.bmp";
        printf("Output file not mentioned. Creating %s as default\n",
               encInfo->output_image_fname);
    }

    // Open required files
    if (open_files(encInfo) == failure)
        return failure;

    // Check if image has enough capacity
    if (check_capacity(encInfo) == failure)
        return failure;

    rewind(encInfo->src_image_fptr);
    return success;
}

/* Get file size */
unsigned long int get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    int pos = ftell(fptr);
    rewind(fptr);
    return pos;
}

/* Check image capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    printf("Checking for %s size\n", encInfo->secret_fname);

    // Ensure secret file is not empty
    if (get_file_size(encInfo->secret_fptr) > 0)
        printf("Done. Not Empty\n");

    printf("Checking for %s capacity to handle %s\n",
           encInfo->src_image_fname,
           encInfo->output_image_fname);

    // Calculate image capacity
    unsigned long int image_capacity =
        get_image_size_for_bmp(encInfo->src_image_fptr);

    // Calculate total encoding requirement
    unsigned long int encoding_things =
        (4 + strlen(MAGIC_STRING) + 4 + 4 +
         strlen(encInfo->secret_extn) + 4 +
         get_file_size(encInfo->secret_fptr)) * 8;

    if (image_capacity < encoding_things)
        return failure;

    printf("Done. Found OK\n");
    return success;
}

/* Copy BMP header */
Status copy_bmp_header(EncodeInfo *encInfo)
{
    char header[54];
    fread(header, 54, 1, encInfo->src_image_fptr);
    fwrite(header, 54, 1, encInfo->output_image_fptr);
}

/* Encode one byte into 8 LSBs */
Status encode_1byte_to_lsb(char data, char *buffer_8)
{
    for (int i = 7; i >= 0; i--)
    {
        int bit = (data >> i) & 1;
        buffer_8[7 - i] = (buffer_8[7 - i] & ~1) | bit;
    }
}

/* Encode 4 bytes into 32 LSBs */
Status encode_4byte_to_lsb(int data, char *buffer_32)
{
    for (int i = 31; i >= 0; i--)
    {
        int bit = (data >> i) & 1;
        buffer_32[31 - i] = (buffer_32[31 - i] & ~1) | bit;
    }
}

/* Encode magic string */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[32];

    // Encode length of magic string
    fread(buffer, 32, 1, encInfo->src_image_fptr);
    encode_4byte_to_lsb(strlen(magic_string), buffer);
    fwrite(buffer, 32, 1, encInfo->output_image_fptr);

    // Encode magic string characters
    for (int i = 0; i < strlen(magic_string); i++)
    {
        char temp[8];
        fread(temp, 8, 1, encInfo->src_image_fptr);
        encode_1byte_to_lsb(magic_string[i], temp);
        fwrite(temp, 8, 1, encInfo->output_image_fptr);
    }
}

/* Encode secret file extension */
Status encode_secret_file_extn(EncodeInfo *encInfo)
{
    char buffer[32];

    // Encode extension size
    fread(buffer, 32, 1, encInfo->src_image_fptr);
    encode_4byte_to_lsb(encInfo->secret_extn_size, buffer);
    fwrite(buffer, 32, 1, encInfo->output_image_fptr);

    // Encode extension characters
    for (int i = 0; i < encInfo->secret_extn_size; i++)
    {
        char temp[8];
        fread(temp, 8, 1, encInfo->src_image_fptr);
        encode_1byte_to_lsb(encInfo->secret_extn[i], temp);
        fwrite(temp, 8, 1, encInfo->output_image_fptr);
    }
}

/* Encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // Get secret file size
    encInfo->secret_file_size = get_file_size(encInfo->secret_fptr);

    char buffer[32];

    // Encode secret file size
    fread(buffer, 32, 1, encInfo->src_image_fptr);
    encode_4byte_to_lsb(encInfo->secret_file_size, buffer);
    fwrite(buffer, 32, 1, encInfo->output_image_fptr);

    // Encode secret file data
    for (int i = 0; i < encInfo->secret_file_size; i++)
    {
        char temp[8];
        fread(temp, 8, 1, encInfo->src_image_fptr);
        encode_1byte_to_lsb(getc(encInfo->secret_fptr), temp);
        fwrite(temp, 8, 1, encInfo->output_image_fptr);
    }
}

/* Copy remaining image data */
Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    char ch;

    // Copy remaining bytes from source to output image
    while (fread(&ch, 1, 1, encInfo->src_image_fptr) == 1)
    {
        fwrite(&ch, 1, 1, encInfo->output_image_fptr);
    }

    fclose(encInfo->src_image_fptr);
    fclose(encInfo->output_image_fptr);
}
