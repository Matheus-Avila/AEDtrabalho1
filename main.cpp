#include <stdio.h>
#include <iostream>
#include <string>
#include "arvore.h"

using namespace std;


int main(){
    arvore *arv = new arvore();
    double precisao = arv->avaliadorDePrecisao();
    cout << "Precisão: " << precisao << endl;
    return 0;
}