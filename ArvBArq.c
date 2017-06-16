#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOME_MAX 25

int n_arq = 1;

void cria (int num, char *nomee)
{
    int ni  = 1;
    if (n_arq != 0)
    {
        char nome[NOME_MAX];
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

int retorna_filho(char* arq, int filho_n, char* strin)
{
    FILE *fp = fopen(arq, "rb");
    int num_filhos_total;
    fread(&num_filhos_total, sizeof(int),1,fp);
    if(filho_n > num_filhos_total) return 0;
    fseek(fp,sizeof(int)*num_filhos_total,SEEK_CUR);
    fseek(fp,sizeof(char)*NOME_MAX*filho_n,SEEK_CUR);
    char retorno[NOME_MAX];
    int leu = fread(retorno, sizeof(char),NOME_MAX,fp);
    fclose(fp);
    if(!leu) return 0;
    strcpy(strin, retorno);
    return 1;
}

void ler_arquivo(char* arq)
{
    FILE *fp = fopen(arq, "rb");
    int a;
    fread(&a, sizeof(int), 1, fp);
    printf("%d\n",a);
    int i = 1;
    while(i <= a)
    {
        fread(&a, sizeof(int), 1, fp);
        printf("%d\n",a);
    }
    char saidaaqui[25];
    int leu = 1;
    while(leu)
    {
        leu = retorna_filho(arq, i, saidaaqui);
        printf("%s\n",saidaaqui);
    }
}

void libera(char *arq){
    char nome_filho[NOME_MAX];
    int acao = retorna_filho(arq,0,nome_filho);
    int ind = 0;
    while(acao){
        libera(nome_filho);
        remove(nome_filho);
        ind++;
        acao = retorna_filho(arq,ind,nome_filho);
    }
    remove(arq);
}

void imprime(char *narq, int andar){
    FILE *fp = fopen(narq,"rb");
    if(!fp) return; //close?
    int i, j,nchaves, valor;
    char filho[NOME_MAX];
    fread(&nchaves,sizeof(int),1,fp);
    for(i=0; i < nchaves; i++){
        fclose(fp);
        retorna_filho(narq,i,filho);
        imprime(filho,andar+1);
        for(j=0; j<=andar; j++)
            printf("   ");
        fp = fopen(narq,"rb");
        fseek(fp,sizeof(int)*(i+1),SEEK_SET);
        fread(&valor,sizeof(int),1,fp);
        printf("%d\n", valor);
    }
    if(fp)
        fclose(fp);
    retorna_filho(narq,i,filho);
    imprime(filho,andar+1);
}
