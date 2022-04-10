#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "node.h"

class BPlusTree{

    public:

        BPlusTree();
        ~BPlusTree();
        bool search(int key);
        void insert(int key);
        bool remove(int key);
        void printBFS();
        void printLinkedList();
        void printInorder();

        Node* root;
};


#endif

