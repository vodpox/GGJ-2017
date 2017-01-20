CC = g++
MINGW = i686-w64-mingw32-g++
CPP = $(shell find . -name "*.cpp")
H = $(shell find . -name "*.hpp")
LDFLAGS = -I ~/Dev/tplay/Terminal-Play/src -L ~/Dev/tplay/Terminal-Play/build -lterminalplay -O3

run: build
	./game

build: $(CPP) $(H)
	$(CC) -std=c++11 -static $(LDFLAGS) -c $(CPP) $(H)
	$(CC) *.o $(LDFLAGS) -o game -static
	rm -f $(shell find . -name "*.gch")

windows: $(CPP) $(H)
	$(MINGW) -std=c++11 -static $(LDFLAGS) -c $(CPP) $(H)
	$(MINGW) *.o $(LDFLAGS) -static -o game.exe

clean:
	rm -rf *.o
	rm -f $(shell find . -name "*.gch")
