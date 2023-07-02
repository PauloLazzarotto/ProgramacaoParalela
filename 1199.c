#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    char linha[10];
    int resultado;
} ThreadData;

void *converter_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    char *ptr;
    if (data->linha[1] == 'x') {
        data->resultado = strtol(data->linha, &ptr, 16);
    } else {
        data->resultado = strtol(data->linha, &ptr, 10);
    }

    pthread_exit(NULL);
}

double obter_tempo_atual() {
    struct timeval tempo;
    gettimeofday(&tempo, NULL);
    return tempo.tv_sec + tempo.tv_usec / 1000000.0;
}

int main() {
    ThreadData data;
    pthread_t thread;

    double inicio, fim;
    double tempo_serial = 0.0, tempo_paralelo = 0.0;

    scanf("%s", data.linha);

    inicio = obter_tempo_atual();
    while (data.linha[0] != '-') {
        char *ptr;
        if (data.linha[1] == 'x') {
            data.resultado = strtol(data.linha, &ptr, 16);
        } else {
            data.resultado = strtol(data.linha, &ptr, 10);
        }

        if (data.linha[1] == 'x') {
            printf("%d\n", data.resultado);
        } else {
            printf("0x%X\n", data.resultado);
        }

        scanf("%s", data.linha);
    }
    fim = obter_tempo_atual();
    tempo_serial = fim - inicio;

    inicio = obter_tempo_atual();
    while (data.linha[0] != '-') {
        pthread_create(&thread, NULL, converter_thread, (void *)&data);
        pthread_join(thread, NULL);

        if (data.linha[1] == 'x') {
            printf("%d\n", data.resultado);
        } else {
            printf("0x%X\n", data.resultado);
        }

        scanf("%s", data.linha);
    }
    fim = obter_tempo_atual();
    tempo_paralelo = fim - inicio;

    double speedup = tempo_serial / tempo_paralelo;
    printf("SpeedUp = %.2lf\n", speedup);

    return 0;
}
