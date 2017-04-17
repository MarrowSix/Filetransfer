#include "munp.h"

void sig_child(int signal)
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {}
    return;
}

void processFile(int connfd)
{
    char buf[MAXLINE], *filename;
    char path[] = "/home/pi/Documents/temp/";
    Readline(connfd, buf, MAXLINE);
    filename = new char[strlen(buf)+strlen(path)];

    strcat(filename, path);
    strcat(filename, buf);

    int filefd;
    if ((filefd = open(filename, O_WRONLY | O_CREAT)) == -1) {
        err_sys("open %s error", filename);
    } else {
        strcpy(buf, "success\n");
        Writen(connfd, buf, strlen(buf));
    }
}

int main(int argc, char const *argv[]) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket error");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) {
        err_sys("bind error");
    }

    if (listen(listenfd, LISTENQ)) {
        err_sys("listen error");
    }
    signal(SIGCHLD, sig_child);

    while (1) {
        if ((connfd = accept(listenfd, (SA *)NULL, NULL)) < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                err_sys("accept error");
            }
        }

        pid_t pid;
        if ((pid = fork()) == 0) {
            Close(listenfd);
            processFile(connfd);
            Close(connfd);
            exit(0);
        }
        Close(connfd);
    }

    return 0;
}
