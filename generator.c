#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 17
#define N1 7
#define N2 10
#define K 35
#define KOD_LEN (N * (N - 1) / 2)

int getKodIdx(int r, int c)
{
    int i = r;
    int j = c + N1;
    return (i * N - (i * (i + 1)) / 2) + (j - i - 1);
}

void generateToFile(const char *filename, long count)
{
    FILE *f = fopen(filename, "w");
    if (!f)
    {
        return;
    }

    int total_slots = N1 * N2;
    int *edges = malloc(total_slots * sizeof(int));
    char kod[KOD_LEN + 1];

    for (long g = 0; g < count; g++)
    {
        for (int i = 0; i < total_slots; i++)
        {
            edges[i] = (i < K) ? 1 : 0;
        }

        for (int i = total_slots - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            int temp = edges[i];
            edges[i] = edges[j];
            edges[j] = temp;
        }

        memset(kod, '0', KOD_LEN);
        kod[KOD_LEN] = '\0';

        for (int i = 0; i < total_slots; i++)
        {
            if (edges[i] == 1)
            {
                int r = i / N2;
                int c = i % N2;
                kod[getKodIdx(r, c)] = '1';
            }
        }

        fprintf(f, "%s\n", kod);
    }

    fclose(f);
    free(edges);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        return 1;
    }

    srand(time(NULL));
    const char *filename = argv[1];
    long count = atol(argv[2]);

    generateToFile(filename, count);

    return 0;
}