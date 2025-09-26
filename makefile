.PHONY: trace clean

trace:
	gcc main.c -o trace $(shell sdl2-config --cflags --libs) -lm -fsanitize=address -g
	./trace
tracev2:
	gcc mainv2.c -o tracev2 $(shell sdl2-config --cflags --libs) -lm -fsanitize=address
	./tracev2
clean:
	rm ./trace
