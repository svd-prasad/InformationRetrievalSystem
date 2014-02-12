all	: md5c.o mddriver.c 
	gcc -o patricia convert.c md5c.o mddriver.c patricia.c

	

md5c.o	: md5c.c md5.h global.h
	gcc -c md5c.c -o md5c.o
clean	:
	@rm -f hashvalues.txt
	@rm -f a.out
	@rm -f md5c.o
	@rm -f hfile.txt
	@rm -f stem
	@rm -f steminput
