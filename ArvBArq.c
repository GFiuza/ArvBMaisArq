#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n_arq = 0;

void cria (int num, char *nomee)
{
    int ni  = 1;
    char valor[20];
    sprintf(valor,"%d", num);
    if (n_arq != 0)
    {
        char nome[25];
        sprintf(nome,"%d" , n_arq);
        strcat(nome,".dat");
        strcpy(nomee,nome);
        FILE *fp = fopen(nome,"wb");
        fwrite(&ni,sizeof(int),1,fp);
        fwrite(&num,sizeof(int),1,fp);
        n_arq++;
        fclose(fp);
        return;
    }
    FILE *fp = fopen("raiz.dat", "wb");
    fwrite(&ni,sizeof(int),1,fp);
    fwrite(&num,sizeof(int),1,fp);
    n_arq++;
    fclose(fp);
    strcpy(nomee,"raiz.dat");
}
