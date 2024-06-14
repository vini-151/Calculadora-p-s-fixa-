#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Adicionado para usar a função fgets
#include <math.h>
#define MAX 100

typedef struct Item {
    double chave;
    struct Item *proximo;
} Item;

typedef struct Pilha {
    Item *topo;
    int Tamanho;
} Pilha;

Pilha *criarPilha(); // verifica e aloca memoria conforme o acréscimo de itens
void push(Pilha *p, double chave); // push
double pop(Pilha *p); // pop

int main() {
    double a, b;
    char s[80];
    Pilha *p = criarPilha();

    if (!p) {
        printf("Erro de alocacao de memoria\n");
        exit(1);
    }

    printf("Calculadora \n");
    do {
        printf(":");
        fgets(s, 80, stdin);
        s[strcspn(s, "\n")] = '\0'; // Remove o newline lido pelo fgets

        switch (s[0]) {
            case '+':
                a = pop(p);
                b = pop(p);
                printf("%lf\n", b + a);
                push(p, b + a);
                break;
            case '-':
                a = pop(p);
                b = pop(p);
                printf("%lf\n", b - a);
                push(p, b - a);
                break;
            case '*':
                a = pop(p);
                b = pop(p);
                printf("%lf\n", b * a);
                push(p, b * a);
                break;
            case '/':
                a = pop(p);
                b = pop(p);
                if (a == 0) {
                    printf("Divisao por 0\n");
                    push(p, b); // Empilha novamente os valores
                    push(p, a);
                    break;
                }
                printf("%lf\n", b / a);
                push(p, b / a);
                break;
            case '^':
                a = pop(p);
                b = pop(p);
                printf("%lf\n", pow(b, a));
                push(p, pow(b, a));
                break;
            case '.': /* Mostra conteudo do topo da Pilha */
                a = pop(p);
                push(p, a);
                printf("Valor corrente no topo da pilha: %lf\n", a);
                break;
            case 'q':
                // Encerra o programa
                break;
            default:
                push(p, atoi(s));
                break;
        }
    } while (s[0] != 'q');

    return 0;
}

Pilha *criarPilha() {
    Pilha *P = (Pilha *)malloc(sizeof(Pilha));
    if (P == NULL) {
        printf("Erro: Falha ao alocar memoria para a pilha.\n");
        exit(1);
    }

    P->Tamanho = 0;
    P->topo = NULL;
    return P;
}

void push(Pilha *p, double chave) {
    Item *novoItem = (Item *)malloc(sizeof(Item));

    if (novoItem == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para o item.\n");
        return;
    }

    novoItem->chave = chave;
    novoItem->proximo = p->topo;
    p->topo = novoItem;
    p->Tamanho++;
}

double pop(Pilha *p) {
    if (p->Tamanho == 0) {
        printf("Erro: Pilha vazia.\n");
        exit(1);
    }

    Item *temp = p->topo;
    double chave = temp->chave;
    p->topo = temp->proximo;
    free(temp);
    p->Tamanho--;
    return chave;
}