//
// Created by Xiaoxiang Wu on 5/9/2022.
//

#include "../main.h"

struct dumb_tree_node {

    int index;
    void *value;

    struct dumb_tree_node *left;
    struct dumb_tree_node *right;

    pthread_mutex_t lock;
};

struct dumb_tree {

    struct dumb_tree_node *root;

    pthread_mutex_t root_lock;
    int size;

};

struct dumb_tree *dumb_tree_new() {

    struct dumb_tree *tree = calloc(1, sizeof(struct dumb_tree));
    pthread_mutex_init(&tree->root_lock, NULL);

    return tree;
}

struct dumb_tree_node *dumb_tree_node_new(int index, void *value) {

    struct dumb_tree_node *node = calloc(1, sizeof(struct dumb_tree));
    pthread_mutex_init(&node->lock, NULL);

    return node;
}

void dumb_tree_put(struct dumb_tree *tree, int index, void *value) {

    struct dumb_tree_node *current;

    pthread_mutex_lock(&tree->root_lock);

    current = tree->root;
    if (current == NULL) {
        struct dumb_tree_node *node = dumb_tree_node_new(index, value);
        tree->root = node;
        pthread_mutex_unlock(&tree->root_lock);
        return;
    }

    pthread_mutex_unlock(&tree->root_lock);


    bool cond = true;

    while (cond) {

        pthread_mutex_t *target_lock = &current->lock;

        pthread_mutex_lock(target_lock);

        if (current->index == index) {
            current->value = value;
            cond = false;
        } else if (current->index > index) {
            if (current->left == NULL) {
                current->left = dumb_tree_node_new(index, value);
                cond = false;
            } else {
                current = current->left;
            }
        } else if (current->index < index) {
            if (current->right == NULL) {
                current->right = dumb_tree_node_new(index, value);
                cond = false;
            } else {
                current = current->right;
            }
        } else {
            puts("impossible!");
        }

        pthread_mutex_unlock(target_lock);

    }

}

void *dumb_tree_get(struct dumb_tree *tree, int index) {
    struct dumb_tree_node *current;
    pthread_mutex_lock(&tree->root_lock);
    current = tree->root;
    pthread_mutex_unlock(&tree->root_lock);


    while (current != NULL) {
        pthread_mutex_t *target_lock = &current->lock;

        pthread_mutex_lock(target_lock);

        if (current->index == index) {
            break;
        } else if (current->index > index) {
            current = current->left;
        } else if (current->index < index) {
            current = current->right;
        } else {
            puts("impossible!");
        }

        pthread_mutex_unlock(target_lock);
    }

    void *value = NULL;
    if (current != NULL) {
        value = current->value;
        pthread_mutex_unlock(&current->lock);
    }

    return value;
}

struct thread_arg {

    struct dumb_tree *tree;

    int *array;

    int index_start;
    int index_end;
};

void *insert(void *arg) {

    struct thread_arg args = *(struct thread_arg *) arg;

    for (int i = args.index_start; i != args.index_end; i++) {
        dumb_tree_put(args.tree, args.array[i], args.array + i);
    }

    return NULL;
}

void *update(void *arg) {

    struct thread_arg args = *(struct thread_arg *) arg;

    for (int i = args.index_start; i != args.index_end; i++) {
        dumb_tree_put(args.tree, args.array[i], args.array + i);
    }

    return NULL;
}

void *lookup(void *arg) {

    struct thread_arg args = *(struct thread_arg *) arg;

    for (int i = args.index_start; i != args.index_end; i++) {
        void *value = dumb_tree_get(args.tree, args.array[i]);

        if (value != args.array + i) {
            die("wrong value");
        }
    }

    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 3) return 1;

    int n = atoi(argv[1]);
    int num_thread = atoi(argv[2]);

    int *array = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        array[i] = i;
    }
    shuffle(array, n, sizeof(int));

    struct dumb_tree *tree = dumb_tree_new();

    pthread_t *threads = malloc(num_thread * sizeof(pthread_t));
    struct thread_arg *thread_args = malloc(num_thread * sizeof(struct thread_arg));
    for (int i = 0; i < num_thread; i++) {
        thread_args[i] = (struct thread_arg) {
                .array=array,
                .tree=tree,
                .index_start=n / num_thread * i,
                .index_end=n / num_thread * (i + 1)
        };
    }

    declare_timer
    start_timer

    for (int i = 0; i < num_thread; i++) {
        pthread_create(threads + i, NULL, insert, NULL);
    }
    for (int i = 0; i < num_thread; i++) {
        pthread_join(threads[i], NULL);
    }

    stop_timer("insert");

    start_timer

    for (int i = 0; i < num_thread; i++) {
        pthread_create(threads + i, NULL, update, NULL);
    }
    for (int i = 0; i < num_thread; i++) {
        pthread_join(threads[i], NULL);
    }

    stop_timer("update");

    start_timer

    for (int i = 0; i < num_thread; i++) {
        pthread_create(threads + i, NULL, lookup, NULL);
    }
    for (int i = 0; i < num_thread; i++) {
        pthread_join(threads[i], NULL);
    }

    stop_timer("lookup");


}