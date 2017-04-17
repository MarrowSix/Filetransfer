#include "unp.h"
#include <iostream>
using namespace std;

const short port = 9987;

int main(int argc, char const *argv[]) {
    int clientfd;
    sockaddr_in servaddr;

    if (argc != 3) {
        cout << "usage: <IPAddress> <Filename>" << endl;
    }

    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket errno");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) == 0) {
        err_sys("inet_pton error");
    }

    if (connect(clientfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        err_sys("connect error");
    }

    string filename(argv[2]);
    Writen(clientfd, const_cast<char *>(filename.c_str()), filename.size());


    return 0;
}
