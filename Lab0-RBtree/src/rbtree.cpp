#include <algorithm> 
#include "rbtree.h"

Nodo::Nodo(int dato){
    this->dato = dato;
    color = RED;
    
    left   = nullptr;
    right  = nullptr;
    father = nullptr;
}




int RBtree::getColor(Nodo* &puntero){
    if (puntero == nullptr) return BLACK;
    return puntero->color;
}

void RBtree::setColor(Nodo* &puntero, int color){
    if (puntero == nullptr) return;
    puntero->color = color;
}

/*          
 *        X                            Y
 *       / \     Rotar izquierda      / \
 *      A   Y    --------------->    X   C
 *         / \                      / \
 *        B   C                    A   B
 * 
 * La función se aplica a X
 */
void RBtree::rotarIzquierda(Nodo* &puntero){
    // Nodo Y
    Nodo *hijoDerecho = puntero->right;
    
    
    puntero->right = hijoDerecho->left;         // Asignar como hijo derecho de X a B
    if (puntero->right != nullptr)
        puntero->right->father = puntero;       // Asignar como nuevo padre de B a X


    hijoDerecho->father = puntero->father;      // Asignar que el nuevo padre de Y sea el padre de X
                                                // Esto es para conectar con el resto del arbol
    // Asignar la Y como hijo derecho o izquierdo 
    // En el paso anterior contectamos a Y con el resto del arbol, ahora Y será asignado como hijo del nodo superior
    if (puntero->father == nullptr)
        root = hijoDerecho;                     // Si no hay padre, significa que llegamos a la raiz
    else if (puntero == puntero->father->left)  
        puntero->father->left = hijoDerecho;    // Antes de la rotacion, X era el hijo izquiero
    else
        puntero->father->right = hijoDerecho;   // Antes de la rotacion, X era el hijo derecho

    hijoDerecho->left = puntero;                // Asignar a X como hijo izquierdo de Y
    puntero->father = hijoDerecho;              // Asignar a Y como padre de X
}

/*         
 *        Y                        X
 *       / \     Rotar derecha    / \
 *      X   C    ------------->  A   Y
 *     / \                          / \
 *    A   B                        B   C
 * 
 * La función se aplica a Y
 */
void RBtree::rotarDerecha(Nodo* &puntero){
    // Nodo X
    Nodo *hijoIzquierdo = puntero->left;        


    puntero->left = hijoIzquierdo->right;       // Asignar como hijo izquierdo de Y a B
    if (puntero->left != nullptr)
        puntero->left->father = puntero;        // Asignar como nuevo padre de B a Y


    hijoIzquierdo->father = puntero->father;    // Asignar que el nuevo padre de X sea el padre Y
                                                // Esto es para conectar con el resto del arbol
    // Asignar la X como hijo derecho o izquierdo 
    // En el paso anterior contectamos a X con el resto del arbol, ahora X será asignado como hijo del nodo superior
    if (puntero->father == nullptr)
        root = hijoIzquierdo;                   // Si no hay padre, significa que llegamos a la raiz
    else if (puntero == puntero->father->left)
        puntero->father->left = hijoIzquierdo;  // Antes de la rotacion, Y era el hijo izquiero
    else
        puntero->father->right = hijoIzquierdo;  // Antes de la rotacion, Y era el hijo derecho


    hijoIzquierdo->right = puntero;              // Asignar a Y como hijo derecho de X
    puntero->father = hijoIzquierdo;             // Asignar a X como padre de Y
}



Nodo* RBtree::insertarNodo(Nodo* &padre, Nodo* &puntero){
    // Arbol vacio
    if (padre == nullptr) return puntero;
    
    // Si el nuevo numero es menor al padre
    if(puntero->dato < padre->dato){
        padre->left = insertarNodo(padre->left,puntero);
        padre->left->father = padre;
    }
    // Si el nuevo numero es mayo al padre
    else if (puntero->dato > padre->dato){
        padre->right = insertarNodo(padre->right,puntero);
        padre->right->father = padre;
    }
    return padre;
}


void RBtree::corregirArbol(Nodo* &puntero){
    Nodo* padre  = nullptr;
    Nodo* abuelo = nullptr;
    while ( puntero != root &&  getColor(puntero)==RED && getColor(puntero->father)==RED ){
        padre  = puntero->father;
        abuelo = padre  ->father;

        // El padre esta a la izquierda
        if (padre == abuelo->left){
            Nodo* tio = abuelo->right;

            // CASO I: padre y tio son rojos
            if(  getColor(tio) ==RED ){
                setColor(padre ,BLACK);
                setColor(tio   ,BLACK);
                setColor(abuelo,RED  );
                puntero = padre;
            }
            else{
                // CASO II: padre y el hijo tienen distintas direcciones
                if(puntero == padre->right){
                    rotarIzquierda(padre);
                    puntero = padre;
                    padre = puntero->father;
                }

                // CASO III: padre y el hijo tienen la misma dirección
                rotarDerecha(abuelo);
                std::swap(padre->color,abuelo->color);
                puntero = padre;
            }
        }

        // El padre esta a la derecha
        else{
            Nodo* tio = abuelo->left;

            // CASO I: padre y tio son rojos
            if(  getColor(tio) ==RED ){
                setColor(padre ,BLACK);
                setColor(tio   ,BLACK);
                setColor(abuelo,RED  );
                puntero = padre;
            }
            else{
                // CASO II: padre y el hijo tienen distintas direcciones
                if(puntero == padre->left){
                    rotarDerecha(padre);
                    puntero = padre;
                    padre = puntero->father;
                }

                // CASO III: padre y el hijo tienen la misma dirección
                rotarIzquierda(abuelo);
                std::swap(padre->color,abuelo->color);
                puntero = padre;
            }
        }
    }
    
}

void RBtree::insertar(int dato){
    Nodo *puntero = new Nodo(dato);
    root = insertarNodo(root,puntero);
    corregirArbol(puntero);
}

RBtree::RBtree(){
    root = nullptr;
}

RBtree::~RBtree(){
}
