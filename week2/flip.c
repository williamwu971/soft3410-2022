//
// Created by Xiaoxiang Wu on 6/8/2022.
//

#include "../main.h"

void h_flip(int *matrix, int m, int n) {


    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n / 2; j++) {

            int tmp = matrix[i * n + j];

            matrix[i * n + j] = matrix[i * n + n - j - 1];
            matrix[i * n + n - j - 1] = tmp;

        }
    }
}

void print_matrix(int *matrix, int m, int n) {


    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i * n + j]);

        }
        puts("");
    }
}

int main() {
    int matrix[3][3] = {
            1, 2, 3,
            4, 5, 6,
            7, 8, 9
    };
//... horizontal flip, vertical flip, matrix reverse
/*
Horizontal Flip
3, 2, 1,
6, 5, 4,
9, 8, 7
1
Vertical Flip
7, 8, 9,
4, 5, 6,
1, 2, 3
Matrix Reverse
9, 8, 7,
6, 5, 4,
3, 2, 1
*/

    h_flip(matrix, 3, 3);
    print_matrix(matrix, 3, 3);


    return 0;
}