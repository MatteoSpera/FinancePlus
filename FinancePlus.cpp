#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <locale>

#include <sstream>

struct Data {
    int dia;
    int mes;
    int ano;
};
void printData(Data data)
{
    std::cout   << data.dia / 10 << data.dia %10 << "/" // caso o dia ou mes sejam menores que 10, automaticamente concatena 0 logo antes (ex 01, 02, 03...)
                << data.mes / 10 << data.mes %10 << "/"
                << data.ano;

}

std::string dataToString(Data data)
{
	std::string txt = "";
	txt += std::to_string(data.dia / 10) + std::to_string(data.dia % 10) + "/";
	txt += std::to_string(data.mes / 10) + std::to_string(data.mes % 10) + "/";
	txt += std::to_string(data.ano);

	return txt;
}
int comparaDatas(Data dA, Data dB) //se a > b, retorna 1, se a < b, retorna -1, se a == b, retorna 0
{
    if (dA.ano > dB.ano) return 1;
	else if (dA.ano < dB.ano) return -1;
	else 
	{
		if (dA.mes > dB.mes) return 1;
		else if (dA.mes < dB.mes) return -1;
		else
		{
			if (dA.dia > dB.dia) return 1;
			else if (dA.dia < dB.dia) return -1;
			else return 0;
		}
	}
    return NULL;
}
//TODO - criar métodos para data

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
        if (!p.excluido) status = "Ativo  ";
        if (p.excluido) status = "Inativo";

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
            IndPessoaById aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void organizarArquivoPessoas(Pessoa *pessoas, IndPessoaById *novoIndice, int &quant) // Apaga registros logicamente excluí­dos e reorganiza Índice
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
int inserirPessoa(Pessoa *pessoas, int &quant, Pessoa add, IndPessoaById *indice)
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
    IndPessoaById ind = IndPessoaById{add.id, q};

    int reg = q;// reg = regressivo, contador que diminui
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++; //atualiza a quantidade de registros no fim
	return 0;
}
int bscPessoaById(int id, Pessoa *pessoas, IndPessoaById *indice, int quant)
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
                    std::cout << "\nPessoa Encontrada\n"
                            << p.id << ", " << p.nome << std::endl;
                            return 0;
                }
                cursor++;
            }
            
            std::cout << "\nPessoa Não Encontrada. (Apenas registros Excluídos)";
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
		/*
        else {
            std::cout << "\n bscBinTem bug nesse código: \n";
            std::cout << std::endl << indice[cursor].id << " < " << id << "\ni : " << i << "; cursor: " << cursor << "; f: " << f;
            return 3;
        }
		*/

	}
	std::cout << "\nPessoa Não Encontrada.\n";
    return 1;

}
int excPessoaById(int id, Pessoa *pessoas, IndPessoaById *indice, int &quant)
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

                    std::cout << "\nPessoa Encontrada\n"
                            << p.id << ", " << p.nome << std::endl;
                    std::cout << "\nVocê Confirma a Exclusão deste Registro? \n (Insira [S] para confirmar)";

                    char conf = 0;
                    std::cin >> conf;
                    if (toupper(conf) == 'S') 
                    {
                        pessoas[pos].excluido = true;
                        std::cout << "\nRegistro Excluído com sucesso.\n";
                        quant--;
                        return 0;
                    }
                    else {
						std::cout << "\nExclusão Cancelada.\n";
                    	return -1;
					}
                }
                cursor++;
            }
            std::cout << "\nPessoa Não Encontrada.\n"; 
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
        
		else {
            std::cout << "\n excBin Tem bug nesse código: \n";
            std::cout << std::endl << indice[cursor].id << " < " << id << "\ni : " << i << "; cursor: " << cursor << "; f: " << f;
            return 3;
        }
		
		
	}
	std::cout << "\nPessoa Não Encontrada.\n";
    return 1;
}




struct Categoria_Gasto
{
    // Categoria do Gasto realizado, como entretenimento, comida, locomoção, médico, etc
    int id;
    char descricao[40]; 
	bool excluido;
};
struct IndCategoriaById
{
	int id;
	int pos;
};
void printCategoria(Categoria_Gasto categoria)
{
	std::cout << std::endl << categoria.id << ", " << categoria.descricao << std::endl; 
}
void lExaustCategoriasById(Categoria_Gasto *categorias, IndCategoriaById *indice, int quant)
{
    std::cout   << "\n |-----------------------------|"
    		   	<< "\n |  Categorias - ID crescente  |"
    		   	<< "\n |-----------------------------|"
                << "\n |  ID   | Descrição | Status  |"
                << "\n |-----------------------------|\n";
     for(int i = 0; i < quant; i++) 
     {
        Categoria_Gasto categoria = categorias[indice[i].pos];
        
        std::string status;
        if (!categoria.excluido) status = "Ativo  ";
        if (categoria.excluido) status = "Inativo";

        std::string descricao = categoria.descricao;
        if(descricao.size() > 8) 
        {
            descricao.resize(7, ' ');
            descricao.append(".");
        }
        else descricao.resize(8, ' ');
        std::cout   << " | "<< categoria.id <<"\t | "<< descricao <<"  | "<< status <<" |\n";
     }
     std::cout  << " |-----------------------------|\n"
                << " | " << quant << " Registros                 |\n"
                << " |-----------------------------|\n";
     
}
void criarIndiceCategorias(Categoria_Gasto *categorias, IndCategoriaById *indice, int quant) 
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
            IndCategoriaById aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void organizarArquivoCategorias(Categoria_Gasto *categorias, IndCategoriaById *novoIndice, int &quant)
{
    int qAux; 

    for (int i = 0, j = i; i < quant && j < quant; i++) 
    { 

        while(categorias[j].excluido && j < quant) j++;
        if (j >= quant) break;
        categorias[i] = categorias[j];
        
        if(!categorias[i].excluido) qAux = i+1;
        j++;
    }

    quant = qAux;
    criarIndiceCategorias(categorias, novoIndice, quant);
   
}
int inserirCategoria(Categoria_Gasto *categorias, int &quant, Categoria_Gasto add, IndCategoriaById *indice)
{
	// retorna 0 se der certo, 1 se o id informado já estiver registrado e ativo 
    const int q = quant;
    int cursor = 0;
    for (;add.id > indice[cursor].id && cursor < q; cursor++);

    if((add.id == indice[cursor].id) && (categorias[indice[cursor].pos].excluido == false))
    {
        std::cout << "Operação inválida: Já existe um registro com este código";
        return 1;
    }

    categorias[q] = add; 
    IndCategoriaById ind = IndCategoriaById{add.id, q};

    int reg = q;
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++;
	return 0;
}
int bscCategoriaById(int id, Categoria_Gasto *categorias, IndCategoriaById *indice, int quant)
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
            
            while (indice[cursor].id == id) 
            {
                if (!categorias[indice[cursor].pos].excluido)
                {
                    Categoria_Gasto categoria = categorias[indice[cursor].pos];
                    std::cout << "\nCategoria Encontrada\n";
                    printCategoria(categoria);
                    return 0;
                }
                cursor++;
            }
            
            std::cout << "\nCategoria Não Encontrada. (Apenas registros Excluídos)";
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;

	}
	std::cout << "\nCategoria Não Encontrada.\n";
    return 1;

}
int posCategoriaById(int id, Categoria_Gasto *categorias, IndCategoriaById *indice, int quant) //retorna a posição da categoria na lista, retorna -1 se não achar.
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
            while (indice[cursor].id == id) 
            {
                if (!categorias[indice[cursor].pos].excluido) return indice[cursor].pos;
                cursor++;
            }
            return -1;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
		

	}
    return -1;
}
int excCategoriaById(int id, Categoria_Gasto *categorias, IndCategoriaById *indice, int &quant)
{
	//retorna 0 se achar e excluir, 1 se não achar, 2 se achar mas estiver excluído, -1 se a exclusão for cancelada, 3 se o desenvolvedor for burro

	int i = 0, f = quant;
    int cursor = (i + f) / 2;

	for 
	(;
	f >= i; 
	cursor= (i + f) / 2)
	{
		if (indice[cursor].id == id)
        {
            while (indice[cursor].id == id) 
            {
                if (!categorias[indice[cursor].pos].excluido)
                {
                    int pos = indice[cursor].pos;
                    Categoria_Gasto categoria = categorias[pos];

                    std::cout << "\nCategoria Encontrada\n";
					printCategoria(categoria);
                    std::cout << "\nVocê Confirma a Exclusão deste Registro? \n (Insira [S] para confirmar)";

                    char conf = 0;
                    std::cin >> conf;
                    if (toupper(conf) == 'S') 
                    {
                        categorias[pos].excluido = true;
                        std::cout << "\nRegistro Excluído com sucesso.\n";
                        quant--;
                        return 0;
                    }
                    else {
						std::cout << "\nExclusão Cancelada.\n";
                    	return -1;
					}
                }
                cursor++;
            }
            std::cout << "\nCategoria Não Encontrada.\n"; 
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
        
		else {
            std::cout << "\n excBin Tem bug nesse código: \n";
            std::cout << std::endl << indice[cursor].id << " < " << id << "\ni : " << i << "; cursor: " << cursor << "; f: " << f;
            return 3;
        }
		
		
	}
	std::cout << "\nCategoria Não Encontrada.\n";
    return 1;
}


struct Banco
{
    int id;
    char descricao[40];
	bool excluido;
};
struct IndBancoById
{
	int id;
	int pos;
};
void printBanco(Banco banco)
{
	std::cout << std::endl << banco.id << ", " << banco.descricao << std::endl; 
}
void lExaustBancosById(Banco *bancos, IndBancoById *indice, int quant)
{
    std::cout   << "\n |-----------------------------------|"
    		   	<< "\n |       Bancos - ID crescente       |"
    		   	<< "\n |-----------------------------------|"
                << "\n |  ID   |    Descrição    | Status  |"
                << "\n |-----------------------------------|\n";
     for(int i = 0; i < quant; i++) 
     {
        Banco banco = bancos[indice[i].pos];
        
        std::string status;
        if (!banco.excluido) status = "Ativo  ";
        if (banco.excluido) status = "Inativo";

        std::string descricao = banco.descricao;
        if(descricao.size() > 14) 
        {
            descricao.resize(13, ' ');
            descricao.append(".");
        }
        else descricao.resize(14, ' ');
        std::cout   << " | "<< banco.id <<"\t | "<< descricao <<"  | "<< status <<" |\n";
     }
     std::cout  << " |-----------------------------------|\n"
                << " |            " << quant << " Registros            |\n"
                << " |-----------------------------------|\n";
     
}
void criarIndiceBancos(Banco *bancos, IndBancoById *indice, int quant) 
{
    for (int i = 0; i < quant; i++)
    {
        indice[i].id = bancos[i].id;
        indice[i].pos = i;
    }


    for (int i = 0; i < quant; i++){
    for (int j = i+1; j < quant; j++)
    {
        if (indice[j].id < indice[i].id) 
        {
            IndBancoById aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void organizarArquivoBancos(Banco *bancos, IndBancoById *novoIndice, int &quant) 
{
    int qAux; 

    for (int i = 0, j = i; i < quant && j < quant; i++) 
    { 

        while(bancos[j].excluido && j < quant) j++;
        if (j >= quant) break;
        bancos[i] = bancos[j];
        
        if(!bancos[i].excluido) qAux = i+1;
        j++;
    }

    quant = qAux;
    criarIndiceBancos(bancos, novoIndice, quant);
   
}
int inserirBanco(Banco *bancos, int &quant, Banco add, IndBancoById *indice)
{
	// retorna 0 se der certo, 1 se o id informado já estiver registrado e ativo 
    const int q = quant;
    int cursor = 0;
    for (;add.id > indice[cursor].id && cursor < q; cursor++);

    if((add.id == indice[cursor].id) && (bancos[indice[cursor].pos].excluido == false))
    {
        std::cout << "Operação inválida: Já existe um registro com este código";
        return 1;
    }

    bancos[q] = add; 
    IndBancoById ind = IndBancoById{add.id, q};

    int reg = q;
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++;
	return 0;
}
int bscBancoById(int id, Banco *bancos, IndBancoById *indice, int quant)
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
            
            
            while (indice[cursor].id == id) 
            {
                if (!bancos[indice[cursor].pos].excluido)
                {
                    Banco banco = bancos[indice[cursor].pos];
                    std::cout << "\nBanco Encontrado\n"
                            << banco.id << ", " << banco.descricao << std::endl;
                            return 0;
                }
                cursor++;
            }
            
            std::cout << "\nBanco Não Encontrado. (Apenas registros Excluídos)";
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
		

	}
	std::cout << "\nBanco Não Encontrado.\n";
    return 1;

}
int posBancoById(int id, Banco *bancos, IndBancoById *indice, int quant) //retorna a posição do banco na lista, retorna -1 se não achar.
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
            while (indice[cursor].id == id) 
            {
                if (!bancos[indice[cursor].pos].excluido) return indice[cursor].pos;
                cursor++;
            }
            return -1;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
		

	}
    return -1;
}
int excBancoById(int id, Banco *bancos, IndBancoById *indice, int &quant)
{
	//retorna 0 se achar e excluir, 1 se não achar, 2 se achar mas estiver excluído, -1 se a exclusão for cancelada, 3 se o desenvolvedor for burro

	int i = 0, f = quant;
    int cursor = (i + f) / 2; 

	for 
	(;
	f >= i; 
	cursor= (i + f) / 2)
	{
		if (indice[cursor].id == id)
        {
            while (indice[cursor].id == id) 
            {
                if (!bancos[indice[cursor].pos].excluido)
                {
                    int pos = indice[cursor].pos;
                    Banco banco = bancos[pos];

                    std::cout << "\nBanco Encontrado\n"
                            << banco.id << ", " << banco.descricao << std::endl;
                    std::cout << "\nVocê Confirma a Exclusão deste Registro? \n (Insira [S] para confirmar)";

                    char conf = 0;
                    std::cin >> conf;
                    if (toupper(conf) == 'S') 
                    {

                        bancos[pos].excluido = true;
                        std::cout << "\nRegistro Excluído com sucesso.\n";
                        quant--;
                        return 0;
                    }
                    else {
						std::cout << "\nExclusão Cancelada.\n";
                    	return -1;
					}
                }
                cursor++;
            }
            std::cout << "\nBanco Não Encontrado.\n"; 
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
        
		else {
            std::cout << "\n excBin Tem bug nesse código: \n";
            std::cout << std::endl << indice[cursor].id << " < " << id << "\ni : " << i << "; cursor: " << cursor << "; f: " << f;
            return 3;
        }
		
		
	}
	std::cout << "\nBanco Não Encontrado.\n";
    return 1;
}

struct Conta_Bancaria
{
	int id;
	int idBanco;
	char descricao[40];
	double saldo;
	bool excluido;
};
struct IndContaById
{
	int id;
	int pos;
};
void printConta(Conta_Bancaria conta)
{
	std::string status;
	if (conta.excluido) status = "Suspensa";
	else status = "Ativa";

	std::cout 	<< "\n  ID: " << conta.id 
				<< "\n  Cód Banco:" << conta.idBanco
				<< "\n  Descrição: " << conta.descricao
				<< "\n  Saldo: " << conta.saldo
				<< "\n  Status: " << status
				<< std::endl; 
}
void printContacomBanco(Conta_Bancaria conta, Banco banco)
{
	std::string status;
	if (conta.excluido) status = "Suspensa";
	else status = "Ativa";

	std::cout 	<< "\n  ID: " << conta.id 
				<< "\n  Banco: " << banco.descricao
				<< "\n  Descrição: " << conta.descricao
				<< "\n  Saldo: " << conta.saldo
				<< "\n  Status: " << status
				<< std::endl; 
}
void lExaustContasById(Conta_Bancaria *contas, IndContaById *indice, int quant)
{
    std::cout   << "\n -----------------------------------"
    		   	<< "\n        Contas - ID crescente       "
    		   	<< "\n -----------------------------------";
				
     for(int i = 0; i < quant; i++) 
     {
        Conta_Bancaria conta = contas[indice[i].pos];
        
        printConta(conta);
     }
     std::cout  << " -----------------------------------\n"
                << "             " << quant << " Registros            \n"
                << " -----------------------------------\n";
     
}
void lExaustContasByIdComBanco(Conta_Bancaria *contas, IndContaById *indice, int quant, Banco *bancos, IndBancoById *indBancos, int quantBancos)
{
    std::cout   << "\n -----------------------------------"
    		   	<< "\n        Contas - ID crescente       "
    		   	<< "\n -----------------------------------";
				
     for(int i = 0; i < quant; i++) 
     {
        Conta_Bancaria conta = contas[indice[i].pos];
		Banco banco;

		int posBanco = posBancoById(conta.idBanco, bancos, indBancos, quantBancos);
		if(posBanco >= 0) banco = bancos[posBanco];
		else (banco = Banco{0, "???", false});
		
        printContacomBanco(conta, banco);
     }
     std::cout  << " -----------------------------------\n"
                << "             " << quant << " Registros            \n"
                << " -----------------------------------\n";
     
}
void criarIndiceContas(Conta_Bancaria *contas, IndContaById *indice, int quant) 
{
    for (int i = 0; i < quant; i++)
    {
        indice[i].id = contas[i].id;
        indice[i].pos = i;
    }


    for (int i = 0; i < quant; i++){
    for (int j = i+1; j < quant; j++)
    {
        if (indice[j].id < indice[i].id) 
        {
            IndContaById aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void printIndiceConta(IndContaById *indice, int quant)
{
    std::cout   << "\n |---------------|"
				<< "\n |  indice. Contas  |"
				<< "\n |---------------|"
                << "\n | ID\t | POS.\t |"
                << "\n |---------------|\n";
    //if(printPes) std::cout << "\t";
    for(int i = 0; i < quant; i++) std::cout << " | "<< indice[i].id <<"\t | "<< indice[i].pos <<"\t |\n";
    std::cout   << " |---------------|\n"
                << " | " << quant << " Registros\t |"
                << "\n |---------------|\n";
}
void organizarArquivoContas(Conta_Bancaria *contas, IndContaById *novoIndice, int &quant) 
{
    int qAux; 

    for (int i = 0, j = i; i < quant && j < quant; i++) 
    { 

        while(contas[j].excluido && j < quant) j++;
        if (j >= quant) break;
        contas[i] = contas[j];
        
        if(!contas[i].excluido) qAux = i+1;
        j++;
    }

    quant = qAux;
    criarIndiceContas(contas, novoIndice, quant);
   
}
int inserirConta(Conta_Bancaria *contas, int &quant, Conta_Bancaria add, IndContaById *indice)
{
	// retorna 0 se der certo, 1 se o id informado já estiver registrado e ativo 
    const int q = quant;
    int cursor = 0;
    for (;add.id > indice[cursor].id && cursor < q; cursor++);

    if((add.id == indice[cursor].id) && (contas[indice[cursor].pos].excluido == false))
    {
        std::cout << "Operação inválida: Já existe um registro com este código";
        return 1;
    }

    contas[q] = add; // coloca o registro no fim da lista
    IndContaById ind = IndContaById{add.id, q};

    int reg = q;// reg = regressivo, contador que diminui
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++; //atualiza a quantidade de registros no fim
	return 0;
}
int bscContaById(int id, Conta_Bancaria *contas, IndContaById *indice, int quant)
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
            
            while (indice[cursor].id == id) 
            {
                if (!contas[indice[cursor].pos].excluido)
                {
                    Conta_Bancaria conta = contas[indice[cursor].pos];
                    std::cout << "\nConta Encontrada\n"
                            << conta.id << ", " << conta.descricao << std::endl;
                            return 0;
                }
                cursor++;
            }
            
            std::cout << "\nConta Não Encontrada. (Apenas registros Excluídos)";
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
		/*
        else {
            std::cout << "\n bscBinTem bug nesse código: \n";
            std::cout << std::endl << indice[cursor].id << " < " << id << "\ni : " << i << "; cursor: " << cursor << "; f: " << f;
            return 3;
        }
		*/

	}
	std::cout << "\nConta Não Encontrada.\n";
    return 1;

}
int posContaById(int id, Conta_Bancaria *contas, IndContaById *indice, int quant) //retorna a posição da conta na lista, retorna -1 se não achar.
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
            while (indice[cursor].id == id) 
            {
                if (!contas[indice[cursor].pos].excluido) return indice[cursor].pos;
                cursor++;
            }
            return -1;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
		

	}
    return -1;
}
int excContaById(int id, Conta_Bancaria *contas, IndContaById *indice, int &quant)
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
                if (!contas[indice[cursor].pos].excluido)
                {
                    int pos = indice[cursor].pos;
                    Conta_Bancaria conta = contas[pos];

                    std::cout << "\nConta Encontrada\n"
                            << conta.id << ", " << conta.descricao << std::endl;
                    std::cout << "\nVocê Confirma a Exclusão deste Registro? \n (Insira [S] para confirmar)";

                    char conf = 0;
                    std::cin >> conf;
                    if (toupper(conf) == 'S') 
                    {
                        contas[pos].excluido = true;
                        std::cout << "\nRegistro Excluído com sucesso.\n";
                        quant--;
                        return 0;
                    }
                    else {
						std::cout << "\nExclusão Cancelada.\n";
                    	return -1;
					}
                }
                cursor++;
            }
            std::cout << "\nConta Não Encontrada.\n"; 
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
        
		else {
            std::cout << "\n excBin Tem bug nesse código: \n";
            std::cout << std::endl << indice[cursor].id << " < " << id << "\ni : " << i << "; cursor: " << cursor << "; f: " << f;
            return 3;
        }
		
		
	}
	std::cout << "\nConta Não Encontrada.\n";
    return 1;
}


struct Transacao
{
    int id;
    int idCategoria;
    int idConta;
    Data data;
    float valor;
    char tipo; //Débito = D ; Crédito = C
	bool excluido;
};
struct IndTransacaoByData
{
	Data data;
	int pos;
};
void printTransacao(Transacao transacao)
{
	std::cout << std::fixed;
    std::cout << std::setprecision(2);

	char sinal;
	if(transacao.tipo == 'D') sinal = '-';
	else if(transacao.tipo == 'C') sinal = '+';
	else 
	{
		std::cout << "\n A transação tem um tipo Inválido\n";
		return;
	}	

	std::string status;
	if (transacao.excluido) status = "Estornada";
	else status = "Efetivada";

	std::cout 
	<< "\n ID: " << transacao.id 
	<< "\n Categoria Nº " << transacao.idCategoria
	<< "\n Conta Nº " << transacao.idConta
	<< "\n Data: "; printData(transacao.data);
	std::cout 
	<< "\n Valor: R$ " << sinal << transacao.valor
	<< "\n Status: " << status << std::endl;
	
}
void printTransacaoCompleta(Transacao transacao, Categoria_Gasto categoria, Conta_Bancaria conta, Banco banco)
{
	std::cout << std::fixed;
    std::cout << std::setprecision(2);

	char sinal;
	if(transacao.tipo == 'D') sinal = '-';
	else if(transacao.tipo == 'C') sinal = '+';
	else 
	{
		std::cout << "\n A transação tem um tipo Inválido\n";
		return;
	}	

	std::string status;
	if (transacao.excluido) status = "Estornada";
	else status = "Efetivada";

	std::cout 
	<< "\n ID: " << transacao.id 
	<< "\n Categoria: " << categoria.id << " - " << categoria.descricao
	<< "\n Conta " << conta.id << " no Banco " << banco.descricao
	<< "\n Data: "; printData(transacao.data);
	std::cout 
	<< "\n Valor: R$ " << sinal << transacao.valor
	<< "\n Status: " << status << std::endl;
	
}
/*
		struct Transacao
		{
			int id;
			int idCategoria;
			int idConta;
			Data data;
			float valor;
			char tipo; //Débito = D ; Crédito = C
			bool excluido;
		};
		*/

void criarIndiceTransacoesData(Transacao *transacoes, IndTransacaoByData *indice, int quant) 
{
    for (int i = 0; i < quant; i++)
    {
        indice[i].data = transacoes[i].data;
        indice[i].pos = i;
    }


    for (int i = 0; i < quant; i++){
    for (int j = i+1; j < quant; j++)
    {
        if (comparaDatas(indice[j].data, indice[i].data) < 0) 
        {
            IndTransacaoByData aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void organizarArquivoTransacoesData(Transacao *transacoes, IndTransacaoByData *novoIndice, int &quant) 
{
    int qAux; 

    for (int i = 0, j = i; i < quant && j < quant; i++) 
    { 

        while(transacoes[j].excluido && j < quant) j++;
        if (j >= quant) break;
        transacoes[i] = transacoes[j];
        
        if(!transacoes[i].excluido) qAux = i+1;
        j++;
    }

    quant = qAux;
    criarIndiceTransacoesData(transacoes, novoIndice, quant);
   
}
void lExaustTransacoesPeriodo(Transacao *transacoes, int quant, Data dataInicio, Data dataFim)
{
	std::cout << 0;
	IndTransacaoByData indice[quant];
	criarIndiceTransacoesData(transacoes, indice, quant);
	std::cout << 1;
	organizarArquivoTransacoesData(transacoes, indice, quant);
	float saldoTotal = 0;

    std::cout   << "\n |-------------------------------------------|"
    		   	<< "\n |   Transacoes de "<< dataToString(dataInicio) << " a " << dataToString(dataFim) << "   |"
    		   	<< "\n |-------------------------------------------|"
                << "\n |   ID   | Conta |    Data    |    Valor    |"
				//		 000000 |  000  | xx/xx/xxxx | 99999999.99
                << "\n |-------------------------------------------|\n";
     for(int i = 0; i < quant; i++) 
     {
		Transacao transacao = transacoes[indice[i].pos];
		if(comparaDatas(dataInicio, transacao.data) < 0 && comparaDatas(transacao.data, dataFim) < 0 && !transacao.excluido)
		{
			std::cout << std::fixed;
			std::cout << std::setprecision(2);

			
			
			int contZero;
			int digitos;

			std::string id = "";
			digitos = 0;
			int auxId = transacao.id;
			while (auxId > 0)
			{
				digitos++;
				auxId/=10;
			}
			contZero = 6 - digitos;
			while (contZero > 0)
			{
				id += "0";
				contZero--;
			}
			id += std::to_string(transacao.id);
			
			std::string conta = "";
			digitos = 0;
			int auxConta = transacao.idConta;
			while (auxConta > 0)
			{
				digitos++;
				auxConta/=10;
			}
			contZero = 3 - digitos;
			while (contZero > 0)
			{
				conta.append("0");
				contZero--;
			}
			conta += (std::to_string(transacao.idConta));
			
			std::string data = dataToString(transacao.data);


			std::ostringstream streamValor; //ostringstream = output string stream
			streamValor << std::fixed << std::setprecision(2) << transacao.valor;
			std::string valor = "";
			if(transacao.tipo == 'D') valor += '-';
			else if(transacao.tipo == 'C') valor += '+';
			else valor += '?';
			digitos = 0;
			int auxValor = (int)transacao.valor;
			while (auxValor > 0)
			{
				digitos++;
				auxValor/=10;
			}
			contZero = 7 - digitos;
			
			valor += streamValor.str();
			while (contZero >= 1)
			{
				valor.append(" ");
				contZero--;
			}

			std::cout   << " | " << id << " |  " << conta << "  | " << data  << " | " << valor << " |\n";

			if(transacao.tipo == 'D') saldoTotal -= transacao.valor;
			else if(transacao.tipo == 'C') saldoTotal += transacao.valor;

		}
     }
     std::cout  << " |-------------------------------------------|\n"
                << " |                " << quant << " Registros                |\n"
                << " |-------------------------------------------|\n"
				<< " | Saldo Final: " << saldoTotal << std::endl
				<< " |-------------------------------------------|\n";

     
}





int main()
{
	setlocale(LC_ALL, "");

	const int MAX = 60;

    Pessoa pessoas[MAX];
	IndPessoaById indPessoas[MAX];
	int quantPessoas; // quantia total de registros de Pessoas
	bool testPessoa = false;

	Conta_Bancaria contas[MAX];
	IndContaById indContas[MAX];
	int quantContas;
	bool testConta = false;

	Categoria_Gasto categorias[MAX];
	IndCategoriaById indCategorias[MAX];
	int quantCategorias;
	bool testCategoria = false;

	Banco bancos[MAX];
	IndBancoById indBancos[MAX];
	int quantBancos;
	bool testBanco = false;

	const int MAXTRANSACOES = 200;
	Transacao transacoes[MAXTRANSACOES];
	IndTransacaoByData indTransacoes[MAX];
	int quantTransacoes;
	bool testTrans = true;

	bool criarInicial = true;
	if (criarInicial)
	{
		categorias[0] = Categoria_Gasto{1, "Alimentação", false};
		categorias[1] = Categoria_Gasto{2, "Lazer", false};
		categorias[2] = Categoria_Gasto{3, "Saúde", false};
		categorias[3] = Categoria_Gasto{4, "Emergências", false};
		
		quantCategorias = 4;
		criarIndiceCategorias(categorias, indCategorias, quantCategorias);
		organizarArquivoCategorias(categorias, indCategorias, quantCategorias);


		bancos[0] = Banco{8, "SantoAndré", false};
		bancos[1] = Banco{6, "Banco do Paraguai", true};
		bancos[2] = Banco{2, "Recipiente", false};
		bancos[3] = Banco{3, "VestidoBank", false};

		quantBancos = 4;
		criarIndiceBancos(bancos, indBancos, quantBancos);
		organizarArquivoBancos(bancos, indBancos, quantBancos);


		contas[0] = Conta_Bancaria {5, 2, "Poupança do Seu Zé", 20000.0, false};
		contas[1] = Conta_Bancaria {3, 2, "Laranja", 300000.0, true};
		contas[2] = Conta_Bancaria {6, 1, "Bugada", 0.0, false};
		quantContas = 3;
		criarIndiceContas(contas, indContas, quantContas);

		Conta_Bancaria c1 = Conta_Bancaria{1, 8, "Conta do Marcão", 1800.0, false};
		Conta_Bancaria c2 = Conta_Bancaria{2, 2, "Conta do João", 1800.0, false};

		inserirConta(contas, quantContas, c1, indContas);
		inserirConta(contas, quantContas, c2, indContas);
		organizarArquivoContas(contas, indContas, quantContas);

	}


	if (testConta)
	{
		bancos[0] = Banco{8, "SantoAndré", false};
		bancos[1] = Banco{6, "Banco do Paraguai", true};
		bancos[2] = Banco{2, "Recipiente", false};
		bancos[3] = Banco{3, "VestidoBank", false};

		quantBancos = 4;

		criarIndiceBancos(bancos, indBancos, quantBancos);
		organizarArquivoBancos(bancos, indBancos, quantBancos);

		contas[0] = Conta_Bancaria {5, 2, "Poupança do Seu Zé", 20000.0, false};
		contas[1] = Conta_Bancaria {3, 2, "Laranja", 300000.0, true};
		contas[2] = Conta_Bancaria {6, 1, "Bugada", 0.0, false};
		quantContas = 3;

		Conta_Bancaria c1 = Conta_Bancaria{1, 8, "Conta do Marcão", 1800.0, false};
		Conta_Bancaria c2 = Conta_Bancaria{2, 2, "Conta do João", 1800.0, false};
		criarIndiceContas(contas, indContas, quantContas);

		inserirConta(contas, quantContas, c1, indContas);
		inserirConta(contas, quantContas, c2, indContas);

		lExaustContasById(contas, indContas, quantContas);

		organizarArquivoContas(contas, indContas, quantContas);

		lExaustContasById(contas, indContas, quantContas);
		lExaustContasByIdComBanco(contas, indContas, quantContas, bancos, indBancos, quantBancos);

		printIndiceConta(indContas, quantContas);
		//printContacomBanco(c1, bancos[0]);
	}

	if(testTrans)
	{
		/*
		Data d1 = Data{1, 1, 2001};
		Data d2 = Data{2, 1, 2001};
		Data d3 = Data{1, 2, 2001};
		Data d4 = Data{1, 1, 2002};
		Data d5 = d1;

		std::cout << comparaDatas(d1, d2) << "\n";
		std::cout << comparaDatas(d1, d3) << "\n";
		std::cout << comparaDatas(d1, d4) << "\n";
		std::cout << comparaDatas(d1, d5) << "\n";
		std::cout << comparaDatas(d1, d1) << "\n";
		std::cout << comparaDatas(d2, d3) << "\n";
		std::cout << comparaDatas(d3, d2) << "\n";
		transacoes[0] = Transacao{1, 1, 1, Data{1, 1, 1}, 100.234, 'C', true};
		*/

		/*
		struct Transacao
		{
			int id;
			int idCategoria;
			int idConta;
			Data data;
			float valor;
			char tipo; //Débito = D ; Crédito = C
			bool excluido;
		};
		*/

		Transacao t1{2, 2, 1, Data{25, 9, 2023}, 50.50, 'D', false};
		Transacao t2{20, 2, 10, Data{26, 9, 2023}, 500.50, 'C', false};
		Transacao t3{200, 2, 100, Data{29, 10, 2023}, 5000.50, 'D', false};
		Categoria_Gasto cat1 = categorias[posCategoriaById(t1.idCategoria, categorias, indCategorias, quantCategorias)];
		Conta_Bancaria conta1 = contas[posContaById(t1.idConta, contas, indContas, quantContas)];
		Banco banco1 = bancos[posBancoById(conta1.idBanco, bancos, indBancos, quantBancos)];
		printTransacaoCompleta(t1, cat1, conta1, banco1);
		transacoes[0] = t1;
		transacoes[1] = t2;
		transacoes[2] = t3;
		quantTransacoes = 3;
		criarIndiceTransacoesData(transacoes, indTransacoes, quantTransacoes);
		organizarArquivoTransacoesData(transacoes, indTransacoes, quantTransacoes);
		lExaustTransacoesPeriodo(transacoes, quantTransacoes, Data{24,9,2023}, Data{24,10,2023});


	}

	if(testBanco)
	{
		bancos[0] = Banco{8, "SantoAndré", false};
		bancos[1] = Banco{6, "Banco do Paraguai", true};
		bancos[2] = Banco{2, "Recipiente", false};
		bancos[3] = Banco{3, "VestidoBank", false};

		quantBancos = 4;

		for (int i = 0; i < quantBancos; i++) printBanco(bancos[i]);
		std::cout << "\nCriando Indice\n";
		criarIndiceBancos(bancos, indBancos, quantBancos);
		lExaustBancosById(bancos, indBancos, quantBancos);
		std::cout << "\nIndice Organizado\n";
		organizarArquivoBancos(bancos, indBancos, quantBancos);
		lExaustBancosById(bancos, indBancos, quantBancos);

		Banco bAdd1{1, "Bancao", true};
		Banco bAdd2{4, "Bancao", false};
		Banco bAdd3{5, "Bancao", false};

		std::cout << "\nInserindo Bancos\n";
		inserirBanco(bancos, quantBancos, bAdd1, indBancos);
		inserirBanco(bancos, quantBancos, bAdd2, indBancos);
		inserirBanco(bancos, quantBancos, bAdd2, indBancos); // adicionando um que ja existe
		inserirBanco(bancos, quantBancos, bAdd3, indBancos);
		lExaustBancosById(bancos, indBancos, quantBancos);

		std::cout << "\nIndice Organizado Apos Inserção\n";
		organizarArquivoBancos(bancos, indBancos, quantBancos);
		lExaustBancosById(bancos, indBancos, quantBancos);

		inserirBanco(bancos, quantBancos, bAdd1, indBancos);
		bscBancoById(bAdd1.id, bancos, indBancos, quantBancos); // tentando achar um banco logicamente excluido
		bscBancoById(7, bancos, indBancos, quantBancos);

		bscBancoById(2, bancos, indBancos, quantBancos);
		excBancoById(2, bancos, indBancos, quantBancos);
		bscBancoById(2, bancos, indBancos, quantBancos);

		
	}
	
	
	if(testPessoa)
	{
		
		pessoas[0] = Pessoa{8, "Marcolino", false};
		pessoas[1] = Pessoa{6, "Armando", true};
		pessoas[2] = Pessoa{2, "João", false};
		pessoas[3] = Pessoa{3, "Pedro", false};

		quantPessoas = 4;


		Pessoa pAdd1{1, "Rodrigo", false};
		Pessoa pAdd2{7, "Cesar", true};
		Pessoa pAdd3{10, "Tulio", false};


		inserirPessoa(pessoas, quantPessoas, pAdd1, indPessoas);
		inserirPessoa(pessoas, quantPessoas, pAdd2, indPessoas);
		inserirPessoa(pessoas, quantPessoas, pAdd3, indPessoas);

		criarIndicePessoas(pessoas, indPessoas, quantPessoas);
		printIndicePessoa(indPessoas, quantPessoas);
		
		lExaustPessoasById(pessoas, indPessoas, quantPessoas);


		for (int i = 1; i < quantPessoas; i++) {
			bscPessoaById(i, pessoas, indPessoas, quantPessoas);

			};

		organizarArquivoPessoas(pessoas, indPessoas, quantPessoas);
		printIndicePessoa(indPessoas, quantPessoas);


		std::cout   << "\n\n--------------------------------------------------------\n\nTestando Exclusão\n"
					<< "\nTentando Excluir id 8 (existe)\n";
		excPessoaById(8, pessoas, indPessoas, quantPessoas);
		std::cout   << "\nTentando Excluir id 8 de novo\n";
		excPessoaById(8, pessoas, indPessoas, quantPessoas);

	}

	if(testCategoria)
	{

		categorias[0] = Categoria_Gasto{1, "Alimentação", false};
		categorias[1] = Categoria_Gasto{2, "Lazer", false};
		categorias[2] = Categoria_Gasto{3, "Saúde", false};
		categorias[3] = Categoria_Gasto{4, "Emergências", false};
		quantCategorias = 4;

		Categoria_Gasto cAdd1{8, "A", false};
		Categoria_Gasto cAdd2{6, "B", true};
		Categoria_Gasto cAdd3{9, "C", true};

		inserirCategoria(categorias, quantCategorias, cAdd1, indCategorias);
		inserirCategoria(categorias, quantCategorias, cAdd2, indCategorias);
		inserirCategoria(categorias, quantCategorias, cAdd3, indCategorias);

		criarIndiceCategorias(categorias, indCategorias, quantCategorias);
		

		for (int i = 1; i <= quantCategorias; i++) {
			bscCategoriaById(i, categorias, indCategorias, quantCategorias);

			};

		organizarArquivoCategorias(categorias, indCategorias, quantCategorias);
		lExaustCategoriasById(categorias, indCategorias, quantCategorias);


		std::cout   << "\n\n--------------------------------------------------------\n\nTestando Exclusão de Categoria\n"
					<< "\nTentando Excluir id 8 (existe)\n";
		excCategoriaById(8, categorias, indCategorias, quantCategorias);
		std::cout   << "\nTentando Excluir id 8 de novo\n";
		excCategoriaById(8, categorias, indCategorias, quantCategorias);
	}

	return 0;
}