#include "ics.h"
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

pid_t kernel_id;
IOinfo *shdIO_info;

void IOhandler(int signal) {
  if (fork() == 0) {
    sleep(3);
    kill(kernel_id, SIGUSR2);
    exit(0);
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

  signal(SIGCHLD, SIG_IGN);
  signal(SIGUSR2, IOhandler);

  while (1) {
    sleep(1);
    kill(kernel_id, SIGUSR1);
  }

  exit(0);
}
