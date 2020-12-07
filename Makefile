KDIR = /lib/modules/`uname -r`/build
all:
	make -C /lib/modules/`uname -r`/build/ M=`pwd` modules
	`cc test.c -o test`
clean:
	make -C /lib/modules/`uname -r`/build/ M=`pwd` clean
	rm test