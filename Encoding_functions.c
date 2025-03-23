#include <stdio.h>  // Standard I/O library
#include "Encode_function_header_file.h" // Header file for encoding functions
#include "types.h"  // Header file for custom types
#include "Magic_string.h" // Header file for common utilities
#include <string.h> // String manipulation functions

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;                         // Variables to store width and height
    fseek(fptr_image, 18, SEEK_SET);            // Seek to 18th byte in BMP file
    fread(&width, sizeof(int), 1, fptr_image);  // Read width (4 bytes)
    fread(&height, sizeof(int), 1, fptr_image); // Read height (4 bytes)
    return width * height * 3;                  // Return image size in bytes
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    puts(COLOR_BOLD_GREEN "INFO: Opening required files" COLOR_RESET); // Log message
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");    // Open source image file in read mode
    if (encInfo->fptr_src_image == NULL)                               // Check if file opening failed
    {
        perror("fopen");                                                                                                       // Print error message
        fprintf(stderr, COLOR_BOLD_SLOW_BLINKING_RED "ERROR: Unable to open file %s\n" COLOR_RESET, encInfo->src_image_fname); // Log error
        return e_failure;                                                                                                      // Return failure
    }
    else
    {
        printf(COLOR_BOLD_GREEN "INFO: Opened %s\n" COLOR_RESET, encInfo->src_image_fname); // Log success
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r"); // Open secret file in read mode
    if (encInfo->fptr_secret == NULL)                         // Check if file opening failed
    {
        perror("fopen");                                                                                                    // Print error message
        fprintf(stderr, COLOR_BOLD_SLOW_BLINKING_RED "ERROR: Unable to open file %s\n" COLOR_RESET, encInfo->secret_fname); // Log error
        return e_failure;                                                                                                   // Return failure
    }
    else
    {
        printf(COLOR_BOLD_GREEN "INFO: Opened %s\n" COLOR_RESET, encInfo->secret_fname); // Log success
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w"); // Open stego image file in write mode
    if (encInfo->fptr_stego_image == NULL)                              // Check if file opening failed
    {
        perror("fopen");                                                                                                         // Print error message
        fprintf(stderr, COLOR_BOLD_SLOW_BLINKING_RED "ERROR: Unable to open file %s\n" COLOR_RESET, encInfo->stego_image_fname); // Log error
        return e_failure;                                                                                                        // Return failure
    }
    else
    {
        printf(COLOR_BOLD_GREEN "INFO: Opened %s\n" COLOR_RESET, encInfo->stego_image_fname); // Log success
    }

    return e_success; // Return success
}

uint get_file_size(FILE *file)
{
    fseek(file, 0, SEEK_END); // Move file pointer to the end
    return ftell(file);       // Return the current position (file size)
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);                            // Get size of secret file
    printf(COLOR_BOLD_GREEN "INFO: Checking for %s size\n" COLOR_RESET, encInfo->secret_fname); // Log message
    if (encInfo->size_secret_file != 0)                                                         // Check if file is not empty
    {
        puts(COLOR_BOLD_GREEN "INFO: Done. Not Empty" COLOR_RESET); // Log success
    }
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);                                                             // Get image capacity
    printf(COLOR_BOLD_GREEN "INFO: Checking for %s capacity to handle %s\n" COLOR_RESET, encInfo->src_image_fname, encInfo->secret_fname); // Log message
    uint magic_string_len = strlen(MAGIC_STRING);                                                                                          // Get length of magic string
    strtok(encInfo->secret_fname, ".");                                                                                                    // Extract file name without extension
    char *ext = strtok(NULL, ".");                                                                                                         // Extract file extension
    strcpy(encInfo->extn_secret_file, ext);                                                                                                // Copy extension to EncodeInfo
    uint file_ext_size = strlen(ext);                                                                                                      // Get length of file extension

    uint total_size = 4 + magic_string_len + 4 + file_ext_size + 4 + encInfo->size_secret_file; // Calculate total size needed
    if (encInfo->image_capacity < total_size * 8)                                               // Check if image capacity is insufficient
    {
        return e_failure; // Return failure
    }
    else
    {
        puts(COLOR_BOLD_GREEN "INFO: Done. Found OK" COLOR_RESET); // Log success
    }
    return e_success; // Return success
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if (!strstr(argv[2], ".bmp")) // Check if source image file is not BMP
    {
        puts(COLOR_BOLD_SLOW_BLINKING_RED "ERROR: Invalid Source Image File. Only BMP files are allowed" COLOR_RESET); // Log error
        return e_failure;                                                                                              // Return failure
    }
    else
    {
        encInfo->src_image_fname = argv[2]; // Set source image file name
    }
    if (!strstr(argv[3], ".txt")) // Check if secret file is not TXT
    {
        puts(COLOR_BOLD_SLOW_BLINKING_RED "ERROR: Invalid Secret File. Only TXT files are allowed" COLOR_RESET); // Log error
        return e_failure;                                                                                        // Return failure
    }
    else
    {
        encInfo->secret_fname = argv[3];                                       // Set secret file name
        strcpy(encInfo->extn_secret_file, strstr(encInfo->secret_fname, ".")); // Copy file extension
    }
    if (argv[4] != NULL && strstr(argv[4], ".bmp")) // Check if output file is specified and is BMP
    {
        encInfo->stego_image_fname = argv[4]; // Set stego image file name
    }
    else
    {
        puts(COLOR_BOLD_GREEN "INFO: Output File not mentioned. Creating steged_img.bmp as default" COLOR_RESET); // Log default file creation
        encInfo->stego_image_fname = "steged_img.bmp";                                                            // Set default stego image file name
    }
    if (open_files(encInfo) == 0) // Open required files
    {
        puts(COLOR_BOLD_GREEN "INFO: Done" COLOR_RESET);                             // Log success
        puts(COLOR_BOLD_GREEN "INFO: ## Encoding Procedure Started ##" COLOR_RESET); // Log start of encoding
    }
    char data[2];                               // Buffer to read BMP signature
    fread(data, 1, 2, encInfo->fptr_src_image); // Read first 2 bytes of BMP file
    if (data[0] == 0x42 && data[1] == 0x4d)     // Check if BMP signature is valid
    {
        if (check_capacity(encInfo) == e_failure) // Check if image has enough capacity
        {
            return e_failure; // Return failure
        }
        else
        {
            return e_success; // Return success
        }
    }
}

Status copy_bmp_header(FILE *src_img, FILE *dest_img)
{
    puts(COLOR_BOLD_GREEN "INFO: Copying Image Header" COLOR_RESET); // Log message
    char bmp_header[54];                                             // Buffer to store BMP header
    rewind(src_img);                                                 // Reset source image file pointer
    rewind(dest_img);                                                // Reset destination image file pointer

    if ((fread(bmp_header, 1, 54, src_img)) != 54) // Read BMP header
    {
        perror(COLOR_BOLD_SLOW_BLINKING_RED "Failed to read BMP header" COLOR_RESET); // Log error
        return 1;                                                                     // Return failure
    }
    if (fwrite(bmp_header, 1, 54, dest_img) != 54) // Write BMP header to destination
    {
        perror(COLOR_BOLD_SLOW_BLINKING_RED "Failed to write BMP header" COLOR_RESET); // Log error
        return 1;                                                                      // Return failure
    }
    return 0; // Return success
}

Status encode_int_tolsb(int data, char *image_buffer)
{
    int index = 0;                // Initialize index
    for (int i = 31; i >= 0; i--) // Loop through all 32 bits of integer
    {
        if (data & (1 << i)) // Check if bit is 1
        {
            image_buffer[index] = image_buffer[index] | 1; // Set LSB to 1
        }
        else
        {
            image_buffer[index] = image_buffer[index] & ~1; // Set LSB to 0
        }
        index++; // Increment index
    }
}

Status encode_byte_tolsb(char data, char *image_buffer)
{
    int index = 0;               // Initialize index
    for (int i = 7; i >= 0; i--) // Loop through all 8 bits of byte
    {
        if (data & (1 << i)) // Check if bit is 1
        {
            image_buffer[index] = image_buffer[index] | 1; // Set LSB to 1
        }
        else
        {
            image_buffer[index] = image_buffer[index] & ~1; // Set LSB to 0
        }
        index++; // Increment index
    }
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    puts(COLOR_BOLD_GREEN "INFO: Encoding Magic String Signature" COLOR_RESET); // Log message
    int i = 0;                                                                  // Initialize index
    char str[8];                                                                // Buffer to store 8 bytes
    int lem = strlen(magic_string);                                             // Get length of magic string
    for (i = 0; i < lem; i++)                                                   // Loop through each character of magic string
    {
        if (fread(str, 1, 8, encInfo->fptr_src_image) == 8) // Read 8 bytes from source image
        {
            encode_byte_tolsb(magic_string[i], str); // Encode character into LSBs
        }
        fwrite(str, 1, 8, encInfo->fptr_stego_image); // Write encoded bytes to stego image
    }
    return e_success; // Return success
}

Status encode_secret_file_extn_size(long int size, EncodeInfo *encInfo)
{
    char str1[32];                                  // Buffer to store 32 bytes
    fread(str1, 1, 32, encInfo->fptr_src_image);    // Read 32 bytes from source image
    encode_int_tolsb(size, str1);                   // Encode size into LSBs
    fwrite(str1, 1, 32, encInfo->fptr_stego_image); // Write encoded bytes to stego image
    return e_success;                               // Return success
}

Status encode_secret_file_extn(const char *ext, EncodeInfo *encInfo)
{
    printf(COLOR_BOLD_GREEN "INFO: Encoding %s File Extension\n" COLOR_RESET, encInfo->secret_fname); // Log message
    int i = 0;                                                                                        // Initialize index
    char str[8];                                                                                      // Buffer to store 8 bytes
    for (i = 0; i < 3; i++)                                                                           // Loop through first 3 characters of extension
    {
        fread(str, 1, 8, encInfo->fptr_src_image);    // Read 8 bytes from source image
        encode_byte_tolsb(ext[i], str);               // Encode character into LSBs
        fwrite(str, 1, 8, encInfo->fptr_stego_image); // Write encoded bytes to stego image
    }
    return e_success; // Return success
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    printf(COLOR_BOLD_GREEN "INFO: Encoding %s File Size\n" COLOR_RESET, encInfo->secret_fname); // Log message
    char str1[32];                                                                               // Buffer to store 32 bytes
    fread(str1, 1, 32, encInfo->fptr_src_image);                                                 // Read 32 bytes from source image
    encode_int_tolsb(file_size, str1);                                                           // Encode file size into LSBs
    fwrite(str1, 1, 32, encInfo->fptr_stego_image);                                              // Write encoded bytes to stego image
    return e_success;                                                                            // Return success
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf(COLOR_BOLD_GREEN "INFO: Encoding %s File Data\n" COLOR_RESET, encInfo->secret_fname); // Log message
    int i = 0;                                                                                   // Initialize index
    char str[8];                                                                                 // Buffer to store 8 bytes
    char sec[encInfo->size_secret_file];                                                         // Buffer to store secret file data
    fseek(encInfo->fptr_secret, 0, SEEK_SET);                                                    // Reset secret file pointer
    fread(sec, 1, encInfo->size_secret_file, encInfo->fptr_secret);                              // Read secret file data
    for (i = 0; i < encInfo->size_secret_file; i++)                                              // Loop through each byte of secret file
    {
        fread(str, 1, 8, encInfo->fptr_src_image);    // Read 8 bytes from source image
        encode_byte_tolsb(sec[i], str);               // Encode byte into LSBs
        fwrite(str, 1, 8, encInfo->fptr_stego_image); // Write encoded bytes to stego image
    }
    return e_success; // Return success
}

Status copy_remaining_img_data(FILE *src, FILE *dest)
{
    puts(COLOR_BOLD_GREEN "INFO: Copying Left Over Data" COLOR_RESET); // Log message
    char ch;                                                           // Variable to store a byte
    while (fread(&ch, 1, 1, src) > 0)                                  // Read remaining bytes from source image
    {
        fwrite(&ch, 1, 1, dest); // Write bytes to destination image
    }
    return e_success; // Return success
}

Status do_encoding(EncodeInfo *encInfo)
{
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == 0) // Copy BMP header
    {
        puts(COLOR_BOLD_GREEN "INFO: Done" COLOR_RESET);     // Log success
        if (encode_magic_string(MAGIC_STRING, encInfo) == 0) // Encode magic string
        {
            puts(COLOR_BOLD_GREEN "INFO: Done" COLOR_RESET);                                   // Log success
            if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == 0) // Encode file extension size
            {
                if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == 0) // Encode file extension
                {
                    puts(COLOR_BOLD_GREEN "INFO: Done" COLOR_RESET);                      // Log success
                    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == 0) // Encode file size
                    {
                        puts(COLOR_BOLD_GREEN "INFO: Done" COLOR_RESET); // Log success
                        if (encode_secret_file_data(encInfo) == 0)       // Encode file data
                        {
                            puts(COLOR_BOLD_GREEN "INFO: Done" COLOR_RESET);                                      // Log success
                            if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == 0) // Copy remaining image data
                            {
                                puts(COLOR_BOLD_GREEN "INFO: Done\033[0m");                                  // Log success
                                puts(COLOR_BOLD_GREEN "INFO: ## Encoding Done Successfully ##" COLOR_RESET); // Log completion
                            }
                            else
                            {
                                puts("\033[0;31mERROR: Failed to copy remaining image data\033[0m"); // Log error
                                return e_failure;                                                    // Return failure
                            }
                        }
                        else
                        {
                            puts("\033[0;31mERROR: Failed to encode secret file data\033[0m"); // Log error
                            return e_failure;                                                  // Return failure
                        }
                    }
                    else
                    {
                        puts("\033[0;31mERROR: Failed to encode secret file size\033[0m"); // Log error
                        return e_failure;                                                  // Return failure
                    }
                }
                else
                {
                    puts("\033[0;31mERROR: Failed to encode secret file extension\033[0m"); // Log error
                    return e_failure;                                                       // Return failure
                }
            }
            else
            {
                puts("\033[0;31mERROR: Failed to encode secret file extension size\033[0m"); // Log error
                return e_failure;                                                            // Return failure
            }
        }
        else
        {
            puts("\033[0;31mERROR: Failed to encode magic string\033[0m"); // Log error
            return e_failure;                                              // Return failure
        }
    }
    else
    {
        puts("\033[0;31mERROR: Failed to copy BMP header\033[0m"); // Log error
        return e_failure;                                          // Return failure
    }
}
