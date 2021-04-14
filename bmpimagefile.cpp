#include <bits/stdc++.h>
#include <stdlib.h>
#include "bmpimagefile.h"

using namespace std;

static int validate_BMPHeader(BMPImgHeader * header);
static void print_BMPHeader(BMPImgHeader * header);

/**
 *
 * Loads an BMP image file, returning the Image structure containing pixels
 *
**/
Image * read_BMPImage(FILE * fptr) {

    int error = 0;
    BMPImgHeader header;
    //fread(&header, sizeof(BMPHeader), 1, fptr);

    // Space allocation for new BMPImage
    Image * image = (Image *)malloc(sizeof(Image));
    if(image == NULL) {
	    fprintf(stderr, "Error: failed to allocate space for writing\n");
	    return image;
	}
    // Get BMP Image Header
    if( fread(&(image->header), sizeof(image->header), 1, fptr) != 1 )
    {
        fprintf(stderr, "Header not readable.\n");
        if(image) {
            free_BMPImage(image);
            image = NULL;
            fclose(fptr);
        }
        return image;
    }

    // Assign BMP Image Header
    header = image->header;
    //Print the Header Info
    print_BMPHeader(&header);
    //Validating headers and assumption
    error = validate_BMPHeader(&header);

    if(!error)
    {
        fprintf(stderr, "Headers are invalid in file. \n");
        if(image) {
            free_BMPImage(image);
            image = NULL;
            fclose(fptr);
        }
        return image;
    }

    //Set the normalized height
    image->height = header.fheight<0 ? -header.fheight : header.fheight;

    try
    {
        // Space allocate for image data
        image -> pixels = (Pixel **)malloc(sizeof(Pixel *) * image->height);

        for(int i=0; i<image->height; i++)
        {
            (image -> pixels)[i] = (Pixel *)malloc(sizeof(Pixel)*header.fwidth);
            fread((image->pixels)[i], sizeof(Pixel), header.fwidth, fptr);
        }
    }
    catch(int i)
    {
        fprintf(stderr, "Index %d is out of range while reading the file\n", i);
        if(image) {
            free_BMPImage(image);
            image = NULL;
            fclose(fptr);
        }
        return image;
    }
    catch(std::bad_alloc & exception)
    {
        std::cerr << "bad_alloc identified: " << exception.what();
        if(image) {
            free_BMPImage(image);
            image = NULL;
            fclose(fptr);
        }
        return image;
    }
    catch (char * str)
    {
        fprintf(stderr, "Exception : %s\n", str);
        if(image) {
            free_BMPImage(image);
            image = NULL;
            fclose(fptr);
        }
        return image;
    }

    return image;
}

/**
 * Validating subset of BMP header info.
 */
static int validate_BMPHeader(BMPImgHeader * header)
{
    // Ensuring File is BMP type
    if (header->ftype != BMP_MAGIC_ID)
    {
        fprintf(stderr, "Please check file type as it should be BMP file. \n");
        return FALSE;
    }

    // Assuming there is no compression
    if (header->fcompression != 0)
    {
        fprintf(stderr, "Please ensure there is no compression. \n");
        return FALSE;
    }
    // Ensuring bits per pixel are 24
    if (header->fbits != 24)
    {
        fprintf(stderr, "Please check bpp as assumed it should be 24. \n");
        return FALSE;
    }

    return TRUE;
}

// print_BMPHeader - Prints the Header Info

static void print_BMPHeader(BMPImgHeader * header)
{
    printf("BMP File Header information:\n");
    printf("Type of File (should be %x): %x\n", BMP_MAGIC_ID, header->ftype);
    printf("Size of the File: %d\n", header->fsize);
    printf("Image Data Offset: %d\n", header->foffset);
    printf("Size of Headers: %d\n", header->fheader_size);
    printf("Image Width: %d\n", header->fwidth);
    printf("Image Height: %d\n", header->fheight);
    printf("Num of Color Planes: %d\n", header->fplanes);
    printf("Bits Per Pixel: %d\n", header->fbits);
    printf("Compression Type: %d\n", header->fcompression);
}

/**
 * Free memory allocated to the Pixel structure and image structure
 *
 * read_BMPImage(...) (above) allocates memory for pixel and the image structure.
 * This functions task is to clean up any allocated resources.
**/
void free_BMPImage(Image * image) {

    //Clean Each Pixel row allocated Memory
    for(int i=0; i<image->height; i++)
    {
        free((image->pixels)[i]);
    }

    //Free Pixel allocated memory
    free(image->pixels);
    //Free up memory provided to image
    free(image);
}

/**
 *
 * Save the image data to the passed final file.
 *
**/
void write_BMPImage(Image * image, FILE * fptr) {

    // First write Header Info for the Image
    if( fwrite(&(image->header), sizeof(image->header), 1, fptr) != 1 )
    {
        fprintf(stderr, "Header not written.\n");
        if(image) {
            free_BMPImage(image);
            image = NULL;
            fclose(fptr);
        }
        return;
    }

    // Now write The Pixels info for the Image
    try
    {
        for(int i=0; i<image->height; i++) {
            Pixel * row = (image->pixels)[i];
            fwrite(row, sizeof(Pixel), (image->header).fwidth, fptr);
        }
    }
    catch(int i)
    {
        fprintf(stderr, "Index %d is out of range while writing the file\n", i);
        return;
    }
    catch (char * str)
    {
        fprintf(stderr, "Exception : %s\n", str);
        return;
    }

}
