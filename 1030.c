#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    int n;
    int k;
    int result;
} ThreadData;

int F(int n, int k) {
    if (n == 1) {
        return 0;
    }
    return (F(n - 1, k) + k) % n;
}

void *f_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    int n = data->n;
    int k = data->k;
    int result = F(n, k) + 1;

    data->result = result;

    pthread_exit(NULL);
}

double obter_tempo_atual() {
    struct timeval tempo;
    gettimeofday(&tempo, NULL);
    return tempo.tv_sec + tempo.tv_usec / 1000000.0;
}

int main() {
    int X;
    scanf("%d", &X);

    ThreadData data[X];
    pthread_t threads[X];

    double inicio, fim;
    double tempo_serial = 0.0, tempo_paralelo = 0.0;
    int num_threads = 2;

    for (int i = 0; i < X; i++) {
        int n, k;
        scanf("%d %d", &n, &k);

        data[i].n = n;
        data[i].k = k;
    }

    inicio = obter_tempo_atual();
    for (int i = 0; i < X; i++) {
        data[i].result = F(data[i].n, data[i].k) + 1;
    }
    fim = obter_tempo_atual();
    tempo_serial = fim - inicio;

    inicio = obter_tempo_atual();
    for (int i = 0; i < X; i++) {
        pthread_create(&threads[i], NULL, f_thread, (void *)&data[i]);
    }

    for (int i = 0; i < X; i++) {
        pthread_join(threads[i], NULL);
    }
    fim = obter_tempo_atual();
    tempo_paralelo = fim - inicio;

    for (int i = 0; i < X; i++) {
        printf("Case %d: %d\n", i + 1, data[i].result);
    }

    double speedup = tempo_serial / tempo_paralelo;
    printf("SpeedUp = %.2lf\n", speedup);

    return 0;
}
