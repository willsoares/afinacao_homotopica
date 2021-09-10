/**
 * File from class repository
 * Author: Luiz Eduardo da Silva
 */ 

#define CREATOR "# CREATOR: Image Processing usind C-Ansi - ByDu\n"

typedef int *image;

#define ERROR(x, y) \
    if (x)          \
    y

#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#define VIEW "./i_view32"
#elif defined(__linux__)
#define VIEW "eog"
#endif

void errormsg(char *, ...);
image img_alloc(int, int);
void img_free(image);
void img_info(char *, int, int);
int img_readpbm(char *, image *, int *, int *);
void img_writepbm(image, char *, int, int);