## Image_Steganography-C
Image steganography in C using LSB technique for secure encoding and decoding of secret data in BMP files.

## Features
- Hide secret messages in images (Encoding)
-  Extract hidden messages from images (Decoding)
-  Supports BMP images
-  User-friendly command-line interface
-  Sample images included for testing (beautiful.bmp)

##  How to Run
### 1. Clone the repository:

```bash
git clone https://https://github.com/VuppaliSravani/Image_Steganography
cd steganography-c
```
### 2️. Compile the Program
gcc src/main.c src/encode.c src/decode.c -I include -o steganography

### 3. How to run
- Encode ./steganography -e beautiful.bmp secret.txt <output.bmp>(optional)
- Decode ./steganography -d output.bmp <output.txt>(optional)
- Use -e to encode information
- Use -d to decode information

## Notes
- Follow the prompts to enter the BMP file path and view its metadata.
- A sample BMP file (beautiful.bmp) is included for testing.
- During encoding, a temporary file is used to avoid disturbing the original file.
- After encoding, the temporary file is decoded to see the encoded data.

## Skills Learned
C Programming: File handling, structures,Bitwise Operators and pointers Binary Data Processing: Reading and interpreting BMP files Multimedia File Handling: Understanding BMP image structure and LSB steganography.

## Dependencies
GCC or any C compiler

## Screeenshots
### Encode
<img width="1568" height="524" alt="image" src="https://github.com/user-attachments/assets/2fc6bc12-1280-487f-ade6-412c5962289e" />

### Decode
<img width="1364" height="399" alt="image" src="https://github.com/user-attachments/assets/e6560c7f-6e87-4aa9-a90a-3676a6d30574" />

## Sample Images
beautiful.bmp – sample image for testing output.bmp and output.txt – generated files during program execution

## Project Structure
Image_Steganography-C/
│
├─ README.md
├─ main.c
├─ encode.c
├─ encode.h
├─ decode.c
├─ decode.h
├─ common.h
├─ beautiful.bmp
├─ secret.txt
├─ default_data_file.txt
├─ output.bmp
├─ a.out
└─ .gitignore

  





