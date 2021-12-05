#ifndef _arvore_h_
#define _arvore_h_
#include <fstream>
#include "no.h"
#include <vector>
#include <iterator>
#include <set>

using std::string;
using std::vector;

class arvore
{
private:
    no *raiz;

public:
    arvore();
    ~arvore();
    void init(no *n);
    void selecionaAtributo();
};

arvore::arvore()
{
    raiz = new no();
    atributo aux;
    this->raiz->atributos = new atributo[4];
    std::fstream f_leitura;
    f_leitura.open("entrada");
    int tam_f_leitura;
    std::string str_read;
    getline(f_leitura, str_read, ',');
    aux.tipo = str_read;
    this->raiz->atributos[0] = aux;
    getline(f_leitura, str_read, ',');
    aux.tipo = str_read;
    this->raiz->atributos[1] = aux;
    getline(f_leitura, str_read, ',');
    aux.tipo = str_read;
    this->raiz->atributos[2] = aux;
    getline(f_leitura, str_read, ',');
    aux.tipo = str_read;
    this->raiz->atributos[3] = aux;
    getline(f_leitura, str_read, '\n');
    //definicao dos atributos feita!
    //partindo para a criação das amostras dos nós
    amostra sample;
    while (!f_leitura.eof())
    {
        getline(f_leitura, str_read, ',');
        sample.atributos[0] = str_read;
        this->raiz->atributos[0].divisoes.insert(str_read);

        getline(f_leitura, str_read, ',');
        sample.atributos[1] = str_read;
        this->raiz->atributos[1].divisoes.insert(str_read);

        getline(f_leitura, str_read, ',');
        sample.atributos[2] = str_read;
        this->raiz->atributos[2].divisoes.insert(str_read);

        getline(f_leitura, str_read, ',');
        sample.atributos[3] = str_read;
        this->raiz->atributos[3].divisoes.insert(str_read);

        getline(f_leitura, str_read, '\n');
        sample.classe = str_read;
        if (sample.classe == "SIM")
        {
            this->raiz->aumenta_num_sim();
        }
        else
        {
            this->raiz->aumenta_num_nao();
        }
        this->raiz->setAmostra(sample);
    }
    f_leitura.close();
    //Amostras incluidas na raiz.
    //Possiveis valores de cada atributo já estao definidos
    //TODO Ordenar atributos de acordo com atributo entropia(ou ganho). Nao sei fazer isso
    //Treinando a arvore de classificação
    init(raiz);
}

arvore::~arvore()
{
    //Deletar os filhos e a raiz
}

void arvore::init(no *n){
    if (n->getClasse() == "SIM" || n->getClasse() == "NÃO"){
        std::cout << "Meu tipo: " << n->getAtributoDivisao()  << " Minha classe: " << n->getClasse() << std::endl; 
    }
    else{
    std::cout << "MEu tipo: " << n->getAtributoDivisao() << std::endl;
    int atributo_selecionado = n->ganhoAtributos();
    n->atributos[atributo_selecionado].candidato = false;
    std::cout << "Atributo: " << n->atributos[atributo_selecionado].tipo << "  Classe: " << n->getClasse() << std::endl;
    //Verifica se no já está com classe
    //Se não estiver, faz proximos testes
    
        if (atributo_selecionado < 4){ //TODO Verificar esse if
            std::set<std::string>::iterator itr;
            std::vector<amostra> *amostras = n->getAmostras();
            std::vector<no> *filhos = n->getFilhos();
            for (itr = this->raiz->atributos[atributo_selecionado].divisoes.begin(); itr != this->raiz->atributos[atributo_selecionado].divisoes.end(); itr++)
            {
                no *filho = new no(*itr, n->atributos);
                n->setFilho(*filho);
            }
            // Pra cada amostra de n
            for (int x = 0; x < amostras->size(); ++x)
            {
                amostra value = (*amostras)[x];
                for (int y = 0; y < filhos->size(); ++y)
                { // Compare a amostra.atributos[atributo_selecionada] de n com o atributo divisão de cada filho
                    if (value.atributos[atributo_selecionado] == (*filhos)[y].getAtributoDivisao())
                    {
                        (*filhos)[y].setAmostra(value);
                        if (value.classe == "SIM")
                            (*filhos)[y].aumenta_num_sim();
                        else
                            (*filhos)[y].aumenta_num_nao();
                        break;
                    }
                }
            }
            amostras->clear();
            for (int z = 0; z < filhos->size(); ++z){
                no filho = (*filhos)[z];
                if (filho.getAmostras()->size() != 0){ //TODO verificar se esse if faz sentido!!!!!!!!
                    if (filho.get_num_sim() == filho.getAmostras()->size()){
                        filho.setClasse();
                        std::cout << "Sou :" << filho.getAtributoDivisao() << " e minha classe eh: " << filho.getClasse() << std::endl;
                    }else{
                        if (filho.get_num_nao() == filho.getAmostras()->size()){ 
                            filho.setClasse();
                            std::cout << "Sou :" << filho.getAtributoDivisao() << " e minha classe eh: " << filho.getClasse() << std::endl;
                        }else init(&filho); // Chamada recursiva para cada filho de n
                    }
                }
                // else{
                //     n->getFilhos()->erase(n->getFilhos()->begin()+z);//TODO verificar !!!!
                // }
            }
        }
        else
        {
            //Acabaram os atributos!
            //procurar qual classe é mais frequente e dar esse rotulo ao nó
            n->setClasse();
        }
    }
}

#endif