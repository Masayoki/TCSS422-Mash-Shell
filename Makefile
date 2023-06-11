$(CC) = gcc

mash: mash.c
	$(CC) mash.c -o mash

clean:
	rm *.O mash
