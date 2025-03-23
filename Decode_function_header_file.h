#ifndef DECODE_H // Include guard to prevent multiple inclusions of this header file
#define DECODE_H

#include "types.h" // Include user-defined types from types.h

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define COLOR_BOLD_SLOW_BLINKING "\e[1;5m"        // Define bold slow blinking text color
#define COLOR_BOLD_SLOW_BLINKING_RED "\e[1;5;31m" // Define bold slow blinking red text color
#define COLOR_BOLD_RED "\e[1;5;31m" // Define bold red text color
#define COLOR_BOLD_BLUE "\e[1;34m"                // Define bold blue text color
#define COLOR_RESET "\e[0m"                       // Define reset text formatting
#define COLOR_BOLD_GREEN "\e[1;32m"               // Define bold green text color
#define MAX_SECRET_BUF_SIZE 1                     // Define maximum buffer size for secret data
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8) // Define maximum buffer size for image data
#define MAX_FILE_SUFFIX 4                         // Define maximum file suffix length

typedef struct _DecodeInfo // Define a structure to hold decoding information
{
    /* Source Image info */
    char *src_image_fname; // Pointer to source image file name
    FILE *fptr_src_image;  // File pointer for source image
    uint image_capacity;   // Capacity of the image to hold data
    uint bits_per_pixel;   // Bits per pixel in the image
    char image_data[MAX_IMAGE_BUF_SIZE]; // Buffer to hold image data

    /* Secret File Info */
    char *secret_fname; // Pointer to secret file name
    FILE *fptr_secret;  // File pointer for secret file
    char extn_secret_file[MAX_FILE_SUFFIX + 1]; // Buffer to hold secret file extension
    char secret_data[MAX_SECRET_BUF_SIZE]; // Buffer to hold secret data
    long size_secret_file; // Size of the secret file

    /* Stego Image Info */
    char *stego_image_fname; // Pointer to stego image file name
    FILE *fptr_stego_image;  // File pointer for stego image

} DecodeInfo;

/* Decoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo); // Validate decoding arguments

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo); // Perform decoding process

/* Get File pointers for i/p and o/p files */
Status decode_open_files(DecodeInfo *decInfo); // Open input and output files for decoding

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image); // Get the size of the BMP image

/* Get file size */
uint get_file_size(FILE *fptr); // Get the size of a file

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image); // Copy BMP header from source to destination

/* Store Magic String */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo); // Decode and store the magic string

/* Encode secret file extenstion size*/
Status decode_secret_file_extn_size(long int size, DecodeInfo *decInfo); // Decode secret file extension size

/* Encode secret file extenstion */
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo); // Decode secret file extension

/* Encode secret file size */
Status decode_secret_file_size(long file_size, DecodeInfo *decInfo); // Decode secret file size

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo); // Decode secret file data

/* Encode function, which does the real encoding */
Status decode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image); // Decode data into the image

/* Encode a byte into LSB of image data array */
Status decode_byte_to_lsb(char data, char *image_buffer); // Decode a byte into the least significant bit of the image buffer

#endif // End of include guard
