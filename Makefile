CFLAGS=-O2 -march=native

OBJ_FILES=bin/main.o

all: SuperAlienBro

SuperAlienBro: $(OBJ_FILES)
	gcc -o SuperAlienBro $(OBJ_FILES) $(CFLAGS)

bin/%.o: src/%.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ_FILES) SuperAlienBro