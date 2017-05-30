#include "sources/munp.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    int clientfd;
    sockaddr_in servaddr;
    char recvline[MAXLINE], sendline[MAXLINE];

    if (argc != 3) {
        printf("usage: <IPAddress> <Filename>\n");
    }

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket errno");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) == 0) {
        err_sys("inet_pton error");
    }

    if (connect(clientfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        err_sys("connect error");
    }

    char *filename = argv[2];

    if (access(filename, F_OK | R_OK | W_OK) < 0) {
        err_quit("file %s is no exist", filename);
    }

    filename = strrchr(argv[2], '/') + 1;

    Writen(clientfd, filename, strlen(filename));
//     cout << "write success" << endl;
    printf("write file %s success\n", filename);
    Readline(clientfd, recvline, sizeof(recvline));

    if (strcmp(recvline, "success\n")) {
        err_quit("recvline not equal to success\n");
    }

    int filefd, n;
    // open file with readonly
    if ((filefd = open(argv[2], O_RDONLY)) < 0) {
        err_sys("open file %s failure", argv[2]);
    }

    while ((n = Readline(filefd, sendline, sizeof(recvline))) != 0) {
        Writen(clientfd, sendline, strlen(sendline));
    }
    Close(filefd);

    return 0;
}
