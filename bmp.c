#include "bmp.h"

int validate(int argc, char *argv[], FILE **file, BMPHeader *bmpheader, DIBHeader *dibheader) {
    // Check if user inputs only one argument
    if (argc < 3) {
        fprintf(stderr, "ERROR: Missing arguments.\n");
        return 0;
    }

    // Open the file
    *file = fopen(argv[2], "r+");
    if (*file == NULL) {
        fprintf(stderr, "ERROR: File not found.\n");
        return 0;
    }

    if (!validateBMPHeader(*file, bmpheader)) {
        fprintf(stderr, "ERROR: The format is not supported.\n");
        fclose(*file);
        return 0;
    }

    if (!validateDIBHeader(*file, dibheader)) {
        fprintf(stderr, "ERROR: The format is not supported.\n");
        fclose(*file);
        return 0;
    }

    return 1;
}

int vaidateHide(int argc, char *argv[], FILE **file, FILE **file2, 
                BMPHeader *bmpheader, BMPHeader *bmpheader2, DIBHeader *dibheader, DIBHeader *dibheader2) {
    // Check if user inputs only one argument
    if (argc < 4) {
        fprintf(stderr, "ERROR: Missing arguments.\n");
        return 0;
    }

    // Open the files
    *file = fopen(argv[2], "r+");
    *file2 = fopen(argv[3], "r+");
    if (*file == NULL || *file2 == NULL) {
        fprintf(stderr, "ERROR: File not found.\n");
        return 0;
    }

    if (!validateBMPHeader(*file, bmpheader)) {
        fprintf(stderr, "ERROR: The format is not supported.\n");
        fclose(*file);
        return 0;
    }

    if (!validateBMPHeader(*file2, bmpheader2)) {
        fprintf(stderr, "ERROR: The format is not supported.\n");
        fclose(*file2);
        return 0;
    }

    if (!validateDIBHeader(*file, dibheader)) {
        fprintf(stderr, "ERROR: The format is not supported.\n");
        fclose(*file);
        return 0;
    }

    if (!validateDIBHeader(*file2, dibheader2)) {
        fprintf(stderr, "ERROR: The format is not supported.\n");
        fclose(*file2);
        return 0;
    }

    if (dibheader->width != dibheader2->width || dibheader->height != dibheader2->height) {
        fprintf(stderr, "ERROR: Files don't have equal width or height.\n");
        fclose(*file);
        fclose(*file2);
        return 0;
    }

    return 1;
}

int validateBMPHeader(FILE *file, BMPHeader *bmpheader) {

    // Check if fread runs successfully
    if (fread(bmpheader, sizeof(*bmpheader), 1, file) != 1) return 0;

    // Check if the format identifier is BM
    return (bmpheader->identifier == 0x4D42) ? 1 : 0;
}

int validateDIBHeader(FILE *file, DIBHeader *dibheader) {

    // Check if fread runs successfully
    if (fread(dibheader, sizeof(*dibheader), 1, file) != 1) return 0;

    // Check if the size of DIB header is 40
    if (dibheader->header_size != 40) return 0;

    // Check if the bits per pixel field is 24
    if (dibheader->num_bit != 24) return 0;

    return 1;
}

const char* identifierToString(uint16_t identifier) {
    static char str[3];

    // Get the 8 LSB
    str[0] = (identifier & 0xFF);

    // Get the 8 MSB
    str[1] = (identifier >> 8);

    // Add the null terminator
    str[2] = '\0';

    return str;
}

int calculatePaddings(int width) {
    return (4 - (width * sizeof(Pixel)) % 4) % 4;
}