#! make -f
#

CC	= gcc
CFLAGS	= -Wall -O3

all: ip-sort

ip-sort: ip-sort.c

test: ip-sort
	cat ip-sort.tsv | ./ip-sort | md5sum
	@echo 24e7a7b2270daee89c64d3ca5fb3da1a

clean:
	rm -f ip-sort
