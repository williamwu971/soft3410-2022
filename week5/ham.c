//
// Created by xiaoxiang on 8/28/22.
//

#include "../main.h"

struct simple_list {

    size_t size;
    void **values;
};

struct simple_list *list_new() {

    struct simple_list *list = malloc(sizeof(struct simple_list));
    list->size = 0;
    list->values = NULL;

    return list;
}

void *list_get(struct simple_list *list, size_t idx) {

//    printf("%d\n", list->size > idx);
    return list->size > idx ? list->values[idx] : NULL;
}

void list_put(struct simple_list *list, size_t idx, void *value) {

    if (list->size <= idx) {
        list->values = realloc(list->values, sizeof(void *) * (idx + 1));
        list->size = idx + 1;
    }

    list->values[idx] = value;
}

void list_del(struct simple_list *list, size_t idx) {

    if (list->size > idx) {
        list->values[idx] = NULL;
    }
}


void parent(int read_fd, int write_fd) {

    struct simple_list *list = list_new();
//    puts("parent starts");
    int run = 1;

    while (run) {

        char buf[1024];
        read(read_fd, buf, 1024);

        char buf0[128];
        char buf1[128];
        char buf2[128];
        sscanf(buf, "%s %s %s", buf0, buf1, buf2);

        if (strcmp(buf0, "GET") == 0) {
            sprintf(buf, "GET %d: %s\n", atoi(buf1),
                    (char *) list_get(list, atoi(buf1)));
        } else if (strcmp(buf0, "PUT") == 0) {
            void *value = malloc(strlen(buf2) + 1);
            memcpy(value, buf2, strlen(buf2) + 1);
            list_put(list, atoi(buf1), value);

            sprintf(buf, "PUT %d\n", atoi(buf1));
        } else if (strcmp(buf0, "DEL") == 0) {

            list_del(list, atoi(buf1));

            sprintf(buf, "DEL %d\n", atoi(buf1));
        } else {
            run = 0;
            sprintf(buf, "BYE!!\n");
        }

        write(write_fd, buf, strlen(buf) + 1);

    }

//    puts("parent ends");

}

void child(int read_fd, int write_fd) {
    char *commands[] = {
            "PUT 0 hello",
            "PUT 1 world",
            "GET 0",
            "GET 1",
            "DEL 0",
            "GET 0",
            "GET 1",
            "PUT 0 hey",
            "GET 0",
            "EXIT"
    };


    char buf[128];
    for (int i = 0; i < sizeof(commands) / sizeof(void *); i++) { // bad thing
        write(write_fd, commands[i], strlen(commands[i]) + 1);
        read(read_fd, buf, 128);
        printf("=== %s", buf);
    }
}

int main() {

    int ptc[2];
    pipe(ptc);
    int ctp[2];
    pipe(ctp);

    if (fork()) {
        parent(ctp[0], ptc[1]);
    } else {
        child(ptc[0], ctp[1]);
    }
}