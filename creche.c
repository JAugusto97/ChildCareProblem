#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#define N_CRIANCAS 8
#define N_ADULTOS 3

int _criancas = 0, _adultos = 0, _criancas_esperando = 0, _adultos_saindo = 0;  // Variaveis globais.

sem_t fila_adultos;
sem_t fila_criancas;
pthread_mutex_t mutex;

int minimo(int a, int b){
  if(a < b)
    return a;
  return b;
}

void * criancasThread() {

  pthread_mutex_lock(&mutex); // Entra na RC

  if(_criancas < 3 * _adultos){ // Se existe uma vaga
    _criancas++;                // Entra
    printf("Crianca %X entrou\n ===>Variaveis globais: _crianças = %d; _adultos = %d; _criancas_esperando = %d; _adultos_saindo = %d;\n\n",(int) pthread_self(), _criancas, _adultos, _criancas_esperando, _adultos_saindo);
    pthread_mutex_unlock(&mutex); // Sai da RC
  }
  else{       // Nao tem vaga
    _criancas_esperando++;  // +1 crianca esperando para entrar
    printf("Crianca %X tentando entrar\n ===>Variaveis globais: _crianças = %d; _adultos = %d; _criancas_esperando = %d; _adultos_saindo = %d;\n\n",(int) pthread_self(), _criancas, _adultos, _criancas_esperando, _adultos_saindo);
    sleep(2);
    pthread_mutex_unlock(&mutex); // Sai da RC
    sem_wait(&fila_criancas); // crianca dorme ate liberar uma vaga
  }


  //Logica de espera
  sleep(8);


  pthread_mutex_lock(&mutex);
  _criancas--;
  printf("Crianca %X saiu\n ===>Variaveis globais: _crianças = %d; _adultos = %d; _criancas_esperando = %d; _adultos_saindo = %d;\n\n",(int) pthread_self(), _criancas, _adultos, _criancas_esperando, _adultos_saindo);
  sleep(2);

  if(_adultos_saindo && (_criancas <= 3 * (_adultos - 1))){  // Se tem adulto querendo sair e o nro de criancas tirando 1 adulto ainda eh seguro
    _adultos_saindo--;  // Decrementa a fila de adultos querendo sair
    _adultos--;
    printf("Adulto %X saiu\n ===>Variaveis globais: _crianças = %d; _adultos = %d; _criancas_esperando = %d; _adultos_saindo = %d;\n\n",(int) pthread_self(), _criancas, _adultos, _criancas_esperando, _adultos_saindo);
    sleep(2);
    sem_post(&fila_adultos);  // Acorda um adulto que queria sair e ele sai
  }
  if(_criancas_esperando && (_criancas < 3 * _adultos)){ // Se tem criancas esperando e tem espaco pra ela
    sem_post(&fila_criancas); // Ela entra
    _criancas_esperando--;  // -1 crianca esperando
    _criancas++;    // +1 crianca dentro
    printf("Crianca %X entrou\n ===>Variaveis globais: _crianças = %d; _adultos = %d; _criancas_esperando = %d; _adultos_saindo = %d;\n\n",(int) pthread_self(), _criancas, _adultos, _criancas_esperando, _adultos_saindo);
    sleep(2);
  }
  pthread_mutex_unlock(&mutex); // Sai da RC

  pthread_exit(NULL);
}

void * adultosThread() {

  int n, i;

  pthread_mutex_lock(&mutex); // Entra na RC
  _adultos++; // +1 adulto na creche
  printf("Adulto %X entrou\n ===>Variaveis globais: _crianças = %d; _adultos = %d; _criancas_esperando = %d; _adultos_saindo = %d;\n\n",(int) pthread_self(), _criancas, _adultos, _criancas_esperando, _adultos_saindo);
  if (_criancas_esperando){   // Se tem criancas esperando
    n = minimo(3, _criancas_esperando); // Pega n criancas da fila, com n <= 3

    for (i = 0; i < n; i++) {
      sem_post(&fila_criancas); // Acorda n criancas
      _criancas_esperando--;  // -n criancas na fila
      _criancas++;  // +n criancas dentro
      printf("Crianca %X entrou\n ===>Variaveis globais: _crianças = %d; _adultos = %d; _criancas_esperando = %d; _adultos_saindo = %d;\n\n",(int) pthread_self(), _criancas, _adultos, _criancas_esperando, _adultos_saindo);
      sleep(2);
    }

  }

  pthread_mutex_unlock(&mutex); // sai da RC


  //Logica de espera
  sleep(4);


  pthread_mutex_lock(&mutex); // Entra na RC
  if ((_criancas <= 3 * (_adultos - 1)) && ((_adultos > 1) || (_criancas_esperando == 0 && _criancas == 0))){
    _adultos--;
    printf("Adulto %X saiu\n ===>Variaveis globais: _crianças = %d; _adultos = %d; _criancas_esperando = %d; _adultos_saindo = %d;\n\n",(int) pthread_self(), _criancas, _adultos, _criancas_esperando, _adultos_saindo);
    sleep(2);
    pthread_mutex_unlock(&mutex);
  }else{
    _adultos_saindo++;
    printf("Adulto %X tentando sair\n ===>Variaveis globais: _crianças = %d; _adultos = %d; _criancas_esperando = %d; _adultos_saindo = %d;\n\n",(int) pthread_self(), _criancas, _adultos, _criancas_esperando, _adultos_saindo);
    sleep(2);
    pthread_mutex_unlock(&mutex);
    sem_wait(&fila_adultos);
  }

  pthread_exit(NULL);

}

int main(int argc, char const *argv[]) {

  sem_init(&fila_adultos, 0, 0);
  sem_init(&fila_criancas, 0, 0);
  pthread_mutex_init(&mutex, NULL);


  int n_criancas, n_adultos, retorno;

  if (argc == 1) { // Nao passou o numero de criancas e adultos
    n_criancas = N_CRIANCAS;
    n_adultos = N_ADULTOS;
  }

  if (argc > 3) { // Passou mais do que 2 parametros
    printf("Programa %s com número de entradas incompativel! (%d)\n", argv[0], argc);
    exit(0);
  }

  if (argc == 3){
    n_criancas = atoi(argv[1]);
    n_adultos = atoi(argv[2]);
  }

  printf("Estado Inicial do programa \n\n ===>Variaveis globais: _crianças = %d; _adultos = %d; _criancas_esperando = %d; _adultos_saindo = %d;\n\n", _criancas, _adultos, _criancas_esperando, _adultos_saindo);


  pthread_t* threads_criancas = (pthread_t*) malloc(sizeof(pthread_t) * n_criancas);
  pthread_t* threads_adultos = (pthread_t*) malloc(sizeof(pthread_t) * n_adultos);

  printf("crianca %d\n", n_criancas);
  printf("adulto %d\n", n_adultos);

  for (int i=0; i<n_criancas; i++){
    retorno = pthread_create(&threads_criancas[i], NULL, criancasThread, NULL);
    if (retorno) {  // Erro na criacao da thread crianca
      printf("Erro na criação de uma thread criança!\n");
      exit(0);
    }
  }

  for (int i=0; i<n_adultos; i++){
    retorno = pthread_create(&threads_adultos[i], NULL, adultosThread, NULL);
    if (retorno) { // Erro na criacao da thread adulto
      printf("Erro na criação de uma thread adulto!\n");
      exit(0);
    }
  }

  for (int i=0; i<n_adultos; i++)
    pthread_join(threads_adultos[i], NULL);
  for (int i=0; i<n_criancas; i++)
    pthread_join(threads_criancas[i], NULL);

  free(threads_criancas);
  free(threads_adultos);

  return 0;
}
