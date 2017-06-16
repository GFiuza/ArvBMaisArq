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

int retorna_filho(char* arq, int filho_n, char* strin){ //Testada. Está OK.
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

void ler_arquivo(char* arq){ //Testada. Está OK.
    FILE *fp = fopen(arq, "rb");
    int a, n;
    fread(&a, sizeof(int), 1, fp);
    printf("%d\n",a);
    int i = 1;
    while(i <= a)
    {
        fread(&n, sizeof(int), 1, fp);
        printf("%d\n",n);
        i++;
    }
    char saidaaqui[25];
    int leu = 1;
    i=0;
    leu = retorna_filho(arq, i, saidaaqui);
    while(leu)
    {
        printf("%s\n",saidaaqui);
        i++;
        leu = retorna_filho(arq, i, saidaaqui);
    }
}

void libera(char *arq){
    char nome_filho[NOME_MAX];
    int ind = 0, acao = retorna_filho(arq,ind,nome_filho);
    while(acao){
        libera(nome_filho);
        remove(nome_filho);
        ind++;
        acao = retorna_filho(arq,ind,nome_filho);
    }
    remove(arq);
}

void imprime(char *narq, int andar){ //Testada. Está OK.
    FILE *fp = fopen(narq,"rb");
    if(!fp) return; //close?
    int i, j,nchaves, valor;
    char filho[NOME_MAX];
    fread(&nchaves,sizeof(int),1,fp);
    for(i=0; i < nchaves; i++){
        fclose(fp);
        if (retorna_filho(narq,i,filho))
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
    if (retorna_filho(narq,i,filho))
        imprime(filho,andar+1);
}

int busca(char* narq, int ch, char* resp){ //Testada. Está OK.
    FILE *f = fopen(narq, "rb");
    if (!f) return 0;
    int nchaves, filho = 0, n, i;
    fread(&nchaves, sizeof(int), 1, f);
    for (i = 1; i <= nchaves; i++){
        fread(&n, sizeof(int), 1, f);
        if (n < ch) filho++;
        else if (n > ch) break;
        else{
            fclose(f);
            strcpy(resp, narq);
            return 1;
        }
    }
    if (i > nchaves) return 0;
    fseek(f, ((nchaves-i)*sizeof(int) + (filho*sizeof(char)*NOME_MAX)), SEEK_CUR);
    char temp[NOME_MAX];
    fread(temp, sizeof(char), NOME_MAX, f);
    return busca(temp, ch, resp);
}

int main(){
    //Pequeno teste que eu fiz
    char nome[] = "teste.dat", nome2[] = "teste2.dat", nome3[] = "teste3.dat", r[NOME_MAX];
    FILE *f = fopen(nome, "wb");
    if (!f) return -1;
    int teste[] = {2, 3, 6};
    fwrite(teste, sizeof(int), 3, f);
    fwrite(nome2, sizeof(char), 25, f);
    fwrite(nome3, sizeof(char), 25, f);
    fclose(f);
    f = fopen(nome2, "wb");
    if (!f) return -1;
    int teste2[] = {2, 1, 2};
    fwrite(teste2, sizeof(int), 3, f);
    fclose(f);
    f = fopen(nome3, "wb");
    if (!f) return -1;
    int teste3[] = {2, 4, 5};
    fwrite(teste3, sizeof(int), 3, f);
    fclose(f);
    imprime(nome, 0);
    if (busca(nome, 5, r)){
        printf("numero encontrado no arquivo %s\n", r);
    }
    else{
        printf("numero nao encontrado na arvore");
    }
    ler_arquivo(nome);
    libera(nome);
    return 0;
}
