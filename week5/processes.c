//
// Created by xiaoxiang on 8/28/22.
//

#include "../main.h"


void child_handle(int sig) {
    puts("signal received in child");
}

int main(void) {
    int n = 4;
    puts("about to fork");

    int fds[2];
    pipe(fds);

    pid_t pid = fork();

    if (pid < 0) {
        perror("unable to fork");
        return 1;
    } else if (pid == 0) {
//        puts("child");
        signal(SIGUSR1, child_handle);

        write(fds[1], "child", 6);
        pause();

        n++;
//        sleep(1);
    } else {
        puts("parent");

        char buf[128];
        read(fds[0], buf, 128);
        printf("received : %s\n", buf);
        kill(pid, SIGUSR1);

        n *= 2;
// wait(NULL);
    }
    printf("fork returned %d, n is %d\n", pid, n);
    return 0;
}