.PHONY: all clean
all:
	g++ -fopenmp -lpthread main.cpp vector3.cpp -o main.exe
run: all
	.\main.exe .\dragon.obj .\1.obj 0.1
clean:
	rm *.o