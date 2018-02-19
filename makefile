.RECIPEPREFIX +=

gol: avl.o board.o main.o
  gcc -Wall -g -o gol avl.o board.o main.o -lncurses

main.o: avl.o board.o
  gcc -Wall -g -c main.c

board.o: avl.o board.c
  gcc -Wall -g -c board.c

avl.o: 
  gcc -Wall -g -c avl.c

clean:
  rm gol *.o
