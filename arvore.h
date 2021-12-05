#ifndef _arvore_h_
#define _arvore_h_
#include <fstream>
#include "no.h"
#include <vector>
#include <iterator>
#include <set>

using std::string;
using std::vector;

using namespace std;

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
    // definicao dos atributos feita!
    // partindo para a criação das amostras dos nós
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
    // Amostras incluidas na raiz.
    // Possiveis valores de cada atributo já estao definidos
    // Treinando a arvore de classificação
    init(raiz);
}

arvore::~arvore()
{
    // TODO Deletar os filhos e a raiz
}

double arvore::avaliadorDePrecisao()
{
    std::fstream f_leitura;
    f_leitura.open("classificar");
    std::string str_read;
    amostra samples[3];
    amostra sample;
    int i = 0;
    while (!f_leitura.eof())
    {
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

    no *noBusca;
    string resultadosArvore[3];
    int z = 0;
    // Para cada amostra lida
    for (int i = 0; i < 3; i++)
    {
        resultadosArvore[i] = this->raiz->buscaFilho(samples[i]);
    }


    double precisaoDaClassificacao = 0;
    for (int i = 0; i < 3; i++)
    { // compare a classe encontrada pela arvore com a classe já previamente lida
        if (resultadosArvore[i] == samples[i].classe)
            precisaoDaClassificacao++;
    }
    precisaoDaClassificacao = precisaoDaClassificacao / 3;

    return precisaoDaClassificacao;
}

string arvore::situacaoEspecifica(){

    amostra sample;
    cout << "Digite o tempo:" << endl;
    cin >> sample.atributos[0];
    cout << "Digite a temperatura:" << endl;
    cin >> sample.atributos[1];
    cout << "Digite a umidade:" << endl;
    cin >> sample.atributos[2];
    cout << "Digite a força do vento:" << endl;
    cin >> sample.atributos[3];
    sample.classe = "0";
    cout << "A resposta da arvore de classificacao é " << raiz->buscaFilho(sample);
    return "Fim da execução";
}

void arvore::init(no *n)
{
    if (n->getClasse() == "SIM" || n->getClasse() == "NÃO")
    {
    }
    else
    {
        std::cout << "MEu tipo: " << n->getAtributoDivisao() << std::endl;
        int atributo_selecionado = n->ganhoAtributos();
        n->atributos[atributo_selecionado].candidato = false;
        std::cout << "Atributo: " << n->atributos[atributo_selecionado].tipo << "  Classe: " << n->getClasse() << std::endl;
        // Verifica se no já está com classe
        // Se não estiver, faz proximos testes

        if (atributo_selecionado < 4)
        { // TODO Verificar esse if
            std::set<std::string>::iterator itr;
            std::vector<amostra> *amostras = n->getAmostras();
            for (itr = this->raiz->atributos[atributo_selecionado].divisoes.begin(); itr != this->raiz->atributos[atributo_selecionado].divisoes.end(); itr++)
            {
                no *filho = new no(*itr, n->atributos);
                n->setFilho(filho);
            }
            // Pra cada amostra de n
            for (int x = 0; x < amostras->size(); ++x)
            {
                amostra value = (*amostras)[x];
                for (int y = 0; y < n->filhos.size(); ++y)
                { 
                    no* filho = n->filhos[y];
                    // Compare a amostra.atributos[atributo_selecionada] de n com o atributo divisão de cada filho
                    if (value.atributos[atributo_selecionado] == n->filhos[y]->getAtributoDivisao())
                    {
                        n->filhos[y]->setAmostra(value);
                        if (value.classe == "SIM")
                            n->filhos[y]->aumenta_num_sim();
                        else
                            n->filhos[y]->aumenta_num_nao();
                        break;
                    }
                }
            }
            amostras->clear();
            for (int z = 0; z < n->filhos.size(); ++z)
            {
                if (n->filhos[z]->getAmostras()->size() != 0)
                {
                    if (n->filhos[z]->get_num_sim() == n->filhos[z]->getAmostras()->size())
                    {
                        n->filhos[z]->setClasse();
                        std::cout << "Sou :" << n->filhos[z]->getAtributoDivisao() << " e minha classe eh: " << n->filhos[z]->getClasse() << std::endl;
                    }
                    else
                    {
                        if (n->filhos[z]->get_num_nao() == n->filhos[z]->getAmostras()->size())
                        {
                            n->filhos[z]->setClasse();
                            std::cout << "Sou :" << n->filhos[z]->getAtributoDivisao() << " e minha classe eh: " << n->filhos[z]->getClasse() << std::endl;
                        }
                        else
                            init(n->filhos[z]); // Chamada recursiva para cada filho de n
                    }
                }
                else{
                    n->getFilhos().erase(n->getFilhos().begin()+z);//TODO verificar !!!!
                }
            }
        }
        else
        {
            // Acabaram os atributos!
            // procurar qual classe é mais frequente e dar esse rotulo ao nó
            n->setClasse();
        }
    }
}

#endif