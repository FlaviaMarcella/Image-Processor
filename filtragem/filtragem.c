/*-------------------------------------------------------------------------
 * Histograma
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/imagelib.h"

image filtro(image In)
{
    float w[3][3] = {{0, 1, 0}, {1, 4, 1}, {0, 1, 0}};
    image out =  img_clone(img);
    int nc = img->nc;
    int nl = img->nl;
    int max =  img->ml;
    int *pixels = out->px;
    for (int i = 0; i < nl; i++){
        for (int j = 0; j < nc; j++){
            int sum = 0;
            for (int y = -1; y <= 1; y++){
                for (int x = -1; x <= 1; x++){
                    sum = sum + w[y + 1][x + 1] * img->px[(i + y) * nc + (j + x)];
                }
            }
            sum = sum < 0 ? -1 * sum : sum;
            sum = sum > max ? max : sum;
            out->px[i * nc + j] = sum;
        }
        return out;
    }
}

void msg(char *s)
{
    printf("\n Filtragem de Imagens");
    printf("\n-------------------------------");
    printf("\nUso:  %s  nome-imagem[.pgm] \n\n", s);
    printf("    nome-imagem[.pgm] é o nome do arquivo da imagem \n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    char nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    if (argc < 2)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

    //-- read image
    In = img_get(nameIn, GRAY);
    //-- transformation
    Out = equaliza(In);
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
