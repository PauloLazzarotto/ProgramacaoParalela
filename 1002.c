#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

typedef struct {
    double raio;
    double area;
} ThreadData;

void *calcula_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    double raio = data->raio;
    double PI = 3.14159;
    double area = PI * pow(raio, 2);

    data->area = area;

    pthread_exit(NULL);
}

double calcular_area(double raio) {
    ThreadData data;
    data.raio = raio;

    pthread_t thread;
    pthread_create(&thread, NULL, calcula_thread, (void *)&data);
    pthread_join(thread, NULL);

    return data.area;
}

double obter_tempo_atual() {
    struct timeval tempo;
    gettimeofday(&tempo, NULL);
    return tempo.tv_sec + tempo.tv_usec / 1000000.0;
}

int main() {
    double raio;
    scanf("%lf", &raio);

    double inicio, fim;
    double area_serial, area_paralela;
    int num_threads = 2;

    inicio = obter_tempo_atual();
    area_serial = calcular_area(raio);
    fim = obter_tempo_atual();
    double tempo_serial = fim - inicio;

    inicio = obter_tempo_atual();
    ThreadData data[num_threads];
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        data[i].raio = raio;
        pthread_create(&threads[i], NULL, calcula_thread, (void *)&data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    area_paralela = data[0].area + data[1].area;
    fim = obter_tempo_atual();
    double tempo_paralelo = fim - inicio;

    printf("Área Serial = %.4lf\n", area_serial);
    printf("Área Paralela = %.4lf\n", area_paralela);

    double speedup = tempo_serial / tempo_paralelo;
    printf("SpeedUp = %.2lf\n", speedup);

    return 0;
}
