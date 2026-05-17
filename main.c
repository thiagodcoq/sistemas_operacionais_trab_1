#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[]){
    int n = 3; 
    pid_t pids[n+1];
    
    pids[0] = fork();
    if (pids[0]==0){
        //InterControllerSim
        while(1){
                printf("InterControllerSim rodando\n");
                sleep(1);
            }
            exit(0);
    }
    for(int i=1;i<=3;i++){
        pids[i]=fork();
        if (pids[i]==0){
            //filho i
            while(1){
                printf("[A%d] rodando\n", i);
                sleep(1);
            }
            exit(0);
        }
    }
    while(1){
    printf("[KernelSim] rodando\n");
    sleep(1);
}

    return 0;
}