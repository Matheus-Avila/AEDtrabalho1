#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

/* ESTRUTURA PARA O NÓ */
struct no {
    int dados; // número relacionado ao atributo
    char atributo[15]; // atributo daquele nó (tempo, temperatura, umidade ou vento)
    struct no *prox; // ponteiro 'prox' apontando para o próximo nó
};

/* ESTRUTURA PARA A ÁRVORE */
struct arvDecisao {
    int dados;
    int dados_coletados;
    double ent; // entropia
    int pos; // posição
    int filhos;
    int valor_i;
    struct arvDecisao *c[50];
}raiz;

/* ESTRUTURA PARA INSERIR NOVO NÓ */
struct no *criar_novoNo(struct no *raiz, char atributo[15], int d) {
    struct no *ponteiro = raiz, *temp;

    temp = (struct no*)malloc(sizeof(struct no)); // aloca memória para a estrutura do tipo no 'temp'
    strcpy(temp->atributo, atributo); // o campo 'atributo' da estrutura 'temp' recebe o 'atributo' que foi entrada da função
    temp->dados = d;
    temp->prox = NULL;
    if(raiz == NULL) // se a raiz = NULL, a raiz recebe o nó temporário 'temp'
        raiz = temp;
    else { // caso contrário, desloca-se pelos próximos nós 'ponteiro' até achar um que o campo 'prox' aponte pra NULL
        while(ponteiro->prox != NULL) {
            ponteiro = ponteiro->prox;
        }
        ponteiro->prox = temp; // o nó 'temp' é adicionado no campo 'prox' desse nó
    }

    return raiz;
}


/* FUNÇÃO PARA BUSCAR UM NÓ CUJO ATRIBUTO SEJA IGUAL A STRING LIDA NO ARQUIVO */
int busca(struct no *raiz, char atributo[15]) {
    struct no *temp = raiz;
    while(temp != NULL) { // compara o campo atributo de temp com a entrada da busca (atributo lido)
        if(strcmp(temp->atributo,atributo) == 0)
            return temp->dados;
        else
            temp = temp->prox; //
    }

    if(temp == NULL)
        return 3;
    else
        return 0;
}

/* FUNÇÃO P/ EXIBIR MAPEAMENTO DE ATRIBUTOS E INTEIROS */
void exibe(struct no *raiz)
{
    struct no *temp = raiz;

    if(temp == NULL)
        return;
    while(temp != NULL) {
        printf("%d->%s\n",temp->dados,temp->atributo); // cada string (tempo, temperatura, umidade ou vento) é relacionada a um número
        temp = temp->prox;
    }
}

/* FUNÇÃO PARA CALCULAR ENTROPIA */
double entropia(int a[500][500], int obj, int atributo, int prox_atributo, int val, struct arvDecisao *raiz1) {
    int x, k;
    x = a[1][atributo-1];
    struct arvDecisao *temp;
    temp = raiz1;
    int i;
    double b, c, d, total = 1.0;
    double cont1 = 0, cont2 = 0 ;

    if(temp == NULL && prox_atributo == atributo - 1) {
        for(i = 1; i < obj; i++) {
            if(a[i][prox_atributo] == x)
                cont1++;
            else
                cont2++;
        }
    }

    if(temp == NULL  && prox_atributo != atributo-1) {

        for(i = 1; i < obj; i++) {
            if(a[i][prox_atributo] == val) {
                if(a[i][atributo-1] == x)
                    cont1 = cont1 + 1;
                else
                    cont2 = cont2 + 1;
            }
        }
    }
    b = cont1/(cont1 + cont2);
    c = cont2/(cont1 + cont2);


    if(cont1 == 0.000000 || cont2 == 0.000000)
        d = 0 ;
    else
        d =  ((cont1 + cont2)/(obj - 1))*(-(b*(log(b)/log(2)) + c*(log(c)/log(2))));

    return d;
}

void encontra_k(int a[500][500], int b[50][50], int obj, int atributo){

    int i, j, k, cont = 1;
    int band  = 0 ;


    for(k = 0; k < atributo ;k++) {
        b[k][1] = a[1][k];
        i = 1;
        while(i < obj){
            for(j = 1 ; j < cont; j++){
                if(a[i][k] == b[k][j]) {
                    band = 1;
                }
            }
            if(band  == 0){
                b[k][cont] = a[i][k];
                cont++;
            }
            band = 0;
            i++;

        }
        b[k][0] = cont - 1;

        cont = 1;
    }
}


int encontra_max(double *ganho1 , int atributo){

    int i = 0 ;
    int max1 = 0 ;
    double max;
    max = ganho1[0];
    for(i = 1; i <= atributo; i++ ) {
        if(ganho1[i] > max){
            max1 = i;
            max = ganho1[i];
        }
    }
    return max1;
}


double encontra_soma(double colecao[50],int tamanho){

    int i, j;
    double soma = 0;
    for(i = 0; i < tamanho ; i ++){
        soma = soma + colecao[i];
    }
    return soma;
}


int* modificaA(int a[500][500], int A_mod[500][500], int max1, int atributo, int obj, int val, int dim[2]){

    int i, j, k;
    int linha = 0, col = 0, temp = 0, col2 = 0;

    for(j = 0 ; j < obj ; j++){
        if(j == 0 ) {
            for(k = 0; k < atributo; ) {
                if(k == max1) {
                    k++;
                }
                else {
                    A_mod[linha][col2] = a[j][k];
                    k++;
                    col2++;
                }
            }
            linha++;
        }
        else {
            if(a[j][max1] == val) {
                for(i = 0; i < atributo; ) {
                    if(i == max1){
                        i++;
                    }
                    else {
                        A_mod[linha][col] = a[j][i];
                        temp = col++;
                        i++;
                    }
                }
                col = 0;
                linha++;
            }
        }
    }
    dim[0] = linha;
    dim[1] = atributo - 1;
    return dim;
}

struct arvDecisao* ganhosRecursivo(int Z[500][500], int b[50][50], struct arvDecisao *pai, double ent, int max1, int atributo, int obj, int band, int atributo_inicial, int a[500][500]){

    int i =0 ;
    int j;
    int A_mod[500][500], B_mod[50][50];
    double col3[50], col1[50], ent1[50], entropia1[50][50], ganho1[50];
    int dim[2];
    if(band == 1) {
        band = 0;
        pai->ent = ent;
        pai->dados = 0;
        for(i = 0; i  < atributo; i++) {
            for(j = 0; j < obj; j++) {
                A_mod[j][i] = Z[j][i];
            }
        }
        encontra_k(A_mod, B_mod, obj, atributo);
    }

    else {
          for(i = 0; i < atributo; i++) {

            for(j = 0; j < obj; j++){
                A_mod[j][i] = Z[j][i];

            }
        }

        modificaA(A_mod, A_mod, max1, atributo, obj, pai->pos, dim);
        obj = dim[0];
        atributo = dim[1];
        pai->ent = entropia(A_mod, obj, atributo, i, B_mod[i][j], NULL);
        if(pai->ent == 0) {
             int sd =  pai->dados ;
             pai->dados = A_mod[1][atributo - 1];
             pai->dados_coletados = atributo - 1;
             pai->filhos = 0;
             printf("\n\tnode %d is terminated with row value %d ,  child of %d i posotion value %d\n",pai->dados, pai->pos , sd, pai->valor_i);
             return 0;
        }
        else {
            encontra_k(A_mod, B_mod, obj, atributo);
        }
    }

    for(i = 0; i < atributo - 1 ; i++) {
        for(j = 1; j <= B_mod[i][0]; j++){
            col3[j-1] = entropia(A_mod, obj, atributo, i, B_mod[i][j], NULL);

        }
        entropia1[i] = encontra_soma(col3, B_mod[i][0]);
        ganho1[i] = pai->ent - entropia1[i];
    }

    int temp = max1;
    max1 = encontra_max(ganho1, atributo - 1);
    pai->dados = A_mod[0][max1];
    pai->filhos = B_mod[max1][0];

    for(i = 0; i <= atributo_inicial; i++){
        if(pai->dados == a[0][i]){
            pai->dados_coletados = i;
            break;
        }
    }

    printf("\n\tCurrent no is %d and is child of %d , through  row value %d,i position %d coloum number %d \n", pai->dados,Z[0][temp],pai->pos,pai->valor_i,pai->dados_coletados);

    for(i = 1; i <= B_mod[max1][0]; i++) {
        pai->c[i] = (struct arvDecisao *)malloc(sizeof(struct arvDecisao));
        pai->c[i]->pos = B_mod[max1][i];
        pai->c[i]->dados = A_mod[0][max1];
        pai->c[i]->ent = pai->ent;
        pai->c[i]->valor_i = i;
        ganhosRecursivo(A_mod, B_mod, pai->c[i], pai->c[i]->ent, max1, atributo, obj, band, atributo_inicial, a);
    }

    return pai;
}

int* valor_teste(int a[500][500], int atributo, int obj) {

    int i, j, band = 0, *class, temp, cont = 1;
    temp = a[1][atributo-1];
    class = (int *)malloc(obj*sizeof(int));

    class[1] = temp;

    for(i = 2; i <= obj; i++ ) {
        for(j = 1; j <= cont ; j++){
               if(a[i][atributo-1] == class[j]) {
                    band = 1;
                    break;
               }
        }
        if(band == 0){
            cont++;
            class[cont] = a[i][atributo-1];
        }
        band = 0;
    }

    class[0] = cont;
        printf("\n");
    return class;
}

int classificacao(int a[500][500], struct arvDecisao *raiz, int pos_dados_teste, int *valor_teste){

    int temp, armazenar, band = 0, class,i;

    struct arvDecisao *ptr;
    ptr = raiz;

    while(band != 1){
        temp = a[pos_dados_teste][ptr->dados_coletados];
        for(i =1; i <= ptr->filhos; i++) {
                if(ptr->c[i]->pos == temp) {
                    ptr = ptr->c[i];

              }
            }
             for(i = 1; i <= valor_teste[0]; i++) {
                 if(valor_teste[i] == ptr->dados) {
                    class = valor_teste[i];
                    band = 1;
                 }
                }
    }
    return class;
}


/* FUNÇÃO PRINCIPAL */
int main(){

    struct no *primeiro = NULL; // inicializa o ponteiro para o 1º nó como nulo
    struct arvDecisao *raiz = NULL, *pai = NULL, *raiz_temp = NULL;
    // inicializa os ponteiros para a raiz, o pai e a raiz temporária da árvore de decisão como nulos

    int k = 4, t, j; // k = nº de atributos que pode ter (tempo, temperatura, umidade e vento)
    int a[500][500], obj, atributo, x, *d, treinamento, teste, *valorTeste, class;

    double r, dados, erro = 0, precisao = 0, total;

    FILE *fp;

    char *pt; // ponteiro para caracter, auxiliar para leitura do arquivo
    const char s[2] = ","; // caracter que irá gerar a divisão entre as strings do arquivo
    char str[200]; // string a ser lida no arquivo

    fp = fopen("dados.txt","r"); // abrindo o arquivo de dados

    int i = 0;

    /* INICIANDO A LEITURA DO ARQUIVO */
    while(fgets(str,200,fp) != NULL) { // enquanto há linhas para ler
        int j = 0;
        pt = strtok(str,s);
        // a função 'strtok' recebe como parâmetro a string a ser dividida (str) e outra string (s) contendo os caracteres
        // pelos quais str será dividida

        while(pt != NULL) { // enquanto há palavras para ler na linha
            t = busca(primeiro,pt); // busca na árvore um nó cujo campo atributo seja igual a string lida no arquivo (pt)
            if(t != 3) { // (encontrou o nó)
                a[i][j] = t;
                j++;
            }
            if(t == 3) { // quando não encontra o nó retorna 3
                primeiro = criar_novoNo(primeiro,pt,k); // cria-se um novo nó com esse atributo
                a[i][j] = k;
                j++;
                k++;
            } //
            pt = strtok(NULL,s);
        }

        i++;
        atributo = j;
    }

    obj = i;
    exibe(primeiro);

    fclose(fp); // fecha o arquivo

    d = (int *)malloc(atributo * sizeof(int));

    for(k = 0 ; k < atributo ; k++) {
        d[k] = k;
    }
    for(i = 0; i < obj; i++) {
        printf("\n");

        for(j=0;j<atributo;j++) {
            printf("%d\t", a[i][j]);
        }
    }

    treinamento = obj*75/100;
    teste = treinamento;
    printf("\nTraining Data\n");

    for(i = 0; i < treinamento; i++) {
        printf("\n");
        for(j=0;j<atributo;j++) {
            printf("%d\t", a[i][j]);
        }
    }
    printf("\nTest Data");

    for(i = teste; i < obj; i++) {
        printf("\n");

        for(j = 0; j < atributo; j++) {
            printf("%d\t",a[i][j]);
        }
    }


    dados =  entropia(a, treinamento, atributo, atributo-1, 0, NULL);
    int band = 1; // bandeira (indica se o valor foi ou não encontrado)

    pai = (struct arvDecisao *)malloc(sizeof(struct arvDecisao));

    raiz = ganhosRecursivo(a, NULL, pai, dados, atributo, atributo, treinamento, band, atributo, a);
    return 0;
}
