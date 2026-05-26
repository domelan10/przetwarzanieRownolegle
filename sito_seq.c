#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define N 17
#define EPS 1e-7
#define MAX_ITER 500

void getEigenvalues(double m[N][N], int n, double evals[N])
{
    for (int it = 0; it < MAX_ITER; it++)
    {
        double max_v = 0;
        int p = 0, q = 1;
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (fabs(m[i][j]) > max_v)
                {
                    max_v = fabs(m[i][j]);
                    p = i;
                    q = j;
                }
            }
        }
        if (max_v < 1e-12)
            break;

        double theta = (m[q][q] - m[p][p]) / (2.0 * m[p][q]);
        double t;
        if (fabs(theta) < 1e10)
        {
            t = (theta >= 0) ? 1.0 / (theta + sqrt(1 + theta * theta)) : -1.0 / (-theta + sqrt(1 + theta * theta));
        }
        else
        {
            t = 0.5 / theta;
        }

        double c = 1.0 / sqrt(1 + t * t);
        double s = t * c;
        double tau = s / (1.0 + c);
        double apq = m[p][q];

        m[p][p] -= t * apq;
        m[q][q] += t * apq;
        m[p][q] = m[q][p] = 0;

        for (int i = 0; i < n; i++)
        {
            if (i != p && i != q)
            {
                double aip = m[i][p], aiq = m[i][q];
                m[i][p] = m[p][i] = aip - s * (aiq + aip * tau);
                m[i][q] = m[q][i] = aiq + s * (aip - aiq * tau);
            }
        }
    }
    for (int i = 0; i < n; i++)
        evals[i] = m[i][i];
}

int decodeGraph6(const char *g6, double adj[N][N])
{
    if (g6[0] < 63 || g6[0] > 126)
        return 0;
    int n = g6[0] - 63;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            adj[i][j] = 0.0;

    int char_ptr = 1;
    int bit_ptr = 0;
    int current_val = 0;

    for (int j = 1; j < n; j++)
    {
        for (int i = 0; i < j; i++)
        {
            if (bit_ptr == 0)
            {
                current_val = g6[char_ptr++] - 63;
                bit_ptr = 6;
            }
            if ((current_val >> (bit_ptr - 1)) & 1)
            {
                adj[i][j] = adj[j][i] = 1.0;
            }
            bit_ptr--;
        }
    }
    return n;
}

int decodeInput(const char *line, double adj[N][N])
{
    int len = strlen(line);
    if (len == (N * (N - 1) / 2) && (line[0] == '0' || line[0] == '1'))
    {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                adj[i][j] = 0.0;
        int idx = 0;
        for (int i = 0; i < N; i++)
            for (int j = i + 1; j < N; j++)
            {
                if (line[idx++] == '1')
                    adj[i][j] = adj[j][i] = 1.0;
            }
        return N;
    }
    return decodeGraph6(line, adj);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f)
        return 1;

    long max_lines = -1;
    if (argc >= 3)
    {
        max_lines = atol(argv[2]);
        if (max_lines <= 0)
        {
            fclose(f);
            return 1;
        }
    }

    char line[1024];
    double adj[N][N], evals[N];
    int id = 1;
    long lines_read = 0;

    clock_t start_time = clock();

    while ((max_lines == -1 || lines_read < max_lines) && fscanf(f, "%1023s", line) == 1)
    {
        lines_read++;

        int n = decodeInput(line, adj);
        if (n <= 0)
            continue;

        getEigenvalues(adj, n, evals);

        int isIntegral = 1;
        for (int i = 0; i < n; i++)
        {
            double rounded = round(evals[i]);
            if (fabs(evals[i] - rounded) > EPS)
            {
                isIntegral = 0;
                break;
            }
        }

        if (isIntegral)
        {
            printf("%d: %s\n", id++, line);
            fflush(stdout);
        }
    }

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Czas dzialania dla %ld grafow: %f s\n", lines_read, time_spent);

    fclose(f);
    return 0;
}