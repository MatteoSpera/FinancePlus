#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <locale>
#include <ctime>
#include <windows.h>
#include <conio.h>

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
Data lerData()
{
	Data data;
	int n;

	std::cout << "\nInsira Dia: ";
	std::cin >> n;
	std::cin.ignore();
	data.dia = n;

	std::cout << "Insira M�s: ";
	std::cin >> n;
	std::cin.ignore();
	data.mes = n;

	std::cout << "Insira Ano: ";
	std::cin >> n;
	std::cin.ignore();
	data.ano = n;

	return data;
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

void printIndicePessoa(IndiceId *indice, int quant)
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
void lExaustPessoasById(Pessoa *pessoas, IndiceId *indice, int quant)
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
void criarIndicePessoas(Pessoa *pessoas, IndiceId *indice, int quant) 
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
            IndiceId aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void organizarArquivoPessoas(Pessoa *pessoas, IndiceId *novoIndice, int &quant) // Apaga registros logicamente exclu�dos e reorganiza �ndice
{
    // inclui apenas os que n�o est�o logicamente exclu�dos
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
    IndiceId ind = IndiceId{add.id, q};

    int reg = q;// reg = regressivo, contador que diminui
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++; //atualiza a quantidade de registros no fim
	return 0;
}
int bscPessoaById(int id, Pessoa *pessoas, IndiceId *indice, int quant)
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
            
            std::cout << "\nPessoa N�o Encontrada. (Apenas registros Exclu�dos)\n\n";
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
	std::cout << "\nPessoa N�o Encontrada.\n\n";
    return 1;

}
int excPessoaById(int id, Pessoa *pessoas, IndiceId *indice, int &quant)
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
                    std::cin >> conf;
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
	bool excluido;
};
void printCategoria(Categoria_Gasto categoria)
{
	std::cout << std::endl << categoria.id << ", " << categoria.descricao << std::endl << std::endl; 
}
void lExaustCategoriasById(Categoria_Gasto *categorias, IndiceId *indice, int quant)
{
    std::cout   << "\n |-----------------------------|"
    		   	<< "\n |  Categorias - ID crescente  |"
    		   	<< "\n |-----------------------------|"
                << "\n |  ID   | Descri��o | Status  |"
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
void criarIndiceCategorias(Categoria_Gasto *categorias, IndiceId *indice, int quant) 
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
            IndiceId aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void organizarArquivoCategorias(Categoria_Gasto *categorias, IndiceId *novoIndice, int &quant)
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
Categoria_Gasto lerCategoria(Categoria_Gasto categoria)
{
	std::cout << "\nInsira a Descri��o da Categoria: ";
	std::cin.ignore();
	gets(categoria.descricao);

	return categoria;

}
int inserirCategoria(Categoria_Gasto *categorias, int &quant, Categoria_Gasto add, IndiceId *indice)
{
	// retorna 0 se der certo, 1 se o id informado j� estiver registrado e ativo 
    const int q = quant;
    int cursor = 0;
    for (;add.id > indice[cursor].id && cursor < q; cursor++);

    if((add.id == indice[cursor].id) && (categorias[indice[cursor].pos].excluido == false))
    {
        std::cout << "Opera��o inv�lida: J� existe um registro com este c�digo";
        return 1;
    }

    categorias[q] = add; 
    IndiceId ind = IndiceId{add.id, q};

    int reg = q;
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++;
	return 0;
}
int bscCategoriaById(int id, Categoria_Gasto *categorias, IndiceId *indice, int quant)
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
            
            std::cout << "\nCategoria N�o Encontrada. (Apenas registros Exclu�dos)\n\n";
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;

	}
	std::cout << "\nCategoria N�o Encontrada.\n\n";
    return 1;

}
int posCategoriaById(int id, Categoria_Gasto *categorias, IndiceId *indice, int quant) //retorna a posi��o da categoria na lista, retorna -1 se n�o achar.
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
int excCategoriaById(int id, Categoria_Gasto *categorias, IndiceId *indice, int &quant)
{
	//retorna 0 se achar e excluir, 1 se n�o achar, 2 se achar mas estiver exclu�do, -1 se a exclus�o for cancelada, 3 se o desenvolvedor for burro

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
                    std::cout << "\nVoc� Confirma a Exclus�o deste Registro? \n (Insira [S] para confirmar)";

                    char conf = 0;
                    std::cin >> conf;
                    if (toupper(conf) == 'S') 
                    {
                        categorias[pos].excluido = true;
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
            std::cout << "\nCategoria N�o Encontrada.\n"; 
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
	std::cout << "\nCategoria N�o Encontrada.\n";
    return 1;
}


struct Banco
{
    int id;
    char descricao[40];
	bool excluido;
};
void printBanco(Banco banco)
{
	std::cout << std::endl << banco.id << ", " << banco.descricao << std::endl << std::endl; 
}
void lExaustBancosById(Banco *bancos, IndiceId *indice, int quant)
{
    std::cout   << "\n |-----------------------------------|"
    		   	<< "\n |       Bancos - ID crescente       |"
    		   	<< "\n |-----------------------------------|"
                << "\n |  ID   |    Descri��o    | Status  |"
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
void criarIndiceBancos(Banco *bancos, IndiceId *indice, int quant) 
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
            IndiceId aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void organizarArquivoBancos(Banco *bancos, IndiceId *novoIndice, int &quant) 
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
Banco lerBanco(Banco banco)
{
	char descricao[40];
	std::cout << "\nInsira a Descri��o da Banco: ";
	std::cin.ignore();
	gets(banco.descricao);

	return banco;

}
int inserirBanco(Banco *bancos, int &quant, Banco add, IndiceId *indice)
{
	// retorna 0 se der certo, 1 se o id informado j� estiver registrado e ativo 
    const int q = quant;
    int cursor = 0;
    for (;add.id > indice[cursor].id && cursor < q; cursor++);

    if((add.id == indice[cursor].id) && (bancos[indice[cursor].pos].excluido == false))
    {
        std::cout << "Opera��o inv�lida: J� existe um registro com este c�digo";
        return 1;
    }

    bancos[q] = add; 
    IndiceId ind = IndiceId{add.id, q};

    int reg = q;
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++;
	return 0;
}
int bscBancoById(int id, Banco *bancos, IndiceId *indice, int quant)
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
            
            std::cout << "\nBanco N�o Encontrado. (Apenas registros Exclu�dos)\n\n";
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
		

	}
	std::cout << "\nBanco N�o Encontrado.\n\n";
    return 1;

}
int posBancoById(int id, Banco *bancos, IndiceId *indice, int quant) //retorna a posi��o do banco na lista, retorna -1 se n�o achar.
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
int excBancoById(int id, Banco *bancos, IndiceId *indice, int &quant)
{
	//retorna 0 se achar e excluir, 1 se n�o achar, 2 se achar mas estiver exclu�do, -1 se a exclus�o for cancelada, 3 se o desenvolvedor for burro

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
                    std::cout << "\nVoc� Confirma a Exclus�o deste Registro? \n (Insira [S] para confirmar)";

                    char conf = 0;
                    std::cin >> conf;
                    if (toupper(conf) == 'S') 
                    {

                        bancos[pos].excluido = true;
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
            std::cout << "\nBanco N�o Encontrado.\n"; 
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
	std::cout << "\nBanco N�o Encontrado.\n";
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
void printConta(Conta_Bancaria conta)
{
	std::string status;
	if (conta.excluido) status = "Cancelada";
	else status = "Ativa";

	std::cout 	<< "\n  ID: " << conta.id 
				<< "\n  C�d Banco:" << conta.idBanco
				<< "\n  Descri��o: " << conta.descricao
				<< "\n  Saldo: " << conta.saldo
				<< "\n  Status: " << status
				<< std::endl << std::endl; 
}
void printContacomBanco(Conta_Bancaria conta, Banco banco)
{
	std::string status;
	if (conta.excluido) status = "Cancelada";
	else status = "Ativa";

	std::cout 	<< "\n  ID: " << conta.id 
				<< "\n  Banco: " << banco.descricao
				<< "\n  Descri��o: " << conta.descricao
				<< "\n  Saldo: " << conta.saldo
				<< "\n  Status: " << status
				<< std::endl << std::endl; 
}
void lExaustContasById(Conta_Bancaria *contas, IndiceId *indice, int quant)
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
void lExaustContasByIdComBanco(Conta_Bancaria *contas, IndiceId *indice, int quant, Banco *bancos, IndiceId *indBancos, int quantBancos)
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
void lstSaldos(Conta_Bancaria *contas, IndiceId *indice, int quant, Banco *bancos, IndiceId *indBancos, int quantBancos)
{
	std::cout   << "\n |-------------------------------------------|"
    		   	<< "\n |               Listando Saldos             |"
    		   	<< "\n |-------------------------------------------|"
                << "\n | Conta |        Banco       |     Saldo    |"
				//	   |  000  | Banco blablablabla | 99999999.99  |
                << "\n |-------------------------------------------|\n";
	int cont = 0;
	double saldoTotal;
	for(int i = 0; i < quant; i++) 
	{
		Conta_Bancaria conta = contas[indice[i].pos];
		if( !conta.excluido)
		{
			std::cout << std::fixed;
			std::cout << std::setprecision(2);

			Banco banco;
			int posBanco = posBancoById(conta.idBanco, bancos, indBancos, quantBancos);
			if(posBanco >= 0) banco = bancos[posBanco];
			else (banco = Banco{0, "???", false});

			std::string descBanco = banco.descricao;
			if(descBanco.size() > 18) 
			{
				descBanco.resize(17, ' ');
				descBanco.append(".");
			}
			else descBanco.resize(18, ' ');
			
			int contZero;
			int digitos;

		
			std::string id = "";
			digitos = 0;
			int auxConta = conta.id;
			while (auxConta > 0)
			{
				digitos++;
				auxConta/=10;
			}
			contZero = 3 - digitos;
			while (contZero > 0)
			{
				id.append("0");
				contZero--;
			}
			id += (std::to_string(conta.id));
			


			std::ostringstream streamSaldo; 
			streamSaldo << std::fixed << std::setprecision(2) << conta.saldo;
			std::string saldo = "";
			if(conta.saldo > 0) saldo += '+';
			else if(conta.saldo == 0) saldo += ' ';
			digitos = 0;
			int auxSaldo = (int)conta.saldo;
			if (auxSaldo < 0) auxSaldo *= -1;
			if (auxSaldo == 0) auxSaldo = 1;
			while (auxSaldo > 0)
			{
				digitos++;
				auxSaldo/=10;
			}
			contZero = 7 - digitos;
			
			saldo += streamSaldo.str();
			while (contZero >= 1)
			{
				saldo.append(" ");
				contZero--;
			}

			std::cout   << " |  " << id << "  | " << descBanco <<  " | " << saldo << "  |\n";

			saldoTotal += conta.saldo;
			cont++;
		}
     }
	std::cout  	<< " |-------------------------------------------|\n"
				<< " |                " << cont << " Registros                |\n"
				<< " |-------------------------------------------|\n"
				<< " | Saldo Final: " << saldoTotal << std::endl
				<< " |-------------------------------------------|\n";

     
}
void criarIndiceContas(Conta_Bancaria *contas, IndiceId *indice, int quant) 
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
            IndiceId aux = indice[i];
            indice[i] = indice[j];
            indice[j] = aux;
        }
    }}
    
}
void printIndiceConta(IndiceId *indice, int quant)
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
void organizarArquivoContas(Conta_Bancaria *contas, IndiceId *novoIndice, int &quant) 
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
Conta_Bancaria lerConta(Conta_Bancaria conta)
{
	std::cout << "\nInsira a Descri��o da Conta: ";
	std::cin.ignore();
	gets(conta.descricao);
	std::cout << "\nInsira o Saldo da Conta: ";
	std::cin >> conta.saldo;

	return conta;

}
int inserirConta(Conta_Bancaria *contas, int &quant, Conta_Bancaria add, IndiceId *indice)
{
	// retorna 0 se der certo, 1 se o id informado j� estiver registrado e ativo 
    const int q = quant;
    int cursor = 0;
    for (;add.id > indice[cursor].id && cursor < q; cursor++);

    if((add.id == indice[cursor].id) && (contas[indice[cursor].pos].excluido == false))
    {
        std::cout << "Opera��o inv�lida: J� existe um registro com este c�digo";
        return 1;
    }

    contas[q] = add; // coloca o registro no fim da lista
    IndiceId ind = IndiceId{add.id, q};

    int reg = q;// reg = regressivo, contador que diminui
    for (;reg > cursor; reg--) indice[reg] = indice[reg-1];
    indice[cursor] = ind;
    
    quant++; //atualiza a quantidade de registros no fim
	return 0;
}
int bscContaById(int id, Conta_Bancaria *contas, IndiceId *indice, int quant)
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
            
            std::cout << "\nConta N�o Encontrada. (Apenas registros Exclu�dos)\n\n";
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
	std::cout << "\nConta N�o Encontrada.\n\n";
    return 1;

}
int posContaById(int id, Conta_Bancaria *contas, IndiceId *indice, int quant) //retorna a posi��o da conta na lista, retorna -1 se n�o achar.
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
int excContaById(int id, Conta_Bancaria *contas, IndiceId *indice, int &quant)
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
                if (!contas[indice[cursor].pos].excluido)
                {
                    int pos = indice[cursor].pos;
                    Conta_Bancaria conta = contas[pos];

                    
                    std::cout << "\nVoc� Confirma a Exclus�o deste Registro? \n (Insira [S] para confirmar)";

                    char conf = 0;
                    std::cin >> conf;
                    if (toupper(conf) == 'S') 
                    {
                        contas[pos].excluido = true;
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
            std::cout << "\nConta N�o Encontrada.\n"; 
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
	std::cout << "\nConta N�o Encontrada.\n";
    return 1;
}


struct Transacao
{
    int id;
    int idCategoria;
    int idConta;
    Data data;
    float valor;
    char tipo; //D�bito = D ; Cr�dito = C
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
		std::cout << "\n A transa��o tem um tipo Inv�lido\n";
		return;
	}	

	std::string status;
	if (transacao.excluido) status = "Estornada";
	else status = "Efetivada";

	std::cout 
	<< "\n ID: " << transacao.id 
	<< "\n Categoria N� " << transacao.idCategoria
	<< "\n Conta N� " << transacao.idConta
	<< "\n Data: "; printData(transacao.data);
	std::cout 
	<< "\n Valor: R$ " << sinal << transacao.valor
	<< "\n Status: " << status << std::endl << std::endl;
	
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
		std::cout << "\n A transa��o tem um tipo Inv�lido\n";
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
	<< "\n Status: " << status << std::endl << std::endl;
	
}
/*
		struct Transacao
		{
			int id;
			int idCategoria;
			int idConta;
			Data data;
			float valor;
			char tipo; //D�bito = D ; Cr�dito = C
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
	IndTransacaoByData indice[quant];
	criarIndiceTransacoesData(transacoes, indice, quant);
	organizarArquivoTransacoesData(transacoes, indice, quant);
	float saldoTotal = 0;

    std::cout   << "\n |-------------------------------------------|"
    		   	<< "\n |   Transacoes de "<< dataToString(dataInicio) << " a " << dataToString(dataFim) << "   |"
    		   	<< "\n |-------------------------------------------|"
                << "\n |   ID   | Conta |    Data    |    Valor    |"
				//		 000000 |  000  | xx/xx/xxxx | 99999999.99
                << "\n |-------------------------------------------|\n";
	int cont = 0;
     for(int i = 0; i < quant; i++) 
     {
		Transacao transacao = transacoes[indice[i].pos];
		if(comparaDatas(dataInicio, transacao.data) <= 0 && comparaDatas(transacao.data, dataFim) <= 0 && !transacao.excluido)
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
			cont++;
		}
     }
     std::cout  << " |-------------------------------------------|\n"
                << " |                " << cont << " Registros                |\n"
                << " |-------------------------------------------|\n"
				<< " | Saldo Final: " << saldoTotal << std::endl
				<< " |-------------------------------------------|\n";

     
}
Transacao lerTransacao(Transacao transacao)
{
	std::cout << "\nInsira o Valor da Transa��o: ";
	std::cin.ignore();
	std::cin >> transacao.valor;
	while(transacao.valor <= 0)
	{
		std::cout << "\nO valor da Transa��o deve ser positivo ";
		std::cin >> transacao.valor;
	}


	std::cout << "\n\nInsira o Tipo da Transa��o - Cr�dito [C] ou D�bito [D] ";
	
	char tipo = 0;
	std::cin >> tipo;
	tipo = toupper(tipo);
	
	while(tipo != 'C' && tipo != 'D')
	{
		std::cout << "\nInsira um Tipo v�lido [C/D] ";
		std::cin >> tipo;
		tipo = toupper(tipo);
	}
	transacao.tipo = tipo;

	return transacao;

}


void criarIndiceTransacoesById(Transacao *transacoes, IndiceId *indice, int quant) 
{
    for (int i = 0; i < quant; i++)
    {
        indice[i].id = transacoes[i].id;
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
void organizarArquivoTransacoesById(Transacao *transacoes, IndiceId *novoIndice, int &quant) 
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
    criarIndiceTransacoesById(transacoes, novoIndice, quant);
   
}

int inserirTransacao(Transacao *transacoes, int &quant, Transacao add, IndiceId *indId, IndTransacaoByData *indData)
{
	// retorna 0 se der certo, 1 se o id informado j� estiver registrado e ativo 
    const int q = quant;
    int cursor = 0;
    for (;add.id > indId[cursor].id && cursor < q; cursor++);

    if((add.id == indId[cursor].id) && (transacoes[indId[cursor].pos].excluido == false))
    {
        std::cout << "Opera��o inv�lida: J� existe um registro com este c�digo";
        return 1;
    }

    transacoes[q] = add; 
    IndiceId indcId = IndiceId{add.id, q};

    int reg = q;
    for (;reg > cursor; reg--) indId[reg] = indId[reg-1];
    indId[cursor] = indcId;
    
	cursor = 0;
	for (;comparaDatas(add.data, indData[cursor].data) > 0 && cursor < q; cursor++);
	IndTransacaoByData indcData = IndTransacaoByData{add.data, q};
	reg = q;
	for (;reg > cursor; reg--) indData[reg] =  indData[reg-1];
	indData[cursor] = indcData; 

    quant++;
	return 0;
}

int bscTransacaoById(int id, Transacao *transacoes, IndiceId *indice, int quant)
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
            
            
            while (indice[cursor].id == id) 
            {
                if (!transacoes[indice[cursor].pos].excluido)
                {
                    Transacao transacao = transacoes[indice[cursor].pos];
                    std::cout << "\nTransa��o Encontrada\n";
                    printTransacao(transacao);
                    return 0;
                }
                cursor++;
            }
            
            std::cout << "\nTransa��o N�o Encontrada. (Apenas registros Exclu�dos)\n\n";
            return 2;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
		

	}
	std::cout << "\nTransa��o N�o Encontrada.\n\n";
    return 1;

}
int posTransacaoById(int id, Transacao *transacoes, IndiceId *indice, int quant) //retorna a posi��o da transa��o na lista, retorna -1 se n�o achar.
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
                if (!transacoes[indice[cursor].pos].excluido) return indice[cursor].pos;
                cursor++;
            }
            return -1;
        }
        else if(indice[cursor].id > id) f = cursor-1;
        else if(indice[cursor].id < id) i = cursor+1;
		

	}
    return -1;
}
int excTransacaoById(int id, Transacao *transacoes, IndiceId *indice, int &quant)
{
	//retorna 0 se achar e excluir, 1 se n�o achar, 2 se achar mas estiver exclu�do, -1 se a exclus�o for cancelada, 3 se o desenvolvedor for burro

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
                if (!transacoes[indice[cursor].pos].excluido)
                {
                    int pos = indice[cursor].pos;
                    Transacao transacao = transacoes[pos];

                    
                    std::cout << "\nVoc� Confirma a Exclus�o deste Registro? \n (Insira [S] para confirmar)";

                    char conf = 0;
                    std::cin >> conf;
                    if (toupper(conf) == 'S') 
                    {

                        transacoes[pos].excluido = true;
                        std::cout << "\nTransa��o Estornada com sucesso.\n";
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
            std::cout << "\nTransa��o N�o Encontrada.\n"; 
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
	std::cout << "\nTransa��o N�o Encontrada.\n";
    return 1;
}

void voltaMenu()
{
	std::cout << "\n\nAperte Qualquer Tecla para Voltar ao Menu\n";
	getch();
}


int main()
{
	setlocale(LC_ALL, "");

	const int MAX = 60;

    Pessoa pessoas[MAX];
	IndiceId indPessoas[MAX];
	int quantPessoas; // quantia total de registros de Pessoas
	bool testPessoa = false;

	Conta_Bancaria contas[MAX];
	IndiceId indContas[MAX];
	int quantContas;
	bool testConta = false;

	Categoria_Gasto categorias[MAX];
	IndiceId indCategorias[MAX];
	int quantCategorias;
	bool testCategoria = false;

	Banco bancos[MAX];
	IndiceId indBancos[MAX];
	int quantBancos;
	bool testBanco = false;

	const int MAXTRANSACOES = 200;
	Transacao transacoes[MAXTRANSACOES];
	IndTransacaoByData indTransData[MAX];
	IndiceId indTransId[MAX];
	int quantTransacoes;
	bool testTrans = true;

	bool criarInicial = true;
	if (criarInicial)
	{
		categorias[0] = Categoria_Gasto{1, "Alimenta��o", false};
		categorias[1] = Categoria_Gasto{2, "Lazer", false};
		categorias[2] = Categoria_Gasto{3, "Sa�de", false};
		categorias[3] = Categoria_Gasto{5, "Caridade", false};
		categorias[4] = Categoria_Gasto{6, "Emerg�ncias", false};
		categorias[5] = Categoria_Gasto{7, "Trabalho", false};
		categorias[6] = Categoria_Gasto{8, "Multas", true};
		categorias[7] = Categoria_Gasto{9, "Bicos", true};
		
		quantCategorias = 8;
		criarIndiceCategorias(categorias, indCategorias, quantCategorias);


		bancos[0] = Banco{1, "SantoAndr�", false};
		bancos[1] = Banco{2, "Banco do Paraguai", true};
		bancos[2] = Banco{3, "Recipiente", false};
		bancos[3] = Banco{4, "VestidoBank", false};
		bancos[4] = Banco{5, "Brolesco", false};
		bancos[5] = Banco{6, "Jamerindus", true};

		quantBancos = 6;
		criarIndiceBancos(bancos, indBancos, quantBancos);


		contas[0] = Conta_Bancaria {1, 5, "Poupan�a do Seu Z�", 20000.0, false};
		contas[1] = Conta_Bancaria{2, 3, "Conta do Marc�o", 1800.0, false};
		contas[2] = Conta_Bancaria{3, 4, "Conta do Jo�o", -1800.0, false};
		contas[3] = Conta_Bancaria{4, 5, "Fundos da ONG IBP", 400, false};
		contas[4] = Conta_Bancaria{5, 1, "Fundos da ONG JDA", 0, false};
		contas[5] = Conta_Bancaria{6, 4, "Conta Laranja", 400, true};
		contas[6] = Conta_Bancaria{7, 5, "Conta Fantasma", 400, true};

		quantContas = 6;
		criarIndiceContas(contas, indContas, quantContas);
		
		transacoes[0] = Transacao{1, 2, 1, Data{25, 9, 2023}, 50.50, 'D', false};
		transacoes[1] = Transacao{2, 6, 2, Data{26, 9, 2023}, 2000, 'C', false};
		transacoes[2] = Transacao{3, 3, 4, Data{29, 10, 2023}, 5000.00, 'C', false};

		quantTransacoes = 3;
		criarIndiceTransacoesById(transacoes, indTransId, quantTransacoes);
		criarIndiceTransacoesData(transacoes, indTransData, quantTransacoes);
		organizarArquivoTransacoesById(transacoes, indTransId, quantTransacoes);
		organizarArquivoTransacoesData(transacoes, indTransData, quantTransacoes);
	}
	
	time_t now = time(0);
    tm *agora = localtime(&now);
    Data hoje = Data{agora->tm_mday, 1+agora->tm_mon, 1900+agora->tm_year};

	bool run = true;
	bool constClear = false;
	char op;

	
	while(run)
	{
		std::cout   << "\n\n|------------------------------------------|\n"
					<< "|                "; printData(hoje); std::cout	<< "                |\n";
        std::cout   << "|------------------------------------------|\n"
					<< "|               Finance Plus               |\n"
					<< "|------------------------------------------|\n"
                    << "INFORME A OPERA��O QUE DESEJA FAZER: \n"
                    << "A - Consultar Itens\n"
                    << "B - Listar Itens\n"
                    << "C - Inserir novos Itens\n"
                    << "D - Excluir Itens\n"
					<< "E - Organizar Arquivos\n"
					<< "F - Manipular Transa��es\n"
					<< "G - Listar Saldos de todas as contas\n"
                    << "X - Sair do programa\n"
					<< "\n";

		std::cin >> op;
        std::cout << "\nVoc� escolheu ";

		switch(toupper(op))
        {	
			case 'A':
			{
				char opCon;
				std::cout << "Consultar Itens\n";
				std::cout << "Que Tipo de Item voc� gostaria de Consultar?\n"
				<< "1 - Categoria\n"
				<< "2 - Banco\n"
				<< "3 - Conta\n";

				std::cin.ignore();
				std::cin >> opCon;
				std::cout << "\nVoc� escolheu Consultar ";
				switch(toupper(opCon))
        		{
					case '1':
					{	
						std::cout << "uma Categoria\n";
						std::cout << "Insira o ID da Categoria que deseja procurar: ";
						int id;
						std::cin.ignore();
						std::cin >> id;
						bscCategoriaById(id, categorias, indCategorias, quantCategorias);
						break;
					}
					case '2':
					{	
						std::cout << "um Banco\n";
						std::cout << "Insira o ID do Banco que deseja procurar: ";
						int id;
						std::cin.ignore();
						std::cin >> id;
						bscBancoById(id, bancos, indBancos, quantBancos);
						break;
					}
					case '3':
					{	
						std::cout << "uma Conta\n";
						std::cout << "Insira o ID da Conta que deseja procurar: ";
						int id;
						std::cin.ignore();
						std::cin >> id;
						int posConta = posContaById(id, contas, indContas, quantContas);
						if(posConta != -1)
						{
							Conta_Bancaria conta = contas[posConta];
							Banco banco = bancos[posBancoById(conta.idBanco, bancos, indBancos, quantBancos)];
							printContacomBanco(conta, banco);
						}
						else std::cout << "\nConta n�o Encontrada\n";
						break;
					}
					default:
					{
						std::cout << "Uma Op��o Inv�lida\n";
						break;
					}
				}
				break;
			}
			case 'B':
			{
				char opLst;
				std::cout << "Listar Itens\n";
				std::cout << "Quais itens voc� gostaria de listar?\n"
				<< "1 - Categorias\n"
				<< "2 - Bancos\n"
				<< "3 - Contas\n";

				std::cin.ignore();
				std::cin >> opLst;
				std::cout << "\nVoc� escolheu Listar ";
				switch(toupper(opLst))
        		{
					case '1':
					{	
						std::cout << "todas as Categorias\n";
						lExaustCategoriasById(categorias, indCategorias, quantCategorias);
						break;
					}
					case '2':
					{	
						std::cout << "todos os Bancos\n";
						lExaustBancosById(bancos, indBancos, quantBancos);
						break;
					}
					case '3':
					{	
						std::cout << "todas as Contas\n";
						lExaustContasByIdComBanco(contas, indContas, quantContas, bancos, indBancos, quantBancos);
						break;
					}
					default:
					{
						std::cout << "Uma Op��o Inv�lida\n";
						break;
					}
				}
				break;
			}
			case 'C':
			{
				char opIns;
				std::cout << "Inserir novos Itens\n";
				std::cout << "Que Tipo de Item voc� gostaria de Inserir?\n"
				<< "1 - Categoria\n"
				<< "2 - Banco\n"
				<< "3 - Conta\n";

				std::cin >> opIns;
				std::cout << "\nVoc� escolheu Inserir ";
				switch(toupper(opIns))
        		{
					case '1':
					{	
						std::cout << "uma Categoria";
						int newId = 1;
						// procura um Id que esteja Livre para inserir nova Categoria
						while(posCategoriaById(newId, categorias, indCategorias, quantCategorias) != -1) newId++;
						Categoria_Gasto categoria{newId, "", false};
						categoria = lerCategoria(categoria);
						if(inserirCategoria(categorias, quantCategorias, categoria, indCategorias) == 0)
						std::cout << "\nInser��o feita com Sucesso! \n";
						break;
					}
					case '2':
					{
						std::cout << "um Banco";
						int newId = 1;
						while(posBancoById(newId, bancos, indBancos, quantBancos) != -1) newId++;
						Banco banco{newId, "", false};
						banco = lerBanco(banco);
						inserirBanco(bancos, quantBancos, banco, indBancos);
						std::cout << "\nInser��o feita com Sucesso! \n";
						break;
					}
					case '3':
					{
						std::cout << "uma Conta";
						int newId = 1;
						while(posContaById(newId, contas, indContas, quantContas) != -1) newId++;
						
						int codBanco = 0;
						std::cout << "\nInsira o C�digo do Banco: ";
						std::cin >> codBanco;
						while(posBancoById(codBanco, bancos, indBancos, quantBancos) == -1)
						{
							std::cout << "\nC�digo Inv�lido, Insira Outro: ";
							std::cin >> codBanco;
						}
						Conta_Bancaria conta{newId, codBanco, "", 0, false};
						conta = lerConta(conta);

						printContacomBanco(conta, bancos[posBancoById(codBanco, bancos, indBancos, quantBancos)]);
						std::cout << "\nVoc� Confirma a Inser��o desta conta? \n (Insira [S] para confirmar)";
						char conf = 0;
						std::cin >> conf;
						if (toupper(conf) == 'S') 
						{
							if(inserirConta(contas, quantContas, conta, indContas) == 0)
							std::cout << "\nInser��o feita com Sucesso! \n";
							break;
						}
						else {
							std::cout << "\nInser��o Cancelada.\n";
							break;
						}
						
					}
					
					default:
					{
						std::cout << "Uma Op��o Inv�lida";
						break;
					}
				}
				
				break;
			}
			case 'D':
			{
				char opExc;
				std::cout << "Excluir Itens\n";
				std::cout << "Que Tipo de Item voc� gostaria de Excluir?\n"
				<< "1 - Categoria\n"
				<< "2 - Banco\n"
				<< "3 - Conta\n";
				std::cin >> opExc;
				std::cout << "\nVoc� escolheu Excluir ";
				switch(toupper(opExc))
        		{
					case '1':
					{	
						std::cout << "uma Categoria";
						std::cout << "\nInsira o ID da Categoria que deseja Excluir: ";
						int id;
						std::cin.ignore();
						std::cin >> id;
						excCategoriaById(id, categorias, indCategorias, quantCategorias);
						break;
					}
					case '2':
					{
						std::cout << "um Banco";
						std::cout << "\nInsira o ID do Banco que deseja Excluir: ";
						int id;
						std::cin.ignore();
						std::cin >> id;
						excBancoById(id, bancos, indBancos, quantBancos);
						break;
					}
					case '3':
					{	
						std::cout << "uma Conta";
						std::cout << "\nInsira o ID da Conta que deseja Excluir: ";
						int id;
						std::cin.ignore();
						std::cin >> id;
						while(posContaById(id, contas, indContas, quantContas) == -1)
						{
							std::cout << "\nC�digo Inv�lido, Insira Outro: ";
							std::cin >> id;
						}
						int posConta = posContaById(id, contas, indContas, quantContas);
						Banco banco = bancos[posBancoById(contas[posConta].idBanco, bancos, indBancos, quantBancos)];
						std::cout << "\nConta Encontrada\n";
						printContacomBanco(contas[posConta], banco);
						excContaById(id, contas, indContas, quantContas);

						break;
					}
				}
				break;
			}
			case 'E':
			{
				std::cout 	<< "Organizar Arquivos\n\n"
							<< "ATEN��O!!!\n"
							<< "Esta Opera��o ir� excluir permanentemente todos os registros Inativos\n"
							<< "de Categorias, Bancos, e Contas\n"
							<< "\nVoc� Confirma a Reorganiza��o dos Arquivos? \n (Insira [S] para confirmar)";
				char conf = 0;
				std::cin >> conf;
				if (toupper(conf) == 'S') 
				{
					organizarArquivoBancos(bancos, indBancos, quantBancos);
					organizarArquivoCategorias(categorias, indCategorias, quantCategorias);
					organizarArquivoContas(contas, indContas, quantContas);
					organizarArquivoBancos(bancos, indBancos, quantBancos);
					
					std::cout << "\nArquivos Reorganizados com Sucesso! \n";
					break;
				}
				else {
					std::cout << "\nReorganiza��o Cancelada.\n";
					break;
				}
				break;
			}
			case 'F':
			{
				std::cout << "Manipular Transa��es\n";
				std::cout << "O que voc� gostaria de Fazer? \n"
				<< "1 - Consultar uma Transa��o por seu ID\n"
				<< "2 - Consultar todas as Transa��es efetuadas dentro de um Per�odo\n"
				<< "3 - Consultar todas as Transa��es j� efetuadas\n"
				<< "4 - Efetuar uma Nova Transa��o\n"
				<< "5 - Estornar uma Transa��o\n"
				<< "\n";

				char opTran;
				std::cin.ignore();
				std::cin >> opTran;
				std::cout << "\nVoc� escolheu ";
				switch(toupper(opTran))
        		{
					case '1':
					{	
						std::cout << "Consultar uma Transa��o por seu ID\n";
						std::cout << "Insira o ID da Transacao que deseja procurar: ";
						int id;
						std::cin.ignore();
						std::cin >> id;
						int posTran = posTransacaoById(id, transacoes, indTransId, quantTransacoes);
						if (posTran!= -1)
						{
							std::cout << "\nTransa��o n�o encontrada.\n";
							Transacao transacao = transacoes[posTran];
							Categoria_Gasto categoria = categorias[posCategoriaById(transacao.idCategoria, categorias, indCategorias, quantCategorias)];
							Conta_Bancaria conta = contas[posContaById(transacao.idConta, contas, indContas, quantContas)];
							Banco banco = bancos[posBancoById(conta.idBanco, bancos, indBancos, quantBancos)];

							printTransacaoCompleta(transacao, categoria, conta, banco);
						}
						else std::cout << "\nTransa��o n�o encontrada.\n";
						break;
					}
					case '2':
					{
						std::cout << "Consultar todas as Transa��es efetuadas dentro de um Per�odo\n";
						std::cout << "\nInsira a Data de In�cio:\n";
						Data dA = lerData();

						std::cout << "\n\nInsira a Data Final:\n";
						Data dB = lerData();

						lExaustTransacoesPeriodo(transacoes, quantTransacoes, dA, dB);
				
						break;
					}
					case '3':
					{
						std::cout << "Consultar todas as Transa��es j� efetuadas\n";
						Data dA = transacoes[indTransData[0].pos].data;
						Data dB = transacoes[indTransData[quantTransacoes-1].pos].data;
						lExaustTransacoesPeriodo(transacoes, quantTransacoes, dA, dB);

						break;
					}
					case '4':
					{
						std::cout << "Efetuar uma Nova Transa��o\n";
						int newId = 1;
						while(posTransacaoById(newId, transacoes, indTransId, quantTransacoes) != -1) newId++;

						int idCat = 0;
						std::cout << "\n\nInsira o C�digo da Categoria: ";
						std::cin >> idCat;
						while(posCategoriaById(idCat, categorias, indCategorias, quantCategorias) == -1)
						{
							std::cout << "\nC�digo Inv�lido, Insira Outro: ";
							std::cin >> idCat;
						}

						int idConta = 0;
						std::cout << "\n\nInsira o C�digo da Conta: ";
						std::cin >> idConta;
						while(posContaById(idConta, contas, indContas, quantContas) == -1)
						{
							std::cout << "\nC�digo Inv�lido, Insira Outro: ";
							std::cin >> idConta;
						}


						Transacao transacao{newId, idCat, idConta, hoje, 0, 0, false};
						transacao = lerTransacao(transacao);
						inserirTransacao(transacoes, quantTransacoes, transacao, indTransId, indTransData);
						std::cout << "\nTransa��o Efetuada com Sucesso! \n";
						
						double saldo = transacao.valor;
						if(transacao.tipo == 'C') saldo *= -1;

						contas[posContaById(idConta, contas, indContas, quantContas)].saldo = saldo;

						break;
					}
					case '5':
					{
						std::cout << "Estornar uma Transa��o\n";
						std::cout << "\nInsira o ID da Transa��o que deseja Excluir: ";
						int id;
						std::cin.ignore();
						std::cin >> id;
						while(posTransacaoById(id, transacoes, indTransId, quantTransacoes) == -1)
						{
							std::cout << "\nC�digo Inv�lido, Insira Outro: ";
							std::cin >> id;
						}
						Transacao transacao = transacoes[posTransacaoById(id, transacoes, indTransId, quantTransacoes)];
						
						std::cout << "\nTransacao Encontrada\n";
						
						Categoria_Gasto categoria = categorias[posCategoriaById(transacao.idCategoria, categorias, indCategorias, quantCategorias)];
						Conta_Bancaria conta = contas[posContaById(transacao.idConta, contas, indContas, quantContas)];
						Banco banco = bancos[posBancoById(conta.idBanco, bancos, indBancos, quantBancos)];

						printTransacaoCompleta(transacao, categoria, conta, banco);
						excTransacaoById(id, transacoes, indTransId, quantTransacoes);
						break;
					}
				}
				break;
			}
            case 'G':
			{
				std::cout 	<< "Listar Saldos de todas as contas\n\n";
				lstSaldos(contas, indContas, quantContas, bancos, indBancos, quantBancos);
				break;
			}
			case 'X':
			{
				std::cout << "Sair do Programa. Encerrando...\n\n";
				run = false;
				return 0;
				break;
			}
			default:
            {
                std::cout << "Uma Op��o Inv�lida";
                break;
            }
		}

	voltaMenu();
	}

	bool teste = false;
	if(teste)
	{
	


	if (testConta)
	{
		bancos[0] = Banco{8, "SantoAndr�", false};
		bancos[1] = Banco{6, "Banco do Paraguai", true};
		bancos[2] = Banco{2, "Recipiente", false};
		bancos[3] = Banco{3, "VestidoBank", false};

		quantBancos = 4;

		criarIndiceBancos(bancos, indBancos, quantBancos);
		organizarArquivoBancos(bancos, indBancos, quantBancos);

		contas[0] = Conta_Bancaria {5, 2, "Poupan�a do Seu Z�", 20000.0, false};
		contas[1] = Conta_Bancaria {3, 2, "Laranja", 300000.0, true};
		contas[2] = Conta_Bancaria {6, 1, "Bugada", 0.0, false};
		quantContas = 3;

		Conta_Bancaria c1 = Conta_Bancaria{1, 8, "Conta do Marc�o", 1800.0, false};
		Conta_Bancaria c2 = Conta_Bancaria{2, 2, "Conta do Jo�o", 1800.0, false};
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
			char tipo; //D�bito = D ; Cr�dito = C
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
		criarIndiceTransacoesData(transacoes, indTransData, quantTransacoes);
		organizarArquivoTransacoesData(transacoes, indTransData, quantTransacoes);
		lExaustTransacoesPeriodo(transacoes, quantTransacoes, Data{24,9,2023}, Data{24,10,2023});


	}

	if(testBanco)
	{
		bancos[0] = Banco{8, "SantoAndr�", false};
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

		std::cout << "\nIndice Organizado Apos Inser��o\n";
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
		pessoas[2] = Pessoa{2, "Jo�o", false};
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


		std::cout   << "\n\n--------------------------------------------------------\n\nTestando Exclus�o\n"
					<< "\nTentando Excluir id 8 (existe)\n";
		excPessoaById(8, pessoas, indPessoas, quantPessoas);
		std::cout   << "\nTentando Excluir id 8 de novo\n";
		excPessoaById(8, pessoas, indPessoas, quantPessoas);

	}

	if(testCategoria)
	{

		categorias[0] = Categoria_Gasto{1, "Alimenta��o", false};
		categorias[1] = Categoria_Gasto{2, "Lazer", false};
		categorias[2] = Categoria_Gasto{3, "Sa�de", false};
		categorias[3] = Categoria_Gasto{4, "Emerg�ncias", false};
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


		std::cout   << "\n\n--------------------------------------------------------\n\nTestando Exclus�o de Categoria\n"
					<< "\nTentando Excluir id 8 (existe)\n";
		excCategoriaById(8, categorias, indCategorias, quantCategorias);
		std::cout   << "\nTentando Excluir id 8 de novo\n";
		excCategoriaById(8, categorias, indCategorias, quantCategorias);
	}

	return 0;
	}
return 0;
}