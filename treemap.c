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
    TreeNode* pointer_node = tree->root ;
    int resultado ;
    do {
        // Si ambas "direcciones" del puntero son nulas, rompemos el ciclo
        if (pointer_node->left == NULL && pointer_node->right == NULL) break ;

        resultado = tree->lower_than(key, pointer_node->pair->key) ;
        if (resultado > 0) pointer_node = pointer_node->left ;
        else pointer_node = pointer_node->right ;
    } while (pointer_node != NULL) ;

    // Si son iguales se retorna (no queremos datos repetidos)
    if (is_equal(tree, key, pointer_node->pair->key)) return ;
    TreeNode* new_node = createTreeNode(key, value) ;

    // Creación del emparejamiento del nodo
    new_node->parent = pointer_node ;
    // Si la llave es menor se inserta el nodo a la izquierda, si no, a la derecha (lower_than)
    if (tree->lower_than(key, pointer_node->pair->key) > 0) pointer_node->left = new_node ;
    else pointer_node->right = new_node ;
    tree->current = new_node ;
}

TreeNode * minimum(TreeNode * x){
    do {
        if (x->left == NULL) break ;
    } while((x = x->left) != NULL) ;
    return x ;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    // Si es una HOJA (no tiene hijos)
    if (node->left == NULL && node->right == NULL) {
        // Si no tiene padre (es la raiz), la asignamos a NULL
        if (node->parent == NULL) {
            // Dependiendo del caso, habria que implementar un removeRoot (solo si es necesario)
            tree->root = NULL ;
        } else {
            // Si tiene padre, y el padre apunta al hijo a la izquierda, lo anulamos. Lo mismo para la derecha 
            if (node == node->parent->left) node->parent->left = NULL ;
            else node->parent->right = NULL ;
        }
        free(node->pair) ;
        free(node) ;
        return ;
    }

    // Si tiene UN hijo 
    if (node->left == NULL || node->right == NULL) {
        TreeNode* child ;
        if (node->left == NULL) child = node->right ;
        else child = node->left ; 

        if (node->parent == NULL) tree->root = child ;
        else {
            if (node == node->parent->left) node->parent->left = child ;
            else node->parent->right = child ;
        }

        child->parent = node->parent ;
        free(node->pair) ;
        free(node) ;
        return ;
    }

    // Si tiene DOS hijos
    TreeNode* minimo = minimum(node->right) ;
    node->pair->key = minimo->pair->key ;
    node->pair->value = minimo->pair->value ;
    removeNode(tree, minimo) ;
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
    do {
        resultado = tree->lower_than(key, node->pair->key) ;
        
        // Si el resultado es 0 y la llave es igual que la llave de mi nodo
        if (resultado == 0 && is_equal(tree, key, node->pair->key)) {
            tree->current = node ;
            return node->pair ;
        }

        // Si la llave es MENOR que la otra llave (resultado POSITIVO) va a la izquierda, si no a la derecha
        if (resultado > 0) node = node->left ;
        else node = node->right ;
    } while (node != NULL) ;

    return NULL ;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* node = tree->root ;
    if (node == NULL) return NULL ;
    TreeNode* ub_node = NULL ;
    int resultado ;
    do {
        if (is_equal(tree, key, node->pair->key)) {
            tree->current = node ;
            return node->pair ;
        }
        // La llave tiene que ser la mas chica (o igual) de los nodos
        resultado = tree->lower_than(key, node->pair->key) ;

        if (resultado > 0) { node = node->left ; } 
        else { ub_node = node ; node = node->right ; }
    } while (node != NULL) ;

    if (ub_node != NULL) {
        tree->current = ub_node ;
        return ub_node->pair ;
    }

    return NULL ;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL ;
    TreeNode* primero = minimum(tree->root->left) ;
    tree->current = primero ;
    return primero->pair ;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* current = tree->current ;
    if (current == NULL) return NULL ;
    
    // Si hay un sucesor del current, busca el valor minimo
    if (current->right != NULL) current = minimum(current->right) ;
    else {
        // El padre del current
        TreeNode* parent = current->parent ;
        // Tiene que ir subiendo hasta encontrar el ancestro más viejo (el primero)
        while (parent != NULL && parent->right == current) {
            // El current se vuelve el padre
            current = parent ;
            // El padre se vuelve su padre (abuelo del current)
            parent = parent->parent ;
        }
        // Va comprobando que el hijo del padre sea el current
        // Cuando termina (el hijo no es el sucesor del padre)
        // Convierte el current a ese hijo
        current = parent ;
    }

    tree->current = current ;
    return (current != NULL) ? current->pair : NULL ;
}
