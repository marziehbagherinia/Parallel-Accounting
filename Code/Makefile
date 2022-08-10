all : fileDir Store City State Worker StoreCalculator.out

fileDir:
	@mkdir -p build
	@mkdir -p bin
	@mkdir -p namedPipe

# Store : 
Store : build/StoreMain.o build/Store.o build/Tools.o
	g++ build/StoreMain.o build/Store.o build/Tools.o -o bin/Store
	
build/StoreMain.o : src/StoreMain.cpp src/Store.cpp src/Store.h
	g++ -c src/StoreMain.cpp -o build/StoreMain.o

build/Store.o :  src/Store.cpp src/Store.h
	g++ -c src/Store.cpp -o build/Store.o
	
# City : 
City : build/CityMain.o build/City.o build/Tools.o
	g++ build/CityMain.o build/City.o build/Tools.o -o bin/City
	
build/CityMain.o : src/CityMain.cpp src/City.cpp src/City.h
	g++ -c src/CityMain.cpp -o build/CityMain.o

build/City.o :  src/City.cpp src/City.h
	g++ -c src/City.cpp -o build/City.o

# State : 
State : build/StateMain.o build/State.o build/Tools.o
	g++ build/StateMain.o build/State.o build/Tools.o -o bin/State
	
build/StateMain.o : src/StateMain.cpp src/State.cpp src/State.h
	g++ -c src/StateMain.cpp -o build/StateMain.o

build/State.o :  src/State.cpp src/State.h
	g++ -c src/State.cpp -o build/State.o

# Worker : 
Worker : build/WorkerMain.o build/Worker.o build/Tools.o
	g++ build/WorkerMain.o build/Worker.o build/Tools.o -o bin/Worker
	
build/WorkerMain.o : src/WorkerMain.cpp src/Worker.cpp src/Worker.h
	g++ -c src/WorkerMain.cpp -o build/WorkerMain.o

build/Worker.o :  src/Worker.cpp src/Worker.h
	g++ -c src/Worker.cpp -o build/Worker.o

# LoadBalancer
StoreCalculator.out :  build/Main.o build/LoadBalancer.o build/Tools.o

	g++ build/Main.o build/LoadBalancer.o build/Tools.o -o  StoreCalculator.out

build/Main.o : src/Main.cpp src/LoadBalancer.h src/LoadBalancer.cpp 
	g++ -c src/Main.cpp -o build/Main.o

build/Tools.o : src/Tools.cpp src/Tools.h
	g++ -c src/Tools.cpp -o build/Tools.o
	
build/LoadBalancer.o : src/LoadBalancer.cpp src/LoadBalancer.h
	g++ -c src/LoadBalancer.cpp -o build/LoadBalancer.o

clean :
	rm -f -r build
	rm -f -r bin
	rm -f -r namedPipe
	rm -f StoreCalculator.out
