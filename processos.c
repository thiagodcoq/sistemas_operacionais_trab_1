#include "processos.h"
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 30

/*
 * Como as chamadas de IO so existem para um unico dispositivo, essa informacao
 * sera omitida. Informacoes sobre o tipo de pedido ('R' e 'W') serao usadas por
 * didatismo, sem efeito pratico
 */
void sys_call(IOinfo *io_info, char c) {
  io_info->type = c;
  io_info->pid_requester = getpid();
  signal(getppid(), SIGRTMIN);
}

void filho1(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    if (i == 5 || i == 13) {
      sys_call(io_info, 'R');
    } else if (i == 27) {
      sys_call(io_info, 'W');
    }

    sleep(1);
  }

  exit(0);
}

void filho2(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    if (i == 8 || i == 19) {
      sys_call(io_info, 'R');
    }

    sleep(1);
  }

  exit(0);
}

void filho3(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    if (i == 2 || i == 10) {
      sys_call(io_info, 'R');
    } else if (i == 25) {
      sys_call(io_info, 'W');
    }

    sleep(1);
  }

  exit(0);
}

void filho4(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    if (i == 9) {
      sys_call(io_info, 'W');
    } else if (i == 25) {
      sys_call(io_info, 'R');
    }

    sleep(1);
  }

  exit(0);
}

void filho5(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    if (i == 15) {
      sys_call(io_info, 'W');
    }

    sleep(1);
  }

  exit(0);
}

void filho6(IOinfo *io_info) {

  for (int i = 0; i < MAX; i++) {
    sleep(1);
  }

  exit(0);
}
