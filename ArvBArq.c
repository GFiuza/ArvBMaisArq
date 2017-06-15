#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n_arq = 0;

char* cria (int num)
{
    char ni  = '1';
    char pulaae = '\n';
    char valor[20];
    sprintf(valor,"%d", num);
    if (n_arq != 0)
    {
        char nome[25];
        sprintf(nome,"%d" , n_arq);
        strcat(nome,".dat");
        FILE *fp = fopen(nome,"w");
        fwrite(&ni,sizeof(char),1,fp);
        fwrite(&pulaae,sizeof(char),1,fp);
        char i;
        int j = 0;
        for(i = valor[j]; i != '\0'; i = valor[j])
        {
            fwrite(&i, sizeof(char), 1, fp);
            j++;
        }
        n_arq++;
        return nome;
    }
    FILE *fp = fopen("raiz.dat", "w");
    fwrite(&ni,sizeof(char),1,fp);
    fwrite(&pulaae,sizeof(char),1,fp);
    char i;
    int j = 0;
    for(i = valor[j]; i != '\0'; i = valor[j])
    {
        fwrite(&i, sizeof(char), 1, fp);
        j++;
    }
    n_arq++;
    return "raiz.dat";
}
