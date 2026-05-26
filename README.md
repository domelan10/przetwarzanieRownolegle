# Narzędzia do poszukiwania grafów całkowitych (n=17, k=35)

Ten projekt zawiera zestaw narzędzi przeznaczonych do znajdowania i weryfikacji **grafów całkowitych** — grafów, których wartości własne macierzy sąsiedztwa są liczbami całkowitymi. Narzędzia te są specjalnie dostrojone do przestrzeni poszukiwań grafów o $n=17$ wierzchołkach i $k=35$ krawędziach.

## Komponenty projektu

### Generatory

* **`gen_c.c`**: Narzędzie do wyszukiwania heurystycznego wykorzystujące **symulowane wyżarzanie** (Simulated Annealing). Zaczyna od losowego grafu i „zamienia” krawędzie, aby zminimalizować funkcję „energii” (odległość wartości własnych od najbliższych liczb całkowitych). Jest to najszybszy sposób na znalezienie nowych grafów całkowitych.
* **`generator_s.c`**: Generuje partię losowych grafów **spójnych**.
* **`generator.c`**: Generuje partię losowych grafów **dwudzielnych** (podzielonych na $7 + 10 = 17$ wierzchołków).

### Weryfikatory (Sita)

* **`sito_seq.c`**: Program sekwencyjny, który odczytuje listę grafów (w formacie bitmaski lub Graph6) i wypisuje te, które są całkowite.
* **`sito_openmp.c`**: Wydajna, równoległa wersja sita wykorzystująca bibliotekę OpenMP do użycia wszystkich dostępnych rdzeni procesora.

### Narzędzia pomocnicze

* **`prop_json.cu`**: Narzędzie CUDA, które raportuje właściwości sprzętowe GPU w formacie JSON, przydatne do diagnostyki środowiska.
* **`cgrafy17.js`**: Baza danych znanych grafów całkowitych dla $n=17$ w formacie JSON/JavaScript.

## Kompilacja

Dostarczony plik `Makefile` upraszcza proces budowania programów. Wymagane są: `gcc`, biblioteki `OpenMP` oraz kompilator `nvcc` (CUDA).

```bash
make
```

## Wykorzystanie

### 1. Wyszukiwanie heurystyczne

Uruchom narzędzie do symulowanego wyżarzania, aby wyszukać pojedynczy graf całkowity:

```bash
./gen_c.bin
```

### 2. Generowanie pakietowe i sito

Wygeneruj dużą liczbę grafów kandydackich, a następnie zweryfikuj je za pomocą równoległego sita:

* **Dla ogólnych grafów spójnych:**

    ```bash
    ./generator_s.bin candidates.txt 1000000
    ./sito_seq.bin candidates.txt
    ./sito_openmp.bin candidates.txt
    ```

* **Dla grafów dwudzielnych:**

    ```bash
    ./generator.bin bipartite_candidates.txt 1000000
    ./sito_seq.bin bipartite_candidates.txt
    ./sito_openmp.bin bipartite_candidates.txt
    ```

### 3. Użycie Nauty (Genbg)

Plik Makefile zawiera regułę do generowania grafów dwudzielnych za pomocą narzędzia `nauty-genbg`:

```bash
make grafy_17_35.g6
./sito_seq.bin grafy_17_35.g6
./sito_openmp.bin grafy_17_35.g6
```

## Szczegóły matematyczne

* **Solver wartości własnych**: Wykorzystuje algorytm Jacobiego dla macierzy symetrycznych do znajdowania wartości własnych.
* **Sprawdzanie całkowitości**: Graf jest uznawany za całkowity, jeśli różnica między każdą wartością własną a najbliższą jej liczbą całkowitą jest mniejsza niż $10^{-7}$.
* **Przestrzeń poszukiwań**: Dla $n=17$ istnieje $\binom{17}{2} = 136$ możliwych pozycji na krawędzie. Bitmaska „Kod” reprezentuje te 136 pozycji w porządku nadprzekątniowym (indeksowanie górnotrójkątne).order.

---
*Autor: Dominik Durlik*  
*Data: 05.2026*
