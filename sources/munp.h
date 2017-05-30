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

#define	SA	struct sockaddr
#define	LISTENQ		1024   /* 2nd argument to listen() */

const int MAXLINE = 4096;	/* max text line length */
const int BUFFSIZE = 8192;	/* buffer size for reads and writes */

const int SERV_PORT = 9987;

const char path[] = "./";     /* file path */

ssize_t writen(int, const void *, size_t);
void Writen(int, void *, size_t);
ssize_t bwriten(int fd, const int16_t& bdata, size_t n);

ssize_t Readline(int, void *, size_t);

void Close(int fd);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
int Socket(int family, int type, int protocol);
void Listen(int fd, int backlog);

ssize_t Read(int fd, void *ptr, size_t nbytes);
ssize_t	readn(int fd, void *vptr, size_t n);
ssize_t Readn(int fd, void *ptr, size_t nbytes);

void	 err_dump(const char *, ...);
void	 err_msg(const char *, ...);
void	 err_quit(const char *, ...);
void	 err_ret(const char *, ...);
void	 err_sys(const char *, ...);

#endif
