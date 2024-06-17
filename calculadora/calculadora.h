#ifndef EXPRESSAO_H
#define EXPRESSAO_H

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
char *getFormaInFixa(char *str);
char *removeParenteses(char *inFixa);

#endif