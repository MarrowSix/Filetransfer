#include "sources/munp.h"
#include <string>
#include <algorithm>
#include <assert.h>

const int16_t FNEXIST = 0x0001;
const int16_t PFLAG = 0x0010;
const int16_t ACK = 0x0100;
const size_t SLEEP = 10;

int main(int argc, char* argv[])
{
    int clientfd;
    sockaddr_in servaddr;
    char recvline[MAXLINE], sendline[MAXLINE];

    if (argc != 2) {
        printf("usage: <IPAddress>\n");
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

    std::string filename {argv[1]};
    
    while (1) {
        int16_t flag = htons(PFLAG);
        Writen(clientfd, &flag, sizeof(flag));
        
        int16_t rflag = 0;
        Readn(clientfd, &rflag, sizeof(rflag));
        
        if (rflag == flag) {
            printf("success\n");
            char buf[MAXLINE];
            while(Readline(clientfd, buf, sizeof(buf))) {
                printf("filename: %s", buf);
                
                int16_t ack = htons(ACK);
                Writen(clientfd, &ack, sizeof(ack));
            }
        }
        
    }

    return 0;
}