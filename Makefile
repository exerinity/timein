timein: execute.c find.c listofcities.c
	gcc -O2 -o timein execute.c find.c listofcities.c

clean:
	rm -f timein