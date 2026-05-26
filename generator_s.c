#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 17
#define K 35
#define KOD_LEN (N * (N - 1) / 2)

int isConnected(int adj[N][N])
{
    int visited[N] = {0};
    int queue[N];
    int head = 0, tail = 0;

    visited[0] = 1;
    queue[tail++] = 0;

    while (head < tail)
    {
        int v = queue[head++];
        for (int i = 0; i < N; i++)
        {
            if (adj[v][i] && !visited[i])
            {
                visited[i] = 1;
                queue[tail++] = i;
            }
        }
    }

    return (tail == N);
}

void generateToFile(const char *filename, long count)
{
    FILE *f = fopen(filename, "w");
    if (!f)
    {
        return;
    }

    int edges[KOD_LEN];
    int adj[N][N];
    char kod[KOD_LEN + 1];

    long found = 0;
    while (found < count)
    {
        for (int i = 0; i < KOD_LEN; i++)
            edges[i] = (i < K) ? 1 : 0;

        for (int i = KOD_LEN - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            int temp = edges[i];
            edges[i] = edges[j];
            edges[j] = temp;
        }

        memset(adj, 0, sizeof(adj));
        int idx = 0;
        for (int i = 0; i < N; i++)
        {
            for (int j = i + 1; j < N; j++)
            {
                if (edges[idx] == 1)
                {
                    adj[i][j] = adj[j][i] = 1;
                }
                idx++;
            }
        }

        if (isConnected(adj))
        {
            for (int i = 0; i < KOD_LEN; i++)
            {
                kod[i] = edges[i] ? '1' : '0';
            }
            kod[KOD_LEN] = '\0';
            fprintf(f, "%s\n", kod);
            found++;
        }
    }

    fclose(f);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        return 1;
    }

    srand(time(NULL));
    generateToFile(argv[1], atol(argv[2]));
    return 0;
}