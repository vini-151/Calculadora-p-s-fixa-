#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para isdigit()

// Estrutura para um item da pilha
typedef struct Item {
    char *expressao;
    struct Item *proximo;
} Item;

// Estrutura para a pilha
typedef struct {
    Item *topo;
    int Tamanho;
} Pilha;

// Função para empilhar um valor na pilha
void push(Pilha *p, char *expressao) {
    Item *novoItem = (Item *)malloc(sizeof(Item));

    if (novoItem == NULL) {
        printf("Erro: Não foi possível alocar memória para o item.\n");
        exit(1);
    }

    // Alocar memória para a expressão e copiar
    novoItem->expressao = (char *)malloc((strlen(expressao) + 1) * sizeof(char));
    strcpy(novoItem->expressao, expressao);

    novoItem->proximo = p->topo;
    p->topo = novoItem;
    p->Tamanho++;
}

// Função para desempilhar um valor da pilha
char *pop(Pilha *p) {
    if (p->Tamanho == 0) {
        printf("Erro: Pilha vazia.\n");
        exit(1);
    }

    Item *temp = p->topo;
    char *expressao = temp->expressao;
    p->topo = temp->proximo;
    free(temp);
    p->Tamanho--;
    return expressao;
}

// Função para verificar se um caractere é um sinal matemático
int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Função para converter uma expressão postfix para infix (anteriormente postfix_to_infix)
char *getFormaInFixa(char *Str) {
    Pilha pilha;
    pilha.topo = NULL;
    pilha.Tamanho = 0;

    int tamanho = strlen(Str);
    for (int i = 0; i < tamanho; i++) {
        // Se for um operando, empilha ele mesmo
        if (isdigit(Str[i])) {
            char operando[2] = { Str[i], '\0' };
            push(&pilha, operando);
        } else if (is_operator(Str[i])) {
            // Se for um operador, desempilha dois operandos e forma a expressão infixa
            char *operando2 = pop(&pilha);
            char *operando1 = pop(&pilha);

            // Cria a expressão infixa
            char *infixa = (char *)malloc((strlen(operando1) + strlen(operando2) + 4) * sizeof(char));
            sprintf(infixa, "(%s %c %s)", operando1, Str[i], operando2);

            // Empilha a expressão infixa
            push(&pilha, infixa);

            // Libera memória dos operandos
            free(operando1);
            free(operando2);
        } else {
            // Se encontrar caracteres inválidos, como espaço em branco
            printf("Erro: Caractere inválido na expressão.\n");
            exit(1);
        }
    }

    // A pilha deve conter exatamente uma expressão infixa
    if (pilha.Tamanho != 1) {
        printf("Erro: Expressão postfix inválida.\n");
        exit(1);
    }

    // Desempilha e retorna a expressão infixa
    return pop(&pilha);
}

char *removerEspacos(char *str) {
    char *strSemEspacos = malloc((strlen(str)) + 1); //nova string sem espaços
    int i = 0;
    int j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            strSemEspacos[j] = str[i];
            j++;
      }
    }
    strSemEspacos[j] = '\0';

    printf("%s\n", strSemEspacos);

    return (char*) strSemEspacos;
}

int main() {
    char *postfix = "8 5 2 4 + * + ";

    int tamanho = sizeof(postfix);

    postfix = removerEspacos(postfix);

    char *infixa = getFormaInFixa(postfix);

    printf("Expressão infixada: %s\n", infixa);

    free(infixa); // Libera a memória alocada para a expressão infixa

    return 0;
}