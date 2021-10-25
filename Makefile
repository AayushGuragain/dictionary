CC = gcc

all: parser search

parser:parser.c
	$(CC) -g -o parser parser.c -std=gnu11 -Wall -Werror

search: search.c
	$(CC) -g -o search search.c -std=gnu11 -Wall -Werror
	
run: clean card search
	./search

card: parser
	./parser cards.csv
	
clean:
	rm -f parser search index.bin cards.bin no_of_cards.bin