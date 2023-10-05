#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <locale>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <sstream>

using namespace std;

struct IndiceId
{
	int id;
    int pos;
};

struct Pessoa
{
    int id;
    char nome[40];
	bool excluido;
};

void lExaustPessoasById(Pessoa *pessoas, IndiceId *indice, int quant)
{
    cout   << "\n |----------------------------|"
    		   	<< "\n |   Pessoas - ID crescente   |"
    		   	<< "\n |----------------------------|"
                << "\n |  ID   |   Nome   | Status  |"
                << "\n |----------------------------|\n";
     for(int i = 0; i < quant; i++) 
     {
        Pessoa p = pessoas[indice[i].pos];
        
        string status;
        if (!p.excluido) status = "Ativo  ";
        if (p.excluido) status = "Inativo";

        string nome = p.nome;
        if(nome.size() > 8) 
        {
            nome.resize(7, ' ');
            nome.append(".");
        }
        else nome.resize(8, ' ');
        cout   << " | "<< p.id <<"\t | "<< nome <<" | "<< status <<" |\n";
     }
     cout  << " |----------------------------|\n"
                << " | " << quant << " Registros                |\n"
                << " |----------------------------|\n";
     
}
void criarIndicePessoas(Pessoa *pessoas, IndiceId *indice, int quant) 
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
            IndiceId aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void organizarArquivoPessoas(Pessoa *pessoas, IndiceId *novoIndice, int &quant) // Apaga registros logicamente excluí­dos e reorganiza Índice
{
    // inclui apenas os que não estão logicamente excluí­dos
    int qAux; // variavel auxiliar para quant

    for (int i = 0, j = i; i < quant && j < quant; i++) 
    { 

        while(pessoas[j].excluido && j < quant) j++;
        if (j >= quant) break;
        pessoas[i] = pessoas[j];
        
        if(!pessoas[i].excluido) qAux = i+1;
        j++;
    }

    quant = qAux;
    criarIndicePessoas(pessoas, novoIndice, quant);
   
}
int inserirPessoa(Pessoa *pessoas, int &quant, Pessoa add, IndiceId *indice)
{
	// retorna 0 se der certo, 1 se o id informado já estiver registrado e ativo 
    const int q = quant;
    int cursor = 0;
    for (;add.id > indice[cursor].id && cursor < q; cursor++);

    if((add.id == indice[cursor].id) && (pessoas[indice[cursor].pos].excluido == false))
    {
        cout << "Operação inválida: Já existe um registro com este código";
        return 1;
    }

    pessoas[q] = add; // coloca o registro no fim da lista
    IndiceId ind = IndiceId{add.id, q};

    int reg = q;// reg = regressivo, contador que diminui
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++; //atualiza a quantidade de registros no fim
	return 0;
}
int bscPessoaById(int id, Pessoa *pessoas, IndiceId *indice, int quant)
//retorna 0 se achar, 1 se não, 2 se achar mas estiver excluído, 3 se o desenvolvedor for burro
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
            supondo que haja diversos usuarios com o mesmo id (Um ou mais excluídos, e um ativo)
            no índice, eles estarão registrados um após o outro, e o ativo será o último,
            devido à maneira como a inserção funciona.
            Verifica todos os usuários com este id. Se o mais recente for válido, printa e retorna,
            senão, fala q não encontrou e retorna
            */
            
            while (indice[cursor].id == id) 
            {
                if (!pessoas[indice[cursor].pos].excluido)
                {
                    Pessoa p = pessoas[indice[cursor].pos];
                    cout << "\nPessoa Encontrada\n"
                            << p.id << ", " << p.nome << endl;
                            return 0;
                }
                cursor++;
            }
            
            cout << "\nPessoa Não Encontrada. (Apenas registros Excluídos)\n\n";
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;

	}
	cout << "\nPessoa Não Encontrada.\n\n";
    return 1;

}
int excPessoaById(int id, Pessoa *pessoas, IndiceId *indice, int &quant)
{
	//retorna 0 se achar e excluir, 1 se não achar, 2 se achar mas estiver excluído, -1 se a exclusão for cancelada, 3 se o desenvolvedor for burro

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

                    cout << "\nPessoa Encontrada\n"
                            << p.id << ", " << p.nome << endl;
                    cout << "\nVocê Confirma a Exclusão deste Registro? \n (Insira [S] para confirmar)";

                    char conf = 0;
                    cin >> conf;
                    if (toupper(conf) == 'S') 
                    {
                        pessoas[pos].excluido = true;
                        cout << "\nRegistro Excluído com sucesso.\n";
                        quant--;
                        return 0;
                    }
                    else {
						cout << "\nExclusão Cancelada.\n";
                    	return -1;
					}
                }
                cursor++;
            }
            cout << "\nPessoa Não Encontrada.\n"; 
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
        
		else {
            cout << "\n excBin Tem bug nesse código: \n";
            cout << endl << indice[cursor].id << " < " << id << "\ni : " << i << "; cursor: " << cursor << "; f: " << f;
            return 3;
        }
		
		
	}
	cout << "\nPessoa Não Encontrada.\n";
    return 1;
}

int main()
{
	const int MAX = 50;
	Pessoa pessoas[MAX];
	IndiceId indPessoas[MAX];
	int quantPessoas; // quantia total de registros de Pessoas
	bool testPessoa = false;
return 0;
}