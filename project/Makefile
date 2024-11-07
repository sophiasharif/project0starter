CC=g++  -g
OPENSSL_DIR=/opt/homebrew/opt/openssl@3
CFLAGS=-I$(OPENSSL_DIR)/include -L$(OPENSSL_DIR)/lib -lssl -lcrypto  # Add OpenSSL include and lib flags

all: clean build

default: build

build: server.cpp client.cpp util.cpp socket.cpp server_socket.cpp
	${CC} -o server server.cpp util.cpp socket.cpp server_socket.cpp rdtlayer.cpp packet.cpp ${CFLAGS}
	${CC} -o client client.cpp util.cpp socket.cpp client_socket.cpp rdtlayer.cpp packet.cpp ${CFLAGS}

clean:
	rm -rf server client *.bin *.out *.dSYM

zip: clean
	rm -f project0.zip
	mkdir -p project
	cp *.cpp *.h Makefile project
	zip project0.zip project/*
