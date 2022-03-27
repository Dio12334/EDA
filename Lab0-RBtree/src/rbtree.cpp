#include <algorithm> 
#include <type_traits>
#include "rbtree.h"
#include <iostream>
#define DERECHA true
#define IZQUIERDA false

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
                puntero = abuelo;
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
                puntero = abuelo;
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
     setColor(root, BLACK);
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

void RBtree::preorder(Nodo* nodo, std::vector<int>& pre){
    if(!nodo)
        return;

    pre.emplace_back(nodo->dato);

    preorder(nodo->left, pre);

    preorder(nodo->right, pre);

}
std::vector<int> RBtree::preorden(){
    std::vector<int> pre;
    preorder(root, pre);
    return pre;
}
void RBtree::inorder(Nodo* nodo, std::vector<int>& in){
     if(!nodo)
        return;

    inorder(nodo->left, in);

    in.emplace_back(nodo->dato);
    
    inorder(nodo->right, in);
   
}
std::vector<int> RBtree::inorden(){
    std::vector<int> in;
    inorder(root, in);
    return in;
}
void RBtree::postorder(Nodo* nodo, std::vector<int>& post){
    if(!nodo)
        return;

    postorder(nodo->left, post);

    postorder(nodo->right, post);

    post.emplace_back(nodo->dato);
    

}
std::vector<int> RBtree::postorden(){
    std::vector<int> post;
    postorder(root, post);
    return post;
}

/*Usa busqueda binaria para encontrar el nodo a eliminar, en caso de no encontrarlo
 * retorna un nullptr*/
Nodo* RBtree::buscar(int dato){
    Nodo* actual = root;
    if(!actual)
        return nullptr;
    while(actual->dato != dato and (actual->left or actual->right)){
        if(actual->dato < dato)
            actual = actual->right;
        else
            actual = actual->left;
    }
    if(actual->dato == dato)
        return actual;
    else
        return nullptr;
}

/*Aplica la elimincación de Binary Search tree y retorna el nodo que se tiene que eliminar
 * en caso no exista retorna nullptr*/
Nodo* RBtree::eliminacionBS(int dato){
    
    Nodo* eliminado = nullptr;
    // Busca si el nodo existe
    if((eliminado = buscar(dato))){
        // encuentra su sucesor y si el sucesor tiene hijos vuelve a buscar sucesor
        // hasta ser una hoja
        Nodo* reemplazo = encontrarSucesor(eliminado);
        eliminado->dato = reemplazo->dato;
        while(reemplazo->right){
            Nodo* reemplazoLoop = encontrarSucesor(reemplazo);
            reemplazo->dato = reemplazoLoop->dato;
            reemplazo = reemplazoLoop;
        }
        return reemplazo;
    }
    return nullptr;

}

Nodo* RBtree::encontrarSucesor(Nodo* aEliminar){
    Nodo* actual = aEliminar;
    //encuentra al menor del sub-arbol derecho
    if(actual->right){
        actual = actual->right;
        while(actual->left){
            actual = actual->left;
        }
    }
    return actual;
}

void RBtree::eliminarNodo(int dato){
    //Aplica eliminación de BinarySearch
    Nodo* aEliminar = eliminacionBS(dato);
    if(aEliminar){
        // si lo logró encontrar y "eliminar" se procede a eliminar por RED BLACK
        aEliminar->color = (aEliminar->color == BLACK)? DOUBLEBLACK:RED;
        eliminacionRB(aEliminar);
        // nos falta liberar la memoria que ocupamos en el heap
        if(aEliminar != root){
           if(aEliminar->father->left == aEliminar)
               aEliminar->father->left = nullptr;
           else if(aEliminar->father->right == aEliminar)
               aEliminar->father->right = nullptr;
        }
        else
            root = nullptr;
        delete aEliminar;
    }
}

void RBtree::addBlack(Nodo* nodo){
    nodo->color = (nodo->color == RED)? BLACK:DOUBLEBLACK;
}
void RBtree::eliminacionRB(Nodo* nodo){
    // CASO 1: si el nodo es rojo solo se elimina
    // -> retornamos que la eliminación de la hoja se procesa afuera
    if(nodo->color == RED){
        return ;
    }
    // CASO 2: si el nodo es root solo quitamos el DOUBLEBLACK
    // -> si el root es DOUBLEBLACK lo volvemos black
    if(nodo == root){
        setColor(nodo, BLACK);
        return;
    }
    if(nodo->color == DOUBLEBLACK){
        Nodo* hermano = (nodo->father->left == nodo)? nodo->father->right: nodo->father->left;
        bool lado = (nodo->father->left == nodo)? IZQUIERDA:DERECHA;
        //CASO 1.2: El nodo es negro pero tiene un unico hijo rojo
        //-> intercambiamos al nodo con su unico hijo rojo
        //-> el hijo se vuelve BLACK
        if(nodo->left){
            if(!nodo->right && nodo->left->color == RED){
                nodo->left->father = nodo->father;
                setColor(nodo->left, BLACK);
                if(lado == DERECHA){
                    nodo->father->right = nodo->left;
                }
                else
                    nodo->father->left = nodo->left;
                    
                return;
            }
        }

        if(hermano->color == BLACK){ 
            // CASO 3: si el nodo es DOUBLEBLACK y su hermano es BLACK al igual que sus hijos
            // cabe resaltar que nullptr se considera negro y para evitar segmentation faults tengo que hacer este grupo de ifs
            // ->le añadimos BLACK al padre
            // -> el hermano se vuelve RED
            // -> si el padre es DOUBLEBLACK volvemos a llamar a la función en el padre
            if(!hermano->left and !hermano->right){
                addBlack(nodo->father);
                setColor(hermano, RED);
                if(nodo->father->color == DOUBLEBLACK)
                    eliminacionRB(nodo->father);
                return;
            }
            else if(!hermano->left){
                if(hermano->right->color == BLACK) {
                    addBlack(nodo->father);
                    setColor(hermano, RED);
                    if(nodo->father->color == DOUBLEBLACK)
                        eliminacionRB(nodo->father); 
                }
            }
            else if(!hermano->right){
                if(hermano->left->color == BLACK){
                    addBlack(nodo->father);
                    setColor(hermano, RED);
                    if(nodo->father->color == DOUBLEBLACK)
                        eliminacionRB(nodo->father);
                }
            }
            else if(hermano->left->color == BLACK and hermano->right->color == BLACK){
                addBlack(nodo->father);
                setColor(hermano, RED);
                if(nodo->father->color == DOUBLEBLACK)
                    eliminacionRB(nodo->father);
            }
            //CASO 5: Sobrino lejano es negro y el cercano es rojo
            //->intercambiamos el color del hermano con el sobrino
            //->rotamos al hermano al sentido contrario del DOUBLEBLACK
            //->vamos al caso 6
            else if(lado == IZQUIERDA and hermano->left->color == RED and hermano->right->color == BLACK){
                std::swap(hermano->color, hermano->left->color);
                rotarDerecha(hermano);
                eliminacionRB(nodo);
            }
            else if(lado == DERECHA and hermano->left->color == BLACK and hermano->right->color == RED){
                std::swap(hermano->color, hermano->right->color);
                rotarIzquierda(hermano);
                eliminacionRB(nodo);
            }
            // CASO 6: Sobrino lejano es de color rojo
            // ->cambiamos de color entre el padre y el hermano
            // -> rotamos al padre en dirección del DOUBLEBLACK
            // -> al DOUBLEBLACK lo volvemos BLACK y al sobrino lejano lo volvemos BLACK
            else if(lado == DERECHA and hermano->left->color == RED){
                Nodo* rojoLejano = hermano->left;
                std::swap(hermano->color, nodo->father->color);
                rotarDerecha(nodo->father);
                setColor(nodo, BLACK);
                setColor(rojoLejano, BLACK);
            }
            else if(lado == IZQUIERDA and hermano->right->color == RED){
                Nodo* rojoLejano = hermano->right;
                std::swap(hermano->color, nodo->father->color);
                rotarDerecha(nodo->father);
                setColor(nodo, BLACK);
                setColor(rojoLejano, BLACK);
            }
        }
        //CASO 4: El hermano es de color rojo
        //-> cambiamos el color del hermano con el padre
        //-> rotamos al padre en dirección del DOUBLEBLACK
        //-> volvemos a llamar para procesar el siguiente caso
        else if(hermano->color == RED){
            std::swap(hermano->color, nodo->father->color);
            if(lado == IZQUIERDA)
                rotarIzquierda(nodo->father);
            else 
                rotarDerecha(nodo->father);
            eliminacionRB(nodo);
        }
    }
}
