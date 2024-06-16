#include <stdio.h>

#include "calculadora.h"

int main() {
    char posFixa[] = "0.5 45 sen 2 ^ +";
    
    // Convertendo para a forma infixa
    char *inFixa = getFormaInFixa(posFixa);

    inFixa = removeParenteses(inFixa);

    printf("Expressao Infixa: %s\n", inFixa);
    
    // Calculando o valor da expressão
    float valor = getValor(posFixa);
    printf("Valor da Expressao: %.2f\n", valor);
    
    return 0;
}