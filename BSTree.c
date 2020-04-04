#include <stdio.h>
//#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct bstree {
    int key; /* Ключ */
    char *value; /* Данные */
    struct bstree *left;
    struct bstree *right;
};

struct bstree *bstree_create(int key, char *value)
{
	struct bstree *node;
	node = malloc(sizeof(*node));
	if (node != NULL) {
		node->key = key;
		node->value = (char *)malloc(sizeof(node->value) * strlen(value));
		sprintf(node->value, "%s", value);
		//node->value = value;
		node->left = NULL;
		node->right = NULL;
	}
	return node;
}

void bstree_add(struct bstree *tree, int key, char *value)
{
	struct bstree *parent, *node;
	if (tree == NULL)
		return;
/* Отыскиваем листовой узел */
	for (parent = tree; tree != NULL; ) {
		parent = tree;
		if (key < tree->key)
			tree = tree->left;
		else if (key > tree->key)
			tree = tree->right;
		else
			return;
	}
/* Создаем элемент и связываем с узлом */
	node = bstree_create(key, value);
	if (key < parent->key)
		parent->left = node;
	else
		parent->right = node;
}

struct bstree *bstree_lookup(struct bstree *tree, int key)
{
	while (tree != NULL) {
		if (key == tree->key) {
			return tree;
		} else if (key < tree->key) {
			tree = tree->left;
		} else {
			tree = tree->right;
		}
	}
	return tree;
}

struct bstree *bstree_min(struct bstree *tree)
{
	if (tree == NULL)
		return NULL;
	while (tree->left != NULL)
		tree = tree->left;
	return tree;
}

struct bstree *bstree_max(struct bstree *tree)
{
	if (tree == NULL)
		return NULL;
	while (tree->right != NULL)
		tree = tree->right;
	return tree;
}

/*int main()
{
    struct bstree *tree, *node;
    tree = bstree_create(0, "Тигр");
    bstree_add(tree, 1, "Лев");
    bstree_add(tree, 2, "Волк");
    node = bstree_lookup(tree, 2);
    printf("Value = %s\n", node->value);
    node = bstree_min(tree);
    printf("Min: value = %s\n", node->value);
    return 0;
}*/