CC = gcc
CFLAGS = -O3 -Wall
LDFLAGS = -lm
OMP_FLAGS = -fopenmp
NVCC = nvcc

TARGETS = gen_c.bin generator_s.bin generator.bin sito_seq.bin sito_openmp.bin sito_cuda.bin prop_json.bin grafy_17_35.g6

all: $(TARGETS)

gen_c.bin: gen_c.c
	$(CC) $(CFLAGS) gen_c.c -o gen_c.bin $(LDFLAGS)

generator_s.bin: generator_s.c
	$(CC) $(CFLAGS) generator_s.c -o generator_s.bin

generator.bin: generator.c
	$(CC) $(CFLAGS) generator.c -o generator.bin

sito_seq.bin: sito_seq.c
	$(CC) $(CFLAGS) sito_seq.c -o sito_seq.bin $(LDFLAGS)

sito_openmp.bin: sito_openmp.c
	$(CC) $(CFLAGS) $(OMP_FLAGS) sito_openmp.c -o sito_openmp.bin $(LDFLAGS)

sito_cuda.bin: sito_cuda.cu
	$(NVCC) -O3 -Xcompiler -fopenmp sito_cuda.cu -o sito_cuda.bin -lgomp

prop_json.bin: prop_json.cu
	$(NVCC) prop_json.cu -o prop_json.bin
	./prop_json.bin > devices.json

grafy_17_35.g6:
	nauty-genbg -c 8 9 35:35 0/1024 > grafy_17_35.g6

clean:
	rm -f $(TARGETS)