#include "BplusTree.h"

    Node::Node(): 
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

    Node::~Node(){}

    bool Node::addKey(int key, Node* node){
        
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

    Node* Node::search(int key){
        if(!nKeys)
            return nullptr;
        for(size_t i = 0; i < nKeys; i++){
            if(key >= keys[i]){
                if(children[i+1])
                    return children[i+1]->search(key);
                else if(key == keys[i])
                    return this;
                else 
                    return nullptr;
            }
            else if(key < keys[i]){
                if(children[i])
                    return children[i]->search(key);
                else 
                    return nullptr;
            }
        }
        return nullptr;
    }

    Node* Node::searchProperLeaf(int key){
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
    Node* Node::createSibling(){
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

    void Node::killNode(){
        for(size_t i = 0; i < ORDER; i++){
            if(children[i]){
                children[i]->killNode();
            }
        }
        delete this;
    }

    void Node::getBackHead(){
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

    void Node::printNode(const char* message){
        std::cout<<message;
        for(size_t i = 0; i < nKeys; i++)
            std::cout<<keys[i]<<" ";
    }

