#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    int a;
    int b;
    int soma;
} ThreadData;

void *soma_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    int a = data->a;
    int b = data->b;
    int soma = a + b;

    data->soma = soma;

    pthread_exit(NULL);
}

int calcular_soma(int a, int b) {
    ThreadData data;
    data.a = a;
    data.b = b;

    pthread_t thread;
    pthread_create(&thread, NULL, soma_thread, (void *)&data);
    pthread_join(thread, NULL);

    return data.soma;
}

double obter_tempo_atual() {
    struct timeval tempo;
    gettimeofday(&tempo, NULL);
    return tempo.tv_sec + tempo.tv_usec / 1000000.0;
}

int main() {
    int a, b;
    scanf("%d", &a);
    scanf("%d", &b);

    double inicio, fim;
    int soma_serial, soma_paralela;
    int num_threads = 2;

    inicio = obter_tempo_atual();
    soma_serial = calcular_soma(a, b);
    fim = obter_tempo_atual();
    double tempo_serial = fim - inicio;

    inicio = obter_tempo_atual();
    ThreadData data[num_threads];
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        data[i].a = a;
        data[i].b = b;
        pthread_create(&threads[i], NULL, soma_thread, (void *)&data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    soma_paralela = (data[0].soma + data[1].soma);
    fim = obter_tempo_atual();
    double tempo_paralelo = fim - inicio;

    printf("SOMA Serial = %d\n", soma_serial);
    printf("SOMA Paralela = %d\n", soma_paralela);

    double speedup = tempo_serial / tempo_paralelo;
    printf("SpeedUp = %.2lf\n", speedup);

    return 0;
}
