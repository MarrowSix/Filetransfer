#include "munp.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
    int clientfd;
    sockaddr_in servaddr;
    char recvline[MAXLINE];

    if (argc != 3) {
        cout << "usage: <IPAddress> <Filename>" << endl;
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

    // string filename(argv[2]);
    // char *filename = new char[strlen(argv[2]) + 1];
    // filename[strlen(argv[2])] = '\n';
    // filename[strlen(argv[2])+1] = '\0';

    Writen(clientfd, argv[2], strlen(argv[2]));
    cout << "write success" << endl;
    Readline(clientfd, recvline, sizeof(recvline));

    if (strcmp(recvline, "success\n")) {
        err_quit("recvline not equal to success\n");
    }



    return 0;
}
