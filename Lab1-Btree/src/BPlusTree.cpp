#include "BPlusTree.h"
#include <cmath>
#include <limits>

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

    /*addKey recibe dos parametros, la key a insertar y su hijo derecho si es que 
     * existe (por defecto es nullptr)*/
    bool Node::addKey(int key, Node* node){
        //la key se va a insertar en su lugar desplazando al resto de elementos a su derecha
        //lo mismo con su hijo respectivo
        int itValue = key;
        Node* itNode = node;
        nKeys++;

        bool mySonIsALeaf = (children[0])? children[0]->leaf:false;
        for(size_t i = 0; i < nKeys; i++){
            if(itValue < keys[i] ){
                int temp = keys[i];
                keys[i] = itValue;
                itValue = temp;
                
                Node* tempNode = children[i+1];
                children[i+1] = itNode;
                itNode = tempNode;
                if(mySonIsALeaf)
                    children[i]->rightSibling = children[i+1];
            }   
        }
        nChildren += (nKeys == 1)? 2:1;
        //retorna verdadero si el nodo no se sobrellenado
        //y false en caso contrario
        if(nKeys - 1 == ORDER - 1) 
            return false;
        return true;
    }

    /*Busca la key en el arbol y retorna el nodo en el que se encuentra*/
    Node* Node::search(int key){
        if(!nKeys)
            return nullptr;
        for(size_t i = 0; i < nKeys; i++){
            if(key == keys[i] && leaf){
                return this;
            }
            else if(key >= keys[i] && i == nKeys-1){
                if(children[i+1])
                    return children[i+1]->search(key);
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

    /*Busca la key en el arbol y retorna el nodo en el que debería ir*/
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

    /*Este metodo divide al nodo en 2 y retorna a su otra mitad*/
    Node* Node::createSibling(){
        Node* sibling = new Node();
        for(size_t i = (ORDER)/2; i < ORDER; i++){
            sibling->addKey(keys[i]);
            keys[i] = std::numeric_limits<int>::max();
            nKeys--; 
        }
        for(size_t i = (ORDER)/2 + 1; i < ORDER+1; i++){
            sibling->children[i - (ORDER)/2] = children[i];
            children[i] = nullptr;
            nChildren--;
        }
        return sibling;
    }

    /*Lo llamo para liberar la memoria y no causar memory leaks*/
    void Node::killNode(){
        for(size_t i = 0; i < ORDER; i++){
            if(children[i]){
                children[i]->killNode();
            }
        }
        delete this;
    }
    
    /*Cuando un nodo interno se divide tiene que pasar su valor del medio
     * haciendo que el hijo derecho pierda su key en posicion 0,
     * este metodo mueve todos los keys 1 a la izquierda y lo mismo con sus hijos*/
    void Node::getBackHead(){
        size_t i;
        for( i = 1; i < nKeys; i++){
            keys[i-1] = keys[i];
            children[i-1] = children[i];
            children[i-1]->father = this;
        }
        children[i-1] = children[i];
        children[i-1]->father = this;
        children[i] = nullptr;
        keys[nKeys-1] = std::numeric_limits<int>::max();
        nKeys--;
        nChildren--;

    }

    /*Printea el nodo y puedes ponerle un mensaje si quieres para debugear*/
    void Node::printNode(const char* message){
        std::cout<<message;
        for(size_t i = 0; i < nKeys; i++)
            std::cout<<keys[i]<<" ";
    }
    
    void Node::simpleDeletion(size_t positionOfKey){
        for(size_t i = positionOfKey +1; i <= nKeys; i++){
            keys[i-1] = keys[i];
        }
        nKeys--;
        nChildren--;
    }

    Node* Node::findLeftSibling(){
        if(!father)
            return nullptr;
        for(size_t i = 0; i < father->nChildren;i++){
            if(father->children[i] == this){
                if(i == 0 && father->father){
                    Node* uncle = father->findLeftSibling();
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

    Node* Node::findRightSibling(){
        if(leaf)
            return rightSibling;
        if(!father)
            return nullptr;
        for(size_t i = 0; i < father->nChildren; i++){
            if(father->children[i] == this){
                if(!father->children[i+1] && father->father){
                    Node* uncle = father->findRightSibling();
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

void Node::arrangeNode(size_t positionOfChildToShift){
    for(size_t i = positionOfChildToShift+1; i <= nChildren; i++){
        children[i-1] = children[i];
        if(i > 1)
            keys[i-2] = keys[i-1];
    }
}

size_t Node::getPositionInFatherList(){
    size_t i;
    for(i = 0; i < father->nChildren;i++){
        if(father->children[i] == this)
            return i;
    }
    return -1;
}

Node* Node::rearrangeInternals(){
    Node* rightSibling = findRightSibling();
    Node* leftSibling = findLeftSibling();

    if(!father){
        return this;
    }

    if(leftSibling){
        if(leftSibling->nChildren-1 >= std::ceil(ORDER/2.0)){
            addKey(leftSibling->keys[nKeys-1], leftSibling->children[nChildren-1]);
            leftSibling->arrangeNode(nChildren-1);
            leftSibling->nKeys--;
            leftSibling->nChildren--;
        }
        else{
            father->keys[getPositionInFatherList()-1] = std::numeric_limits<int>::max();
            father->nKeys--;
            father->children[getPositionInFatherList()] = nullptr;
            father->nChildren--;
            leftSibling->pushBackChildren(this);
        }
    }
    else if(rightSibling){
        if(rightSibling->nChildren-1 >= std::ceil(ORDER/2.0)){
            addKey(rightSibling->children[0]->keys[0], rightSibling->children[0]);
            rightSibling->simpleDeletion(0);
            rightSibling->nKeys--;
            rightSibling->nChildren--;
        }
        else{
            
        }
 
    }
    return this;
}
void Node::pushFrontChildren(Node* nodeToPush){
    for(size_t i = nChildren; i > 0; i--){
        children[i] = children[i-1];
    }
    for(size_t i = nKeys; i > 0; i--)
        keys[i] = keys[i-1];
    for(size_t i = 0; i < nodeToPush->nChildren; i++){
        
    }
}
void Node::pushBackChildren(Node* nodeToPush){
    for(size_t i = 0; i < nodeToPush->nChildren; i++){
        addKey(nodeToPush->children[i]->keys[0], nodeToPush->children[i]);
    }
}

inline void replaceKeyOfImmediateFather(Node* node){
    node->father->keys[node->getPositionInFatherList()-1] = node->keys[0];
}
inline void replaceKeyOfGrandfather(Node* node){
    node->father->father->keys[node->father->getPositionInFatherList()-1] = node->keys[0];
}

inline void replaceInternalNodes(Node* node, size_t pos, int oldValue){
    if(!node->getPositionInFatherList() && !pos){
        
        Node* father = node->father;
        while(father->father){
            if(father->father->keys[father->getPositionInFatherList()-1] == oldValue){
                father->father->keys[father->getPositionInFatherList()-1] = node->keys[0];
                break;
            }
            father = father->father;
        }
    }
    else if(!pos){
        replaceKeyOfImmediateFather(node);
    }
}

inline void mergeLeafs(Node* toMerge, Node* mergeTo){
    for(size_t i = 0; i < toMerge->nKeys; i++){
        mergeTo->addKey(toMerge->keys[i]);
        toMerge->nKeys--;
        toMerge->nChildren--;
    }
    toMerge->nChildren--;

}

inline void keyUP(Node* node, size_t positionOnFL){
    int keyUP;
    if(!positionOnFL){
        keyUP = node->father->keys[0];
        node->father->father->keys[node->father->getPositionInFatherList()-1] = keyUP;
    }
}
inline void shiftRight(Node* node){
    for(size_t i = node->nKeys; i > 0; i--){
        node->keys[i] = node->keys[i-1];
        node->children[i+1] = node->children[i];
    }
    node->children[1] = node->children[0];
    node->keys[0] = node->children[1]->keys[0];
}
inline void sendChildrenRight(Node* from, Node* to){
    shiftRight(to);
    to->children[0] = from->children[from->nChildren-1];
    from->children[from->nChildren-1] = nullptr;
    from->nChildren--;
}

inline void sendChildrenLeft(Node* from, Node* to){
    to->addKey(from->children[0]->keys[0], from->children[0]);
    from->getBackHead();
}

Node* mergeInternals(Node* node, Node* root){
    Node* leftSibling = node->findLeftSibling();
    Node* rightSibling = node->findRightSibling();
    if(leftSibling){
        if(leftSibling->nChildren-1 >= std::ceil(ORDER/2.0) ){
            sendChildrenRight(leftSibling, node);
            replaceInternalNodes(node, 0, node->keys[1]);
            return root;
        }
        else{
            for(size_t i = 0; i < node->nChildren; i++){
                leftSibling->addKey(node->children[i]->keys[0], node->children[i]);
                node->children[i] = nullptr;
            }
            node->nChildren = 0;
            size_t position = node->getPositionInFatherList();
            if(position)
                node->father->keys[position-1] = std::numeric_limits<int>::max();
            node->father->children[position] = nullptr;
            node->father->nChildren--;
            node->father->nKeys--;
            Node* father = node->father;
            delete node;
            if(father->nChildren < std::ceil(ORDER/2.0))
                return mergeInternals(father, root);
            return root;
        }
    }
    else if(rightSibling){
        if(rightSibling->nChildren-1 >= std::ceil(ORDER/2.0)){
            sendChildrenLeft(rightSibling, node);
            return root;
        }
        else{
            for(size_t i = 0; i < rightSibling->nChildren; i++){
                node->addKey(rightSibling->children[i]->keys[0], rightSibling->children[i]);
            }
            for(size_t i = 0; i < node->nChildren; i++){
                rightSibling->children[i] = node->children[i];
                if(i < node->nKeys)
                    rightSibling->keys[i] = node->keys[i];
            }
            rightSibling->nKeys = node->nKeys;
            rightSibling->nChildren = node->nChildren;
            node->nChildren = 0;
            size_t position = node->getPositionInFatherList();
            if(position)
                node->father->keys[position-1] = std::numeric_limits<int>::max();
            node->father->children[position] = nullptr;
            node->father->nChildren--;
            node->father->nKeys--;

            Node* father = node->father;
            delete node;
            if(father->nChildren < std::ceil(ORDER/2.0))
                return mergeInternals(father, root);
            return root;
        }
    }
    else{
        if(node->nChildren > 1)
            return node;
        else{
            Node* child = node->children[0];
            node->children[0] = nullptr;
            node->nChildren--;
            child->father = nullptr;
            delete node;
            return child;
        }
    }
    return node;
}

/*Return the root after the deletion of the key*/
Node* Node::moreConciseDelete(int key, Node* root){
    size_t pos; 
    for(pos = 0; pos < nKeys; pos++){
        if(keys[pos] == key)
            break;
    }
    // if root you just delete 
    if(leaf && !father){
        simpleDeletion(pos);
        return this;
    }
    // if you are a leaf but the key you remove doesnt leave you with less than half
    else if(leaf && nKeys - 1 >= std::ceil(ORDER/2.0) - 1){
        simpleDeletion(pos);
        replaceInternalNodes(this, pos, key);
        return root;
    }// if you need help of your immediate siblings
    else if(leaf && nKeys-1 < std::ceil(ORDER/2.0)-1){
        Node* leftSibling = findLeftSibling();
        Node* rightSibling = findRightSibling();
        //if you have a left sibling
        if(leftSibling){
            //borrow from left 
            if(leftSibling->nKeys-1 >= std::ceil(ORDER/2.0)-1){
                simpleDeletion(pos);
                addKey(leftSibling->keys[leftSibling->nKeys-1]);
                leftSibling->simpleDeletion(leftSibling->nChildren-1);
                replaceInternalNodes(this, pos, key);
                return root;
            }// merge with left
            else{
                simpleDeletion(pos);
                mergeLeafs(this, leftSibling);
                size_t positionOnFather = getPositionInFatherList();
                father->arrangeNode(positionOnFather);
                replaceInternalNodes(father->children[0], 0, key);
                father->nKeys--;
                father->nChildren--;
                leftSibling->rightSibling = rightSibling;
                if(father->nChildren < std::ceil(ORDER/2.0))
                    return mergeInternals(father, root);
                return root;
            }
        }
        // if you instead have a right sibling
        else if(rightSibling){
            //borrow right
            if(rightSibling->nKeys-1 >= std::ceil(ORDER/2.0)-1){
                simpleDeletion(pos);
                addKey(rightSibling->keys[0]);
                rightSibling->simpleDeletion(0);
                replaceKeyOfImmediateFather(rightSibling);
                replaceInternalNodes(this, pos, key);
                return root;
            } // merge with right
            else{
                simpleDeletion(pos);
                mergeLeafs(this, rightSibling);
                size_t positionOnFather = getPositionInFatherList();
                father->arrangeNode(positionOnFather);
                replaceInternalNodes(father->children[0], 0, key);
                father->nKeys--;
                father->nChildren--;
                if(father->nChildren < std::ceil(ORDER/2.0))
                    return  mergeInternals(father, root);
                return root;
            }
        }
    }
    return root;
}
