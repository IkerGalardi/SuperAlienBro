INCLUDES=-Isrc/ -Iext/glad/include -Iext/stb_image/include -Iext/cglm/include
CFLAGS=-march=native $(INCLUDES) -Wall -Wextra -ggdb
LDFLAGS=-lglfw -lm

OBJ_FILES=bin/main.o \
		  bin/glad.o \
		  bin/game.o \
		  bin/stb_image.o \
		  bin/tileset.o \
		  bin/animation.o \
		  bin/globals.o \
		  bin/background.o \
		  bin/level.o \
		  bin/common.o

STATIC_LIBS=bin/libcglm.a

all: SuperAlienBro

run: SuperAlienBro
	./SuperAlienBro

SuperAlienBro: $(OBJ_FILES) $(STATIC_LIBS)
	gcc -o SuperAlienBro $(OBJ_FILES) $(CFLAGS) $(LDFLAGS) -Lbin/ -lcglm

bin/%.o: src/%.c
	gcc -c -o $@ $< $(CFLAGS)

bin/%.o: src/gfx/%.c
	gcc -c -o $@ $< $(CFLAGS)

bin/glad.o: ext/glad/src/glad.c
	gcc -c -o $@ $< $(CFLAGS)

bin/stb_image.o: ext/stb_image/src/stb_image.c
	gcc -c -o $@ $< $(CFLAGS)

bin/libcglm.a:
	sh build-cglm.sh

clean:
	rm -f $(OBJ_FILES) $(STATIC_LIBS) SuperAlienBro