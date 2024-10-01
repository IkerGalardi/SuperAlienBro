CFLAGS=-O2 -march=native -Iext/glad/include -Iext/stb_image/include -Wall -Wextra
LDFLAGS=-lglfw -lm

OBJ_FILES=bin/main.o \
          bin/glad.o \
		  bin/game.o \
		  bin/stb_image.o \
		  bin/tilemap.o

all: SuperAlienBro

run: SuperAlienBro
	./SuperAlienBro

SuperAlienBro: $(OBJ_FILES)
	gcc -o SuperAlienBro $(OBJ_FILES) $(CFLAGS) $(LDFLAGS)

bin/%.o: src/%.c
	gcc -c -o $@ $< $(CFLAGS)

bin/glad.o: ext/glad/src/glad.c
	gcc -c -o $@ $< $(CFLAGS)

bin/stb_image.o: ext/stb_image/src/stb_image.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ_FILES) SuperAlienBro