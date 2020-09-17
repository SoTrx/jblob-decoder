#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Reading the file 4 bytes at a time
#define BUFFER_SIZE 4
#define TRUE 1
#define FALSE 0
// Maximum allowed size of a string (prevent from using malloc)
#define STR_SIZE 1000
// "-XXXXXXXX.jpg" length. Assume no pdf will contain more than 8-digits pages.
// Should be more than enough for any sane PDF 
#define MAX_PREFIX_LENGTH 12

/**
 * Check if the provided buffer is a JPEG (JFIF) header
 **/
char isJPEGHeader(unsigned char *buffer);

/**
 * Open the path with the provided accessMode, kill the program
 * if it fails
 **/
static inline FILE * sFOpen(unsigned char *name, const char * accessMode);

int main(int argc, char *argv[]){
    if(argc < 2) {
        fprintf(stderr, "No file argument provided\n");
        return EXIT_FAILURE;
    }
    // Only using static allocation
    if (strlen(argv[1]) >= STR_SIZE - MAX_PREFIX_LENGTH){
        fprintf(stderr, "Filename too long");
        return EXIT_FAILURE;
    }
    //Input
    FILE *f = sFOpen(argv[1], "rb");

    //Output
    char name[STR_SIZE] = {0};
    char returnCode = EXIT_SUCCESS;
    int fileNumber = 0;
    FILE *fOut = NULL;
    unsigned char buffer[BUFFER_SIZE] = {0};
    
    while(!feof(f)){
        fread(buffer, BUFFER_SIZE, 1, f);
        if(isJPEGHeader(buffer)){
            if(fOut) fclose(fOut);
            fileNumber++;
            snprintf(name, sizeof(name), "%s%s%d%s", argv[1], "-", fileNumber, ".jpg");
            fOut = sFOpen(name, "wb");
        }
        // If not ouput file is open, no JPEG Header have been found 
        // So better avoid writing garbage and exit the program
        if(fOut == NULL) {
            fprintf(stderr, "Input file doesn't contains any JPEG Data !");
            returnCode = EXIT_FAILURE;
            break;
        }

        fwrite(buffer, BUFFER_SIZE, 1, fOut);
    }
    fclose(f);
    fclose(fOut);
    return returnCode;
}

char isJPEGHeader(unsigned char *buffer){
    unsigned char JPEG_HEADER[BUFFER_SIZE] = {0xFF, 0xD8, 0xFF, 0xE0};
    for (unsigned char i = 0; i < BUFFER_SIZE; i++){
        if (buffer[i] != JPEG_HEADER[i]) return FALSE;
    }
    return TRUE;
}

static inline FILE * sFOpen(unsigned char *name, const char * accessMode){
    FILE *f = fopen(name, accessMode);
    if(ferror(f)) {
        perror("Error while opening file");
        exit(EXIT_FAILURE);
    }
    return f;
}