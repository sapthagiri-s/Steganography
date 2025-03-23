#ifndef ENCODE_H // Include guard to prevent multiple inclusions of this header file
#define ENCODE_H

#include "Return_types.h" // Include user-defined types from types.h

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */
#define COLOR_BOLD_SLOW_BLINKING "\e[1;5m"        // Define ANSI escape code for bold slow blinking text
#define COLOR_BOLD_SLOW_BLINKING_RED "\e[1;5;31m" // Define ANSI escape code for bold slow blinking red text
#define COLOR_BOLD_BLUE "\e[1;34m"                // Define ANSI escape code for bold blue text
#define COLOR_RESET "\e[0m"                       // Define ANSI escape code to reset text formatting
#define COLOR_BOLD_GREEN "\e[1;32m"               // Define ANSI escape code for bold green text
#define MAX_SECRET_BUF_SIZE 1                     // Maximum buffer size for secret data
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8) // Maximum buffer size for image data (8 times secret buffer size)
#define MAX_FILE_SUFFIX 4                         // Maximum length of file suffix (e.g., ".txt")

typedef struct _EncodeInfo // Structure to hold encoding-related information
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
    char extn_secret_file[MAX_FILE_SUFFIX + 1]; // Extension of the secret file
    char secret_data[MAX_SECRET_BUF_SIZE]; // Buffer to hold secret data
    long size_secret_file; // Size of the secret file

    /* Stego Image Info */
    char *stego_image_fname; // Pointer to stego image file name
    FILE *fptr_stego_image;  // File pointer for stego image

} EncodeInfo;


/* Encoding function prototype */


/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo); // Function to read and validate encoding arguments

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo); // Function to perform encoding

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo); // Function to open input and output files

/* check capacity */
Status check_capacity(EncodeInfo *encInfo); // Function to check if the image has enough capacity for encoding

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image); // Function to get the size of a BMP image

/* Get file size */
uint get_file_size(FILE *fptr); // Function to get the size of a file

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image); // Function to copy BMP image header

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo); // Function to encode a magic string into the image

/* Encode secret file extenstion size*/
Status encode_secret_file_extn_size(long int size,EncodeInfo *encInfo); // Function to encode the size of the secret file extension

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo); // Function to encode the secret file extension

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo); // Function to encode the size of the secret file

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo); // Function to encode the secret file data

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image); // Function to encode data into the image

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer); // Function to encode a byte into the least significant bit of the image buffer

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest); // Function to copy remaining image data from source to stego image

#endif // End of include guard
