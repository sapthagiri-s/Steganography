#include <stdio.h>  // Include standard input-output header
#include "encode.h" // Include header file for encoding functions
#include "decode.h" // Include header file for decoding functions
#include "types.h"  // Include header file for custom types
#include <string.h> // Include string manipulation functions

// Define color codes for terminal output
#define COLOR_BOLD_SLOW_BLINKING "\e[1;5m"        // Bold slow blinking text
#define COLOR_BOLD_SLOW_BLINKING_RED "\e[1;5;31m" // Bold slow blinking red text
#define COLOR_BOLD_BLUE "\e[1;34m"                // Bold blue text
#define COLOR_RESET "\e[0m"                       // Reset text formatting

int main(int argc, char *argv[]) // Main function with command-line arguments
{
    EncodeInfo encInfo; // Structure to hold encoding information
    DecodeInfo decInfo; // Structure to hold decoding information

    if (argc == 1) // Check if no arguments are provided
    {
        // Print help message for encoding and decoding
        printf("Help : \n");
        printf("For Encoding : \n");
        printf(COLOR_BOLD_BLUE "-e" COLOR_RESET " <inputfile.bmp> <secretfile.txt> <optional - outputfile.bmp> \n");
        printf("For Decoding : \n");
        printf(COLOR_BOLD_BLUE "-d" COLOR_RESET " <inputfile.bmp> <optional - outputfile.txt>\n");
        return e_unsupported; // Return unsupported operation
    }
    else if (argc == 2) // Check if only one argument is provided
    {
        if (!strcmp(argv[1], "-e")) // Check if the argument is "-e"
        {
            // Print help message for encoding
            printf("Help : \n");
            printf("For Encoding : \n");
            printf(COLOR_BOLD_BLUE "-e" COLOR_RESET " <inputfile.bmp> <secretfile.txt> <optional - outputfile.bmp> \n");
        }
        else if (!strcmp(argv[1], "-d")) // Check if the argument is "-d"
        {
            // Print help message for decoding
            printf("For Decoding : \n");
            printf(COLOR_BOLD_BLUE "-d" COLOR_RESET " <inputfile.bmp> <optional - outputfile.txt>\n");
        }
        else // Handle invalid arguments
        {
            // Print general help message
            printf("Help : \n");
            printf("For Encoding : \n");
            printf(COLOR_BOLD_BLUE "-e" COLOR_RESET " <inputfile.bmp> <secretfile.txt> <optional - outputfile.bmp> \n");
            printf("For Decoding : \n");
            printf(COLOR_BOLD_BLUE "-d" COLOR_RESET " <inputfile.bmp> <optional - outputfile.txt>\n");
        }
        return e_unsupported; // Return unsupported operation
    }
    else if (argc >= 3) // Check if at least two arguments are provided
    {
        if (!strcmp(argv[1], "-e")) // Check if the first argument is "-e"
        {
            if (argc >= 4) // Check if sufficient arguments are provided for encoding
            {
                // Validate encoding arguments and perform encoding
                if (read_and_validate_encode_args(argv, &encInfo) == e_success)
                {
                    do_encoding(&encInfo); // Perform encoding
                }
                else // Handle validation failure
                {
                    printf(COLOR_BOLD_SLOW_BLINKING_RED "Encode read and validate failed\n" COLOR_RESET);
                    return e_failure; // Return failure
                }
            }
            else // Handle insufficient arguments for encoding
            {
                // Print help message for encoding
                printf("Help : \n");
                printf("For Encoding : \n");
                printf(COLOR_BOLD_BLUE "-e" COLOR_RESET " <inputfile.bmp> <secretfile.txt> <optional - outputfile.bmp> \n");
                return e_unsupported; // Return unsupported operation
            }
        }
        else if (!strcmp(argv[1], "-d")) // Check if the first argument is "-d"
        {
            if (argc >= 3) // Check if sufficient arguments are provided for decoding
            {
                // Validate decoding arguments and perform decoding
                if (read_and_validate_decode_args(argv, &decInfo) == e_success)
                {
                    do_decoding(&decInfo); // Perform decoding
                }
                else // Handle validation failure
                {
                    printf(COLOR_BOLD_SLOW_BLINKING_RED "Decode read and validate failed\n" COLOR_RESET);
                    return e_failure; // Return failure
                }
            }
            else // Handle insufficient arguments for decoding
            {
                // Print help message for decoding
                printf("For Decoding : \n");
                printf(COLOR_BOLD_BLUE "-d" COLOR_RESET " <inputfile.bmp> <optional - outputfile.txt>\n");
                return e_unsupported; // Return unsupported operation
            }
        }
    }
    return e_success; // Return success
}
