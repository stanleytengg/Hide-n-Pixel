#include "bmp.h"
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *file = NULL;
    BMPHeader bmpheader;
    DIBHeader dibheader;

    // Shows the info of image
    if (strcmp(argv[1], "--info") == 0) {

        // Check if there's any error
        if (!validate(argc, argv, &file, &bmpheader, &dibheader)) return 1;

        printf("=== BMP Header ===\n");
        printf("Type: %s\n", identifierToString(bmpheader.identifier));
        printf("Size: %u\n", bmpheader.size);
        printf("Reserved 1: %u\n", bmpheader.reserve1);
        printf("Reserved 2: %u\n", bmpheader.reserve2);
        printf("Image offset: %u\n\n", bmpheader.offset);

        printf("=== DIB Header ===\n");
        printf("Size: %u\n", dibheader.header_size);
        printf("Width: %u\n", dibheader.width);
        printf("Height: %u\n", dibheader.height);
        printf("# color planes: %u\n", dibheader.color_planes);
        printf("# bits per pixel: %u\n", dibheader.num_bit);
        printf("Compression scheme: %u\n", dibheader.compression);
        printf("Image size: %u\n", dibheader.image_size);
        printf("Horizontal resolution: %u\n", dibheader.hresolution);
        printf("Vertical resolution: %u\n", dibheader.vresolution);
        printf("# colors in palette: %u\n", dibheader.colors_palette);
        printf("# important colors: %u\n", dibheader.num_imp_color);

        fclose(file);
    }

    // Reveals the hidden image
    else if (strcmp(argv[1], "--reveal") == 0) {
        // Check if there's any error
        if (!validate(argc, argv, &file, &bmpheader, &dibheader)) return 1;

        // Move the file pointer to the start of pixel array
        fseek(file, bmpheader.offset, SEEK_SET);

        // Loop through all pixels
        for (int i = 0; i < dibheader.height; i++) {
            for (int j = 0; j < dibheader.width; j++) {
                Pixel pixel;

                // Read in each pixel
                fread(&pixel, sizeof(pixel), 1, file);

                // Swap 4MSB with its 4LSB
                pixel.blue = (pixel.blue >> 4) | (pixel.blue << 4);
                pixel.green = (pixel.green >> 4) | (pixel.green << 4);
                pixel.red = (pixel.red >> 4) | (pixel.red << 4);

                // Redirect the pointer back to the front of the pixel that is being modified
                fseek(file, -sizeof(pixel), SEEK_CUR);

                // Write the swapped pixel back to the file
                fwrite(&pixel, sizeof(pixel), 1, file);
            }

            // Calculate how many padding each row needs
            int padding = calculatePaddings(dibheader.width);
            
            // Skip padding bytes
            fseek(file, padding, SEEK_CUR);
        }

        fclose(file);
    }

    else if (strcmp(argv[1], "--superreveal") == 0) {
        // Check if there's any error
        if (!validate(argc, argv, &file, &bmpheader, &dibheader)) return 1;

        unsigned char buffer[CHUNK_SIZE];
        size_t bytesRead;
        long position = bmpheader.offset;

        fseek(file, position, SEEK_SET);

        while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, file)) > 0) {

            // Loop through all pixels in the chunk
            for (size_t i = 0; i < bytesRead; i += 3) {
                // Process each pixel in the buffer
                buffer[i] = (buffer[i] << 4) | (buffer[i] >> 4);
                buffer[i+1] = (buffer[i+1] << 4) | (buffer[i+1] >> 4);
                buffer[i+2] = (buffer[i+2] << 4) | (buffer[i+2] >> 4);
            }

            // Write back the modified buffer
            fseek(file, position, SEEK_SET);
            fwrite(buffer, 1, bytesRead, file);
            position += bytesRead;
        }

        fclose(file);
    }

    else if (strcmp(argv[1], "--hide") == 0) {
        FILE *file2 = NULL;
        BMPHeader bmpheader2;
        DIBHeader dibheader2;

        // Check if there's any error
        if (!vaidateHide(argc, argv, &file, &file2, &bmpheader, &bmpheader2, &dibheader, &dibheader2))
            return 1;

        // Move the files' pointers to the start of pixel arrays
        fseek(file, bmpheader.offset, SEEK_SET);
        fseek(file, bmpheader2.offset, SEEK_SET);
        
        for (int i = 0; i < dibheader.height; i++) {
            for (int j = 0; j < dibheader.width; j++) {
                Pixel pixel, pixel2;

                // Read in each pixel
                fread(&pixel, sizeof(pixel), 1, file);
                fread(&pixel2, sizeof(pixel2), 1, file2);

                // Merge 4MSB from file 1 with 4LSB from file 2
                pixel.blue = (pixel.blue & 0xF0) | (pixel2.blue >> 4);
                pixel.green = (pixel.green & 0xF0) | (pixel2.green >> 4);
                pixel.red = (pixel.red & 0xF0) | (pixel2.red >> 4);

                // Redirect the pointer back to the front of the pixel that is being modified
                fseek(file, -sizeof(pixel), SEEK_CUR);

                // Write the modified pixel back to the file
                fwrite(&pixel, sizeof(pixel), 1, file);
            }

            // Calculate how many padding each row needs
            int padding = calculatePaddings(dibheader.width);
            
            // Skip padding bytes
            fseek(file, padding, SEEK_CUR);
            fseek(file2, padding, SEEK_CUR);
        }
        
        fclose(file);
        fclose(file2);
    }

    else if (strcmp(argv[1], "--superhide") == 0) {
        FILE *file2 = NULL;
        BMPHeader bmpheader2;
        DIBHeader dibheader2;

        // Check if there's any error
        if (!vaidateHide(argc, argv, &file, &file2, &bmpheader, &bmpheader2, &dibheader, &dibheader2))
            return 1;

        unsigned char buffer[CHUNK_SIZE], buffer2[CHUNK_SIZE];
        size_t bytesRead, bytesRead2;
        long position = bmpheader.offset;

        fseek(file, position, SEEK_SET);

        while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, file)) > 0 &&
                (bytesRead2 = fread(buffer2, 1, CHUNK_SIZE, file2)) > 0) {

            // Loop through all pixels in the chunk
            for (size_t i = 0; i < bytesRead; i += 3) {
                // Process each pixel in the buffer
                buffer[i] = (buffer[i] & 0xF0) | (buffer2[i] >> 4);
                buffer[i+1] = (buffer[i+1] & 0xF0) | (buffer2[i+1] >> 4);
                buffer[i+2] = (buffer[i+2] & 0xF0) | (buffer2[i+2] >> 4);
            }

            // Write back the modified buffer
            fseek(file, position, SEEK_SET);
            fwrite(buffer, 1, bytesRead, file);
            position += bytesRead;
        }

        fclose(file);
        fclose(file2);
    }

    else {
        printf("ERROR: wrong arguments\n");
        return 1;
    }

    return 0;
}