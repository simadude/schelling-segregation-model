build/bin/schelling: build/ui.o build/agentgrid.o build/main.o
	mkdir -p build/bin
	g++ -lfltk build/main.o build/ui.o build/agentgrid.o -o build/bin/schelling

build/main.o: src/main.cpp src/ui.h
	g++ -lfltk -c src/main.cpp -I src -o build/main.o

build/ui.o: src/ui.cpp src/ui.h
	mkdir -p build
	g++ -lfltk -c src/ui.cpp -I src -o build/ui.o

build/agentgrid.o: src/agentgrid.cpp src/agentgrid.h
	mkdir -p build
	g++ -lfltk -c src/agentgrid.cpp -I src -o build/agentgrid.o

clean:
	rm -rf build
