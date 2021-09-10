/*------------------------------------------------------------
 *           UNIFAL - Universidade Federal de Alfenas.
 *             BACHARELADO EM CIENCIA DA COMPUTACAO.
 * Trabalho..: Afinamento homotópico
 * Disciplina: PROCESSAMENTO DE IMAGENS
 * Professor.: Luiz Eduardo da Silva
 * Aluno.....: Willian de Souza Soares
 * Data......: 09/09/2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include "imagelib.h"

char masks[2][2][9] =
    {
        2, 0, 2,
        1, 1, 1,
        2, 1, 2,

        2, 0, 0,
        1, 1, 2,
        2, 1, 2,

        0, 0, 0,
        2, 1, 2,
        1, 1, 1,

        2, 0, 2,
        1, 1, 1,
        2, 1, 2};

char family[8][9];


bool can_thin(image img, int nl, int nc, int y, int x, char m[9], int *max)
{
    int mi, ma, value;
    mi = 255;
    ma = 0;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (y + i - 1 > nl - 1 || y + i - 1 < 0 ||
                x + j - 1 > nc - 1 || x + j - 1 < 0)
                value = 0;
            else
                value = *(img + (y + i - 1) * nc + x + j - 1);
            if (m[i * 3 + j] == 0 && value > ma)
                ma = value;
            if (m[i * 3 + j] == 1 && value < mi)
                mi = value;
        }
    }
    value = *(img + y * nc + x);
    if (value > ma && value <= mi)
    {
        *max = ma;
        return true;
    }
    return false;
}

void turn_to_right(char arr[9], int n)
{
    char c;
    for (int i = 0; i < n; i++)
    {
        c = arr[0];
        arr[0] = arr[3];
        arr[3] = arr[6];
        arr[6] = arr[7];
        arr[7] = arr[8];
        arr[8] = arr[5];
        arr[5] = arr[2];
        arr[2] = arr[1];
        arr[1] = c;
    }
}

void generate_mask_family(int mask_number)
{
    char mask1[9], mask2[9];
    for (int i = 0; i < 9; i++)
    {
        mask1[i] = masks[mask_number][0][i];
        mask2[i] = masks[mask_number][1][i];
    }
    for (int m = 0; m < 8; m++)
    {
        if (m % 2)
        {
            for (int i = 0; i < 9; i++)
                family[m][i] = mask2[i];
            turn_to_right(mask2, 2);
        }
        else
        {
            for (int i = 0; i < 9; i++)
                family[m][i] = mask1[i];
            turn_to_right(mask1, 2);
        }
    }
}


void thinning(image *In, int nl, int nc, int qtd_masks)
{
    bool end, has_thinned;
    int value;
    image img = img_alloc(nl, nc);
    generate_mask_family(qtd_masks);

    for (int i = 0; i < nl; i++)
        for (int j = 0; j < nc; j++)
            *(img + i * nc + j) = *(*In + i * nc + j);
    do
    {
        end = true;
        for (int m = 0; m < 8; m++)
        {
            has_thinned = false;
            for (int i = 1; i < nl - 1; i++)
                for (int j = 1; j < nc - 1; j++)
                {
                    if (can_thin(*In, nl, nc, i, j, family[m], &value))
                    {
                        *(img + i * nc + j) = value;
                        has_thinned = true;
                        end = false;
                    }
                }
            if (has_thinned)
                for (int i = 0; i < nl; i++)
                    for (int j = 0; j < nc; j++)
                        *(*In + i * nc + j) = *(img + i * nc + j);
        }
    } while (!end);
    img_free(img);
}

void msg(char *s)
{
    printf("\nHomotopic Thinning");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  <IMG.PGM>\n\n", s);
    printf("    <IMG.PGM> is pgm image file \n\n");
    exit(1);
}


char *remove_ext(char* myStr) {
    char *retStr;
    char *lastExt;
    if (myStr == NULL) return NULL;
    if ((retStr = malloc (strlen (myStr) + 1)) == NULL) return NULL;
    strcpy (retStr, myStr);
    lastExt = strrchr (retStr, '.');
    if (lastExt != NULL)
        *lastExt = '\0';
    return retStr;
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    clock_t start, finish;
    int OK, nc, nl;
    char *filename, *command;
    image In;

    OK = img_readpbm(argv[1], &In, &nl, &nc);
    if (OK)
    {
        printf("\nHomotopic Thinning");
        img_info(argv[1], nl, nc);
        start = clock();
        thinning(&In, nl, nc, 0); // Mask M
        thinning(&In, nl, nc, 1); // Mask L
        finish = clock();
        float diff = (((float)finish - (float)start) / CLOCKS_PER_SEC);

        printf("\nRun time: %fs", diff);
        
        filename = remove_ext(argv[1]);
        sprintf(filename, "%s-%s", filename, "thin.pbm");
        img_writepbm(In, filename, nl, nc);
        sprintf(command, "%s %s &", VIEW, filename);
        system(command);
        img_free(In);
    }

    return 0;
}
