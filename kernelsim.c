#include "auxiliar.h"
#include "fila.h"
#include "ics.h"
#include "processos.h"
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define QTD_FILHOS 3
#define ICS 0
#ifndef SIGRTMIN
#define SIGRTMIN 34
#endif

int processos_vivos = QTD_FILHOS;
int running = 0;
int ready[QTD_FILHOS];
int waiting[QTD_FILHOS];
pid_t pids[QTD_FILHOS + 1];
IOinfo *io_info;

void timeSliceHandler(int signal);
void IOreturnHandler(int signal);
void IOrequestHandler(int signal);
void selecionaRespectFilho(IOinfo *io_info, int i);
int escalonador();
void childHandler(int signal);

int main(int argc, char *argv[]) {
  int segmento;
  segmento = shmget(IPC_PRIVATE, sizeof(IOinfo),
                    IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  io_info = (IOinfo *)shmat(segmento, 0, 0);

  signal(SIGUSR1, timeSliceHandler);
  signal(SIGRTMIN, IOreturnHandler);
  signal(SIGUSR2, IOrequestHandler);
  signal(SIGCHLD, childHandler);

  fila_criar(ready, QTD_FILHOS);
  fila_criar(waiting, QTD_FILHOS);
  for (int i = 1; i <= QTD_FILHOS; i++) {
    fila_inserir(ready, QTD_FILHOS, i);
  }

  pids[ICS] = fork();
  if (pids[ICS] == 0) {
    InterControllerSim(io_info);
  }

  for (int i = 1; i <= QTD_FILHOS; i++) {
    pids[i] = fork();
    if (pids[i] == 0) {
      selecionaRespectFilho(io_info, i);
    } else {
      kill(pids[i], SIGSTOP);
    }
  }

  escalonador();

  // ve se ta tudo vazio e ngm rodando
  while (processos_vivos > 0) {
    sleep(1);
  }

  kill(pids[ICS], SIGTERM);
  print_time("[Kernel] todos os processos terminaram");

  shmdt(io_info);
  shmctl(segmento, IPC_RMID, 0);

  return 0;
}

void timeSliceHandler(int signal) {
  if (running == 0)
    return;
  if (fila_vazia(ready, QTD_FILHOS))
    return;
  int proximo = fila_remover(ready, QTD_FILHOS);
  if (proximo == -1)
    return; // ninguém pra rodar

  kill(pids[running], SIGSTOP);
  print_time("[Kernel] A%d pausado", running);
  fila_inserir(ready, QTD_FILHOS, running);

  running = proximo;

  kill(pids[running], SIGCONT);
  print_time("[Kernel] A%d rodando", running);
}

void IOreturnHandler(int signal) {
  int indice = fila_remover(waiting, QTD_FILHOS);
  if (indice == -1)
    return;
  print_time("[Kernel] I/O terminou, desbloqueando...");
  print_time("[Kernel] A%d voltou pra fila ready", indice);
  fila_inserir(ready, QTD_FILHOS, indice);

  if (running == 0) {
    escalonador();
  }
}

void IOrequestHandler(int signal) {
  print_time("[Kernel] syscall recebida de PID=%d", io_info->pid_requester);

  int indice = -1;
  for (int i = 1; i <= QTD_FILHOS; i++) {
    if (pids[i] == io_info->pid_requester) {
      indice = i;
      break;
    }
  }

  if (indice == -1)
    return;

  print_time("[Kernel] indice encontrado = %d", indice);

  kill(pids[indice], SIGSTOP);
  print_time("[Kernel] A%d bloqueado por I/O", indice);
  fila_inserir(waiting, QTD_FILHOS, indice);
  kill(pids[ICS], SIGRTMIN + 1);

  int proximo = fila_remover(ready, QTD_FILHOS);
  print_time("[Kernel] proximo a rodar = %d", proximo);
  if (proximo == -1)
    return;
  running = proximo;
  kill(pids[running], SIGCONT);
}

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

int escalonador() {
  running = fila_remover(ready, QTD_FILHOS);
  if (running == -1)
    return -1;
  kill(pids[running], SIGCONT);
  return 0;
}

void childHandler(int signal) {
  int status;
  pid_t morto;

  while ((morto = waitpid(-1, &status, WNOHANG)) > 0) {
    int encontrado = 0;
    for (int i = 1; i <= QTD_FILHOS; i++) {
      if (pids[i] == morto) {
        print_time("[Kernel] A%d terminou", i);
        processos_vivos--;
        if (running == i)
          running = 0;
        encontrado = 1;
        break;
      }
    }
    // Se não encontrou, era filho do ICS
  }
}
