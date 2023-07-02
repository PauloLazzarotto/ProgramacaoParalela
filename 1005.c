#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct {
    double nota1;
    double nota2;
    double media;
} ThreadData;

void *media_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    double nota1 = data->nota1;
    double nota2 = data->nota2;
    double media = (nota1 * 3.5 + nota2 * 7.5) / 11;

    data->media = media;

    pthread_exit(NULL);
}

double calcular_media(double nota1, double nota2) {
    ThreadData data;
    data.nota1 = nota1;
    data.nota2 = nota2;

    pthread_t thread;
    pthread_create(&thread, NULL, media_thread, (void *)&data);
    pthread_join(thread, NULL);

    return data.media;
}

double obter_tempo_atual() {
    struct timeval tempo;
    gettimeofday(&tempo, NULL);
    return tempo.tv_sec + tempo.tv_usec / 1000000.0;
}

int main() {
    double nota1, nota2;
    printf("Digite a primeira nota: ");
    scanf("%lf", &nota1);
    printf("Digite a segunda nota: ");
    scanf("%lf", &nota2);

    double inicio, fim;
    double media_serial, media_paralela;
    int num_threads = 2;

    inicio = obter_tempo_atual();
    media_serial = calcular_media(nota1, nota2);
    fim = obter_tempo_atual();
    double tempo_serial = fim - inicio;

    inicio = obter_tempo_atual();
    ThreadData data[num_threads];
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        data[i].nota1 = nota1;
        data[i].nota2 = nota2;
        pthread_create(&threads[i], NULL, media_thread, (void *)&data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    media_paralela = (data[0].media + data[1].media) / 2;
    fim = obter_tempo_atual();
    double tempo_paralelo = fim - inicio;

    printf("MEDIA Serial = %.5lf\n", media_serial);
    printf("MEDIA Paralela = %.5lf\n", media_paralela);

    double speedup = tempo_serial / tempo_paralelo;
    printf("SpeedUp = %.2lf\n", speedup);

    return 0;
}
