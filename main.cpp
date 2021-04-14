#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#include <bits/stdc++.h>
#include "bmpimagefile.h"
using namespace std;

int main(int argc, char ** argv) {
    if(argc == 2 && strcmp("--help", argv[1]) == 0 ){
        cout<<"--help \t\t\t Display this information"<<endl;
        cout<<"--command \t\t See the command to execute the file"<<endl;
        cout<<"--error-help \t\t Display information related to different types of errors"<<endl;
        return 0;
    }
    else if(argc == 2 && strcmp("--error-help", argv[1]) == 0 ){
        cout<<"bad-alloc \t\t\t Please follow this link for more information about memory allocation issue:  \
        \t\t\t\t http://www.cplusplus.com/reference/new/bad_alloc/"<<endl;
        cout<<"out-of-bound \t\t\t Please follow this link for more information index out of bound issue:  \
        \t\t\t\t http://cplusplus.com/forum/beginner/248886/"<<endl;
        cout<<"iostate-file \t\t\t Please follow this link for more information about file open,read and write related  \
        \t\t\t\t issues and exceptions: http://www.cplusplus.com/reference/ios/ios/exceptions/"<<endl;
        cout<<"image-header-error \t\t Please follow this link for more information about BMP File Headers:  \
        \t\t\t\t\t https://en.wikipedia.org/wiki/BMP_file_format \t\t \
        \t\t\t\t\t\t https://stackoverflow.com/questions/22112412/reading-bmp-file-c-trouble-with-reading-bmp-header \
        "<<endl;
        return 0;
    }
    else if(argc == 2 && strcmp("--command", argv[1]) == 0){
        cout<<"Command to run the program is  : .\\main.exe <image_name.bmp> <result_image_name.bmp>"<<endl;
    }
    else if (argc != 3) {
        fprintf(stderr, "Usage ./validate inputFile.bmp outputFile.bmp\n");
        return EXIT_FAILURE;
    }

    FILE * inputFile = fopen(argv[1], "rb");
    char * outputFile = argv[2];

    if (!inputFile )
    {
        printf("Issue with opening the file. Could not open image files!\n");
        return EXIT_SUCCESS;
    }

    Image * srcImage = read_BMPImage(inputFile);
    fclose(inputFile);

    if(srcImage == NULL)
    {
        return EXIT_SUCCESS;
    }
    //negate the image
    try
    {
        for(int i = 0; i < srcImage -> height; i++) {
            for(int j = 0; j < (srcImage -> header).fwidth; j++)
            {
                (srcImage -> pixels)[i][j].blue = (unsigned char)(255-int((srcImage -> pixels)[i][j].blue));
                (srcImage -> pixels)[i][j].green = (unsigned char)(255-int((srcImage -> pixels)[i][j].green));
                (srcImage -> pixels)[i][j].red = (unsigned char)(255-int((srcImage -> pixels)[i][j].red));
                (srcImage -> pixels)[i][j].alpha = (unsigned char)(255-int((srcImage -> pixels)[i][j].alpha));
            }
        }
    }
    catch(int i)
    {
        printf("Exception : Index %d is out of range\n", i);
        return EXIT_SUCCESS;
    }
    catch (char * str)
    {
        printf("Exception : %s\n", str);
        return EXIT_SUCCESS;
    }

    FILE * fptr = fopen(outputFile, "wb");
    write_BMPImage(srcImage, fptr);
    fclose(fptr);

    cout<<"File is updated."<<endl;
    return EXIT_SUCCESS;
}
