#ifndef BSTREE_H
#define BSTREE_H

struct bstree {
    int key; /* Ключ */
    char *value; /* Данные */
    struct bstree *left;
    struct bstree *right;
};

struct bstree *bstree_create(int key, char *value);
void bstree_add(struct bstree *tree, int key, char *value);
struct bstree *bstree_lookup(struct bstree *tree, int key);
struct bstree *bstree_min(struct bstree *tree);
struct bstree *bstree_max(struct bstree *tree);

#endif