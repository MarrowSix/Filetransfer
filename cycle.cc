#include "sources/munp.h"
#include <string>
#include <algorithm>
#include <assert.h>

const size_t SLEEP = 10;
bool cflag = false;

void checkFile(int connfd, std::string name)
{
    name += '\n';
    Writen(connfd, const_cast<char *>(name.c_str()), name.size());
    
    int16_t fack = 0;
    Readn(connfd, &fack, sizeof(fack));
    fack = ntohs(fack);
    
    if (fack == FNEXIST) {
        printf("data file %s not exist\n", name.c_str());
        exit(1);
    }
}

void proformMessage(int connfd, std::string &name)
{
    char buf[MAXLINE];
//     if (!cflag) {
        checkFile(connfd, name);
//         cflag = true;
//     }
    int filefd;
    // open file and create it with 0664 if not exist
    if ((filefd = open(name.c_str(), O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR | S_IWGRP | S_IROTH)) == -1) {
        err_sys("open %s error", name.c_str());
    }
            
    while (Readline(connfd, buf, sizeof(buf)) != 0) {
        Writen(filefd, buf, strlen(buf));
    }
    
    Close(filefd);
   
}


int main(int argc, char* argv[])
{
    int clientfd;
    sockaddr_in servaddr;

    if (argc != 3) {
        printf("usage: <IPAddress> <Filename>\n");
        exit(1);
    }

again:
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket errno");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) == 0) {
        err_sys("inet_pton error");
    }

    std::string filename {argv[2]};
    
//     while (1) {
        if (connect(clientfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            printf("connect error\n");
            exit(1);
        }
        
//         printf("%d ", PFLAG);
        int16_t flag = htons(PFLAG);
        Writen(clientfd, &flag, sizeof(flag));
        
        int16_t rflag = 0;
        Readn(clientfd, &rflag, sizeof(rflag));
        rflag = ntohs(rflag);
//         printf("%d \n", rflag);
        
        if (rflag == PFLAG) {
            printf("success connected to %s\n", argv[1]);
            printf("receiving file %s\n", argv[1]);
            proformMessage(clientfd, filename);
            printf("received file %s\n", argv[1]);
        }
        
        Close(clientfd);
        
        unsigned long s = SLEEP;
        while ((s = sleep(s)))
            ;
    
        goto again;
//     }

    return 0;
}
