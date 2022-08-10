generator: 3d.o
	gcc -Wall -Werror -std=c11 -ggdb3 -o generator generator.c 3d.o
3d.o: 3d.c
	gcc -Wall -Werror -std=c11 -ggdb3 -c 3d.c
clean:
	-rm 3d.o generator