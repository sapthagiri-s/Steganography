#include<stdio.h> // Include standard input-output header file
#ifndef TYPES_H // Check if TYPES_H is not defined
#define TYPES_H // Define TYPES_H

/* User defined types */
typedef unsigned int uint; // Define uint as an alias for unsigned int

/* Status will be used in fn. return type */
typedef enum // Define an enumeration for status
{
    e_success, // Represents success
    e_failure  // Represents failure
} Status;

typedef enum // Define an enumeration for operation types
{
    e_encode,      // Represents encode operation
    e_decode,      // Represents decode operation
    e_unsupported  // Represents unsupported operation
} OperationType;

#endif // End of TYPES_H definition
