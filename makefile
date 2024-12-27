OBJ:= $(patsubst %.c, %.o, $(wildcard *.c))
minishell.out: $(OBJ)
	gcc -o $@ $^  
clean:
	rm *.o