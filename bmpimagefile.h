#ifndef BMPIMAGEFILE_H_INCLUDED
#define BMPIMAGEFILE_H_INCLUDED

#include <iostream>
#include <stdint.h>

#define BMP_MAGIC_ID 0x4d42
#define TRUE 1
#define FALSE 0

#pragma pack(push)
#pragma pack(1)

typedef struct BMPHeader {
    uint16_t ftype;               // Magic ID
    uint32_t fsize;               // Size of file in bytes
    uint16_t freserved1;          // Not used
    uint16_t freserved2;          // Not used
    uint32_t foffset;             // Image Data offset in bytes
    uint32_t fheader_size;        // Size of header in bytes
    int32_t  fwidth;              // Image Width
    int32_t  fheight;             // Image Height
    uint16_t fplanes;             // No. of color plane
    uint16_t fbits;               // BPP - Bits per pixel
    uint32_t fcompression;        // Compression type
    uint32_t fimagesize;          // size of the image in bytes
    int32_t  fxresolution;        // X Pixels per meter
    int32_t  fyresolution;        // Y Pixels per meter
    uint32_t fncolors;            // Num of colors
    uint32_t fimpcolors;          // Important colors
} BMPImgHeader;

typedef struct pixel_struct {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
	uint8_t alpha;
} Pixel;

typedef struct BMP_Image_struct {
	BMPImgHeader header;
	Pixel * * pixels;
	int height; // to incorporate the negative axis as height in image - normalized height
} Image;

#pragma pack(pop)

Image * read_BMPImage(FILE * fptr);
void free_BMPImage(Image * image);
void write_BMPImage(Image * image, FILE * fptr);

#endif // BMPIMAGEFILE_H_INCLUDED
