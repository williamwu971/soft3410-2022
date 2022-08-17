#include "nbody.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {

    long num_of_iterations;
    double dt;
    char *filename = NULL;
    int num_of_bodies = 0;

    // parse config
    if (argc >= 5) {
        num_of_iterations = strtol(argv[1], NULL, 10);
        dt = strtod(argv[2], NULL);
        if (strcmp("-f", argv[3]) == 0) {
            filename = argv[4];
        } else {
            num_of_bodies = strtol(argv[4], NULL, 10);
        }
    } else {
        return 1;
    }

    // generate bodies
    struct body *bodies = NULL;

    if (filename) {

        // using given input file
        FILE *f = fopen(filename, "r");
        char line[BUF_LEN];
        while (fgets(line, BUF_LEN, f)) {
            if (strcmp("\n", line) == 0) continue;
            bodies = realloc(bodies, ++num_of_bodies * sizeof(struct body));
            bodies[num_of_bodies - 1] = (struct body) {
                    strtod(strtok(line, ","), NULL),
                    strtod(strtok(NULL, ","), NULL),
                    strtod(strtok(NULL, ","), NULL),
                    strtod(strtok(NULL, ","), NULL),
                    strtod(strtok(NULL, ","), NULL),
                    strtod(strtok(NULL, ","), NULL),
                    strtod(strtok(NULL, ","), NULL),
                    {UNLOCKED}
            };
        }

        fclose(f);
    } else {
        bodies = malloc(sizeof(struct body) * num_of_bodies);

        // sun
        bodies[0] = (struct body) {0, 0, 0,
                                   0, 0, 0,
                                   SOLARMASS};

        // others
        double current_x = 0;
        for (int i = 1; i < num_of_bodies; i++) {
            current_x += 50;
            bodies[i] = (struct body) {current_x, 0, 0, 0,
                                       sqrt(GCONST * SOLARMASS / current_x),
                                       0, PLANETMASS};
        }
    }

    // calculate energy before running
    double start_energy = energy(bodies, num_of_bodies);
    helper *h = start_program(dt, bodies, num_of_bodies, num_of_iterations);

    // execute functions
    for (long i = 0; i < num_of_iterations; i++) {

        step(h);

    }

    // calculate energy after running
    double end_energy = energy(bodies, num_of_bodies);

    printf("starting at %f ending at %f\n", start_energy, end_energy);

    end_program(h);
    free(bodies);
    return 0;
}
