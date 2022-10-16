//
// Created by xiaoxiang on 10/16/22.
//

#include "../main.h"

int main(int argc, char **argv) {

    if (argc == 1) return 1;

    char *input_buffer;

    FILE *f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    size_t buffer_size = ftell(f);
    input_buffer = malloc(sizeof(char) * buffer_size);
    rewind(f);
    fread(input_buffer, sizeof(char), buffer_size, f);
    fclose(f);
//    printf("%s\n", input_buffer);

#pragma omp parallel for
    for (size_t i = 0; i < buffer_size; i++) {
        printf("thread %d\n", omp_get_thread_num());
        switch (input_buffer[i]) {
            case 'A':
                input_buffer[i] = 'T';
                break;
            case 'T':
                input_buffer[i] = 'A';
                break;
            case 'C':
                input_buffer[i] = 'G';
                break;
            case 'G':
                input_buffer[i] = 'C';
                break;
            default:
                break;

        }
    }

    printf("%s", input_buffer);
}