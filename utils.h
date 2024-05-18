#ifndef UTILS_H
#define UTILS_H

void ClearConsole();

int GetMenuOption();

void ClearInputBuffer();

void InputAnyText();

int countDigits(int n);

void split_string(char* str, char delimitador, char* tokens[], int* num_tokens);

#endif 