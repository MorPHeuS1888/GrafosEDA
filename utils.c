#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void ClearConsole()
{
    #ifdef _WIN32
        system("cls");
    #else 
        printf("\033[2J\033[H");
    #endif
}

int GetMenuOption()
{
    int valor;
    scanf_s("%d", &valor);
    return valor;
}

void ClearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void InputAnyText()
{
    char input[100]; // Assuming a maximum input length of 100 characters
    fgets(input, sizeof input, stdin);
    fgets(input, sizeof input, stdin);
}

int countDigits(int n) 
{
    int count = 0;

    // Handle negative numbers by taking absolute value
    if (n < 0) {
        n = -n;
    }

    // Count digits
    do {
        count++;
        n /= 10;
    } while (n != 0);

    return count;
}

void split_string(char* str, char delimitador, char* tokens[], int* num_tokens) 
{
    char* ptr = str;
    char* ptr1 = str;
    int i = 0;

    while (*ptr != '\0') 
    {
        if (*ptr == delimitador)
        {
            *ptr = '\0'; // Substitui o espaco por um caractere nulo
            tokens[i] = ptr1; // Armazena o endereco do token
            ptr++; // Avanca o ponteiro para o proximo caractere
            ptr1 = ptr;
            i++;
        }
        else {
            ptr++; // Avanca o ponteiro para o proximo caractere
        }
    }

    tokens[i] = ptr1; // Armazena o ultimo token
    *num_tokens = i + 1; // Numero de tokens
}
