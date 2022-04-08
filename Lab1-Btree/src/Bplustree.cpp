#include "Bplustree.h"
#include "node.h"
#define PROPER_LEAF true

BPlusTree::BPlusTree():root(new Leaf(this)){
}
BPlusTree::~BPlusTree(){
    root->killNode();
}
bool BPlusTree::search(int key){
    return root->search(key);
}
void BPlusTree::insert(int key){
    Node* properLeaf = root->search(key , PROPER_LEAF); 
    properLeaf->addKey(key);    
}
void BPlusTree::remove(int key){
    Node* leafFromDeletion = root->search(key);
    if(leafFromDeletion){
        leafFromDeletion->deleteKey(key);
    }
}
void BPlusTree::printBFS(){

}
void BPlusTree::printLinkedList(){

}
void BPlusTree::printInorder(){
    std::cout<<"gonna print\n";
    root->inorder();
    std::cout<<"\n";

}
