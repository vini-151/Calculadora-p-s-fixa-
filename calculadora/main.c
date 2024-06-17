#include <stdio.h>

#include "calculadora.h"

int main() {
    char postfix[512] = "0.5 45 sen 2 ^ +"; 

    float resultado = getValor(postfix);

    char *infixa = getFormaInFixa(postfix);
    infixa = removeParenteses(infixa);

    return 0;
}
