#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

// Estrutura para um item da pilha
typedef struct Item {
    int tipo; // 0 para operador, 1 para número
    union {
        char operador;
        double numero;
    } valor;
    struct Item *proximo;
} Item;

// Estrutura para a pilha
typedef struct {
    Item *topo;
    int Tamanho;
} Pilha;

// Estrutura para encapsular a expressão matemática
typedef struct {
    char posFixa[512]; // Expressão na forma pos fixa, como 3 12 4 + *
    char inFixa[512];  // Expressão na forma pos fixa, como 3 * (12 + 4)
    float Valor;       // Valor numérico da expressão
} Expressao;

// Protótipos das funções
char *getFormaInFixa(char *Str);
float getValor(char *Str);

// Função para empilhar um valor na pilha
void push(Pilha *p, int tipo, double chave) {
    Item *novoItem = (Item *)malloc(sizeof(Item));

    if (novoItem == NULL) {
        printf("Erro: Não foi possível alocar memória para o item.\n");
        exit(1);
    }

    novoItem->tipo = tipo;
    if (tipo == 1) {
        novoItem->valor.numero = chave;
    } else {
        novoItem->valor.operador = (char) chave;
    }
    novoItem->proximo = p->topo;
    p->topo = novoItem;
    p->Tamanho++;
}

// Função para desempilhar um valor da pilha
double pop(Pilha *p) {
    if (p->Tamanho == 0) {
        printf("Erro: Pilha vazia.\n");
        exit(1);
    }

    Item *temp = p->topo;
    double chave = temp->valor.numero;
    p->topo = temp->proximo;
    free(temp);
    p->Tamanho--;
    return chave;
}

// Função para verificar se um caractere é um sinal matemático
int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Função para calcular o valor da expressão postfix
float getValor(char *Str) {
    Pilha pilha = { NULL, 0 }; // Inicializa a pilha

    int i = 0;
    while (Str[i] != '\0') {
        if (isspace(Str[i])) {
            i++;
            continue; // Pula espaços em branco
        }

        if (is_operator(Str[i])) {
            if (pilha.Tamanho < 2) {
                printf("Erro: Expressão inválida.\n");
                return 0.0f;
            }
            double a = pop(&pilha);
            double b = pop(&pilha);
            switch (Str[i]) {
                case '+':
                    push(&pilha, 1, b + a);
                    break;
                case '-':
                    push(&pilha, 1, b - a);
                    break;
                case '*':
                    push(&pilha, 1, b * a);
                    break;
                case '/':
                    if (a == 0) {
                        printf("Erro: Divisão por zero.\n");
                        return 0.0f;
                    }
                    push(&pilha, 1, b / a);
                    break;
                case '^':
                    push(&pilha, 1, pow(b, a));
                    break;
            }
            i++;
        } else {
            // Se não for operador, assume que é um número e empilha
            if (isdigit(Str[i]) || Str[i] == '.') {
                double numero = atof(&Str[i]);
                push(&pilha, 1, numero);

                // Avança o índice até o próximo não dígito
                while (isdigit(Str[i]) || Str[i] == '.') {
                    i++;
                }
            } else {
                // Caractere inválido na expressão
                printf("Erro: Caractere inválido na expressão.\n");
                return 0.0f;
            }
        }
    }

    // O resultado final deve estar no topo da pilha
    float resultado = (float)pop(&pilha);
    return resultado;
}

// Função para obter a forma inFixa de uma expressão postfix
char *getFormaInFixa(char *str) {
    static char infixa[512];
    char stack[100][512];
    int top = -1;
    char tempStr[512];
    strcpy(tempStr, str);  // Faz uma cópia da string original para uso com strtok
    char *token = strtok(tempStr, " ");
    
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            strcpy(stack[++top], token);
        } else {
            char op1[512], op2[512], temp[512];
            strcpy(op2, stack[top--]);
            if (top >= 0) {
                strcpy(op1, stack[top--]);
            } else {
                op1[0] = '\0';
            }
            if (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
                strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 || strcmp(token, "log") == 0) {
                sprintf(temp, "%s(%s)", token, op2);
            } else {
                sprintf(temp, "(%s %s %s)", op1, token, op2);
            }
            strcpy(stack[++top], temp);
        }
        token = strtok(NULL, " ");
    }
    strcpy(infixa, stack[top]);
    return infixa;
}

char *removeParenteses(char *inFixa) {
    if (inFixa != NULL && strlen(inFixa) > 0) {
        inFixa[0] = ' ';

        size_t tamanho = strlen(inFixa);

        if (tamanho > 0 && inFixa[tamanho - 1] == ')') {
            inFixa[tamanho - 1] = ' ';
        }
    }

    return inFixa;
}

int main() {
    char postfix[512] = "3 4 + 5 *"; 

    float resultado = getValor(postfix);

    char *infixa = getFormaInFixa(postfix);

    infixa = removeParenteses(infixa);

    printf("forma infixa: %s\n", infixa);

    printf("Resultado: %.2f\n", resultado);

    return 0;
}