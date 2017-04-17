#include "munp.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {
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

    string filename(argv[2]);
    Writen(clientfd, const_cast<char *>(filename.c_str()), filename.size());
    Readline(clientfd, recvline, sizeof(recvline));

    if (strcmp(recvline, "success\n")) {
        err_quit("recvline not equal to success\n");
    }



    return 0;
}
