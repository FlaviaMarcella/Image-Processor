/*-------------------------------------------------------------------------
 * Histograma
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/imagelib.h"


// this filter permissive to high frequencies, it is used to enhance the edges of the image
image filtro_passaAlta(image img)
{
    float w[3][3] = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};
    image out =  img_clone(img);
    int nc = img->nc; // largura da imagem
    int nl = img->nr; // altura da imagem
    int max =  img->ml; 
    int *pixels = out->px;
    for (int i = 1; i < nl - 1; i++){
        for (int j = 1; j < nc - 1; j++){
            int sum = 0;
            for (int y = -1; y <= 1; y++){
                for (int x = -1; x <= 1; x++){
                    sum = sum + w[y + 1][x + 1] * img->px[(i + y) * nc + (j + x)]; //formula de convolução: g(x,y) = w * f(x,y) = somatorio de w(i,j) * f(x+i, y+j)
                }
            }
            sum = sum < 0 ? -1 * sum : sum;
            sum = sum > max ? max : sum;
            out->px[i * nc + j] = sum;
        }
    }
    return out;
}

// this filter permissive to low frequencies, it is used to blur the image and remove noise
image filtro_passaBaixa(image img)
{
    float w[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    image out =  img_clone(img);
    int nc = img->nc; // largura da imagem
    int nl = img->nr; // altura da imagem
    int max =  img->ml; 
    int *pixels = out->px;
    for (int i = 1; i < nl - 1; i++){
        for (int j = 1; j < nc - 1; j++){
            int sum = 0;
            for (int y = -1; y <= 1; y++){
                for (int x = -1; x <= 1; x++){
                    sum = sum + w[y + 1][x + 1] * img->px[(i + y) * nc + (j + x)]; //formula de convolução: g(x,y) = w * f(x,y) = somatorio de w(i,j) * f(x+i, y+j)
                }
            }
            out->px[i * nc + j] = sum/9;
        }
    }
    return out;
}

image filtro_sobel_misto(image img)
{
    float w[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    image out =  img_clone(img);
    int nc = img->nc; // largura da imagem
    int nl = img->nr; // altura da imagem
    int max =  img->ml; 
    int *pixels = out->px;
    for (int i = 1; i < nl - 1; i++){
        for (int j = 1; j < nc - 1; j++){
            
            int z1 = img->px[(i - 1) * nc + (j - 1)];
            int z2 = img->px[(i - 1) * nc + (j)];
            int z3 = img->px[(i - 1) * nc + (j + 1)];
            int z4 = img->px[(i) * nc + (j - 1)];
            int z5 = img->px[(i) * nc + (j)];
            int z6 = img->px[(i) * nc + (j + 1)];
            int z7 = img->px[(i + 1) * nc + (j - 1)];
            int z8 = img->px[(i + 1) * nc + (j)];
            int z9 = img->px[(i + 1) * nc + (j + 1)];

            int sum = abs((z7 + 2 * z8 + z9) - (z1 + 2 * z2 + z3)) + abs((z3 + 2 * z6 + z9) - (z1 + 2 * z4 + z7)); // formula do filtro de sobel misto  
            sum = sum > max ? max : sum;

            // out->px[i * nc + j] = sum;

            // Para fazer o negativo da imagem, basta subtrair o valor do pixel do valor máximo da imagem (255 para imagens de 8 bits)
            out->px[i * nc + j] = max - sum;
        }
    }
    return out;
}

image filtro(image img, float w[3][3])
{
    image out =  img_clone(img);
    int nc = img->nc; // largura da imagem
    int nl = img->nr; // altura da imagem
    int max =  img->ml; 
    int *pixels = out->px;
    for (int i = 1; i < nl - 1; i++){
        for (int j = 1; j < nc - 1; j++){
            int sum = 0;
            for (int y = -1; y <= 1; y++){
                for (int x = -1; x <= 1; x++){
                    sum = sum + w[y + 1][x + 1] * img->px[(i + y) * nc + (j + x)]; //formula de convolução: g(x,y) = w * f(x,y) = somatorio de w(i,j) * f(x+i, y+j)
                }
            }
            sum = sum < 0 ? -1 * sum : sum;
            sum = sum > max ? max : sum;
            out->px[i * nc + j] = sum;
        }
    }
    return out;
}

void msg(char *s)
{
    printf("\nFilter Image");
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

    // float w[3][3] = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}}; // matriz de convolução para filtro passa alta  
    // float w[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}; // matriz de convolução para filtro passa baixa
    // float w[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}; // sobel filter - horizontal
    // float w[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}}; // sobel filter - vertical

    image In, Out;
    if (argc < 2)
        msg(argv[0]);

    img_name(argv[1], nameIn, nameOut, GRAY, GRAY);

    //-- read image
    In = img_get(nameIn, GRAY);
    //-- transformation

        // Out = filtro_passaAlta(In);
        // Out = filtro_passaBaixa(In);
        // Out = filtro(In, w);
        Out = filtro_sobel_misto(In); // w é a matriz de convolução, pode ser usada para criar filtros personalizados
    
    //-- save image
    img_put(Out, nameOut, GRAY);

    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
