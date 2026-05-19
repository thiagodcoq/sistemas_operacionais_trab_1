#include "ics.h"
#include "fila.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TAM_FILA_IO 6

pid_t kernel_id;
IOinfo *shdIO_info;
int io_ocupado = 0;
int fila_pedidos[TAM_FILA_IO];

static void iniciar_io(void) {
  if (io_ocupado)
    return;
  if (fila_vazia(fila_pedidos, TAM_FILA_IO)) {
    return;
  }

  fila_remover(fila_pedidos, TAM_FILA_IO);
  io_ocupado = 1;

  if (fork() == 0) {
    sleep(3);
    kill(kernel_id, SIGRTMIN);
    exit(0);
  }
}

static void tratar_sigchld(int sig) {
  int status;
  pid_t p;

  while ((p = waitpid(-1, &status, WNOHANG)) > 0) {
    io_ocupado = 0;
    iniciar_io();
  }
}

void IOhandler(int signal) {
  if (fila_inserir(fila_pedidos, TAM_FILA_IO, 1) == -1) {
    return;
  }
  if (!io_ocupado) {
    iniciar_io();
  }

  return;
}

/*
 * Recebe SIGUSR2 quando comecar IO
 * Envia SIGUSR2 quando terminar IO
 * Envia SIGUSR1 quando termina Time Slice (1seg)
 *
 * Em suma:
 * SIGUSR1 = IQR0
 * SIGUSR2 = IQR1
 */
void InterControllerSim(IOinfo *io_info) {
  kernel_id = getppid();
  shdIO_info = io_info;

  fila_criar(fila_pedidos, 6);

  signal(SIGCHLD, tratar_sigchld);
  signal(SIGRTMIN + 1, IOhandler);

  while (1) {
    sleep(1);
    kill(kernel_id, SIGUSR1);
  }

  exit(0);
}
