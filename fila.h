#ifndef FILA_H
#define FILA_H

/*
 * fila.h - Mini biblioteca de fila FIFO com vetores de inteiros.
 * Entrada vazia = 0. Nao use 0 como valor valido na fila.
 */

/* Inicializa todas as entradas do vetor com 0 (fila vazia).
 * Em caso de erro (ponteiro NULL ou tamanho invalido), chama exit(1). */
void fila_criar(int *fila, int tamanho);

/* Insere 'valor' no primeiro slot vazio (0) encontrado.
 * Retorna  0 em caso de sucesso.
 * Retorna -1 em caso de erro (fila cheia, ponteiro NULL, tamanho invalido ou valor == 0). */
int fila_inserir(int *fila, int tamanho, int valor);

/* Remove e retorna o primeiro elemento da fila (FIFO), deslocando os demais.
 * Retorna o valor removido em caso de sucesso.
 * Retorna -1 em caso de erro (fila vazia, ponteiro NULL ou tamanho invalido). */
int fila_remover(int *fila, int tamanho);

/* Verifica se a fila esta vazia.
 * Retorna  1 se vazia.
 * Retorna  0 se nao vazia.
 * Retorna -1 em caso de erro (ponteiro NULL ou tamanho invalido). */
int fila_vazia(int *fila, int tamanho);

#endif /* FILA_H */
