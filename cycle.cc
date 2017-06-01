#include "sources/munp.h"

// rapidjson
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/reader.h"
#include "include/rapidjson/stringbuffer.h"
#include "include/rapidjson/rapidjson.h"
#include "include/rapidjson/filereadstream.h"
#include "include/rapidjson/filewritestream.h"

// bcm
#include "include/bcm/bcm2835.h"

#include <string>
#include <algorithm>
#include <cassert>
#include <cstdio>

const size_t SLEEP = 10;
bool cflag = false;

// detect wherether the file in server was exist
void checkFile(int connfd, std::string name);
// proform message
void proformMessage(int connfd, std::string &name);
// process data of file
void processDatafile(const std::string &name);
// control the gpio of raspberry
void blinkGpio();

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
    checkFile(connfd, name);

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

void blinkGpio()
{
    if (!bcm2835_init()) {
        printf("bcm2835 init error\n");
    }
    
    // set the pin to be an output
    bcm2835_gpio_fsel(RPI_GPIO_P1_11, BCM2835_GPIO_FSEL_OUTP);
    
    // turn on
    bcm2835_gpio_write(RPI_GPIO_P1_11, HIGH);
    bcm2835_delay(1000);
    
    bcm2835_close();
}

void processDatafile(const std::string &name)
{
    using namespace rapidjson;
    
    // read file
    FILE* fp;
    
    if ((fp = fopen(name.c_str(), "rb")) == NULL) {
        err_sys("fopen");
    }
    
    char readBuffer[1<<16];
    FileReadStream in(fp, readBuffer, sizeof(readBuffer));
    
    Document d;
    d.ParseStream(in);
    
    auto pos = d.FindMember("light");
    Value& vl = d[pos->name];
    
    int value = vl.GetInt();
//     printf("light: %d\n", value);
    if (value) {
        blinkGpio();
    }
    
    fclose(fp);
    
}

int main(int argc, char* argv[])
{
    int clientfd;
    sockaddr_in servaddr;
    std::string addr = "104.128.84.142";
//     if (argc != 3) {
//         printf("usage: <IPAddress> <Filename>\n");
//         exit(1);
//     }

again:
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_sys("socket errno");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, addr.c_str(), &servaddr.sin_addr) == 0) {
        err_sys("inet_pton error");
    }

    std::string filename = {argv[1]};
    
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
        printf("success connected to %s\n", addr.c_str());
        printf("receiving file %s\n", addr.c_str());
        proformMessage(clientfd, filename);
        printf("received file %s\n", addr.c_str());
    }
        
    Close(clientfd);
    
    processDatafile(filename);
    
    unsigned long s = SLEEP*2;
    while ((s = sleep(s)))
        ;
    
    goto again;
//     }

    return 0;
}
