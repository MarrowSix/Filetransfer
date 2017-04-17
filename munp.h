#ifndef _UNP_H_
#define _UNP_H_

#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define SA struct sockaddr
#define	LISTENQ		1024   /* 2nd argument to listen() */

const int MAXLINE = 4096;	/* max text line length */
const int BUFFSIZE = 8192;	/* buffer size for reads and writes */

const int SERV_PORT = 9987;

ssize_t writen(int, const void *, size_t);
void Writen(int, void *, size_t);

ssize_t Readline(int, void *, size_t);

void Close(int fd);

void	 err_dump(const char *, ...);
void	 err_msg(const char *, ...);
void	 err_quit(const char *, ...);
void	 err_ret(const char *, ...);
void	 err_sys(const char *, ...);

#endif
