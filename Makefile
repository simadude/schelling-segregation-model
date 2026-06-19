build/bin/schelling:
	mkdir -p build/bin
	g++ -lfltk build/ui.o src/main.cpp -I build -o build/bin/schelling

build/ui.cxx:
	cd build\
	fluid -c ../src/ui.fld

build/ui.o: build/ui.cxx build/ui.h
	g++ -lfltk -c ui.cxx -o ui.o