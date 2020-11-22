all: test_alloc test_ealloc

test_alloc: test_alloc.o alloc.o
	${CC} test_alloc.o alloc.o -o test_alloc

test_alloc.o: alloc/test_alloc.c
	${CC} -c alloc/test_alloc.c -o test_alloc.o

alloc.o: alloc/alloc.h alloc/alloc.c
	${CC} -c alloc/alloc.c -o alloc.o

test_ealloc: test_ealloc.o ealloc.o
	${CC} test_ealloc.o ealloc.o -o test_ealloc

test_ealloc.o: ealloc/test_ealloc.c
	${CC} -c ealloc/test_ealloc.c -o test_ealloc.o

ealloc.o: ealloc/ealloc.h ealloc/ealloc.c
	${CC} -c ealloc/ealloc.c -o ealloc.o

clean:
	rm test_alloc test_alloc.o alloc.o test_ealloc test_ealloc.o ealloc.o