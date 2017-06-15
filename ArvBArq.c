#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOME_MAX 25

int n_arq = 1;

void cria (int num, char *nomee)
{
    int ni  = 1;
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

int retorna_filho(char* arq, int filho_n, char* strin)
{
    FILE *fp = fopen(arq, "rb");
    int num_filhos_total;
    fread(&num_filhos_total, sizeof(int),1,fp);
    if(filho_n > num_filhos_total) return 0;
    fseek(fp,sizeof(int)*num_filhos_total,SEEK_CUR);
    fseek(fp,sizeof(char)*25*filho_n,SEEK_CUR);
    char retorno[25];
    int leu = fread(retorno, sizeof(char)*25,1,fp);
    fclose(fp);
    if(!leu) return 0;
    strcpy(strin, retorno);
    return 1;
}

void libera(char *arq){
    FILE *fp = fopen(arq,"rb+");
    int nchaves;
    fread(&nchaves,sizeof(int),1,fp);
    fseek(fp,nchaves,SEEK_CUR);
    int no_interno;
    char nome[NOME_MAX];
    while(1){
        no_interno = fread(&nome,sizeof(char),NOME_MAX,fp);
        if(!no_interno) break;
        libera(nome);
    }
    fclose(fp);
    remove(arq);
}
