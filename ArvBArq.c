#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOME_MAX 25

int n_arq = 1;

typedef struct no{
    // definição da estrutura responsável por capturar um nó da arvore
    int nchaves;
    int *chave;
    char **filho;
    char nomearq[NOME_MAX];
}TARV;

TARV *inicializa(int t){
    // responsável por iniciar um nó vazio
    TARV *novo = (TARV*)malloc(sizeof(TARV));
    novo->nchaves = 0;
    novo->chave = (int*)malloc(sizeof(int*)*(2*t-1));
    novo->filho = (char**)malloc(sizeof(char*)*2*t);
    int i;
    for(i=0; i < (2*t); i++){
        novo->filho[i] = (char*)malloc(sizeof(char)*NOME_MAX);
        novo->filho[i] = NULL;
    }
    return novo;
}

void libera_no(TARV *no, int t){
    free(no->chave);
    int i;
    for(i=0; i < (2*t); i++)
        free(no->filho[i]);
    free(no->filho);
    free(no);
}

TARV *ler_mp(char *arq, int t){
    /*
        função responsável por ler um nó para MP
        retorna o no com os dados do arq
    */
    FILE *fp = fopen(arq,"rb");
    if(!fp){printf("OPA\n"); exit(1);}// ??
    int tam;
    fseek(fp,0L,SEEK_END);
    tam = ftell(fp);
    fseek(fp,0L,SEEK_SET);
    TARV *novo = (TARV*)malloc(sizeof(TARV));
    fread(&novo->nchaves,sizeof(int),1,fp);
    novo->chave = (int*)malloc(sizeof(int*)*(2*t-1));
    novo->filho = (char**)malloc(sizeof(char*)*2*t);
    int i, chave;
    for(i=0; i < novo->nchaves; i++)
        fread(&novo->chave[i],sizeof(int),1,fp);
    char nome[NOME_MAX];
    for(i=0; i < (2*t); i++){
        novo->filho[i] = (char*)malloc(sizeof(char)*NOME_MAX);
        if(ftell(fp)!=tam){ //verifica se é possivel ler um filho
            fread(&nome,sizeof(char),NOME_MAX,fp);
            strcpy(novo->filho[i],nome);
        }
        else novo->filho[i] = NULL;
    }
    fclose(fp);
    strcpy(novo->nomearq,arq);
    return novo;
}

void cria (int num, char *nomee)
{
    /*Esta função cria um arquivo de nome "n_arq.dat" com uma única chave dentro dele (a chave "num").
     *formato do arquivo criado:
    +-------+
    |1      |
    |num    |
    +-------+
     *onde 1 é o número de chave do arquivo
     *num é a chave criada
     */
    int ni  = 1;
    //if (n_arq != 0)
    //{
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
    //}
    //esta parte do código cria o arquivo chamado "raiz.dat", mas resolvemos não utilizar esta parte
    /*FILE *fp = fopen("raiz.dat", "wb");
    fwrite(&ni,sizeof(int),1,fp);
    fwrite(&num,sizeof(int),1,fp);
    n_arq++;
    fclose(fp);
    strcpy(nomee,"raiz.dat");*/
}

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
}

void libera(char *arq){
    //exclui o arquivo arq e todos os seus filhos (works better on Linux)
    char nome_filho[NOME_MAX];
    int ind = 0, acao = retorna_filho(arq,ind,nome_filho);
    while(acao){
        libera(nome_filho);
        ind++;
        acao = retorna_filho(arq,ind,nome_filho);
    }
    remove(arq);
}

void imprime(char *narq, int andar){ //Testada. Está OK.
    FILE *fp = fopen(narq,"rb");
    if(!fp) return;
    int i, j,nchaves, valor;
    char filho[NOME_MAX];
    fread(&nchaves,sizeof(int),1,fp);
    for(i=0; i < nchaves; i++){
        if (retorna_filho(narq,i,filho))
        {
            fclose(fp); // preciso fechar para não abrir mais do que o SO permite
            imprime(filho,andar+1); // devido a essa chamada recursiva
            fp = fopen(narq,"rb");
        }
        for(j=0; j<=andar; j++)
            printf("   ");
        fseek(fp,sizeof(int)*(i+1),SEEK_SET);
        fread(&valor,sizeof(int),1,fp);
        printf("%d\n", valor);
    }
    fclose(fp);
    if (retorna_filho(narq,i,filho))
        imprime(filho,andar+1);
}

int busca(char* narq, int ch, char* resp){ //Testada. Está OK.
    /*busca onde uma certa chave está localizada
     *se existir retorna 1, e modifica resp como o nome do arquivo onde se encontra a chave
     *se o arquivo não existir, retorna -1
     *caso contrário, retorna 0
     */
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
            if(resp)
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

long int pos_primeiro_filho(FILE *fp){
    // esse metodo retorna a posição do primeiro filho
    // caso seja um nó interno, retorna a posição do filho no arquivo
    // do contrário retorna 0
    fseek(fp,0L,SEEK_END);
    int end = ftell(fp);
    fseek(fp,0L,SEEK_SET);
    int n_chaves;
    fread(&n_chaves,sizeof(int),1,fp);
    fseek(fp,sizeof(int)*n_chaves,SEEK_CUR);
    if(end == ftell(fp))
        return 0;
    return ftell(fp);
}

void salva(TARV *no, char *nome){
    if(!no) return;
    FILE *fp = fopen(nome,"rb+");
    if(!fp){
        cria(-1,nome);
        FILE *fp = fopen(nome,"rb+");
    }
    fwrite(&no->nchaves,sizeof(int),1,fp);
    int i;
    for(i=0;i<no->nchaves;i++)
        fwrite(&no->chave[i],sizeof(int),1,fp);
    //char filho[NOME_MAX];
    for(i=0;no->filho[i];i++){
        //strcpy(filho,no->filho[i]);
        fwrite(&no->filho[i],sizeof(char),NOME_MAX,fp);
    }
    fclose(fp);
}

TARV *divisao(TARV *no, TARV *filho, int pos, int t){
   /*
    * Função para dividir um nó com 2t-1 chaves (no limite)
    * Cria um novo nó para receber parte das chaves e filhos do nó filho
    * novo passa a ser filho de nó
    * por fim, uma chave de filho sobe para o nó
   */
    TARV *novo = inicializa(t);
    novo->nchaves = t-1;
    int i;
    for(i=0;i<t-1;i++)
        novo->chave[i] = filho->chave[t+i];
    if(filho->filho[0]){
        for(i=0;i<t;i++){
            //novo->filho[i] = filho->filho[i+t];
            strcpy(novo->filho[i],filho->filho[i+t]);
            filho->filho[i+t] = NULL;
        }
    }
    filho->nchaves = t-1;
    for(i=no->nchaves;i>=pos;i--){
        //no->filho[i+1] = no->filho[i];
        strcpy(no->filho[i+1],no->filho[i]);
        no->chave[i] = no->chave[i-1];
    }
    char nome_novo[NOME_MAX];
    cria(-1,nome_novo);
    salva(novo,nome_novo);
    libera_no(novo,t);
    //no->filho[pos] = nome_novo;
    strcpy(no->filho[pos],nome_novo);
    no->chave[pos-1] = filho->chave[t-1];
    no->nchaves++;
    ler_arquivo(no->nomearq);
    return no;
}

void insere_aux(TARV *no, int num, int t){
    /*
    *Função para auxiliar a tarefa do insere
    *Recursiva, critério de parada quando nó é folha: realiza a inserção
    *Do contrário procura qual filho do nó será candidato à inserção
    */
    int i = no->nchaves-1;
    if(!no->filho[0]){
        //caso a inserção seja numa folha, insere ordenado a chave
        while((i >= 0) && (num < no->chave[i])){
            no->chave[i+1] = no->chave[i];
            i--;
        }
        no->chave[i+1] = num;
        no->nchaves++;
        salva(no,no->nomearq);
        libera_no(no,t);
        return;
    }
    // procura potencial candidato a receber nó
    while((i >= 0) && (num < no->chave[i])) i--;
    i++; //acerta a posição caso i = -1 ou num > no->chave
    printf("%s\n",no->filho[i]);
    TARV *filho = ler_mp(no->filho[i],t);
    if(filho->nchaves == (2*t)-1){
        // caso o candidato se encontre no limite, uma divisão é necessária
        no = divisao(no,filho,i+1,t);
        //caso o indice precise de correção após a divisão
        if(num > no->chave[i]) i++;
    }
    salva(no,no->nomearq);
    libera_no(no,t);
    insere_aux(filho,num,t);
}

void insere(char *arq,int num, int t){
    if(busca(arq,num,NULL))
        return;
    FILE *existe = fopen(arq,"rb");
    if(!existe){
        cria(num,arq);
        return;
    }
    fclose(existe);
    TARV *no = ler_mp(arq,t);
    // caso o no atual precise de divisão
    if(no->nchaves == (2*t)-1){
printf("1\n");
        char nomeraiz[NOME_MAX];
        cria(-1,nomeraiz);
printf("2\n");
        TARV *raiz = inicializa(t);
printf("3\n");
        raiz->nchaves = 0;
printf("3.25\n");
        strcpy(raiz->filho[0],no->nomearq);
printf("3.5\n");
        strcpy(raiz->nomearq,nomeraiz);
printf("4\n");
        raiz = divisao(raiz,no,1,t);
printf("5\n");
        salva(no,no->nomearq);
        libera_no(no,t);
printf("6\n");
        insere_aux(raiz,num,t);
printf("7\n");
        strcpy(arq,nomeraiz);
printf("show\n");
        return;
    }
    insere_aux(no,num,t);
}

int main(int argc, char *argv[]){
  char raiz[NOME_MAX];
  int num = 0, from, t;
  printf("Insira valor de t: \n");
  scanf("%d\n",&t);
  while(num != -1){
    printf("Digite um numero para adicionar. 0 para imprimir. -9 para remover e -1 para sair\n");
    scanf("%i", &num);
    if(num == -9){
      scanf("%d", &from);
      //arvore = retira(arvore, from, t);
      imprime(raiz,0);
    }
    else if(num == -1){
      printf("\n");
      imprime(raiz,0);
      return 0;
    }
    else if(!num){
      printf("\n");
      imprime(raiz,0);
    }
    else insere(raiz, num, t);
    printf("\n\n");
  }
}
