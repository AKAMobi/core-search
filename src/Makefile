#all:
SHELL = /bin/sh
CC=gcc
CFLAGS =-I include -O
TARGET_wIndexer=windexer
TARGET_wSearcher=wsearcher
TARGET_wQuery=wquery
DIR_wIndexer=./wIndexer/
DIR_wSearcher=./wSearcher/
DIR_wQuery=./wQuery/
DIR_Sample=./Sample/
DEBUG = -g
ALL_DEPENDS = 

wIndexer_DEPENDS = $(DIR_wIndexer)wIndexer.c $(ALL_DEPENDS)
wSearcher_DEPENDS = $(DIR_wSearcher)wSearcher.c $(ALL_DEPENDS)
wQuery_DEPENDS = $(DIR_wQuery)wQuery.c $(ALL_DEPENDS)

all: 
	make $(TARGET_wIndexer) $(TARGET_wSearcher) $(TARGET_wQuery) lib
$(TARGET_wIndexer):$(wIndexer_DEPENDS)
	$(CC) $(DEBUG) $(CFLAGS) -c -o $(TARGET_wIndexer).o -I$(DIR_wIndexer) $(DIR_wIndexer)wIndexer.c
$(TARGET_wSearcher):$(wSearcher_DEPENDS)
	$(CC) $(DEBUG) $(CFLAGS) -o $(TARGET_wSearcher) -I$(DIR_wSearcher) -lm $(DIR_wSearcher)wSearcher.c
$(TARGET_wQuery):$(wQuery_DEPENDS)
	$(CC) $(DEBUG) $(CFLAGS) -c -o $(TARGET_wSearcher).o -I$(DIR_wSearcher) -DSINGLESEARCH $(DIR_wSearcher)wSearcher.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $(TARGET_wQuery).o -I$(DIR_wQuery) $(DIR_wQuery)wQuery.c -I$(DIR_wSearcher)

wsearcher-static: $(wSearcher_DEPENDS)
	$(CC) $(DEBUG) $(CFLAGS) -o $(TARGET_wSearcher)-static -I$(DIR_wSearcher) -lm $(DIR_wSearcher)wSearcher.c --static

lib: wquery windexer wsearcher
	ar cr libwsearch.a *.o
	ranlib libwsearch.a

	
rebuild:
	@exit

clean:
	rm -f wsearcher
	rm -f *.o *.a
	rm -f *-static
	@exit

