//
// Created by xiaoxiang on 9/18/22.
//
#include "../main.h"

char *path = "/home/xiaoxiang/CLionProjects/soft3410-2022/week8/ftp_files";

void *list(void *arg) {
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
}

void stor(void *arg) {
    char buf[128];
    sprintf(buf, "%s/%s", path, (char *) arg);

    int fd = open(buf, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    off_t offt = lseek(fd, 2097151, SEEK_SET);
    assert(offt != -1);

    ssize_t result = write(fd, "", 1);
    assert(result != -1);

    close(fd);
}

void size(void *arg) {

    char filename[128];
    sprintf(filename, "%s/%s", path, (char *) arg);

//    printf("%s\n",filename);
    FILE *f = fopen(filename, "r");

    if (f == NULL)die();

    fseek(f, 0, SEEK_END); // seek to end of file
    size_t size = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET); // seek back to beginning of file

    printf("size for %s is %ld\n", (char *) arg, size);
}

int process(char *buf) {
    if (strcasestr(buf, "LIST")) {
        list(NULL);
    } else if (strcasestr(buf, "RETR")) {

    } else if (strcasestr(buf, "STOR")) {
        stor(strcasestr(buf, " ") + 1);
    } else if (strcasestr(buf, "DELE")) {

    } else if (strcasestr(buf, "RNFR")) {

    } else if (strcasestr(buf, "RNTO")) {

    } else if (strcasestr(buf, "SIZE")) {
        size(strcasestr(buf, " ") + 1);
    } else if (strcasestr(buf, "QUIT")) {
        return 0;
    } else {
        puts("INVALID");
    }

    return 1;
}

int main(int argc, char **argv) {
    int serversocket_fd = -1;
    int clientsocket_fd = -1;
    int port = 9000;
    struct sockaddr_in address;
    int option = 1;
    char buffer[1024];
    serversocket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket_fd < 0) {
        puts("This failed!");
        exit(1);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    setsockopt(serversocket_fd, SOL_SOCKET, SO_REUSEADDR |
                                            SO_REUSEPORT, &option, sizeof(int));
    if (bind(serversocket_fd, (struct sockaddr *) &address,
             sizeof(struct sockaddr_in))) {
        exit(1);
    }
    listen(serversocket_fd, 1024);

    struct pollfd fds[3];
    size_t nfds = 3;

    for (size_t i = 0; i < nfds; i++) {
        // Set each file descriptor
        fds[i].fd = fd;
        fds[i].events = POLLIN | POLLHUP;
    }


// Maybe change the file descriptor to use non-blocking
    while (poll(fds, nfds, -1) >= 0) {
        for (size_t i = 0; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
//What to do if POLLIN was received on
//this file descriptor
            }
        }
//event loop to check
    }

    int cond = 1;
    while (cond) {
        uint32_t addrlen = sizeof(struct sockaddr_in);
        clientsocket_fd = accept(serversocket_fd, (struct sockaddr *) &address, &addrlen);
        read(clientsocket_fd, buffer, 1024);
        char *tailer = strchr(buffer, '\n');
        if (tailer)tailer[0] = '\0';
        puts(buffer);
        cond = process(buffer);
        close(clientsocket_fd);
    }
    close(serversocket_fd);
    return 0;
}