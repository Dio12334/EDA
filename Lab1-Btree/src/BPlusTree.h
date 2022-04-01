#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#define ORDER 21
#include <iostream>
#include <limits>
#include <cmath>
#include <list>

struct Node{
    size_t nChildren, nKeys;
    Node* children[ORDER+1];
    int keys[ORDER];
    Node* father;
    Node* rightSibling;
    bool leaf;

    Node();
    ~Node();

    bool addKey(int key, Node* node = nullptr);
    Node* search(int key);
    Node* searchProperLeaf(int key);
    Node* createSibling();
    void killNode();
    void getBackHead();
    void printNode(const char* message);
};

class BplusTree{
    public:
        BplusTree():
        root(new Node()){}

        ~BplusTree(){
            root->killNode();
        }
        /*insterta la key en el arbol*/
        void insertar(int key){
            Node* insertionNode = root->searchProperLeaf(key);
            if(!insertionNode->addKey(key)) // si el nodo termina recibiendo una key más que su maximo este se divide
                divideNode(insertionNode);
        }
        /*printea inorder*/
        void printInorder(){
            inorder(root);
            std::cout<<"\n";
        }
        
        /*printea bfs*/
        void printBFS(){
            std::list<Node*> vec;
            vec.emplace_back(root);
            while(!vec.empty()){
                Node* printable = vec.front();
                vec.pop_front();
                for(size_t i = 0; i < printable->nChildren; i++){
                    if(printable->children[i])
                        vec.emplace_back(printable->children[i]);
                }
                printable->printNode("");
            }
            std::cout<<"\n";
        }
    private:
        Node* root;
        /*divide el nodo y crea su padre si es que no existe*/
        void divideNode(Node* node){
            Node* sibling = node->createSibling(); // parte el nodo en 2 y crea la otra mitad
            sibling->leaf = node->leaf;// si el nodo que partiste es hoja entonces su otra mitad tambien lo será
            if(!node->father){// si no hay un padre lo va a crear
                Node* newFather = new Node();
                newFather->leaf = false;
                root = newFather;
                sibling->father = node->father = newFather;
                newFather->addKey(sibling->keys[0]); // le agregas la referencia del key medio
                newFather->children[0] = node;
                newFather->children[1] = sibling;
            }
            else{// si ya existe un padre
                sibling->father = node->father;
                node->father->addKey(sibling->keys[0], sibling);// agregas la referencia del key y el hijo nuevo
            }
            if(!sibling->leaf)
                sibling->getBackHead(); // si subiste la referencia con un nodo no hoja mueves todas keys 1 a la izquierda
            if(node->leaf)
                node->rightSibling = sibling;// si eran hojas la vuelvas una linked list
            if(node->father->nKeys == ORDER)// si el padre se llena entonces partes al padre
                divideNode(node->father);
        }

        /*Recursion del printInorder*/
        void inorder(Node* node){
            if(!node)
                return;
            std::cout<<"[ ";
            for(size_t i = 0; i < node->nKeys; i++){
                inorder(node->children[i]);
                std::cout<<node->keys[i]<<" ";
            }
            inorder(node->children[node->nKeys]);
            std::cout<<"] ";
        }

};
#endif

