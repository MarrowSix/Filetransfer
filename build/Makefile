VPATH= ../

PROGS= tcpclient tcpserv

CFLAGS= -g

OBJECT= wrapsock.o wraplinux.o readline.o readn.o writen.o error.o

CC= g++

all: ${PROGS}

tcpclient: tcpclient.o ${OBJECT}
	${CC} ${CFLAGS} -o $@ $^

tcpserv: tcpserv.o ${OBJECT}
	${CC} ${CFLAGS} -o $@ $^

wrapsock: wrapsock.o
	${CC} ${CFLAGS} -o

readline: readline.o
	${CC} ${CFLAGS} -o

writen: writen.o
	${CC} ${CFLAGS} -o

error: error.o
	${CC} ${CFLAGS} -o

wraplinux: wraplinux.o
	${CC} ${CFLAGS} -o

readn: readn.o
	${CC} ${CFLAGS} -o

rm: 
	rm *.o
