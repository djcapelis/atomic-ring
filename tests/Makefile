all: aring_huge aring_2048 aring_64 aring_1

aring_huge:
	cc -std=gnu11 -Os -march=native -o aring_huge aring_huge.c ../aring.o -pthread

aring_2048:
	cc -std=gnu11 -Os -march=native -o aring_2048 aring_2048.c ../aring.o -pthread

aring_64:
	cc -std=gnu11 -Os -march=native -o aring_64 aring_64.c ../aring.o -pthread

aring_1:
	cc -std=gnu11 -Os -march=native -o aring_1 aring_1.c ../aring.o -pthread

clean:
	rm aring_huge aring_2048 aring_64 aring_1
