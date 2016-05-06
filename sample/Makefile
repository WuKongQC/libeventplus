all:test.c test_asyn.c
	gcc -o test test.c -I/usr/local/include -L/local/lib -lhiredis
	gcc -o test_asyn test_asyn.c -I/usr/local/include  -I/usr/include -L/usr/local/lib -lhiredis  -L/usr/lib -levent
