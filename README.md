# Steganography

Steganography tool written in C, designed to hide and extract secret messages within BMP image files. It provides a secure and efficient way to communicate sensitive information without drawing attention.

## Features

- **Hide Messages**: Embed secret messages into BMP image files without altering their appearance.
- **Extract Messages**: Retrieve hidden messages from steganographic BMP files.
- **Support for BMP Format**: Works exclusively with BMP image files for simplicity and efficiency.
- **Customizable Output**: Optionally specify output file names for encoded or decoded data.
- **Lightweight and Fast**: Efficient implementation in C for high performance.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/stegano.git
   cd stegano
   ```

2. Build the project using `gcc`:
   ```bash
   gcc -o stegano Main.c Encoding_functions.c Decoding_functions.c
   ```

   Ensure all required `.c` and `.h` files are in the same directory.

3. Verify the executable `stegano` is created in the current directory.

## Usage

### Hiding a Message
1. Prepare the BMP image file and the secret text file.
2. Run the program to hide the message:
   ```bash
   ./stegano -e input_image.bmp secret_message_file.txt output_image.bmp
   ```

   - `input_image.bmp`: The source BMP file.
   - `secret_message_file.txt`: The text file containing the secret message.
   - `output_image.bmp`: (Optional) The output BMP file with the hidden message. Defaults to `steged_img.bmp` if not provided.

### Extracting a Message
1. Use the steganographic BMP file.
2. Run the program to extract the hidden message:
   ```bash
   ./stegano -d input_image.bmp output_message.txt
   ```

   - `input_image.bmp`: The BMP file with the hidden message.
   - `output_message.txt`: (Optional) The output text file for the extracted message. Defaults to `decoded.txt` if not provided.

### Help
To display usage instructions:
```bash
./stegano
```

## File Structure

- **Main.c**: Entry point of the program, handles command-line arguments.
- **Encoding_functions.c**: Contains functions for encoding messages into BMP files.
- **Decoding_functions.c**: Contains functions for decoding messages from BMP files.
- **Magic_string.h**: Defines the magic string used for identifying steganographic files.
- **Return_types.h**: Defines custom types and enumerations for status and operations.

## Contributing

Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Commit your changes and push them to your fork.
4. Submit a pull request with a detailed description of your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by the need for secure communication.
- Thanks to the open-source community for their contributions.

---
Feel free to explore, contribute, and use STEGANO for your steganography needs!
