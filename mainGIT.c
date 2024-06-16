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

// Protótipos das funções
void push(Pilha *p, int tipo, double chave);
double pop(Pilha *p);
int is_operator(char c);
int is_function(const char *str, int i);
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

// Função para verificar se uma substring é uma função matemática conhecida
int is_function(const char *str, int i) {
    return (strncmp(&str[i], "raiz", 4) == 0 ||
            strncmp(&str[i], "sen", 3) == 0 ||
            strncmp(&str[i], "cos", 3) == 0 ||
            strncmp(&str[i], "tg", 2) == 0 ||
            strncmp(&str[i], "log", 3) == 0);
}

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
                default:
                    printf("Operador desconhecido: %c\n", Str[i]);
                    return 0.0f;
            }
            i++;
        } else if (is_function(Str, i)) {
            // Trata funções
            if (strncmp(&Str[i], "raiz", 4) == 0) {
                if (pilha.Tamanho < 1) {
                    printf("Erro: Expressão inválida.\n");
                    return 0.0f;
                }
                double a = pop(&pilha);
                push(&pilha, 1, sqrt(a));
                i += 4;
            } else if (strncmp(&Str[i], "sen", 3) == 0) {
                if (pilha.Tamanho < 1) {
                    printf("Erro: Expressão inválida.\n");
                    return 0.0f;
                }
                double a = pop(&pilha);
                push(&pilha, 1, sin(a * M_PI / 180)); // Converte para radianos
                i += 3;
            } else if (strncmp(&Str[i], "cos", 3) == 0) {
                if (pilha.Tamanho < 1) {
                    printf("Erro: Expressão inválida.\n");
                    return 0.0f;
                }
                double a = pop(&pilha);
                push(&pilha, 1, cos(a * M_PI / 180)); // Converte para radianos
                i += 3;
            } else if (strncmp(&Str[i], "tg", 2) == 0) {
                if (pilha.Tamanho < 1) {
                    printf("Erro: Expressão inválida.\n");
                    return 0.0f;
                }
                double a = pop(&pilha);
                push(&pilha, 1, tan(a * M_PI / 180)); // Converte para radianos
                i += 2;
            } else if (strncmp(&Str[i], "log", 3) == 0) {
                if (pilha.Tamanho < 1) {
                    printf("Erro: Expressão inválida.\n");
                    return 0.0f;
                }
                double a = pop(&pilha);
                push(&pilha, 1, log10(a));
                i += 3;
            } else {
                printf("Função desconhecida: %s\n", &Str[i]);
                return 0.0f;
            }
        } else {
            // Se não for operador nem função, assume que é um número e empilha
            if (isdigit(Str[i]) || Str[i] == '.') {
                char *endPtr;
                double numero = strtod(&Str[i], &endPtr);
                push(&pilha, 1, numero);
                i = endPtr - Str;
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
        } else if (is_operator(token[0])) {
            char op2[512], op1[512], temp[512];
            strcpy(op2, stack[top--]);
            strcpy(op1, stack[top--]);
            sprintf(temp, "(%s %s %s)", op1, token, op2);
            strcpy(stack[++top], temp);
        } else if (is_function(token, 0)) {
            char op1[512], temp[512];
            strcpy(op1, stack[top--]);
            sprintf(temp, "%s(%s)", token, op1);
            strcpy(stack[++top], temp);
        }
        token = strtok(NULL, " ");
    }
    strcpy(infixa, stack[top]);
    return infixa;
}

char *removeParenteses(char *inFixa) {
    if (inFixa != NULL && strlen(inFixa) > 0) {
        size_t tamanho = strlen(inFixa);

        // Verifica se começa e termina com parênteses
        if (inFixa[0] == '(' && inFixa[tamanho - 1] == ')') {
            // Remove o primeiro e o último caractere
            memmove(inFixa, inFixa + 1, tamanho - 2);
            inFixa[tamanho - 2] = '\0';
        }
    }

    return inFixa;
}

int main() {
    char postfix[512] = "45 60 + 30 cos *"; 

    float resultado = getValor(postfix);

    char *infixa = getFormaInFixa(postfix);
    infixa = removeParenteses(infixa);

    printf("%s\n", infixa);
    printf("Resultado: %.2f\n", resultado);

    return 0;
}
