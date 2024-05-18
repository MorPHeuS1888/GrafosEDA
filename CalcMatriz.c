#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "Grafos.h"

struct vertice* matriz = NULL;
int numeroLinhas = 0;
int numeroColunas = 0;

void DisplayMenu()
{
    ClearConsole();
    printf("+-------------------------------------------------------------------------------------+\n");
    printf("|                   Estruturas de dados avancadas - Listas Ligadas                    |\n");
    printf("|                                                                                     |\n");
    printf("|                            1- Carregar a partir de ficheiro de texto                |\n");
    printf("|                            2- Alteracao dos valores                                 |\n");
    printf("|                            3- Insercao de nova linha                                |\n");
    printf("|                            4- Insercao de nova coluna                               |\n");
    printf("|                            5- Remocao de linha                                      |\n");
    printf("|                            6- Remocao de coluna                                     |\n");
    printf("|                            7- Listagem da Tabela                                    |\n");
    printf("|                            8- Calculo da soma maxima                                |\n");
    printf("|                                                                                     |\n");
    printf("|                            9- Exit                                                  |\n");
    printf("|                                                                                     |\n");
    printf("+-------------------------------------------------------------------------------------+\n");
}

void ListagemTabela(int mostralinhas, int mostracolunas, int mostramaisum)
{
    // Determinar qual o maior valor
    int maior = obtemMaiorValor(matriz);
    // Determinar o numero de digitos do maior valor
    int digitos = countDigits(maior);
    // Construct the format string dynamically
    char format[10]; // Sufficiently large buffer
    sprintf_s(format, sizeof(format), "%%%dd ", digitos);
    // listar
    struct vertice* primeiroDaLinha = verticePorPosicao(matriz, 0, 0);
    struct vertice* currentElemento = primeiroDaLinha;

    for (int linha = 0; linha < numeroLinhas; linha++)
    {
        if (mostralinhas)
        {
            printf(format, linha);
        }
        for (int coluna = 0; coluna < numeroColunas; coluna++)
        {
            // obter valor na linha/coluna
            int valorMatriz = currentElemento->dados->valor;
            // Append null terminator to format string
            format[sizeof(format) - 1] = '\0';
            // Print the number using the constructed format string
            printf(format, valorMatriz);
            // obtem o proximo
            currentElemento = verticeDaDireita(currentElemento);
        }
        currentElemento = verticeDeBaixo(primeiroDaLinha);
        primeiroDaLinha = currentElemento;
        printf("\n");
    }
    if (mostralinhas && mostramaisum)
    {
        printf(format, numeroLinhas);
    }

    if (mostracolunas)
    {
        for (int coluna = 0; coluna < numeroColunas; coluna++)
        {
            printf(format, coluna);
        }
        if (mostracolunas && mostramaisum)
        {
            printf(format, numeroColunas);
        }
        printf("\n");
    }
    printf("\n");
}

void CarregarFicheiro()
{
    printf("Qual e' o nome do ficheiro que quer carregar? ");
    char input[255]; // Assuming a maximum input length of 255 characters
    scanf_s("%s", input, (unsigned)_countof(input));
    FILE* file = fopen(input, "r");
    if (file == NULL)
    {
        printf("Erro ao abrir ficheiro! :( ");
        InputAnyText();
        return;
    }
    ClearInputBuffer();

    printf("Qual e' o caracter para usar como separador? ");
    char separador;
    scanf_s("%c", &separador, 1);

    // limpar a matriz
    limpaMemoria(matriz);

    matriz = NULL;
    numeroColunas = 0;
    numeroLinhas = 0;

    char line[255];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        numeroLinhas++;
        char* tokens[255]; // Array de tokens
        int num_tokens;
        split_string(line, separador, tokens, &num_tokens);
        numeroColunas = num_tokens;
        for (int i = 0; i < num_tokens; i++)
        {
            if (tokens[i] != NULL) 
            {
                int valor = atoi(tokens[i]);
                //printf("A inserir em (%d, %d) o valor %d\n", numeroLinhas-1, i, valor);
                matriz = insereMatriz(matriz, valor, numeroLinhas-1, i);
            }
        }
    }
    fclose(file);
    ListagemTabela(0, 0, 0);
    printf("O ficheiro foi carregado! :)");
    InputAnyText();
}

void InserirValores()
{
    ClearConsole();
    ListagemTabela(1, 0, 0);
    printf("Em que linha quer inserir o valor? ");
    int numeroLinha;
    scanf_s("%d", &numeroLinha);
    while (numeroLinha < 0 || numeroLinha > numeroLinhas)
    {
        printf("Numero de linha errado");
        InputAnyText();
        return;
    }

    ClearConsole();
    ListagemTabela(0, 1, 0);
    printf("Em que coluna quer inserir o valor? ");
    int numeroColuna;
    scanf_s("%d", &numeroColuna);
    while (numeroColuna < 0 || numeroColuna > numeroColunas)
    {
        printf("Numero de coluna errado");
        InputAnyText();
        return;
    }
    
    printf("Qual o valor que quer inserir? ");
    int valor;
    scanf_s("%d", &valor);
    while (valor < 0)
    {
        printf("Valor errado");
        InputAnyText();
        return;
    }

    // alterar o valor na posicao escolhida
    alterarValorVertice(matriz, numeroLinha, numeroColuna, valor);
    ListagemTabela(0, 0, 0);
    printf("Valor alterado :)");
    InputAnyText();
}

void InserirLinha()
{
    ClearConsole();
    ListagemTabela(1, 0, 1);
    printf("Em que posicao quer inserir a linha? ");
    int numeroLinha;
    scanf_s("%d", &numeroLinha);
    if (numeroLinha < 0 || numeroLinha > numeroLinhas)
    {
        printf("Numero de linha errado");
        InputAnyText();
        return;
    }
    // inserir a linha na posicao escolhida
    matriz = insereLinhaMatriz(matriz, numeroLinha, numeroColunas);
    numeroLinhas++;
    ListagemTabela(0, 0, 0);
    printf("linha inserida :)");
    InputAnyText();
}

void InserirColuna()
{
    ClearConsole();
    ListagemTabela(0, 1, 1);
    printf("Em que posicao quer inserir a coluna? ");
    int numeroColuna;
    scanf_s("%d", &numeroColuna);
    if (numeroColuna < 0 || numeroColuna > numeroColunas)
    {
        printf("Numero de coluna errado");
        InputAnyText();
        return;
    }
    // inserir a coluna na posicao escolhida
    matriz = insereColunaMatriz(matriz, numeroColuna, numeroLinhas);
    numeroColunas++;
    ListagemTabela(0, 0, 0);
    printf("Coluna inserida :)");    
    InputAnyText();
}

void RemocaoLinha()
{
    ClearConsole();
    ListagemTabela(1, 0, 0);
    printf("Qual o numero da linha que quer remover? ");
    int numeroLinha;
    scanf_s("%d", &numeroLinha);
    if (numeroLinha < 0 || numeroLinha >= numeroLinhas)
    {
        printf("Numero de linha errado");
        InputAnyText();
        return;
    }
    // remover a linha escolhida
    matriz = removeLinhaMatriz(matriz, numeroLinha);
    numeroLinhas--;
    ListagemTabela(0, 0, 0);
    printf("linha removida :)");
    InputAnyText();
}

void RemocaoColuna()
{
    ClearConsole();
    ListagemTabela(0, 1, 0);
    printf("Qual o numero da coluna que quer remover? ");
    int numeroColuna;
    scanf_s("%d", &numeroColuna);
    if (numeroColuna < 0 || numeroColuna >= numeroColunas)
    {
        printf("Numero de coluna errado");
        InputAnyText();
        return;
    }
    // remover a coluna escolhida
    matriz = removeColunaMatriz(matriz, numeroColuna);
    numeroColunas--;
    ListagemTabela(0, 0, 0);
    printf("Coluna removida :)");
    InputAnyText();
}

struct vertice* CalculaMaxSoma(struct vertice* matriz, int linhas)
{    
    int numeroElementos = contaElementos(matriz);
    if (numeroElementos == 1)
    {
        return insereMatriz(NULL, matriz->dados->valor, 0, 0);
    } 

    struct vertice* resultadoFinal = NULL;
    int somaFinal = 0;
    
    for (int i = 0; i < linhas; i++)
    {
        struct vertice* resultado = NULL;
        // determinar elemento atual (valor)
        struct vertice* verticeAtual = procuraElementoNaPosicao(matriz, i, 0);
        int valorAtual = verticeAtual->dados->valor;
        // criar matriz com todos os elementos depois da linha i e coluna 0 
        struct vertice* novaMatriz = matrizRestante(matriz, i, 0);
        // chamar novamente para calcular a soma da nova matriz 
        resultado = CalculaMaxSoma(novaMatriz, linhas - 1);
        // juntar valor ao resultado da chamada anterior
        resultado = insereMatriz(resultado, valorAtual, i, 0); // so tem 1 dimensao
        // determinar se a nova soma e' maior que a anterior
        int soma = somaElementos(resultado);
        if (soma >= somaFinal)
        {
            // limpar memoria do resultado final anterior
            limpaMemoria(resultadoFinal);
            resultadoFinal = resultado;
            somaFinal = soma;
        }
        // limpar memoria da novaMatriz
        limpaMemoria(novaMatriz);
    }
    return resultadoFinal;
}

void CalculaSoma()
{
    if (numeroLinhas != numeroColunas)
    {
        printf("O calculo da soma maxima, nao repetindo linhas ou colunas, so' e' possivel em matrizes quadradas\n");
        InputAnyText();
        return;
    }

    struct vertice* resultado = CalculaMaxSoma(matriz, numeroLinhas);
    imprimeLista(resultado);
    int soma = somaElementos(resultado);
    printf("A soma final e' %d", soma);
    InputAnyText();

    // limpar memoria do resultado
    limpaMemoria(resultado);
}

int main()
{
  
    int option;
    do
    {
        DisplayMenu();
        option = GetMenuOption();
        switch (option)
        {
        case 1:
            CarregarFicheiro();
            break;
        case 2:
            InserirValores();
            break;
        case 3:
            InserirLinha();
            break;
        case 4:
            InserirColuna();
            break;
        case 5:
            RemocaoLinha();
            break;
        case 6: 
            RemocaoColuna();
            break;
        case 7:
            ClearConsole();
            ListagemTabela(0, 0, 0);
            printf("Pressiona ENTER para continuar");
            InputAnyText();
            break;
        case 8: 
            CalculaSoma();
            break;
        case 9:
            break;

        default:
            break;
        }

    } while (option != 9);


    return 0;
}