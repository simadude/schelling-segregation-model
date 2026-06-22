build/bin/schelling: build/ui.o build/agentgrid.o build/main.o
	mkdir -p build/bin
	g++ build/main.o build/ui.o build/agentgrid.o -lfltk -o build/bin/schelling

build/main.o: src/main.cpp src/ui.h
	g++ -c src/main.cpp -I src -lfltk -o build/main.o

build/ui.o: src/ui.cpp src/ui.h
	mkdir -p build
	g++ -c src/ui.cpp -I src -lfltk -o build/ui.o

build/agentgrid.o: src/agentgrid.cpp src/agentgrid.h
	mkdir -p build
	g++ -c src/agentgrid.cpp -I src -o build/agentgrid.o

build/test_agentgrid.o: tests/test_agentgrid.cpp src/agentgrid.h
	mkdir -p build
	g++ -c tests/test_agentgrid.cpp -I src -o build/test_agentgrid.o

build/bin/test_agentgrid: build/test_agentgrid.o build/agentgrid.o
	mkdir -p build/bin
	g++ build/test_agentgrid.o build/agentgrid.o -o build/bin/test_agentgrid

test: build/bin/test_agentgrid
	./build/bin/test_agentgrid

build/bin/batch_runner: build/batch_runner.o build/agentgrid.o
	mkdir -p build/bin
	g++ build/batch_runner.o build/agentgrid.o -o build/bin/batch_runner

build/batch_runner.o: src/batch_runner.cpp src/agentgrid.h
	mkdir -p build
	g++ -c src/batch_runner.cpp -I src -o build/batch_runner.o

run_batch: build/bin/batch_runner
	./build/bin/batch_runner

clean:
	rm -rf build
