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
    double avaliadorDePrecisao();
    string situacaoEspecifica();
};

arvore::arvore()
{
    raiz = new no();
    atributo aux;
    this->raiz->atributos = new atributo[4];
    std::fstream f_leitura;
    f_leitura.open("entrada");
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
    //Treinando a arvore de classificação
    init(raiz);
}

arvore::~arvore()
{
    //TODO Deletar os filhos e a raiz
}

double arvore::avaliadorDePrecisao(){
    std::fstream f_leitura;
    f_leitura.open("classificar");
    std::string str_read;
    amostra samples [3];
    amostra sample;
    int i = 0;
    while (!f_leitura.eof()){
        getline(f_leitura, str_read, ',');
        sample.atributos[0] = str_read;

        getline(f_leitura, str_read, ',');
        sample.atributos[1] = str_read;

        getline(f_leitura, str_read, ',');
        sample.atributos[2] = str_read;

        getline(f_leitura, str_read, ',');
        sample.atributos[3] = str_read;

        getline(f_leitura, str_read, '\n');
        sample.classe = str_read;
        samples[i] = sample;
        i++;
    }

    f_leitura.close();
    
    no noBusca;
    string resultadosArvore [3];
    //Para cada amostra lida    
    for(int i = 0; i < 3; i++){
        noBusca  = *this->raiz;
        std::vector<no> *filhos = noBusca.getFilhos();
        for (int z = 0; z < filhos->size(); ++z){
            no filho = (*filhos)[z];
            std::cout << "Classe: " << noBusca.getClasse() << std::endl;
            if(noBusca.getClasse() == "NÃO"){
                resultadosArvore[i] = "NÃO";
                std::cout << i << " Nao" << std::endl;
                break;
            }
            if(noBusca.getClasse() == "SIM"){
                resultadosArvore[i] = "SIM";
                std::cout << i << " Sim" << std::endl;
                break;
            }
            for(int j = 0; j < 4; j++){
                //Para cada filho de n busque qual tem o atributoDivisao igual ao da amostra
                std::cout << filho.getAtributoDivisao() << "///" << samples[i].atributos[j] << std::endl;
                if(filho.getAtributoDivisao() == samples[i].atributos[j]){
                    std::cout << filho.getAtributoDivisao() << std::endl;
                    noBusca = (*filhos)[z];
                    break;
                }
            }
            
        }
        //Se nenhum filho atender ao critério de busca pegue o mais frequente no nó atual e retorne a classe
        std::cout << "Alcancei!!!!" << std::endl;
        if(noBusca.get_num_sim() >= noBusca.get_num_nao()) resultadosArvore[i] = "SIM";
            else resultadosArvore[i] = "NÃO";
    }

    double precisaoDaClassificacao = 0;

    for(int i = 0; i < 3; i++){//compare a classe encontrada pela arvore com a classe já previamente lida
        std::cout << resultadosArvore[i] << ", " << samples[i].classe << std::endl;
        if(resultadosArvore[i] == samples[i].classe) precisaoDaClassificacao++;
    }   
    precisaoDaClassificacao = precisaoDaClassificacao/3;

    return precisaoDaClassificacao;
}

// string arvore::situacaoEspecifica(){

// }

void arvore::init(no *n){
    if (n->getClasse() == "SIM" || n->getClasse() == "NÃO"){
        
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
                if (filho.getAmostras()->size() != 0){
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