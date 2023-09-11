#include <stdio.h>
#include <iostream>
#include <string>
#include <locale>

struct Data {
    int dia;
    int mes;
    int ano;
};
//TODO - criar m�todos para data

struct Pessoa
{
    int id;
    char nome[40];
	bool excluido;
};
struct IndPessoaById
{
	int id;
    int pos;
};
void printIndicePessoa(IndPessoaById *indice, int quant)
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
void lExaustPessoasById(Pessoa *pessoas, IndPessoaById *indice, int quant)
{
    std::cout   << "\n |----------------------------|"
    		   	<< "\n |   Pessoas - ID crescente   |"
    		   	<< "\n |----------------------------|"
                << "\n |  ID   |   Nome   | Status  |"
                << "\n |----------------------------|\n";
     for(int i = 0; i < quant; i++) 
     {
        Pessoa p = pessoas[indice[i].pos];
        
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
void criarIndicePessoas(Pessoa *pessoas, IndPessoaById *indice, int quant) 
// cria indice, com todos os registros (incluindo os logicamente deletados, j� que o trabalho de filtrar ser� da fun��o de organiza��o)
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
            IndPessoaById aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void organizarArquivoPessoas(Pessoa *pessoas, IndPessoaById *novoIndice, int &quant) // Apaga registros logicamente exclu�dos e reorganiza �ndice
{
    // inclui apenas os que n�o est�o logicamente exclu�dos
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
int inserirPessoa(Pessoa *pessoas, int &quant, Pessoa add, IndPessoaById *indice)
{
	// retorna 0 se der certo, 1 se o id informado j� estiver registrado e ativo 
    const int q = quant;
    int cursor = 0;
    for (;add.id > indice[cursor].id && cursor < q; cursor++);

    if((add.id == indice[cursor].id) && (pessoas[indice[cursor].pos].excluido == false))
    {
        std::cout << "Opera��o inv�lida: J� existe um registro com este c�digo";
        return 1;
    }

    pessoas[q] = add; // coloca o registro no fim da lista
    IndPessoaById ind = IndPessoaById{add.id, q};

    int reg = q;// reg = regressivo, contador que diminui
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++; //atualiza a quantidade de registros no fim
}
int bscPessoaById(int id, Pessoa *pessoas, IndPessoaById *indice, int quant)
//retorna 0 se achar, 1 se n�o, 2 se achar mas estiver exclu�do, 3 se o desenvolvedor for burro
{
	int i = 0, f = quant;
	int cursor = (i + f) / 2;

	for 
        (;
        f >= i; 
        cursor= (i + f) / 2)
    {
		if (indice[cursor].id == id)
        { 
            /*
            supondo que haja diversos usuarios com o mesmo id (Um ou mais exclu�dos, e um ativo)
            no �ndice, eles estar�o registrados um ap�s o outro, e o ativo ser� o �ltimo,
            devido � maneira como a inser��o funciona.
            Verifica todos os usu�rios com este id. Se o mais recente for v�lido, printa e retorna,
            sen�o, fala q n�o encontrou e retorna
            */
            
            while (indice[cursor].id == id) 
            {
                if (!pessoas[indice[cursor].pos].excluido)
                {
                    Pessoa p = pessoas[indice[cursor].pos];
                    std::cout << "\nPessoa Encontrada\n"
                            << p.id << ", " << p.nome << std::endl;
                            return 0;
                }
                cursor++;
            }
            
            std::cout << "\nPessoa N�o Encontrada. (Apenas registros Exclu�dos)";
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
		/*
        else {
            std::cout << "\n bscBinTem bug nesse c�digo: \n";
            std::cout << std::endl << indice[cursor].id << " < " << id << "\ni : " << i << "; cursor: " << cursor << "; f: " << f;
            return 3;
        }
		*/

	}
	std::cout << "\nPessoa N�o Encontrada.\n";
    return 1;

}
int excPessoaById(int id, Pessoa *pessoas, IndPessoaById *indice, int &quant)
{
	//retorna 0 se achar e excluir, 1 se n�o achar, 2 se achar mas estiver exclu�do, -1 se a exclus�o for cancelada, 3 se o desenvolvedor for burro

	int i = 0, f = quant;
    int cursor = (i + f) / 2; // calcula a media para ficar no meio, mas para inicializar poderia ser quant/2 tbm

	for 
	(;
	f >= i; 
	cursor= (i + f) / 2)
	{
		if (indice[cursor].id == id)
        {
            while (indice[cursor].id == id) 
            {
                if (!pessoas[indice[cursor].pos].excluido)
                {
                    int pos = indice[cursor].pos;
                    Pessoa p = pessoas[pos];

                    std::cout << "\nPessoa Encontrada\n"
                            << p.id << ", " << p.nome << std::endl;
                    std::cout << "\nVoc� Confirma a Exclus�o deste Registro? \n (Insira [S] para confirmar)";

                    char conf = 0;
                    char *entrada;
                    std::cin >> entrada;
                    conf = entrada[0];
                    if (toupper(conf) == 'S') 
                    {
                        pessoas[pos].excluido = true;
                        std::cout << "\nRegistro Exclu�do com sucesso.\n";
                        quant--;
                        return 0;
                    }
                    else {
						std::cout << "\nExclus�o Cancelada.\n";
                    	return -1;
					}
                }
                cursor++;
            }
            std::cout << "\nPessoa N�o Encontrada.\n"; 
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
        
		else {
            std::cout << "\n excBin Tem bug nesse c�digo: \n";
            std::cout << std::endl << indice[cursor].id << " < " << id << "\ni : " << i << "; cursor: " << cursor << "; f: " << f;
            return 3;
        }
		
		
	}
	std::cout << "\nPessoa N�o Encontrada.\n";
    return 1;
}

struct Categoria_Gasto
{
    // Categoria do Gasto realizado, como entretenimento, comida, locomo��o, m�dico, etc
    int id;
    char descricao[40]; 
};
struct IndCategoriaId
{
	int id;
	int pos;
};
void criarIndiceCategorias(Categoria_Gasto *categorias, IndCategoriaId *indice, int quant) 
{
    for (int i = 0; i < quant; i++)
    {
        indice[i].id = categorias[i].id;
        indice[i].pos = i;
    }


    for (int i = 0; i < quant; i++){
    for (int j = i+1; j < quant; j++)
    {
        if (indice[j].id < indice[i].id) 
        {
            IndCategoriaId aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}

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
    char tipo; //D�bito ou Cr�dito
};

int main()
{
	setlocale(LC_ALL, "");

	const int MAX = 60;

    Pessoa pessoas[MAX];
	IndPessoaById indPessoasId[MAX];
	int quantPessoas; // quantia total de registros de Pessoas

	pessoas[0] = Pessoa{8, "Marcolino", false};
    pessoas[1] = Pessoa{6, "Armando", true};
    pessoas[2] = Pessoa{2, "Jo�o", false};
    pessoas[3] = Pessoa{3, "Pedro", false};

	quantPessoas = 4;


	Pessoa pAdd1{1, "Rodrigo", false};
	Pessoa pAdd2{7, "Cesar", true};
	Pessoa pAdd3{10, "Tulio", false};


	inserirPessoa(pessoas, quantPessoas, pAdd1, indPessoasId);
	inserirPessoa(pessoas, quantPessoas, pAdd2, indPessoasId);
	inserirPessoa(pessoas, quantPessoas, pAdd3, indPessoasId);

	criarIndicePessoas(pessoas, indPessoasId, quantPessoas);
	printIndicePessoa(indPessoasId, quantPessoas);
	
	lExaustPessoasById(pessoas, indPessoasId, quantPessoas);


	for (int i = 1; i < quantPessoas; i++) {
		bscPessoaById(i, pessoas, indPessoasId, quantPessoas);

		};

	organizarArquivoPessoas(pessoas, indPessoasId, quantPessoas);
	printIndicePessoa(indPessoasId, quantPessoas);


	std::cout   << "\n\n--------------------------------------------------------\n\nTestando Exclus�o\n"
                << "\nTentando Excluir id 8 (existe)\n";
    excPessoaById(8, pessoas, indPessoasId, quantPessoas);
    std::cout   << "\nTentando Excluir id 8 de novo\n";
    excPessoaById(8, pessoas, indPessoasId, quantPessoas);

    /*
    std::cout   << "Tentando Excluir id 6 (existe mas j� foi exclu�do)\n";
    excPessoaById(6, pessoas, indPessoasId, quantPessoas);
    
    std::cout   << "Tentando Excluir id 8 (n�o existe)\n";
    excPessoaById(8, pessoas, indPessoasId, quantPessoas);
    
    std::cout << "\nTentando Excluir id 2 (existe)\n";
    excPessoaById(2, pessoas, indPessoasId, quantPessoas);
	*/
	
}