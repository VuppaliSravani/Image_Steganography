/*
Name        : VUPPALI SRAVANI
Date        : 23-01-2026
Description : LSB-based image steganography to securely hide and extract secret data within a BMP image without visible distortion.
*/

#include <stdio.h>
#include "encode.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    EncodeInfo encodeInfo;   // Structure to hold encoding related information
    DecodeInfo decodeInfo;   // Structure to hold decoding related information

    // If no command-line arguments are passed
    if (argc == 1)
    {
        // Print correct usage instructions
        printf("Invalid arguments\nEncoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\nDecoding: ./lsb_steg -d <.bmp file> [output file]\n");
        return 0;
    }

    // Check whether the operation is encode or decode
    int opr = check_operation(argv[1]);

    // Encoding operation
    if (opr == encode)
    {
        // Validate number of arguments for encoding
        if (argc < 4 || argc > 5)
        {
            printf("Invalid arguments\nEncoding: ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
            return 0;
        }

        // Perform encoding
        if (do_encoding(argv, &encodeInfo) == failure)
            return 0;
    }
    // Decoding operation
    else if (opr == decode)
    {
        // Validate number of arguments for decoding
        if (argc < 3 || argc > 4)
        {
            printf("Invalid arguments\nDecoding: ./lsb_steg -d <.bmp file> [output file]\n");
            return 0;
        }

        // Perform decoding
        if (do_decoding(argv, &decodeInfo) == failure)
            return 0;
    }
    // Invalid operation option
    else
    {
        printf("Invalid operation\n");
    }

    return 0;   // Exit program successfully
}
