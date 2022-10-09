//
// Created by xiaoxiang on 10/9/22.
//

#include "../main.h"


struct box {

    double width;
    double height;

    double x;
    double x_vel;

    double y;
    double y_vel;

};

int box_collision(struct box *box1, struct box *box2) {
    return (box1 != box2) &&
           (box1->x < (box2->x + box2->width)) &&
           ((box1->x + box1->width) > box2->x) &&
           (box1->y < (box2->y + box2->height)) &&
           ((box1->y + box1->height) > box2->y);
}

void box_tick(struct box *b) {
    b->x += b->x_vel;
    b->y += b->y_vel;
}

void box_normalize(double *a, double *b) {
    double abs_a = *a;
    double abs_b = *b;

    if (abs_a < 0)abs_a *= -1;
    if (abs_b < 0)abs_b *= -1;

    double base = abs_a > abs_b ? abs_a : abs_b;
    if (base < 1)base = 1;

    *a = *a / base;
    *b = *b / base;
}

void box_handle_collision(struct box *box_a, struct box *box_b) {
    double x = box_b->x - box_a->x;
    double y = box_b->y - box_a->y;

    box_normalize(&x, &y);
//    printf("x:%f y:%f\n", x, y);

    double v_a = x * box_a->x_vel + y * box_a->y_vel;
    double v_b = x * box_b->x_vel + y * box_b->y_vel;

    if (v_a <= 0 && v_b >= 0) {
        return;
    }

//    double mR = 1;

    //The velocity of each ball after a collision can be found by solving the quadratic equation
    //given by equating momentum energy and energy before and after the collision and finding the
    //velocities that satisfy this
    //-(mR+1)x^2 2*(mR*vB+vA)x -((mR-1)*vB^2+2*vA*vB)=0
    //first we find the discriminant
//    double a = -2;
    double b = 2 * (v_b + v_a);
    double c = -(2 * v_a * v_b);
    double discriminant = sqrt(b * b + 8 * c);
    double root = (-b + discriminant) / -4;


    if (root - v_b < 0.01) {
        root = (-b - discriminant) / -4;
    }

    double delta_a_x = x * (v_b - root);
    double delta_a_y = y * (v_b - root);
    double delta_b_x = x * (root - v_b);
    double delta_b_y = y * (root - v_b);

    box_a->x_vel += delta_a_x;
    box_a->y_vel += delta_a_y;
    box_b->x_vel += delta_b_x;
    box_b->y_vel += delta_b_y;

}

int main() {

    double canvas_width = 1280;
    double canvas_height = 720;

    size_t num_box = 10;
    struct box *boxes = malloc(sizeof(struct box) * num_box);

    for (size_t i = 0; i < num_box; i++) {
        boxes[i].height = 20;
        boxes[i].width = 20;

        boxes[i].x_vel = rand() % 5;
        boxes[i].y_vel = rand() % 5;

        boxes[i].x = i * 50;
        boxes[i].y = i * 50;
    }


    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    int finished = 0;

    int init_result = SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL Template",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              canvas_width, canvas_height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED
                                  | SDL_RENDERER_PRESENTVSYNC);


    while (!finished) {

//        puts("hello");

        //Sets the background colour
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        //Clears target with a specific drawing colour (prev function defines colour)
        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        for (size_t i = 0; i < num_box; i++) {

            SDL_Rect r;
            r.x = boxes[i].x;
            r.y = boxes[i].y;
            r.w = boxes[i].width;
            r.h = boxes[i].height;

            SDL_RenderFillRect(renderer, &r);
        }

        SDL_RenderPresent(renderer);

        for (size_t i = 0; i < num_box; i++) {

            box_tick(boxes + i);


            // Handle the edges (boxes don't get a choice here)
            if (boxes[i].x + boxes[i].width / 2 > canvas_width) {
                boxes[i].x = (canvas_width - boxes[i].width / 2);
                boxes[i].x_vel = (boxes[i].x_vel * -1);
            }
            if (boxes[i].y + boxes[i].height / 2 > canvas_height) {
                boxes[i].y = (canvas_height - boxes[i].height / 2);
                boxes[i].y_vel = (boxes[i].y_vel * -1);
            }
            if (boxes[i].x - boxes[i].width / 2 < 0) {
                boxes[i].x = (0 + boxes[i].width / 2);
                boxes[i].x_vel = (boxes[i].x_vel * -1);
            }
            if (boxes[i].y - boxes[i].height / 2 < 0) {
                boxes[i].y = (0 + boxes[i].height / 2);
                boxes[i].y_vel = (boxes[i].y_vel * -1);
            }

            for (size_t j = i + 1; j < num_box; j++) {
                if (box_collision(boxes + i, boxes + j)) {
                    box_handle_collision(boxes + i, boxes + j);
                }
            }

        }


        if (SDL_PollEvent(&event)) {
            finished = (event.type == SDL_QUIT);
        }

        usleep(1000);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;


}
