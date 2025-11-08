all:
	$(CC) debug.c ford_fulkerson.c -g -o flkr
clean:
	rm flkr
