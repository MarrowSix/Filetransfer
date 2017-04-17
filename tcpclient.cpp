#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
        perror("socket error");
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) == 0) {
        perror("inet_pton error");
        return -1;
    }

    if (connect(clientfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        return -1;
    }

    string filename(argv[2]);
    write(clientfd, const_cast<char *>(filename.c_str()), filename.size());
    // writen(clientfd, filename.c_str(), filename.size());
    return 0;
}
