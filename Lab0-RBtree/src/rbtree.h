#ifndef RBTREE_H
#define RBTREE_H

#include<string>
#include<iostream>
#include<vector>
// Lista de colores del arbol
enum Color {RED,BLACK, DOUBLEBLACK};

struct Nodo{
    // Atributos
    int dato;
    int color;

    // Relacion con otros Nodos
    Nodo *left;
    Nodo *right;
    Nodo *father;

    // Constructor
    Nodo(int);
};

class RBtree
{
private:
    Nodo *root;
    Nodo *insertarNodo(Nodo* &, Nodo* &);
    void corregirArbol(Nodo* &);

    int getColor(Nodo* &);
    void setColor(Nodo* &, int );

    void rotarIzquierda(Nodo* &);
    void rotarDerecha  (Nodo* &);
    
    void addBlack(Nodo* nodo); 
    Nodo* buscar(int dato);
    Nodo* eliminacionBS(int dato);
    void eliminacionRB(Nodo* nodo);
    Nodo* encontrarSucesor(Nodo* aEliminar);
    // metodos privados
    void preorder(Nodo* nodo, std::vector<int>& pre);
    void inorder(Nodo* nodo, std::vector<int>& in);
    void postorder(Nodo* nodo, std::vector<int>& post);

public:
    RBtree();
    
    void insertar(int);
    void eliminarNodo(int dato);
    std::vector<int> preorden();
    std::vector<int> inorden();
    std::vector<int> postorden();
    void print(){
        print(root);    
    }
    void print(Nodo*node){
        if (node == NULL)
            return;
 
    /* first recur on left child */
        print(node->left);
 
    /* then print the data of node */
        std::cout << node->dato << " ";
 
    /* now recur on right child */
        print(node->right);
    }

    ~RBtree();
};






#endif
