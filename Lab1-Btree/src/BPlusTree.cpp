#include "BPlusTree.h"

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

