#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n_arq = 0;
int const NOME_MAX = 25;

char* cria (int num)
{
    int ni  = 1;
    char valor[20];
    sprintf(valor,"%d", num);
    if (n_arq != 0)
    {
        char nome[NOME_MAX];
        sprintf(nome,"%d" , n_arq);
        strcat(nome,".dat");
        FILE *fp = fopen(nome,"wb");
        fwrite(&ni,sizeof(int),1,fp);
        fwrite(&num,sizeof(int),1,fp);
        n_arq++;
        fclose(fp);
        return &nome;
    }
    FILE *fp = fopen("raiz.dat", "wb");
    fwrite(&ni,sizeof(int),1,fp);
    fwrite(&num,sizeof(int),1,fp);
    n_arq++;
    fclose(fp);
    return "raiz.dat";
}
