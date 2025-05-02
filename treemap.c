#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* new_treeMap = (TreeMap*) malloc(sizeof(TreeMap)) ;
    if (new_treeMap == NULL) return NULL ;
    new_treeMap->root = NULL ;
    new_treeMap->current = NULL ;
    new_treeMap->lower_than = lower_than;
    return new_treeMap ;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {

}

TreeNode * minimum(TreeNode * x){

    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* node = tree->root ;
    int resultado ;
    /*
    if (tree->lower_than(key, node->pair->key) == 0) {
        tree->current = node ;
        return node->pair ;
    }

    if (tree->lower_than(key, node->pair->key) > 0) {
        node = node->right ;
        
    } else if (tree->lower_than(key, node->pair->key) < 0) {
        node = node->left ;
    }*/
    do {
        if (node == NULL) return NULL ;
        resultado = tree->lower_than(key, node->pair->key) ;
        // Si la llave es MENOR que la otra llave (resultado POSITIVO)

        if (resultado == 0 && is_equal(tree, key, node->pair->key)) {
            tree->current = node ;
            return node->pair ;
        }

        if (resultado > 0) node = node->left ;
        else node = node->right ;
    } while (node != NULL) ;

    return NULL ;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
