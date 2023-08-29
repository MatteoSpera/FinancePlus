#include <stdio.h>
#include <iostream>
#include <string>
#include <locale>

struct Data {
    int dia;
    int mes;
    int ano;
};
//TODO - criar métodos para data

struct Pessoa
{
    int id;
    char nome[40];
	bool excluido;
};
struct IndPessoaId
{
	int id;
    int pos;
};
void printIndicePessoa(IndPessoaId *indice, int quant)
{
    std::cout   << "\n |---------------|"
				<< "\n | indice. Pessoas  |"
				<< "\n |---------------|"
                << "\n | ID\t | POS.\t |"
                << "\n |---------------|\n";
    //if(printPes) std::cout << "\t";
    for(int i = 0; i < quant; i++) std::cout << " | "<< indice[i].id <<"\t | "<< indice[i].pos <<"\t |\n";
    std::cout   << " |---------------|\n"
                << " | " << quant << " Registros\t |"
                << "\n |---------------|\n";
}
void printPessoas(Pessoa *pessoas, int quant)
{
    std::cout   << "\n |----------------------------|"
                << "\n |  ID   |   Nome   | Status  |"
                << "\n |----------------------------|\n";
     for(int i = 0; i < quant; i++) 
     {
        Pessoa p = pessoas[i];
        
        std::string status;
        if (p.excluido == 0) status = "Ativo  ";
        if (p.excluido == 1) status = "Inativo";

        std::string nome = p.nome;
        if(nome.size() > 8) 
        {
            nome.resize(7, ' ');
            nome.append(".");
        }
        else nome.resize(8, ' ');
        std::cout   << " | "<< p.id <<"\t | "<< nome <<" | "<< status <<" |\n";
     }
     std::cout  << " |----------------------------|\n"
                << " | " << quant << " Registros                |\n"
                << " |----------------------------|\n";
     
}
void criarIndicePessoas(Pessoa *pessoas, IndPessoaId *indice, int quant) 
// cria indice, com todos os registros (incluindo os logicamente deletados, já que o trabalho de filtrar será da função de organização)
{
    for (int i = 0; i < quant; i++)
    {
        indice[i].id = pessoas[i].id;
        indice[i].pos = i;
    }


    for (int i = 0; i < quant; i++){
    for (int j = i+1; j < quant; j++)
    {
        if (indice[j].id < indice[i].id) 
        {
            IndPessoaId aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void organizarArquivoPessoas(Pessoa *pessoas, IndPessoaId *novoIndice, int &quant) // Apaga registros logicamente excluídos e reorganiza Índice
{
    // inclui apenas os que não estão logicamente excluídos
    int qAux; // variavel auxiliar para quant

    for (int i = 0, j = i; i < quant && j < quant; i++) 
    { 

        while(pessoas[j].excluido == 1 && j < quant) j++;
        if (j >= quant) break;
        pessoas[i] = pessoas[j];
        
        if(pessoas[i].excluido != 1) qAux = i+1;
        j++;
    }

    quant = qAux;
    criarIndicePessoas(pessoas, novoIndice, quant);
   
}
int inserirPessoa(Pessoa *pessoas, int &quant, Pessoa add, IndPessoaId *indice)
{
	// retorna 0 se der certo, 1 se o id informado já estiver registrado e ativo 
    const int q = quant;
    int cursor = 0;
    for (;add.id > indice[cursor].id && cursor < q; cursor++);

    if((add.id == indice[cursor].id) && (pessoas[indice[cursor].pos].excluido == false))
    {
        std::cout << "Operação inválida: Já existe um registro com este código";
        return 1;
    }

    pessoas[q] = add; // coloca o registro no fim da lista
    IndPessoaId ind = IndPessoaId{add.id, q};

    int reg = q;// reg = regressivo, contador que diminui
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++; //atualiza a quantidade de registros no fim
}


struct Categoria_Gasto
{
    // Categoria do Gasto realizado, como entretenimento, comida, locomoção, médico, etc
    int id;
    char descricao[40]; 
};

struct Banco
{
    int id;
    char descricao[40];
};

struct Transacao
{
    int id;
    int idCategoria;
    int idConta;
    Data data;
    float valor;
    char tipo; //Débito ou Crédito
};

int main()
{
	setlocale(LC_ALL, "");

	const int MAX = 60;

    Pessoa pessoas[MAX];
	IndPessoaId indPessoasId[MAX];
	int quantPessoas; // quantia total de registros de Pessoas

	pessoas[0] = Pessoa{5, "Marcolino", false};
    pessoas[1] = Pessoa{6, "Armando", true};
    pessoas[2] = Pessoa{2, "João", false};
    pessoas[3] = Pessoa{3, "Pedro", false};

	quantPessoas = 4;

	printPessoas(pessoas, quantPessoas);

	Pessoa pAdd{1, "Rodrigo", false};

	inserirPessoa(pessoas, quantPessoas, pAdd, indPessoasId);
	printPessoas(pessoas, quantPessoas);

	criarIndicePessoas(pessoas, indPessoasId, quantPessoas);
	printIndicePessoa(indPessoasId, quantPessoas);

	organizarArquivoPessoas(pessoas, indPessoasId, quantPessoas);
	printIndicePessoa(indPessoasId, quantPessoas);
}