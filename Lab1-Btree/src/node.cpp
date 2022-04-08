#include "node.h"
#include <limits>
#include <algorithm>
#include <cmath>
#include "Bplustree.h"

static const int INT_MAX = std::numeric_limits<int>::max();

Node::Node(BPlusTree* tree): tree(tree), keys{0}, father(nullptr), nKeys(0){ 
    std::fill(keys, keys+ORDER, INT_MAX );
}

void Leaf::killNode(){
    delete this;
}

Node::~Node(){
    
}

Node* Leaf::search(int key, bool proper){
    Node* returnValueIfFalse = (proper)? this:nullptr;
    for(size_t i = 0; i < nKeys; i++){
        if(key == keys[i]){
            return this;
        }
        else if(key < keys[i]){
            return returnValueIfFalse;
        }
        else if(key > keys[i] && i == nKeys - 1){
            return returnValueIfFalse;
        }
    }
    return returnValueIfFalse;
}

size_t Node::getPositionOnFather(){
        for(size_t i = 0; i < father->nChildren; i++){
            if(father->children[i] == this)
                return i;
        }
        return 22;
}

void Leaf::divideAndCreateOrAddToFather(){
    Leaf* newSibling = new Leaf(tree);
    const size_t BEGIN = ORDER/2;
    for(size_t i = BEGIN; i < nKeys; i++){
        newSibling->keys[i - BEGIN] = keys[i];
        keys[i] = INT_MAX;
    }
    const size_t SIBKEYS = ORDER - BEGIN;
    nKeys -= SIBKEYS;
    newSibling->nKeys = SIBKEYS;
    newSibling->rightSibling = rightSibling;
    rightSibling = newSibling;

    if(!father){
        father = new Internal(tree);
        if(tree->root == this){
            tree->root = father;
        }
        father->addChildren(this);
    }
    father->addChildren(newSibling);
    newSibling->father = father;
    if(father->nKeys == ORDER)
        return father->divideAndCreateOrAddToFather();

}

bool Node::isFull(){
    return nKeys == ORDER;
}

void Leaf::addChildren(Node* child){

}

void Node::addKey(int key){
    int it = key;
    for(size_t i = 0; i < nKeys + 1; i++){
        if(it < keys[i]){
            int temp = keys[i];
            keys[i] = it;
            it = temp;
        }
    }
    nKeys++;
    if(nKeys == ORDER){
        divideAndCreateOrAddToFather();
    }
}

void Leaf::deleteKey(int key){
    size_t pos;
    for( pos = 0; pos < nKeys; pos++)
        if(keys[pos] == key)
            break;
    if(this == tree->root){
       simpleDeletion(pos);
    }
    else if(nKeys-1 >= std::ceil(ORDER/2.0)-1){
        simpleDeletion(pos);
        if(!pos)
            replaceOldKey(key, keys[0]);
    }
    else{
            Leaf* rightSibling = (Leaf*)findRightSibling();
            if(!rightSibling) rightSibling = new Leaf(tree);
            Leaf* leftSibling = (Leaf*)findLeftSibling();
            if(!leftSibling) leftSibling = new Leaf(tree);
            
            if(rightSibling && rightSibling->nKeys-1 >= std::ceil(ORDER/2.0)-1 && rightSibling->father == father){
                simpleDeletion(pos);
                if(!pos)
                    replaceOldKey(key, keys[0]);
                int old = keys[0];
                keys[nKeys++] = rightSibling->keys[0];
                rightSibling->simpleDeletion(0);
                rightSibling->replaceOldKey(keys[nKeys-1], rightSibling->keys[0]);
                replaceOldKey(old, keys[0]);
            }
            else if(leftSibling && leftSibling->nKeys-1 >= std::ceil(ORDER/2.0)-1 && leftSibling->father == father){
                simpleDeletion(pos);
                int it = leftSibling->keys[leftSibling->nKeys-1];
                for(size_t i = 0; i < nKeys + 1; i++){
                    int temp = keys[i];
                    keys[i] = it;
                    it = temp;
                }
                nKeys++;
                replaceOldKey(key, keys[0]);
                leftSibling->simpleDeletion(leftSibling->nKeys-1);
            }
            else if(rightSibling && rightSibling->father == father){
                simpleDeletion(pos);
               int oldKey = rightSibling->keys[0];
                for(size_t i = 0; i < rightSibling->nKeys; i++){
                    leftSibling->keys[i + nKeys] = leftSibling->keys[i];
                    if(i < nKeys)
                        leftSibling->keys[i] = keys[i];
                }
                rightSibling->nKeys += nKeys;
                nKeys = 0;
                rightSibling->replaceOldKey(oldKey, rightSibling->keys[0]);
                father->shiftLeftFrom(getPositionOnFather());
                if(!pos)
                    replaceOldKey(key, rightSibling->keys[0]);
                if(father->nChildren < std::ceil(ORDER/2.0))
                    father->reorganizeParent();
                delete this;
            }
            else if(leftSibling && leftSibling->father == father){
                simpleDeletion(pos);
                for(size_t i = 0; i < nKeys;i++){
                    leftSibling->keys[nKeys++] = keys[i];
                }
                nKeys = 0;
                replaceOldKey(key, leftSibling->keys[0]);
                father->shiftLeftFrom(getPositionOnFather());
                if(father->nChildren < std::ceil(ORDER/2.0))
                    father->reorganizeParent();
            }
    }
}

void Leaf::simpleDeletion(size_t pos){
    for(size_t i = pos; i < nKeys; i++)
        keys[i] = keys[i+1];
    nKeys--;
}

void Leaf::replaceOldKey(int oldKey, int newKey){
    Node* Father = father;
    Node* current = this;
    while(Father){
        size_t currentPos = current->getPositionOnFather();
        if(currentPos){
            if(Father->keys[currentPos-1] == oldKey){
                Father->keys[currentPos-1] = newKey;
                break;
            }
        }
        current = Father;
        Father = current->father;
    }
}

Node* Leaf::findRightSibling(){
    return rightSibling;
}
Node* Leaf::findLeftSibling(){
        if(!father)
            return nullptr;
        for(size_t i = 0; i < father->nChildren;i++){
            if(father->children[i] == this){
                if(i == 0 && father->father){
                    Internal* uncle = (Internal*)father->findLeftSibling();
                    if(uncle){
                        return uncle->children[uncle->nChildren-1];
                    }
                    else 
                        return uncle;
                }
                else if(i != 0){
                    return father->children[i-1];
                }
                else if(!father->father){
                    return nullptr;
                }
            }
        }
        return nullptr;
}

Internal::Internal(BPlusTree* tree): Node(tree), children{0}, nChildren(0){
}

void Internal::killNode(){
    for(size_t i = 0; i < nChildren; i++)
        children[i]->killNode();
    delete this;
}

Node* Internal::search(int key, bool proper){
    for(size_t i = 0; i < nKeys; i++){
        if(key < keys[i]){
            return children[i]->search(key, proper);
        }
        else if(key >= keys[i] && i == nKeys - 1){
            return children[i+1]->search(key, proper);
        }
    }
    return nullptr;
}

void Internal::addChildren(Node* child){
    children[nChildren] = child;
    nChildren++;
    if(nChildren > 1){
        keys[nKeys] = child->keys[0];
        nKeys++;
    }
}

void Internal::divideAndCreateOrAddToFather(){
    Internal* sibling = new Internal(tree);
    size_t dKeys = std::ceil(ORDER/2.0) - 1, dChilds = dKeys + 1;
    const size_t BEGIN = ORDER/2 + 1;
    for(size_t i = BEGIN; i < nChildren; i++){
        sibling->children[i - BEGIN] = children[i];
        children[i]->father = sibling;
        children[i] = nullptr;
        if(i > BEGIN){
            sibling->keys[i-1 - BEGIN] = keys[i-1];
            keys[i-1] = INT_MAX;
        }
    }
    nKeys -= dKeys;
    nChildren -= dChilds;
    sibling->nChildren = dChilds;
    sibling->nKeys = dKeys;
    if(!father){
        father = new Internal(tree);
        if(tree->root == this){
            tree->root = father;
        }
        father->addChildren(this);
    }
    father->addChildren(sibling);
    father->keys[father->nKeys-1] = keys[nKeys-1];
    keys[nKeys-1] = INT_MAX;
    nKeys--;
    sibling->father = father;
    if(father->nKeys == ORDER)
        father->divideAndCreateOrAddToFather();

}

void Internal::deleteKey(int key){

}
void Internal::replaceOldKey(int oldKey, int newKey){

}

Node* Internal::findLeftSibling(){
        if(!father)
            return nullptr;
        for(size_t i = 0; i < father->nChildren;i++){
            if(father->children[i] == this){
                if(i == 0 && father->father){
                    Internal* uncle = (Internal*)father->findLeftSibling();
                    if(uncle){
                        return uncle->children[uncle->nChildren-1];
                    }
                    else 
                        return uncle;
                }
                else if(i != 0){
                    return father->children[i-1];
                }
                else if(!father->father){
                    return nullptr;
                }
            }
        }
        return nullptr;
}

Node* Internal::findRightSibling(){
        if(!father)
            return nullptr;
        for(size_t i = 0; i < father->nChildren; i++){
            if(father->children[i] == this){
                if(!father->children[i+1] && father->father){
                    Internal* uncle = (Internal*)father->findRightSibling();
                    if(uncle)
                        return uncle->children[0];
                    else 
                        return uncle;
                    
                }
                else if(father->children[i+1]){
                    return father->children[i+1];
                }
                else if(!father->father){
                    return nullptr;
                }
                
            }
        }
        return nullptr;
}
void Internal::reorganizeParent(){
    if(this == tree->root){
        if(nChildren == 1){
            tree->root = children[0];
            children[0] = nullptr;
            delete this;
        }
    }
    else {
        Internal* leftSibling = (Internal*) findLeftSibling();
        if(!leftSibling) leftSibling = new Internal(tree);
        Internal* rightSibling = (Internal*) findRightSibling();
        if(!rightSibling) rightSibling = new Internal (tree);
        if(rightSibling && rightSibling->nChildren -1 >= std::ceil(ORDER/2.0) && rightSibling->father == father){
            addChildren(rightSibling->children[0]);
            rightSibling->children[0]->father = this;
            size_t posOfKeyFather = getPositionOnFather();
            keys[nKeys-1] = father->keys[posOfKeyFather];
            father->keys[posOfKeyFather] = rightSibling->keys[0];
            rightSibling->shiftLeftFrom(0);
        }
        else if(leftSibling && leftSibling->nChildren -1 >= std::ceil(ORDER/2.0) && leftSibling->father == father){
            shifRightFrom(0);
            children[0] = leftSibling->children[leftSibling->nChildren-1];
            children[0]->father = this;
            size_t posOfKeyFather = getPositionOnFather();
            keys[0] = father->keys[posOfKeyFather-1];
            father->keys[posOfKeyFather-1] = leftSibling->keys[leftSibling->nKeys-1];
            leftSibling->shiftLeftFrom(leftSibling->nChildren-1);
        }
        else if(rightSibling && rightSibling->father == father){
            size_t posOfKeyFather = getPositionOnFather();
                for(size_t i = 0; i < rightSibling->nChildren;i++ ){
                    children[nChildren++] = rightSibling->children[i];
                    if(!i){
                        keys[nKeys++] = father->keys[posOfKeyFather];
                    }
                    else{
                        keys[nKeys++] = rightSibling->keys[i-1];
                    }
                }
                for(size_t i = 0; i < nChildren;i++){
                    rightSibling->children[i] = children[i];
                    children[i]->father = rightSibling;
                    children[i] = nullptr;
                    if(i < nKeys)
                        rightSibling->keys[i] = keys[i];
                }
                rightSibling->nKeys += nKeys;
                rightSibling->nChildren += nChildren;
                father->shiftLeftFrom(posOfKeyFather);
                if(father->nChildren < std::ceil(ORDER/2.0))
                    father->reorganizeParent();
                delete this;
        }
        else if(leftSibling && leftSibling->father == father){
            size_t posOfKeyFather = getPositionOnFather();
            for(size_t i = 0; i < nChildren; i++){
                leftSibling->children[leftSibling->nChildren++] = children[i];
                children[i]->father = leftSibling;
                children[i] = nullptr;
                if(!i){
                    leftSibling->keys[leftSibling->nKeys++] = father->keys[posOfKeyFather-1];
                }
                else{
                    leftSibling->keys[leftSibling->nKeys++] = keys[i-1];
                }
            }
            father->shiftLeftFrom(posOfKeyFather);
            if(father->nChildren < std::ceil(ORDER/2.0))
                father->reorganizeParent();
            father = nullptr;
            delete this;
        }
    }
}
