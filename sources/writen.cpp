#include "munp.h"

#include <string>

ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = static_cast<const char *> (vptr);
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR) {
                nwritten = 0;
            } else {
                return -1;
            }
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n);
}

void Writen(int fd, void *ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes) {
        err_sys("writen errno");
    }
}

ssize_t bwriten(int fd, const int16_t& bdata, size_t n)
{
    std::string data = std::to_string(bdata);
    if (writen(fd, data.c_str(), data.size())) {
        return 0;
    }
    return n;
}
