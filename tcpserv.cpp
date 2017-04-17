#include "munp.h"
#include <iostream>
using namespace std;

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

    ssize_t n;
    if ((n = Read(connfd, buf, sizeof(buf))) < 0) {
        err_sys("read error");
    }
    // buf[strlen(buf)-1] = '\0';
    cout << buf << endl;
    filename = new char[strlen(buf)+strlen(path)+1];

    strcat(filename, path);
    strcat(filename, buf);
    cout << filename << endl;

    int filefd;
    // open file and create it with 0664 if not exist
    if ((filefd = open(filename, O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR | S_IWGRP | S_IROTH)) == -1) {
        err_sys("open %s error", filename);
    } else {
        strcpy(buf, "success\n");
        Writen(connfd, buf, strlen(buf));
    }

    while (Readline(connfd, buf, sizeof(buf)) != 0) {
        Writen(filefd, buf, strlen(buf));
    }

    Close(filefd);
}

int main(int argc, char *argv[]) {
    int listenfd, connfd;
    struct sockaddr_in servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    Listen(listenfd, LISTENQ);

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
            cout << "fork" << endl;
            processFile(connfd);
            Close(connfd);
            exit(0);
        }
        Close(connfd);
    }

    return 0;
}
