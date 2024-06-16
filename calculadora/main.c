#include <stdio.h>

#include "calculadora.h"

int main() {
    char posFixa[] = "2 3 + log 5 /";
    
    // Convertendo para a forma infixa
    char *inFixa = getFormaInFixa(posFixa);

    inFixa = removeParenteses(inFixa);

    printf("Expressao Infixa: %s\n", inFixa);
    
    // Calculando o valor da expressão
    float valor = getValor(posFixa);
    printf("Valor da Expressao: %.2f\n", valor);
    
    return 0;
}
