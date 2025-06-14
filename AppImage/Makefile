CC=gcc
CFLAGS = -c -O2 -mtune=generic -pipe -Wall -fPIC
LDFLAGS = -lGL -lSDL2 -lSDL2_image -lGLEW
SOURCES = Sources/mainapp.c \
        Sources/appimage.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=Img/AppImage-Runner

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf Sources/appimage.o Sources/main.o
