/**
 * File from class repository
 * Author: Luiz Eduardo da Silva
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "imagelib.h"

/*-------------------------------------------------------------------------
 * Image allocation and free routines
 *   nl = number of rows
 *   nc = number of columns
 *-------------------------------------------------------------------------*/
image img_alloc(int nl, int nc)
{
    return (image)malloc(nl * nc * sizeof(int));
}

void img_free(image img)
{
    free(img);
}

/*-------------------------------------------------------------------------
 * Display image information
 *   name = file name image
 *   nr = number of rows
 *   nc = number os columns
 *   ml = max grayscale level
 *-------------------------------------------------------------------------*/
void img_info(char *name, int nr, int nc)
{
    printf("\nImage Informations:");
    printf("\n--------------------------\n");
    printf("Image file name.............: %s \n", name);
    printf("Number of rows..............: %d \n", nr);
    printf("Number of columns...........: %d \n\n", nc);
}

/*-------------------------------------------------------------------------
 * Error message
 *   str - string message control
 *   ... - parameters
 *-------------------------------------------------------------------------*/
void errormsg(char *str, ...)
{
    char format[255];
    va_list arg;
    va_start(arg, str);
    sprintf(format, "ERROR: %s\n\n", str);
    vprintf(format, arg);
    va_end(arg);
    exit(1);
}


//Codigo alterado, conformando pbm ao inves de pgm

int img_readpbm(char *nome, image *img, int *nl, int *nc)
{
    int i, j, k, limX, limY;
    char linha[100];
    FILE *arq;
    if ((arq = fopen(nome, "r")) == NULL)
    {
        printf("Erro na ABERTURA do arquivo <%s>\n\n", nome);
        return 0;
    }
    
    /*-- PBM = "P1" --------*/
    fgets(linha, 80, arq);
    if (!strstr(linha, "P1"))
    {
        printf("Erro no FORMATO do arquivo <%s>\n", nome);
        printf("%s\n\n", linha);
        return 0;
    }
    
    /*-- Dimensoes da imagem --*/
    //fgets(linha, 80, arq);
    do
    {
        fgets(linha, 80, arq);
    } while(strchr(linha, '#'));
    sscanf(linha, "%d %d", &limX, &limY);
    
    if (limX == 0 || limY == 0)
    {
        printf("Erro nas DIMENSOES do arquivo <%s>\n\n", nome);
        return 0;
    }
    
    if ((*img = img_alloc(limY, limX)) != NULL)
    {
        for (i = 0; i < limY; i++)
        {
            for (j = 0; j < limX; j++)
            {
                fscanf(arq, "%d", &k);
                if (k != 0 && k != 1)
                {
                    printf("Erro nos DADOS do arquivo <%s>\n\n", nome);
                    return 0;
                }
                *(*img + i * limX + j) = k;
            }
        }
        fclose(arq);
    }
    else
    {
        printf("Erro na ALOCACAO DE MEMORIA para o arquivo <%s>\n\n", nome);
        printf("Rotina img_readpbm\n\n");
        fclose(arq);
        return 0;
    }
    *nc = limX;
    *nl = limY;
    return 1;
}

void img_writepbm(image img, char *nome, int nl, int nc)
{
    int i, j, x, k, valores_por_linha;
    FILE *arq;
    if ((arq = fopen(nome, "w")) == NULL)
    {
        printf("Erro na CRIACAO do arquivo <%s>\n\n", nome);
    }
    else
    {
        fputs("P1\r\n", arq);
        fputs(CREATOR, arq);
        fprintf(arq, "%d %d\r\n", nc, nl);
        
        valores_por_linha = 36;
        for (i = 0, k = 0; i < nl; i++)
            for (j = 0; j < nc; j++)
            {
                x = *(img + i * nc + j);
                fprintf(arq, "%d ", x);
                k++;
                if (k > valores_por_linha)
                {
                    fprintf(arq, "\r\n");
                    k = 0;
                }
            }
    }
    fclose(arq);
}
