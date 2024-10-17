CC=gcc  -g
OPENSSL_DIR=/opt/homebrew/opt/openssl@3
CFLAGS=-I$(OPENSSL_DIR)/include -L$(OPENSSL_DIR)/lib -lssl -lcrypto  # Add OpenSSL include and lib flags

all: clean build

default: build

build: server.c client.c 
	${CC} -o server server.c ${CFLAGS}
	${CC} -o client client.c ${CFLAGS}

clean:
	rm -rf server client *.bin *.out *.dSYM

zip: clean
	rm -f project0.zip
	mkdir -p project
	cp server.c client.c Makefile project
	zip project0.zip project/*
