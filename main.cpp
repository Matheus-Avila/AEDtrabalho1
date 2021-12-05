#include <stdio.h>
#include <iostream>
#include <string>
#include "arvore.h"

using namespace std;


int main(){
    arvore *arv = new arvore();
    cout << "Avaliando a precisao..." << endl;
    double precisao = arv->avaliadorDePrecisao();
    cout << "Precisão: " << precisao*100 << "%" << endl;
    arv->situacaoEspecifica();
    return 0;
}