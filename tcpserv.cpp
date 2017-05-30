#include "sources/munp.h"
#include <iostream>
using namespace std;

const int16_t FNEXIST = 0x0001;
const int16_t PFLAG = 0x0010;
const int16_t ACK = 0x0100;

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
    printf("buf: %s\n", buf);
    filename = new char[strlen(buf)+strlen(path)+1];

    strcat(filename, path);
    strcat(filename, buf);
    printf("filename: %s\n", filename);

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

void proformCheck(int connfd)
{
    int16_t data = htons(PFLAG);
    Writen(connfd, &data, sizeof(data));
    
    std::string name("model\n");
    while (1) {
        Writen(connfd, const_cast<char *>(name.c_str()), name.size());
        
        int16_t ack = 0;
        Readn(connfd, &ack, sizeof(ack));
        ack = ntohs(ack);
        
        if (ack != ACK) {
            break;
        }
    }
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
            int16_t srvFlag = 0;
            Readn(connfd, &srvFlag, sizeof(srvFlag));
            srvFlag = ntohs(srvFlag);
            if (srvFlag == PFLAG) {
                proformCheck(connfd);
            } else {
                processFile(connfd);
            }

            Close(connfd);
            exit(0);
        }
        Close(connfd);
    }

    return 0;
}
