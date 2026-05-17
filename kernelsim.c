#include "fila.h"
#include "ics.h"
#include "processos.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <unistd.h>

#define QTD_FILHOS 3
#define ICS 0
void timeSliceHandler(int signal);
void IOreturnHandler(int signal);
void IOrequestHandler(int signal);
void selecionaRespectFilho(IOinfo *io_info, int i);
int escalonador();

int main(int argc, char *argv[]) {
  int segmento;
  IOinfo *io_info;
  segmento = shmget(IPC_PRIVATE, sizeof(IOinfo),
                    IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  io_info = (IOinfo *)shmat(segmento, 0, 0);

  signal(SIGUSR1, timeSliceHandler);
  signal(SIGUSR2, IOreturnHandler);
  signal(SIGRTMIN, IOrequestHandler);

  int running = 0;
  int ready[QTD_FILHOS];
  int waiting[QTD_FILHOS];
  fila_criar(ready, QTD_FILHOS);
  fila_criar(waiting, QTD_FILHOS);

  pid_t pids[QTD_FILHOS + 1];

  pids[ICS] = fork();
  if (pids[ICS] == 0) {
    InterControllerSim(io_info);
  }

  for (int i = 1; i <= QTD_FILHOS; i++) {
    pids[i] = fork();
    if (pids[i] == 0) {
      fila_inserir(ready, QTD_FILHOS, i);
      selecionaRespectFilho(io_info, i);
    }
  }

  while (1) {
    printf("[KernelSim] rodando\n");
    sleep(1);
    // TODO: adicionar comparador se todas as filas estao vazias
    //   -> se estiverem = sai do while
  }

  shmdt(io_info);
  shmctl(segmento, IPC_RMID, 0);

  return 0;
}

void timeSliceHandler(int signal) { return; }

void IOreturnHandler(int signal);

void IOrequestHandler(int signal);

void selecionaRespectFilho(IOinfo *io_info, int i) {
  if (i == 1)
    filho1(io_info);
  else if (i == 2)
    filho2(io_info);
  else if (i == 3)
    filho3(io_info);
  else if (i == 4)
    filho4(io_info);
  else if (i == 5)
    filho5(io_info);
  else
    filho6(io_info);

  return;
}

int escalonador();
