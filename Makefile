all: debug
	g++ src/main.cpp -o easyjson

debug:
	g++ src/main.cpp -g -DPRINT_DEBUG -o easyjson_debug

clean:
	rm easyjson easyjson_debug
