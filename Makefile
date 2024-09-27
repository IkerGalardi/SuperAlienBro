CFLAGS=-O2 -march=native
LDFLAGS=-lglfw

OBJ_FILES=bin/main.o

all: SuperAlienBro

SuperAlienBro: $(OBJ_FILES)
	gcc -o SuperAlienBro $(OBJ_FILES) $(CFLAGS) $(LDFLAGS)

bin/%.o: src/%.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ_FILES) SuperAlienBro