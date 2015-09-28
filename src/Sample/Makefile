all:	wquery windexer

wquery:	wQuerySample.c
	gcc -g -o wquery wQuerySample.c -L.. -lwsearch -I../wQuery

windexer: wIndexerSample.c
	gcc -g -o windexer wIndexerSample.c -L.. -lwsearch -I../wIndexer

clean:
	rm -f *.o wquery windexer
