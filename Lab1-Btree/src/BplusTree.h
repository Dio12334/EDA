#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#define ORDER 21
#include <iostream>
#include <limits>
#include <cmath>

struct Node{
    size_t nChildren, nKeys;
    Node* children[ORDER+1];
    int keys[ORDER];
    Node* father;
    Node* rightSibling;
    bool leaf;

    Node(): 
    nChildren(0), 
    nKeys(0),  
    father(nullptr),
    rightSibling(nullptr),
    leaf(true){
        for(size_t i = 0; i < ORDER + 1; i++)
            children[i] = nullptr;
        for(size_t i = 0; i < ORDER; i++){
            keys[i] = std::numeric_limits<int>::max();
        }
    }

    ~Node(){
        killNode();
    }

    bool addKey(int key, Node* node = nullptr){
        
        int itValue = key;
        Node* itNode = node;
        nKeys++;
        for(size_t i = 0; i < nKeys; i++){
            if(itValue < keys[i] ){
                int temp = keys[i];
                keys[i] = itValue;
                itValue = temp;
                Node* tempNode = children[i+1];
                children[i+1] = itNode;
                itNode = tempNode;
            }   
        }
        nChildren += (nKeys == 1)? 2:1;
        if(nKeys-1 < ORDER - 1)
            return true;
        else if(nKeys - 1 == ORDER - 1) 
            return false;
        return false;
    }

    Node* search(int key){
        if(!nKeys)
            return nullptr;
        for(size_t i = 0; i < nKeys; i++){
            if(key >= keys[i]){
                if(children[i+1])
                    return children[i+1]->search(key);
                else 
                    return nullptr;
            }
            else if(key < keys[i]){
                if(children[i])
                    return children[i]->search(key);
                else return nullptr;
            }
        }
    }

    Node* searchProperLeaf(int key){
        if(!nKeys)
            return this;
        for(size_t i = 0; i < nKeys; i++){
            if(key >= keys[i] && i == nKeys -1){
                if(children[i+1])
                    return children[i+1]->searchProperLeaf(key);
                else
                    return this;
            }
            else if(key < keys[i]){
                if(children[i])
                    return children[i]->searchProperLeaf(key);
                else 
                    return this;
            }
        }
        return nullptr; 
    }
    Node* createSibling(){
        Node* sibling = new Node();
        for(size_t i = (ORDER-1)/2; i < ORDER; i++){
            sibling->addKey(keys[i]);
            keys[i] = std::numeric_limits<int>::max();
            nKeys--; 
        }
        for(size_t i = (ORDER-1)/2 + 1; i < ORDER+1; i++){
            sibling->children[i - (ORDER-1)/2] = children[i];
            children[i] = nullptr;
            nChildren--;
        }
        return sibling;
    }

    void killNode(){
        for(size_t i = 0; i < ORDER; i++){
            if(children[i])
                children[i]->killNode();
            delete children[i];
        }
    }

    void getBackHead(){
        size_t i;
        for( i = 1; i < nKeys; i++){
            keys[i-1] = keys[i];
            children[i-1] = children[i];
        }
        children[i-1] = children[i];
        children[i] = nullptr;
        keys[nKeys-1] = std::numeric_limits<int>::max();
        nKeys--;
        nChildren--;

    }

    void printNode(const char* message){
        std::cout<<message;
        for(size_t i = 0; i < nKeys; i++)
            std::cout<<keys[i]<<" ";
        std::cout<<"\n";
    }
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
            if(insertionNode->addKey(key)){
                return;
            }
            else{
                divideNode(insertionNode);
            }
        }
        void printInorder(){
            inorder(root);
            std::cout<<"\n";
        }
        
    private:
        Node* root;
        void divideNode(Node* node){
            Node* sibling = node->createSibling();
            if(!node->leaf)
                sibling->leaf = false;
            if(!node->father){
                Node* newFather = new Node();
                newFather->leaf = false;
                root = newFather;
                sibling->father = newFather;
                node->father = newFather;
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

