default: aeif

aeif: aeif.cpp
	g++ aeif.cpp -o aeif -std=c++20 -g -O0

run: aeif
	./aeif $(ARGS)

clean:
	rm aeif
