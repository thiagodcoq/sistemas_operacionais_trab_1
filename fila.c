#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

/* Inicializa todas as entradas com 0 (vazio). */
void fila_criar(int *fila, int tamanho)
{
    if (fila == NULL || tamanho <= 0) {
        fprintf(stderr, "fila_criar: parametros invalidos.\n");
        exit(1);
    }

    for (int i = 0; i < tamanho; i++)
        fila[i] = 0;
}

/* Insere no primeiro slot vazio encontrado (ordem de chegada). */
int fila_inserir(int *fila, int tamanho, int valor)
{
    if (fila == NULL || tamanho <= 0 || valor == 0)
        return -1;

    for (int i = 0; i < tamanho; i++) {
        if (fila[i] == 0) {
            fila[i] = valor;
            return 0;
        }
    }

    return -1; /* fila cheia */
}

/* Remove o primeiro elemento (indice 0) e desloca os demais para a esquerda. */
int fila_remover(int *fila, int tamanho)
{
    if (fila == NULL || tamanho <= 0 || fila[0] == 0)
        return -1; /* fila vazia ou invalida */

    int valor = fila[0];

    for (int i = 0; i < tamanho - 1; i++)
        fila[i] = fila[i + 1];

    fila[tamanho - 1] = 0; /* libera o ultimo slot */

    return valor;
}

/* Retorna 1 se vazia, 0 se nao vazia, -1 em caso de erro. */
int fila_vazia(int *fila, int tamanho)
{
    if (fila == NULL || tamanho <= 0)
        return -1;

    return (fila[0] == 0) ? 1 : 0;
}
