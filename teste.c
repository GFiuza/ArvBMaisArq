#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_MAX 25

int retorna_filho(char* arq, int filho_n, char* strin){ //Testada. Está OK.
    /*esta função retorna se um certo filho_n do nó dado existe
     *caso exista, retorna 1, e modifica strin para ter o nome deste filho
     *caso não exista, apenas retorna 0
     *filho n precisa ser um valor de 0 até o fator de ramificação, caso contrário irá retornar 0 sempre
     *função foi feita para contornar o problema de um certo número máximo de arquivos aberto ao mesmo tempo
     *desta forma é possível fazer chamadas recursivas sem abrir arquivos a cada chamada
     */
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
    //imprime as chaves e seus filhos de um arquivo
    puts(arq);
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
    fclose(fp);
    printf("\n");
}

int main(){
    ler_arquivo("1.dat");
    ler_arquivo("2.dat");
    ler_arquivo("3.dat");
    //ler_arquivo("4.dat");
    return 0;
}
