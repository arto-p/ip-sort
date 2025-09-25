#! make -f
#

CC	= gcc
CFLAGS	= -Wall -O3

MD5HASH	= 24e7a7b2270daee89c64d3ca5fb3da1a

all: ip-sort

ip-sort: ip-sort.c

test: ip-sort
	cat ip-sort.tsv | ./ip-sort | md5sum | grep -wq $(MD5HASH)

clean:
	rm -f ip-sort
