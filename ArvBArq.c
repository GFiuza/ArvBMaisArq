#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOME_MAX 25

int n_arq = 1;

typedef struct no{
    // definição da estrutura responsável por capturar um nó da arvore
    int nchaves;
    int qtdFilhos;
    int *chave;
    char **filho;
    char nomearq[NOME_MAX];
}TARV;

TARV *inicializa(int t){
    // responsável por iniciar um nó vazio
    TARV *novo = (TARV*)malloc(sizeof(TARV));
    novo->nchaves = 0;
    novo->qtdFilhos = 0;
    novo->chave = (int*)malloc(sizeof(int)*(2*t-1));
    novo->filho = (char**)malloc(sizeof(char*)*2*t);
    int i;
    for(i=0; i < (2*t); i++){
        novo->filho[i] = (char*)malloc(sizeof(char)*NOME_MAX);
    }
    return novo;
}

TARV* libera_no(TARV *no, int t){
    if (no){
        free(no->chave);
        int i;
        for(i=0; i < (2*t); i++)
            free(no->filho[i]);
        free(no->filho);
        free(no);
    }
    return NULL;
}

TARV *ler_mp(char *arq, int t){
    /*
        função responsável por ler um nó para MP
        retorna o no com os dados do arq
    */
    FILE *fp = fopen(arq,"rb");
    if(!fp){printf("nao foi possivel abrir o arquivo %s\n", arq); exit(1);}
    fseek(fp,0,SEEK_END);
    int tam = ftell(fp);
    fseek(fp,0,SEEK_SET);
    TARV *novo = inicializa(t);
    strcpy(novo->nomearq,arq);
    fread(&novo->nchaves,sizeof(int),1,fp);
    int i;
    for(i=0;i<novo->nchaves;i++)
        fread(&novo->chave[i],sizeof(int),1,fp);
    char nome[NOME_MAX];
    for(i=0;ftell(fp)<tam;i++){
        fread(&nome,sizeof(char),NOME_MAX,fp);
        int j;
        strcpy(novo->filho[i],nome);
        novo->qtdFilhos++;
    }
    if(novo->qtdFilhos)
        novo->qtdFilhos = novo->nchaves+1;
    fclose(fp);
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
    printf("\n");
    FILE *fp = fopen(arq, "rb");
    int a, n;
    fread(&a, sizeof(int), 1, fp);
    printf("%s\nN Chaves: %d\n",arq,a);
    int i = 1;
    while(i <= a)
    {
        fread(&n, sizeof(int), 1, fp);
        printf("%d\n",n);
        i++;
    }
    char saidaaqui[25];
    int leu;
    i=0;
    leu = retorna_filho(arq, i, saidaaqui);
    if(leu) printf("Nao eh folha\nFilhos: \n");
    else printf("Eh folha\n");
    while(leu)
    {
        printf("%s\n",saidaaqui);
        i++;
        leu = retorna_filho(arq, i, saidaaqui);
    }
    fclose(fp);
    printf("FIM\n\n");
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

int busca(char* narq, int num, char* resp, int t){ //Testada. Está OK.
    /*busca onde uma certa chave está localizada
     *se existir retorna 1, e modifica resp como o nome do arquivo onde se encontra a chave
     *se o arquivo não existir, retorna -1
     *caso contrário, retorna 0
     */
     FILE* f = fopen(narq, "rb");
     if (!f) return 0;
     fclose(f);
     TARV* no = ler_mp(narq, t);
     int i;
     for (i = 0; i < no->nchaves; i++){
        if (no->chave[i] == num){
            if (resp)
                strcpy(resp, narq);
            libera_no(no, t);
            return 1;
        }
        else if (no->chave[i] > num) break;
     }
     if (!no->qtdFilhos){
        libera_no(no, t);
        return 0;
     }
     char nome_filho[NOME_MAX];
     strcpy(nome_filho, no->filho[i]);
     libera_no(no, t);
     return busca(nome_filho, num, resp, t);
     /*
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
    */
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
    FILE *fp = fopen(nome,"wb");
    fwrite(&no->nchaves,sizeof(int),1,fp);
    int i;
    for(i=0;i<no->nchaves;i++)
        fwrite(&no->chave[i],sizeof(int),1,fp);
    char name[NOME_MAX];
    for(i=0;i<no->qtdFilhos;i++){
        strcpy(name,no->filho[i]);
        fwrite(&name,sizeof(char),NOME_MAX,fp);
    }
    fclose(fp);
}

void ler_TARV(TARV *a)
{
    int i;
    printf("Chaves de %s: %d\n", a->nomearq, a->nchaves);
    for(i = 0; i < a->nchaves; i++) printf("%d\n",a->chave[i]);
    printf("Filhos: %d\n", a->qtdFilhos);
    for(i = 0; i < a->qtdFilhos; i++) printf("%s\n",a->filho[i]);
    //printf("\n\n");
}

TARV *divisao(TARV *a, TARV *b, int pos, int t){
   /*
    * Função para dividir um nó com 2t-1 chaves (no limite)
    * Cria um novo nó para receber parte das chaves e filhos do nó b
    * novo passa a ser filho de a
    * por fim, uma chave de b sobe para o a
    */
    TARV *novo = inicializa(t);
    novo->nchaves = t-1;
    int i;
    for(i=0;i<t-1;i++) novo->chave[i] = b->chave[t+i];
    if(b->qtdFilhos > 0){
        for(i=0;i<t;i++){
            strcpy(novo->filho[i],b->filho[i+t]);
            b->qtdFilhos--;
            novo->qtdFilhos++;
        }
    }
    b->nchaves = t-1;
    for(i=a->nchaves;i>=pos;i--){
        strcpy(a->filho[i+1],a->filho[i]);
        a->chave[i] = a->chave[i-1];
    }
    char nome_novo[NOME_MAX];
	cria(novo->chave[0],nome_novo);
    salva(novo,nome_novo);
    strcpy(a->filho[pos],nome_novo);
    libera_no(novo,t);
    a->qtdFilhos++;
    a->chave[pos-1] = b->chave[t-1];
    a->nchaves++;
    return a;
}

void insere_aux(TARV *no, int num, int t){
    /*
    *Função para auxiliar a tarefa do insere
    *Recursiva, critério de parada quando nó é folha: realiza a inserção
    *Do contrário procura qual filho do nó será candidato à inserção
    */
    int i = no->nchaves-1;
    if(no->qtdFilhos == 0){
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
    TARV *filho = ler_mp(no->filho[i],t);
    int trocou_filho = 0;
    if(filho->nchaves == ((2*t)-1)){
        // caso o candidato se encontre no limite, uma divisão é necessária
        no = divisao(no,filho,(i+1),t);
        //caso o indice precise de correção após a divisão
        if(num > no->chave[i]){
            i++;
            trocou_filho = 1;
        }
    }
    if(trocou_filho){
        salva(filho,filho->nomearq);
        libera_no(filho,t);
        filho = ler_mp(no->filho[i],t);
    }
    salva(no,no->nomearq);
    libera_no(no,t);
    insere_aux(filho,num,t);
}

void insere(char *arq,int num, int t){
    if(busca(arq,num,NULL, t)){
        //printf("FON\n");
        return;
    }
    FILE *existe = fopen(arq,"rb");
    if(!existe){
        cria(num,arq);
        return;
    }
    fclose(existe);
    TARV *no = ler_mp(arq,t);
    // caso o no atual precise de divisão
    if(no->nchaves == (2*t)-1){
        TARV *raiz = inicializa(t);
        raiz->nchaves = 0;
        strcpy(raiz->filho[0],no->nomearq);
        char nomeraiz[NOME_MAX];
		cria(-1,nomeraiz);
        strcpy(raiz->nomearq,nomeraiz);
        raiz->qtdFilhos++;
        raiz = divisao(raiz,no,1,t);
        salva(no,no->nomearq);
        libera_no(no,t);
        insere_aux(raiz,num,t);
        strcpy(arq,nomeraiz);
        return;
    }
    insere_aux(no,num,t);
}

void remover(char* narq, int num, int t){
    if (!busca(narq, num, NULL, t)) {printf("%d nao encontrado no arvore\n", num);return;}
    int i;
    TARV* no = ler_mp(narq, t);
    for(i = 0; (i < no->nchaves) && (no->chave[i] < num); i++);
    if (i < no->nchaves && no->chave[i] == num){ //Casos 1, 2A, 2B e 2C
        if (!no->qtdFilhos){ //1
            printf("CASO 1\n");
            int j;
            for(j=i; j<no->nchaves-1;j++) no->chave[j] = no->chave[j+1];
            no->nchaves--;
            if(no->nchaves == 0)
                remove(narq);
            else
                salva(no, narq);
            libera_no(no, t);
            return;
        }
        else{
            TARV* aux = ler_mp(no->filho[i], t);
            if (aux->nchaves >= t){ //2A
                printf("CASO 2A\n");
                TARV* a = aux;
                char nome_filho[NOME_MAX];
                while(a->qtdFilhos){
                    strcpy(nome_filho, a->filho[a->qtdFilhos-1]);
                    libera_no(a, t);
                    a = ler_mp(nome_filho, t);
                }
                int temp = a->chave[a->nchaves-1];
                libera_no(aux, t);
                remover(no->filho[i], temp, t);
                no->chave[i] = temp;
                salva(no, narq);
                libera_no(no, t);
                return;
            }
            TARV* aux2 = ler_mp(no->filho[i+1], t);
            if (aux2->nchaves >= t){ //2B
                printf("CASO 2B\n");
                TARV *a = aux2;
                char nome_filho[NOME_MAX];
                while(a->qtdFilhos){
                    strcpy(nome_filho, a->filho[0]);
                    libera_no(a, t);
                    a = ler_mp(nome_filho, t);
                }
                int temp = a->chave[0];
                libera_no(aux, t);
                libera_no(aux2, t);
                remover(no->filho[i+1], temp, t);
                no->chave[i] = temp;
                salva(no, narq);
                libera_no(no, t);
                return;
            }
            if (aux2->nchaves == t-1 && aux->nchaves == t-1){ //2C
                printf("CASO 2C\n");
                TARV *a = aux;
                TARV *b = aux2;
                a->chave[a->nchaves] = num;          //colocar ch ao final de filho[i]
                int j;
                a->nchaves++;
                for(j=0; j<t-1; j++){ //juntar chave[i+1] com chave[i]
                    a->chave[t+j] = b->chave[j];
                    a->nchaves++;
                }
                for(j=0; j<=t && j<b->qtdFilhos; j++){ //juntar filho[i+1] com filho[i]
                    strcpy(a->filho[t+j],b->filho[j]);
                    a->qtdFilhos++; //será que devo?
                }
                //a->nchaves = 2*t-1;
                for(j=i; j < no->nchaves-1; j++)   //remover ch de arv
                    no->chave[j] = no->chave[j+1];// diminuir nchaves?
                for(j=i+1; j <= no->nchaves && j < no->qtdFilhos-1; j++){ //remover ponteiro para filho[i+1]
                    strcpy(no->filho[j],no->filho[j+1]);
                }
                no->qtdFilhos--;
                no->nchaves--;
                if(no->nchaves == 0)
                {
                    salva(a, no->nomearq);
                    remove(b->nomearq);
                    remove(a->nomearq);
                    libera_no(no, t);
                    libera_no(aux, t);
                    libera_no(aux2, t);
                    remover(narq, num, t);
                    return;
                }
                else
                {
                    char nome_filho[NOME_MAX];
                    strcpy(nome_filho, no->filho[i]);
                    salva(no, narq);
                    salva(a, a->nomearq);
                    remove(b->nomearq);
                    libera_no(no, t);
                    libera_no(aux, t);
                    libera_no(aux2, t);
                    remover(nome_filho, num, t);
                    return;
                }
            }
        }
    }
    TARV *a, *b = NULL;
    a = ler_mp(no->filho[i], t);
    if (a->nchaves == t-1){ //3A 3B
        //if (i < no->nchaves && c->nchaves >=t){ //3A
        if (i < no->nchaves){
            TARV* c = ler_mp(no->filho[i+1], t);
            if (c->nchaves >=t){ //3A
                printf("CASO 3A: i menor que nchaves\n");
                b = c;
                a->chave[t-1] = no->chave[i];   //dar a y a chave i da arv
                a->nchaves++;
                no->chave[i] = b->chave[0];     //dar a arv uma chave de z
                int j;
                for(j=0; j < b->nchaves-1; j++)  //ajustar chaves de z
                {
                    b->chave[j] = b->chave[j+1];
                }
                b->nchaves--;
                if(b->qtdFilhos>0)
                {
                    strcpy(a->filho[a->nchaves],b->filho[0]); //enviar ponteiro menor de z para o novo elemento em y
                    a->qtdFilhos++;
                }
                for(j=0; j < b->qtdFilhos-1; j++)       //ajustar filhos de z //-1
                {
                    strcpy(b->filho[j],b->filho[j+1]);
                }
                b->qtdFilhos--;
                char nome_filho[NOME_MAX];
                strcpy(nome_filho, no->filho[i]);
                salva(no, no->nomearq);
                salva(a, a->nomearq);
                salva(b, b->nomearq);
                libera_no(no, t);
                libera_no(a, t);
                libera_no(b, t);
                remover(nome_filho, num, t);
                return;
            }
            libera_no(c, t);
        }
        //if (i > 0 && (!b) && d->nchaves >=t){ //3A
        if (i > 0 && (!b)){
            TARV* d = ler_mp(no->filho[i-1], t);
            if (d->nchaves >=t){ //3A
                printf("CASO 3A: i igual a nchaves\n");
                b = d;
                int j;
                for(j = a->nchaves; j>0; j--)  //encaixar lugar da nova chave
                    a->chave[j] = a->chave[j-1];
                if(a->qtdFilhos>0)
                {
                    a->qtdFilhos++;
                    for(j = a->nchaves+1; j>0; j--) //encaixar lugar dos filhos da nova chave
                        strcpy(a->filho[j],a->filho[j-1]);
                }
                a->chave[0] = no->chave[i-1];              //dar a y a chave i da arv
                a->nchaves++;
                no->chave[i-1] = b->chave[b->nchaves-1];   //dar a arv uma chave de z
                if(b->qtdFilhos>0)
                {
                    strcpy(a->filho[0],b->filho[b->nchaves]); //enviar ponteiro de z para o novo elemento em y
                    b->qtdFilhos--;
                }
                b->nchaves--;
                char nome_filho[NOME_MAX];
                strcpy(nome_filho, no->filho[i]);
                salva(no, narq);
                salva(a, a->nomearq);
                salva(b, b->nomearq);
                libera_no(no, t);
                libera_no(a, t);
                libera_no(b, t);
                remover(nome_filho, num, t);
                return;
            }
            libera_no(d, t);
        }
        if (!b){
            //if(i < no->nchaves && c->nchaves == t-1){
            if(i < no->nchaves){
                TARV* c = ler_mp(no->filho[i+1], t);
                if (c->nchaves == t-1){ //3B
                    printf("CASO 3B: i menor que nchaves\n");
                    b = c;
                    a->chave[t-1] = no->chave[i];     //pegar chave [i] e coloca ao final de filho[i]
                    a->nchaves++;
                    int j;
                    for(j=0; j < t-1; j++){
                        a->chave[t+j] = b->chave[j];     //passar filho[i+1] para filho[i]
                        a->nchaves++;
                        b->nchaves--;
                    }
                    if(a->qtdFilhos){
                        for(j=0; j<b->qtdFilhos; j++){
                            strcpy(a->filho[t+j],b->filho[j]);
                            a->qtdFilhos++;
                        }
                    }
                    for(j=i; j < no->nchaves-1; j++){ //limpar referências de i
                        no->chave[j] = no->chave[j+1];
                        strcpy(no->filho[j+1],no->filho[j+2]);
                    }
                    no->nchaves--;
                    no->qtdFilhos--; //Devo?
                    if(no->nchaves == 0)
                    {
                        salva(a, no->nomearq);
                        remove(a->nomearq);
                    }
                    else
                    {
                        salva(no, narq);
                        salva(a,a->nomearq);
                    }
                    remove(b->nomearq);
                    libera_no(no, t);
                    libera_no(a, t);
                    libera_no(b, t);
                    remover(narq, num, t);
                    return;
                }
                libera_no(c, t);
            }
            //if((i > 0) && (d->nchaves == t-1)){
            if (i > 0){
                TARV* d = ler_mp(no->filho[i-1], t);
                if (d->nchaves == t-1){ //3B
                printf("CASO 3B: i igual a nchaves\n");
                    b = d;
                    if(i == no->nchaves)
                        b->chave[t-1] = no->chave[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
                    else
                        b->chave[t-1] = no->chave[i];   //pegar chave [i] e poe ao final de filho[i-1]
                    b->nchaves++;
                    int j;
                    for(j=0; j < t-1; j++){
                        b->chave[t+j] = a->chave[j];     //passar filho[i+1] para filho[i]
                        b->nchaves++;
                    }
                    if(b->qtdFilhos){
                        for(j=0; j<a->qtdFilhos; j++){ // j = 0
                            strcpy(b->filho[t+j],a->filho[j]);
                            b->qtdFilhos++;
                        }
                    }
                    no->nchaves--;
                    if(no->nchaves==0)
                    {
                        salva(b,no->nomearq);
                        remove(b->nomearq);
                    }
                    else
                    {
                        salva(no, narq);
                        salva(b, b->nomearq);
                    }
                    remove(a->nomearq);
                    no = libera_no(no, t);
                    a = libera_no(a, t);
                    b = libera_no(b, t);
                    remover(narq, num, t);
                    return;
                }
                libera_no(d, t);
            }
        }
    }
    char prox[NOME_MAX];
    strcpy(prox,no->filho[i]);
    libera_no(no, t);
    libera_no(a, t);
    libera_no(b, t);
    remover(prox, num, t);
}

void imprime_tds_arq(){
    int i;
    char nome[NOME_MAX];
    for(i=1;i<n_arq;i++){
        sprintf(nome,"%d",i);
        strcat(nome,".dat");
	FILE *fp = fopen(nome,"rb");
	if(fp){
           ler_arquivo(nome);
	   fclose(fp);
	}
    }
}

int main(){
  char raiz[NOME_MAX];
  int num = 0, from, t = 1;
  printf("Insira valor de t: \n");
  scanf("%d",&t);
  while(t < 2){
    printf("t precisa ser >= 2...\n");
    scanf("%d",&t);
  }
  while(num != -1){
    printf("Digite um numero para adicionar. 0 para imprimir. -2 para exibir os arquivos, -3 para remover e -1 para sair\n");
    scanf("%i", &num);
    if(num == -3){
      scanf("%d", &from);
      remover(raiz, from, t);
      printf("\narquivo raiz: %s\n", raiz);
      imprime(raiz,0);
    }
    else if(num == -1){
      printf("\n");
      imprime(raiz,0);
	  FILE *fp = fopen(raiz,"rb");
	  if(!fp) return 0;
	  fclose(fp);
      libera(raiz);
      return 0;
    }
    else if(num == -2){
        imprime_tds_arq();
    }
    else if(!num){
      printf("\narquivo raiz: %s\n", raiz);
      imprime(raiz,0);
    }
    else insere(raiz, num, t);
    printf("\n\n");
  }
}
