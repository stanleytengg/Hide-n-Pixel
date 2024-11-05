# BMP Steganography Project

## Overview
This project implements a steganography tool for BMP image files. It allows users to hide secret images within other images and reveal hidden images from BMP files.

## Features
- Info: Display BMP and DIB header information
- Reveal: Uncover hidden images within BMP files
- Hide: Conceal one image within another BMP file
- SuperReveal: Efficiently uncover hidden images (optimized version)
- SuperHide: Efficiently conceal images (optimized version)
  
## Compilation
Use the provided Makefile to compile the project. In Terminal:   
`make`

## Usage
`./bmp_steganography [OPTION] [FILE1] [FILE2]`   
Options:
- `--info`: Display BMP and DIB header information
- `--reveal`: Reveal a hidden image in FILE
- `--hide`: Hide FILE2 within FILE1 (overrides FILE1!)
- `--superreveal`: Efficiently reveal a hidden image in FILE
- `--superhide`: Efficiently hide FILE2 within FILE1 (overrides FILE1!)
 
When using `reveal` or `superreveal` on `coffee.bmp`, a hidden image will appear!
<img src="/Project1/coffee.bmp" alt="coffee" style="width: 50%; height: auto;" />
  
## Implementation Details

### Standard Reveal and Hide
The original `reveal` and `hide` functions process the BMP file pixel by pixel. While functional, these methods can be slow for large images due to frequent I/O operations.

#### Reveal
- Reads each pixel individually
- Swaps the 4 most significant bits with the 4 least significant bits for each color channel
- Writes the modified pixel back to the file

#### Hide
- Reads pixels from both source and target files
- Combines the 4 most significant bits from the target with the 4 most significant bits from the source
- Writes the resulting pixel to the target file

### Optimized SuperReveal and SuperHide
To address performance issues with large files, `superreveal` and `superhide` were implemented. These functions process the BMP file in larger chunks, significantly reducing I/O operations and improving speed.

#### SuperReveal
- Reads the file in chunks of 4096 bytes
- Processes all pixels in the chunk at once
- Writes back the entire modified chunk

#### SuperHide
- Reads both files in 4096-byte chunks
- Combines data from both files in memory
- Writes the resulting chunk to the target file

## Performance
The optimized versions (`superreveal` and `superhide`) show significant performance improvements, especially for large BMP files. They reduce the number of I/O operations by a factor of approximately 1365 compared to the pixel-by-pixel approach.
