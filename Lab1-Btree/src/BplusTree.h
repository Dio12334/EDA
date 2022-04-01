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
        void insertar(int key){
            Node* insertionNode = root->searchProperLeaf(key);
            if(!insertionNode->addKey(key))
                divideNode(insertionNode);
        }
        void printInorder(){
            inorder(root);
            std::cout<<"\n";
        }
        
        void BFS(){
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
        void divideNode(Node* node){
            Node* sibling = node->createSibling();
            sibling->leaf = node->leaf;
            if(!node->father){
                Node* newFather = new Node();
                newFather->leaf = false;
                root = newFather;
                sibling->father = node->father = newFather;
                newFather->addKey(sibling->keys[0]);
                newFather->children[0] = node;
                newFather->children[1] = sibling;
            }
            else{
                sibling->father = node->father;
                node->father->addKey(sibling->keys[0], sibling);
            }
            if(!sibling->leaf)
                sibling->getBackHead();
            if(node->leaf)
                node->rightSibling = sibling;
            if(node->father->nKeys == ORDER)
                divideNode(node->father);
        }
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

