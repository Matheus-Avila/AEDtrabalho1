#ifndef _no_h_
#define _no_h_
#include <vector>
#include <string>
#include <math.h>
#include <set>

using namespace std;

typedef struct atributo
{
    std::string tipo;
    std::set<std::string> divisoes;
    double entropia;
    bool candidato = true;
} atributo;

typedef struct amostra
{
    std::string atributos[4];
    std::string classe;
} amostra;

class no
{
private:
    std::vector<amostra> amostras;
    std::string classe;
    std::string atributoDivisao;
    int num_amostras_sim;
    int num_amostras_nao;

public:
    std::vector<no*> filhos;
    atributo *atributos;
    double entropiaNo();             //pega o numero de amostras de cada classe e define a entropia
    double ganhoAtributos(); // Para cada atributo a partir de atrib em atributos calcule seu ganho
    no(/* args */);
    no(std::string atributoEscolhido, atributo *atributos);
    ~no();
    std::string getClasse();
    void setClasse();
    void verificaClasse();
    std::vector<amostra> *getAmostras();
    void setAmostra(amostra sample);
    std::vector<no*> getFilhos();
    void setFilho(no* filho);
    int get_num_sim();
    int get_num_nao();
    std::string getAtributoDivisao();
    void aumenta_num_sim();
    void aumenta_num_nao();
    std::string buscaFilho(amostra sample);
};

no::no(){
    num_amostras_nao = 0;
    num_amostras_sim = 0;
    this->classe = "0";
}

no::no(std::string atributoEscolhido, atributo *atributos)
{   
    this->atributoDivisao = atributoEscolhido;
    num_amostras_nao = 0;
    num_amostras_sim = 0;
    this->classe = "0";
    this->atributos = atributos; //TODO confetir se precisa dessa linha
}

double no::entropiaNo()
{   double tam = this->amostras.size();
    double simTam = num_amostras_sim / tam;
    double naoTam = num_amostras_nao / tam;

    return -(simTam) * log2(simTam) -(naoTam) * log2(naoTam) ;
}

int encontraIndiceMaiorElemento(double* ganhosDosAtributos){
    int indice = 4;
    double maior = 0;
    for(int i =0; i < 4; i++){
        if(ganhosDosAtributos[i]> maior){
            maior = ganhosDosAtributos[i];
            indice = i;
        } 
    }
    return indice;
}

double no::ganhoAtributos()
{
    double entropiaTotal = entropiaNo();
    double ganhoAtribs[4][3][2] = {0};
    for (int x = 0; x < this->amostras.size(); ++x)
    {

        for (int y = 0; y < 4; ++y)
        { //todos os atributos

            if (atributos[y].candidato == false)
            {   
                
            }else{
                std::set<std::string>::iterator itr;
                int z = 0;
                for (itr = this->atributos[y].divisoes.begin(); itr != this->atributos[y].divisoes.end(); itr++)
                {
                    if (*itr == this->amostras[x].atributos[y])
                    {
                        if (this->amostras[x].classe == "SIM")
                        {
                            ganhoAtribs[y][z][0]++;
                        }
                        else
                        {
                            ganhoAtribs[y][z][1]++;
                        }
                    }
                    ++z;
                }
            }
        }
    }
    //TODO fazer calculo do entropia
    double ganhosDosAtributos [4] = {};
    for (int y = 0; y < 4; y++){

        if (atributos[y].candidato != false){
            double entropiaOpcoes [3];
            for(int z = 0; z <3; ++z){
                double numAmostrasOpcao = ganhoAtribs[y][z][0] + ganhoAtribs[y][z][1];
                double simTam = ganhoAtribs[y][z][0]/numAmostrasOpcao;
                double naoTam = ganhoAtribs[y][z][1]/numAmostrasOpcao;

                if(simTam != 0 && naoTam != 0 && numAmostrasOpcao != 0) entropiaOpcoes[z] = - simTam * log2(simTam)- naoTam * log2(naoTam);
                else entropiaOpcoes[z] = 0;
                
                entropiaOpcoes[z] = entropiaOpcoes[z]*numAmostrasOpcao/this->getAmostras()->size();
            }
            ganhosDosAtributos[y] = entropiaTotal - entropiaOpcoes[0] - entropiaOpcoes[1] - entropiaOpcoes[2];
        }else{
            ganhosDosAtributos[y] = 0;
        }
    }
    return encontraIndiceMaiorElemento(ganhosDosAtributos);
}

std::string no::buscaFilho(amostra sample){
    std::cout << "Buscanco no nobusca " << this->getAtributoDivisao() << std::endl;
    std::cout << "Classe no this " << this->getClasse() << "/ num sim: " << this->get_num_sim() << " " << this->getAmostras()->size()  << std::endl;
    cout << "Num filhos: " << this->getFilhos().size() << endl;
    if (this->get_num_sim() == this->getAmostras()->size() || this->get_num_sim() == this->getAmostras()->size()){
        this->setClasse();
        cout << "Sou folha! " << this->getAtributoDivisao() << " --- " << this->getClasse() << endl;
        return this->getClasse();}
    std::vector<no>::iterator itr;
    std::vector<no*> filhos = this->getFilhos();
    for (int y = 0; y < filhos.size(); ++y){   
        no* filho = filhos[y];
        for(int z = 0; z < 4; ++z){ 
        cout << "sample: " << sample.atributos[z] << "// filho:" << filho->getAtributoDivisao() << endl;
            if (sample.atributos[z] == filho->getAtributoDivisao())
            {   
                cout << "Encontrei um filho compativel! " << filho->getAtributoDivisao() << " --- " << filho->getClasse() << endl;
                return filho->buscaFilho(sample);
            }
        }
    }
    cout << "Não tenho filho correspondente! " << this->getAtributoDivisao() << " --- " << this->getClasse() << endl;
    this->setClasse();
    return this->getClasse();
}

no::~no()
{
}
std::string no::getAtributoDivisao()
{
    return this->atributoDivisao;
}

void no::setAmostra(amostra sample)
{
    amostras.push_back(sample);
}

std::vector<amostra> *no::getAmostras()
{
    return &this->amostras;
}

void no::setFilho(no* filho)
{
    this->filhos.push_back(filho);
}

std::vector<no*> no::getFilhos()
{
    return this->filhos;
}

int no::get_num_nao()
{
    return num_amostras_nao;
}

int no::get_num_sim()
{
    return num_amostras_sim;
}

void no::aumenta_num_nao()
{
    this->num_amostras_nao += 1;
}

void no::aumenta_num_sim()
{
    this->num_amostras_sim += 1;
}

void no::setClasse()
{
    if (this->amostras.size() != 0)
    {
        if (this->get_num_sim() >= this->get_num_nao())
            this->classe = "SIM";
        else
            this->classe = "NÃO";
    }
    else
    {
        this->classe = "0";
    }
}

std::string no::getClasse()
{
    return this->classe;
}

#endif