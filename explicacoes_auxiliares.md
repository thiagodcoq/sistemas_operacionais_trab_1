Esta linha no InterControllerSim faz com que filhos mortos nao virem zumbis.

```C
  signal(SIGCHLD, SIG_IGN);
```

InterControllerSim tem que criar um filho para realizar o controle de IO para um sleep nao atrapalhar o outro.

```C
void IOhandler(int signal) {
  if (fork() == 0) {
```

Ideia de syscall (pedido de IO) dos filhos:
- Cada filho envia SIGRTMIN e envia em uma shmem seu PID.
- SIGRTMIN eh um sinal de usuario
* Como as chamadas de IO so existem para um unico dispositivo, essa informacao sera omitida. Informacoes sobre o tipo de pedido ('R' e 'W') serao usadas por didatismo, sem efeito pratico.
- SIGRTMIN nao funciona no mac, tem que ser rodado no linux para funcionar este projeto.

# Chamadas de IO de cada filho:
filho1:
 - R: 5, 13
 - W: 27
filho2:
 - R: 8, 19
filho3:
 - R: 2, 10
 - W: 25
filho4:
 - R: 25
 - W: 9
filho5:
 - W: 15
filho6:
 - (Nenhum)
