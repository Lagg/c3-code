#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    const char *domain = "lagg.me";
    const char *service = "80";

    struct addrinfo *info = NULL;
    struct addrinfo hints;

    int res;

    memset(&hints, 0, sizeof(hints));

    hints.ai_flags |= AI_CANONNAME;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    res = getaddrinfo(domain, service, &hints, &info);

    if (res == 0 && info) {
        int sock;

        sock = socket(AF_INET, SOCK_STREAM, 0);

        if (connect(sock, info->ai_addr, sizeof(*info->ai_addr)) == 0) {
            const char *req = "GET /\r\n";
            size_t buflen = 512;
            unsigned char *buf = (unsigned char *)malloc(buflen + 1);
            size_t read = 0;

            send(sock, req, strlen(req), 0);

            do {
                read = recv(sock, buf, buflen, 0);
                if (read > 0) {
                    buf[read] = '\0';
                    printf("%s", buf);
                }
            } while (read > 0);

            free(buf);
            close(sock);
            freeaddrinfo(info);
        }
    }

    return 0;
}
