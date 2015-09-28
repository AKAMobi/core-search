#
# 1. Warning:  Invalid library (maybe not a PHP library)
#	need ZEND_GET_MODULE(phpext)
#

SRCDIR=..
PHPINCDIR=./usr-2004-02-18/include/php

#cd $SRCDIR
#cp -R wPHPExt $PHPDIR/ext
#cd $PHPDIR/ext/wPHPExt

all: phpsearchlib.c
	gcc -fpic -DCOMPILE_DL -DCOMPILE_DL_MYSTUFF=1 -I${PHPINCDIR}/TSRM -I${PHPINCDIR} -I${PHPINCDIR}/main -I${PHPINCDIR}/Zend -I${SRCDIR}/include -I${SRCDIR}/wQuery -c -o wPHPExt.o phpsearchlib.c 
	gcc -shared -rdynamic -o wPHPExt.so wPHPExt.o ${SRCDIR}/wquery.o ${SRCDIR}/wsearcher.o

static: phpsearchlib.c
	gcc -fpic -DCOMPILE_DL -DCOMPILE_DL_MYSTUFF=1 -I${PHPINCDIR}/TSRM -I${PHPINCDIR} -I${PHPINCDIR}/main -I${PHPINCDIR}/Zend -I${SRCDIR}/include -I${SRCDIR}/wQuery -c -o wPHPExt.o phpsearchlib.c --static
	gcc -shared -rdynamic -o wPHPExt-static.so wPHPExt.o ${SRCDIR}/wquery.o ${SRCDIR}/wsearcher.o --static

clean:
	rm -f *.o *.so
