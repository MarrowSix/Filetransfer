#include "sources/munp.h"
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
    int16_t data = htons(FFLAG);
    Writen(connfd, &data, sizeof(data));
    
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

bool checkFile(std::string name)
{
    if (access(name.c_str(), F_OK | R_OK | W_OK) < 0) {
        printf("file %s is no exist", name.c_str());
        return true;
    }
    return false;
}

void proformCheck(int connfd)
{
    char buf[MAXLINE];
    char recvline[MAXLINE], sendline[MAXLINE];
    string name;
    int i = 0;
    
    Readline(connfd, buf, sizeof(buf));
    
    while (buf[i] != '\n') {
        name += buf[i];
        i++;
    }
    printf("%s\n", name.c_str());
    
    if (checkFile(name)) {
        int16_t fnflag = htons(FNEXIST);
        Writen(connfd, &fnflag, sizeof(fnflag));
        return;
    } else {
        int16_t fnflag = htons(PFLAG);
        Writen(connfd, &fnflag, sizeof(fnflag));
        printf("data file exist\n");
    }
    // file transfer
    int filefd, n;
    // open file with readonly
    if ((filefd = open(name.c_str(), O_RDONLY)) < 0) {
        err_sys("open file %s failure", name.c_str());
    }

    while ((n = Readline(filefd, sendline, sizeof(recvline))) != 0) {
        Writen(connfd, sendline, strlen(sendline));
    }
    printf("transfer finished\n");
    
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
            int16_t srvFlag = 0;
            Readn(connfd, &srvFlag, sizeof(srvFlag));
            srvFlag = ntohs(srvFlag);
            if (srvFlag == PFLAG) {
                int16_t data = htons(PFLAG);
                Writen(connfd, &data, sizeof(data));
                proformCheck(connfd);
            } else if (srvFlag == FFLAG) {
                processFile(connfd);
            }

            Close(connfd);
            exit(0);
        }
        Close(connfd);
    }

    return 0;
}
