#include "rbtree.h"
#include <iostream>

void printVector(const std::vector<int>& vector){
    for(auto& el: vector){
        std::cout<<el<<" ";
    }
    std::cout<<"\n";
}

int main(){
    
    RBtree arbol;

    int inputs, x;
    std::vector<int> parsed;
    std::cout<<"cantidad de inputs: "<<std::endl;
    std::cin>>inputs;
    int y = inputs;
    std::cout<<"---- insertar nodos ---- \n";
    while(y--){
        std::cin>>x;
        arbol.insertar(x);
    
    }
    std::cout<<"preorden: ";
    printVector(arbol.preorden());
    std::cout<<"inorden: ";
    printVector(arbol.inorden());
    std::cout<<"postorden: ";
    printVector(arbol.postorden());
    std::cout<<"cantidad de nodos a eliminar: "<<std::endl;
    std::cin>>inputs;
    std::cout<<"---- insertar nodos a eliminar ---- \n";
    while(inputs--){
        std::cin>>x;
        arbol.eliminarNodo(x);
    }
    std::cout<<"preorden: ";
    printVector(arbol.preorden());
    std::cout<<"inorden: ";
    printVector(arbol.inorden());
    std::cout<<"postorden: ";
    printVector(arbol.postorden());
 
    return 0;
}
