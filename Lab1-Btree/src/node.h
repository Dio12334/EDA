#ifndef NODE_H
#define NODE_H

#define ORDER 3

#include <iostream>

struct Internal;

struct Node{
    class BPlusTree* tree;    
    int keys[ORDER];
    Internal* father;

    Node(BPlusTree* tree);
    virtual void killNode() = 0;
    virtual Node* search(int key, bool proper = false) = 0;
    virtual void divideAndCreateOrAddToFather() = 0;
    virtual void addChildren(Node* child) = 0;
    virtual void inorder() = 0;
    virtual void deleteKey(int key) = 0;
    virtual void replaceOldKey(int oldKey, int newKey) = 0;
    virtual Node* findRightSibling() = 0;
    virtual Node* findLeftSibling() = 0;
    virtual ~Node();
    bool isFull();
    void setnKeys(size_t n){ nKeys = n;}
    size_t getnKeys() const {return nKeys;}
    void printNode(const char* message){
        std::cout<<message;
        for(size_t i = 0; i < nKeys; i++){
            std::cout<<keys[i]<<" ";
        }
    }
    void addKey(int key);
    size_t getPositionOnFather(); 
    size_t nKeys;
};

struct Leaf: public Node{
    Leaf* rightSibling;
    Leaf(BPlusTree* tree): Node(tree), rightSibling(nullptr){}
    void killNode() override;
    Node* search(int key, bool proper = false) override;
    void divideAndCreateOrAddToFather() override;
    void addChildren(Node* child) override;
    void inorder() override { std::cout<<"[ "; printNode("");std::cout<<"] ";}
    void deleteKey(int key) override;
    void replaceOldKey(int oldKey, int newKey) override;
    Node* findRightSibling() override;
    Node* findLeftSibling() override;
    void simpleDeletion(size_t pos);
};

struct Internal: public Node{
    Node* children[ORDER+1];

    Internal(BPlusTree* tree);
    void killNode() override;
    Node* search(int key, bool proper) override;
    void addChildren(Node* child) override;
    void divideAndCreateOrAddToFather() override;
    void setnChildren(size_t n){nChildren = n;}
    size_t getnChildren() const {return nChildren;}
    void inorder() override {
        std::cout<<"[ "; 
        for(size_t i = 0; i < nKeys; i++){
            children[i]->inorder(); 
            std::cout<<keys[i]<<" ";
        } 
        children[nChildren-1]->inorder();
        std::cout<<"] ";
    }
    size_t nChildren;
    void deleteKey(int key) override;
    void replaceOldKey(int oldKey, int newKey) override;
    Node* findRightSibling() override;
    Node* findLeftSibling() override;
    void shiftLeftFrom(size_t pos){
        for(size_t i = pos; i < nChildren; i++){
            children[i] = children[i+1];
            if(i > 0)
                keys[i-1] = keys[i];
        }
        nChildren--;
        nKeys--;
    }
    void shifRightFrom(size_t pos){
        for(size_t i = nChildren; i > pos; i++){
            children[i] = children[i-1];
            if(i <= nKeys)
                keys[i] = keys[i-1];
        }
        nChildren++;
        nKeys++;
    }
    void reorganizeParent();
};


#endif

