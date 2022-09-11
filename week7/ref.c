//
// Created by xiaoxiang on 9/11/22.
//

#include "ref.h"

struct ref {
    size_t counter;
    void *ref;
    void *dep;
    size_t dep_counter;
};

struct ref *refs = NULL;
size_t num_ref = 0;

void *new_ref(size_t size, void *dep) {

    refs = realloc(refs, ++num_ref * sizeof(struct ref));

    refs[num_ref - 1].counter = 1;
    refs[num_ref - 1].ref = malloc(size);
    refs[num_ref - 1].dep = dep;
    refs[num_ref - 1].dep_counter = 0;

    if (dep != NULL) {
        for (size_t i = 0; i < num_ref; i++) {
            if (refs[i].ref == dep) {
                refs[i].dep_counter++;
            }
        }
    }

    return refs[num_ref - 1].ref;
}


void *inc_ref(void *ref) {
    if (ref == NULL) return NULL;

    for (size_t i = 0; i < num_ref; i++) {
        if (refs[i].ref == ref) {
            refs[i].counter++;
            return ref;
        }
    }
    return NULL;
}

void *dec_ref(void *ref, int deep) {

    if (ref == NULL) return NULL;

    for (size_t i = 0; i < num_ref; i++) {
        if (refs[i].ref == ref) {

            if (deep && refs[i].dep_counter > 0) {
                refs[i].dep_counter--;
            } else if (refs[i].counter > 0) {
                refs[i].counter--;
            }


            if (refs[i].counter == 0 && refs[i].dep_counter == 0) {

                free(refs[i].ref);
                dec_ref(refs[i].dep, 1);
            }

            return ref;
        }
    }
    return NULL;
}

struct tree_node {

    int value;
    struct tree_node *left;
    struct tree_node *right;
};

int main() {

    struct tree_node *root = new_ref(sizeof(struct tree_node), NULL);

    root->left = new_ref(sizeof(struct tree_node), root);
    root->right = new_ref(sizeof(struct tree_node), root);

    root->left->left = new_ref(sizeof(struct tree_node), root->left);
    root->left->right = new_ref(sizeof(struct tree_node), root->left);
//    inc_ref(root);
//    inc_ref(root);

    root->right->left = new_ref(sizeof(struct tree_node), root->right);
    root->right->right = new_ref(sizeof(struct tree_node), root->right);
//    inc_ref(root);
//    inc_ref(root);

    for (size_t i = 0; i < num_ref; i++) {
        printf("idx %zu ref %zu %zu\n", i, refs[i].counter, refs[i].dep_counter);
    }


    dec_ref(root, 0);
    dec_ref(root, 0);
    dec_ref(root, 0);
    dec_ref(root, 0);
    dec_ref(root, 0);

    puts("");
    puts("");
    for (size_t i = 0; i < num_ref; i++) {
        printf("idx %zu ref %zu %zu\n", i, refs[i].counter, refs[i].dep_counter);
    }

    dec_ref(root->right->left, 0);
    dec_ref(root->right->right, 0);
//    dec_ref(root->left->left, 0);
//    dec_ref(root->left->right, 0);

    dec_ref(root->left, 0);
    dec_ref(root->right, 0);

    puts("");
    puts("");
    for (size_t i = 0; i < num_ref; i++) {
        printf("idx %zu ref %zu %zu\n", i, refs[i].counter, refs[i].dep_counter);
    }

}