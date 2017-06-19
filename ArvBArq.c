#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOME_MAX 25

int n_arq = 1;

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
}

void libera(char *arq){
    //exclui o arquivo arq e todos os seus filhos (ou ao menos deveria)
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

void divisao(char *nx, int i, char *ny, int t){
    char novo[NOME_MAX];
    cria(0,novo);
    FILE *fn = fopen(novo,"rb+");
    if(!fn) exit(1);
    int t_menos = t-1;
    fwrite(&t_menos,sizeof(int),1,fn);
    int j, vn, vy;
    FILE *fy = fopen(ny,"rb+");
    if(!fy) exit(1);
    fseek(fy,sizeof(int)*(t+1),SEEK_SET); // y->chave[t] e novo já está na pos correta
    for(j=0; j<t-1; j++){
        fread(&vy,sizeof(int),1,fy); // não preciso de fseek pois ambos vão iterativamente acertando a posição
        fwrite(&vy,sizeof(int),1,fn); // n->chave[j] = y->chave[j+t]
    }
    char filho_y[NOME_MAX];
    int prim_filho_y = pos_primeiro_filho(fy);
    if(prim_filho_y){ //verifica se y tem filho
        // novo já está na pos de arquivo correta
        fseek(fy,prim_filho_y+(sizeof(char)*NOME_MAX*t),SEEK_SET); // y->filho[t];
        for(j=0; j<t; j++){
            fread(&filho_y,sizeof(char),NOME_MAX,fy); // iterativamente pega o proximo filho de y
            fwrite(&filho_y,sizeof(char),NOME_MAX,fn);
            // os filhos de y serão removidos mais tarde no procedimento
        }
    }
    fclose(fn);
    fseek(fy,0L,SEEK_SET);
    fwrite(&t_menos,sizeof(int),1,fy);
    FILE *fx = fopen(nx,"rb+");
    if(!fx) exit(1);
    int n_chaves_x;
    fread(&n_chaves_x,sizeof(int),1,fx);
    fseek(fx,sizeof(int)*n_chaves_x,SEEK_CUR);
    char filho_x[NOME_MAX];
    int prim_filho_x = pos_primeiro_filho(fx);
    for(j = n_chaves_x; j>=i; j--){
        fseek(fx,prim_filho_x+(sizeof(char)*NOME_MAX*(j)),SEEK_SET);
        fread(&filho_x,sizeof(char),NOME_MAX,fx);
        fseek(fx,prim_filho_x+(sizeof(char)*NOME_MAX*(j+1)),SEEK_SET);
        fwrite(&filho_x,sizeof(char),NOME_MAX,fx); // x->filho[j+1]=x->filho[j];
    }
    fseek(fx,prim_filho_x+sizeof(char)*NOME_MAX*i,SEEK_SET);
    fwrite(&novo,sizeof(char),NOME_MAX,fx);
    int vx;
    for(j=n_chaves_x; j>= i; j--){
        fseek(fx,sizeof(int)*j, SEEK_SET); // posicao j-1
        fread(&vx,sizeof(int),1,fx);
        fseek(fx,sizeof(int)*(j+1), SEEK_SET); // posicao j
        fwrite(&vx,sizeof(int),1,fx);
    }
    fseek(fy,sizeof(int)*t,SEEK_SET); // posicao t-1
    fread(&vy,sizeof(int),1,fy);
    fseek(fx,sizeof(int)*i,SEEK_SET); // posicao i-1
    fwrite(&vy,sizeof(int),1,fx);
    // isso aqui não faz perder uma chave ou nome?? testar!!
    // testar linha acima
    //serio
    // nao esquecer
    // rosseti n pode ler isso aqui nao
    n_chaves_x++;
    fseek(fx,0L,SEEK_SET);
    fwrite(&vx,sizeof(int),1,fx); // atualiza qtd de chaves de x
    fclose(fx);
    // acertando os filhos de y e atualizando as chaves de y para t-1
    FILE *aux = fopen("aux.dat","wb");
    if(!aux) exit(1);
    fseek(fy,0L,SEEK_END);
    int tam = ftell(fy);
    int nchaves = t_menos; // esse valor já havia sido estipulado
    fwrite(&nchaves,sizeof(int),1,aux);
    for(i=0; i<=nchaves; i++){
        fread(&vy,sizeof(int),1,fy);
        fwrite(&vy,sizeof(int),1,aux);
    }
    char filho[NOME_MAX];
    fseek(fy,prim_filho_y,SEEK_SET); // prim filho de y não mudou de posição durante o procedimento
    for(i=prim_filho_y;i!=tam;i=ftell(fy)){ // a cada leitura, já tenho a posicão do próximo filho
        fread(&filho,sizeof(char),NOME_MAX,fy);
        fwrite(&filho,sizeof(char),NOME_MAX,aux);
    }
    fclose(fy);
    fclose(aux);
    remove(ny);
    rename("aux.dat",ny);
    fy = fopen(ny,"rb+");
    fclose(fy);
}

void insere(char *no, int num, int t){
    FILE *existe = fopen(no,"rb");
    if(!existe){ // caso o arquivo ainda não exista, significa que é o primeiro nó da arvore
        cria(num,no);
        return;
    }
    int nchaves;
    fread(&nchaves,sizeof(int),1,existe);
    fclose(existe);
    if(busca(no,num,NULL)) return; // caso num já esteja na arvore
    if(nchaves == (2*t)-1){
        char novo[NOME_MAX];
        cria(-1,novo);
        FILE *fp = fopen(novo,"rb+");
        nchaves = 0;
        fwrite(&nchaves,sizeof(int),1,fp);
        fseek(fp,sizeof(int),SEEK_CUR);
        fwrite(no,sizeof(char),25,fp); // nao tenho certeza quanto a essa parte aqui, talvez de merda
        // nao deixem a rosseti ler o comentario acima tb
        // TEM QUE TIRAR QUANDO RESOLVER
        fclose(fp);
        divisao(novo,1,no,t);
        //insere_nao_completos(novo,num,t);
        return;
    }
    //insere_nao_completo(novo,num,t);
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
