.PHONY: trace clean

trace:
	gcc main.c -o trace $(shell sdl2-config --cflags --libs) -lm
	./trace
clean:
	rm ./trace
