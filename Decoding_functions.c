#include <stdio.h>  // Include standard input/output library
#include "Decode_function_header_file.h" // Include decode header file
#include "Return_types.h"  // Include types header file
#include "Magic_string.h" // Include Magic_string header file
#include <string.h> // Include string manipulation library

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    puts(COLOR_BOLD_GREEN "INFO: ## Decoding Procedure Started ##" COLOR_RESET); // Print decoding start message
    if (strstr(argv[2], ".bmp"))
    {
        puts(COLOR_BOLD_GREEN "INFO: Decoding BMP Image" COLOR_RESET); // Print decoding BMP image message
        decInfo->src_image_fname = argv[2];                            // Assign source image filename from arguments
    }
    else
    {
        puts(COLOR_BOLD_RED "ERROR: Invalid File Format" COLOR_RESET); // Print error message
        return e_failure;                                              // Return failure if invalid file format
    }
    if (argv[3] != NULL)
    {
        if (strstr(argv[3], ".txt")) // Check if output file is provided
        {
            decInfo->stego_image_fname = argv[3];                           // Assign output file name from arguments
            puts(COLOR_BOLD_GREEN "INFO: Opening Output File" COLOR_RESET); // Print opening output file message
        }
        else
        {
            puts(COLOR_BOLD_RED "ERROR: Invalid Output File Format" COLOR_RESET); // Print error message
            return e_failure;                                                     // Return failure if invalid output file format
        }
    }
    else
    {
        puts(COLOR_BOLD_GREEN "INFO: Output File not mentioned. Creating decoded.txt as default" COLOR_RESET); // Print default file creation message
        puts(COLOR_BOLD_GREEN "INFO: Opening decoded.txt" COLOR_RESET);                                        // Print opening default file message
        decInfo->stego_image_fname = "decoded.txt";                                                            // Assign default output file name
    }
    decode_open_files(decInfo);                 // Open required files for decoding
    char data[2];                               // Buffer to store BMP signature
    fread(data, 1, 2, decInfo->fptr_src_image); // Read first 2 bytes of the image file
    if (data[0] == 0x42 && data[1] == 0x4d)     // Check if file is a BMP image
    {
        return e_success; // Return success if valid BMP
    }
    else
    {
        return e_failure; // Return failure if not a BMP
    }
}

Status do_decoding(DecodeInfo *decInfo)
{
    if (decode_magic_string(MAGIC_STRING, decInfo) == 0) // Decode magic string
    {
        puts(COLOR_BOLD_GREEN "INFO: Done" COLOR_RESET);                                   // Print success message
        if (decode_secret_file_extn_size(strlen(decInfo->extn_secret_file), decInfo) == 0) // Decode secret file extension size
        {
            if (decode_secret_file_extn(decInfo->extn_secret_file, decInfo) == 0) // Decode secret file extension
            {
                puts(COLOR_BOLD_GREEN "INFO: Done" COLOR_RESET);                      // Print success message
                if (decode_secret_file_size(decInfo->size_secret_file, decInfo) == 0) // Decode secret file size
                {
                    puts(COLOR_BOLD_GREEN "INFO: Done" COLOR_RESET); // Print success message
                    if (decode_secret_file_data(decInfo) == 0)       // Decode secret file data
                    {
                        puts(COLOR_BOLD_GREEN "INFO: Done" COLOR_RESET);                             // Print success message
                        puts(COLOR_BOLD_GREEN "INFO: ## Decoding Done Successfully ##" COLOR_RESET); // Print decoding completion message
                    }
                    else
                    {
                        puts(COLOR_BOLD_RED "ERROR: Decoding secret file data failed" COLOR_RESET); // Print error message
                        return e_failure;                                                           // Return failure if decoding data fails
                    }
                }
                else
                {
                    puts(COLOR_BOLD_RED "ERROR: Decoding secret file size failed" COLOR_RESET); // Print error message
                    return e_failure;                                                           // Return failure if decoding size fails
                }
            }
            else
            {
                puts(COLOR_BOLD_RED "ERROR: Decoding secret file extension failed" COLOR_RESET); // Print error message
                return e_failure;                                                                // Return failure if decoding extension fails
            }
        }
        else
        {
            puts(COLOR_BOLD_RED "ERROR: Decoding secret file extension size failed" COLOR_RESET); // Print error message
            return e_failure;                                                                     // Return failure if decoding extension size fails
        }
    }
    else
    {
        puts(COLOR_BOLD_RED "ERROR: Decoding magic string failed" COLOR_RESET); // Print error message
        return e_failure;                                                       // Return failure if decoding magic string fails
    }
}

Status decode_open_files(DecodeInfo *decInfo)
{
    puts(COLOR_BOLD_GREEN "INFO: Opening required files" COLOR_RESET); // Print file opening message
    decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");    // Open source image file in read mode
    if (decInfo->fptr_src_image == NULL)                               // Check if source image file opened successfully
    {
        perror("fopen");                                                                                                       // Print error message
        fprintf(stderr, COLOR_BOLD_SLOW_BLINKING_RED "ERROR: Unable to open file %s\n" COLOR_RESET, decInfo->src_image_fname); // Print error details
        return e_failure;                                                                                                      // Return failure
    }
    else
    {
        printf(COLOR_BOLD_GREEN "INFO: Opening %s\n" COLOR_RESET, decInfo->src_image_fname); // Print success message for source file
    }
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "w"); // Open stego image file in write mode
    if (decInfo->fptr_stego_image == NULL)                              // Check if stego image file opened successfully
    {
        perror("fopen");                                                                                                         // Print error message
        fprintf(stderr, COLOR_BOLD_SLOW_BLINKING_RED "ERROR: Unable to open file %s\n" COLOR_RESET, decInfo->stego_image_fname); // Print error details
        return e_failure;                                                                                                        // Return failure
    }
    puts(COLOR_BOLD_GREEN "INFO: Done. Opened all required files" COLOR_RESET); // Print success message for file opening
    return e_success;                                                           // Return success
}

Status decode_int_tolsb(long int *data, char *image_buffer)
{
    int index = 0;                // Initialize index to 0
    for (int i = 31; i >= 0; i--) // Loop through 32 bits
    {
        index = index | (image_buffer[31 - i] & 1) << i; // Extract LSB and set corresponding bit in index
    }
    *data = index; // Assign decoded integer to data
}

Status decode_byte_tolsb(char *data, char *image_buffer)
{
    char ch = 0;                 // Initialize character to 0
    for (int i = 7; i >= 0; i--) // Loop through 8 bits
    {
        ch = ch | (image_buffer[7 - i] & 1) << i; // Extract LSB and set corresponding bit in character
    }
    *data = ch; // Assign decoded byte to data
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    puts(COLOR_BOLD_GREEN "INFO: Decoding Magic String Signature" COLOR_RESET); // Print decoding magic string message
    int i = 0;                                                                  // Initialize loop variable
    char str[8];                                                                // Buffer to store 8 bits
    int lem = strlen(magic_string);                                             // Get length of magic string
    char data[lem];                                                             // Buffer to store decoded magic string
    fseek(decInfo->fptr_src_image, 54, SEEK_SET);                               // Move file pointer to pixel data offset
    for (i = 0; i < lem; i++)                                                   // Loop through magic string length
    {
        fread(str, 1, 8, decInfo->fptr_src_image); // Read 8 bits from image
        decode_byte_tolsb(&data[i], str);          // Decode byte from LSB
    }
    return e_success; // Return success
}

Status decode_secret_file_extn_size(long int size, DecodeInfo *decInfo)
{
    char str1[32];                               // Buffer to store 32 bits
    fread(str1, 1, 32, decInfo->fptr_src_image); // Read 32 bits from image
    decode_int_tolsb(&size, str1);               // Decode integer from LSB
    return e_success;                            // Return success
}

Status decode_secret_file_extn(char *ext, DecodeInfo *decInfo)
{
    printf(COLOR_BOLD_GREEN "INFO: Decoding %s File Extenstion\n" COLOR_RESET, decInfo->src_image_fname); // Print decoding extension message
    int i = 0;                                                                                            // Initialize loop variable
    char str[8];                                                                                          // Buffer to store 8 bits
    for (i = 0; i < 3; i++)                                                                               // Loop through 3 bytes (extension length)
    {
        fread(str, 1, 8, decInfo->fptr_src_image); // Read 8 bits from image
        decode_byte_tolsb(&ext[i], str);           // Decode byte from LSB
    }
    return e_success; // Return success
}

Status decode_secret_file_size(long file_size, DecodeInfo *decInfo)
{
    printf(COLOR_BOLD_GREEN "INFO: Decoding %s File Size\n" COLOR_RESET, decInfo->stego_image_fname); // Print decoding file size message
    char data[32];                                                                                    // Buffer to store 32 bits
    fread(data, 1, 32, decInfo->fptr_src_image);                                                      // Read 32 bits from image
    decode_int_tolsb(&decInfo->size_secret_file, data);                                               // Decode integer from LSB
    return e_success;                                                                                 // Return success
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    printf(COLOR_BOLD_GREEN "INFO: Decoding %s File Data\n" COLOR_RESET, decInfo->stego_image_fname); // Print decoding file data message
    int i = 0;                                                                                        // Initialize loop variable
    char str[8];                                                                                      // Buffer to store 8 bits
    char sec[decInfo->size_secret_file];                                                              // Buffer to store secret file data
    fseek(decInfo->fptr_stego_image, 0, SEEK_SET);                                                    // Move file pointer to start of stego image
    for (i = 0; i < decInfo->size_secret_file; i++)                                                   // Loop through secret file size
    {
        fread(str, 1, 8, decInfo->fptr_src_image); // Read 8 bits from image
        decode_byte_tolsb(&sec[i], str);           // Decode byte from LSB
    }
    fwrite(sec, 1, decInfo->size_secret_file, decInfo->fptr_stego_image); // Write decoded data to stego image
    return e_success;                                                     // Return success
}
