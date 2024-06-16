#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "calculadora.h"

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

float getValor(char *str) {
    Expressao expressao;
    float stack[100];
    int top = -1;
    char tempStr[512];
    strcpy(tempStr, str);  // Faz uma cópia da string original para uso com strtok
    char *token = strtok(tempStr, " ");
    
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            stack[++top] = atof(token);
        } else {
            if (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
                strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 || strcmp(token, "log") == 0) {
                float op = stack[top--];
                if (strcmp(token, "raiz") == 0) stack[++top] = sqrt(op);
                else if (strcmp(token, "sen") == 0) stack[++top] = sin(op);
                else if (strcmp(token, "cos") == 0) stack[++top] = cos(op);
                else if (strcmp(token, "tg") == 0) stack[++top] = tan(op);
                else if (strcmp(token, "log") == 0) stack[++top] = log10(op);
            } else {
                float op2 = stack[top--];
                float op1 = stack[top--];
                if (strcmp(token, "+") == 0) stack[++top] = op1 + op2;
                else if (strcmp(token, "-") == 0) stack[++top] = op1 - op2;
                else if (strcmp(token, "*") == 0) stack[++top] = op1 * op2;
                else if (strcmp(token, "/") == 0) stack[++top] = op1 / op2;
                else if (strcmp(token, "^") == 0) stack[++top] = pow(op1, op2);
            }
        }
        
        // Construir a expressão na forma posFixa
        strcat(expressao.posFixa, token);
        strcat(expressao.posFixa, " ");

        token = strtok(NULL, " ");
    }
    
    // Copiar a expressão inFixa final para a estrutura Expressao
    strcpy(expressao.inFixa, expressao.posFixa);

    // Atribuir o valor numérico calculado à estrutura Expressao
    expressao.Valor = stack[top];

    return stack[top];
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