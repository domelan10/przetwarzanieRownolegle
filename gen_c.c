#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define N 17
#define K 35
#define MAX_EDGES (N * (N - 1) / 2)
#define EPSILON 1e-6
#define MAX_ITER 500
#define INITIAL_TEMP 0.1
#define COOLING_RATE 0.9999

// --- Funkcje pomocnicze ---

int is_connected(int adj[N][N])
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

void get_eigenvalues(int adj[N][N], double *evals)
{
    double A[N][N];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = (double)adj[i][j];

    for (int iter = 0; iter < MAX_ITER; iter++)
    {
        int p = 0, q = 1;
        double max_val = fabs(A[0][1]);
        for (int i = 0; i < N; i++)
        {
            for (int j = i + 1; j < N; j++)
            {
                if (fabs(A[i][j]) > max_val)
                {
                    max_val = fabs(A[i][j]);
                    p = i;
                    q = j;
                }
            }
        }
        if (max_val < 1e-12)
            break;

        double theta = (A[q][q] - A[p][p]) / (2.0 * A[p][q]);
        double t = (theta >= 0) ? 1.0 / (theta + sqrt(1.0 + theta * theta))
                                : -1.0 / (-theta + sqrt(1.0 + theta * theta));
        double c = 1.0 / sqrt(1.0 + t * t);
        double s = t * c;
        double tau = s / (1.0 + c);

        A[p][p] -= t * A[p][q];
        A[q][q] += t * A[p][q];
        A[p][q] = A[q][p] = 0.0;

        for (int i = 0; i < N; i++)
        {
            if (i != p && i != q)
            {
                double Aip = A[i][p], Aiq = A[i][q];
                A[i][p] = A[p][i] = Aip - s * (Aiq + Aip * tau);
                A[i][q] = A[q][i] = Aiq + s * (Aip - Aiq * tau);
            }
        }
    }
    for (int i = 0; i < N; i++)
        evals[i] = A[i][i];
}

double calculate_energy(int adj[N][N])
{
    double evals[N];
    get_eigenvalues(adj, evals);
    double energy = 0;
    for (int i = 0; i < N; i++)
    {
        double dist = fabs(round(evals[i]) - evals[i]);
        energy += dist * dist; // Suma kwadratów odchyleń
    }
    return energy;
}

int main()
{
    srand(time(NULL));
    int adj[N][N] = {0};
    int edges_list[MAX_EDGES][2];
    int current_edges[K];
    int idx = 0;

    // Tworzymy listę wszystkich możliwych krawędzi
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            edges_list[idx][0] = i;
            edges_list[idx][1] = j;
            idx++;
        }
    }

    // --- Pętla główna (Restarty) ---
    while (true)
    {
        // 1. Inicjalizacja losowa (spójna)
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                adj[i][j] = 0;

        bool taken[MAX_EDGES] = {false};
        int count = 0;

        // Gwarantujemy spójność na starcie (prosty łańcuch 0-1-2-...-16)
        // To znacznie ułatwia proces poszukiwań

        while (count < K)
        {
            int r;
            r = rand() % MAX_EDGES;
            if (taken[r])
                continue;
            taken[r] = true;
            current_edges[count++] = r;
            adj[edges_list[r][0]][edges_list[r][1]] = adj[edges_list[r][1]][edges_list[r][0]] = 1;
        }

        double current_energy = calculate_energy(adj);
        double temperature = INITIAL_TEMP;

        // 2. Simulated Annealing (Zamiast zwykłego Hill Climbing)
        for (int step = 0; step < 20000; step++)
        {
            if (current_energy < 1e-7 && is_connected(adj))
            {
                printf("\n!!! ZNALEZIONO GRAF CAŁKOWITY !!!\nKod: ");
                char kod[MAX_EDGES + 1];
                memset(kod, '0', MAX_EDGES);
                kod[MAX_EDGES] = '\0';
                for (int i = 0; i < K; i++)
                    kod[current_edges[i]] = '1';
                printf("%s\n", kod);
                printf("\n");
                return 0; // Sukces
            }

            // Próbuj zamienić krawędź
            int edge_to_remove_idx = rand() % K;
            int old_edge_global_idx = current_edges[edge_to_remove_idx];

            int new_edge_global_idx;
            do
            {
                new_edge_global_idx = rand() % MAX_EDGES;
            } while (taken[new_edge_global_idx]);

            // Wykonaj ruch
            adj[edges_list[old_edge_global_idx][0]][edges_list[old_edge_global_idx][1]] = 0;
            adj[edges_list[old_edge_global_idx][1]][edges_list[old_edge_global_idx][0]] = 0;
            adj[edges_list[new_edge_global_idx][0]][edges_list[new_edge_global_idx][1]] = 1;
            adj[edges_list[new_edge_global_idx][1]][edges_list[new_edge_global_idx][0]] = 1;

            double new_energy = calculate_energy(adj);

            // Kryterium Metropolisa (Simulated Annealing)
            double prob = exp((current_energy - new_energy) / temperature);
            if (new_energy < current_energy || (double)rand() / RAND_MAX < prob)
            {
                // Zaakceptuj zmianę
                current_energy = new_energy;
                current_edges[edge_to_remove_idx] = new_edge_global_idx;
                taken[old_edge_global_idx] = false;
                taken[new_edge_global_idx] = true;
                printf("\rEnergia: %f (Krok: %d, T: %.5f)   ", current_energy, step, temperature);
                fflush(stdout);
            }
            else
            {
                // Cofnij zmianę
                adj[edges_list[old_edge_global_idx][0]][edges_list[old_edge_global_idx][1]] = 1;
                adj[edges_list[old_edge_global_idx][1]][edges_list[old_edge_global_idx][0]] = 1;
                adj[edges_list[new_edge_global_idx][0]][edges_list[new_edge_global_idx][1]] = 0;
                adj[edges_list[new_edge_global_idx][1]][edges_list[new_edge_global_idx][0]] = 0;
            }

            temperature *= COOLING_RATE; // Chłodzenie
        }
        printf("\nRestartowanie (utknięto w lokalnym minimum)...\n");
    }

    return 0;
}