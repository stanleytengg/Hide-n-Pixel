#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define CHUNK_SIZE 4096

#pragma pack(1)
typedef struct {
    uint16_t identifier;
    uint32_t size;
    uint16_t reserve1;
    uint16_t reserve2;
    uint32_t offset;
} BMPHeader;

typedef struct {
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t color_planes;
    uint16_t num_bit;
    uint32_t compression;
    uint32_t image_size;
    uint32_t hresolution;
    uint32_t vresolution;
    uint32_t colors_palette;
    uint32_t num_imp_color;
} DIBHeader;

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;
#pragma pack(4)

int validate(int argc, char *argv[], FILE **file, BMPHeader *bmpheader, DIBHeader *dibheader);

int vaidateHide(int argc, char *argv[], FILE **file, FILE **file2, BMPHeader *bmpheader, BMPHeader *bmpheader2, DIBHeader *dibheader, DIBHeader *dibheader2);

int validateBMPHeader(FILE *file, BMPHeader *bmpheader);

int validateDIBHeader(FILE *file, DIBHeader *dibheader);

const char* identifierToString(uint16_t identifier);

int calculatePaddings(int width);

#endif