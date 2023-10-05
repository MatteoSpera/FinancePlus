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

struct Data {
	int dia;
	int mes;
	int ano;
};
void printData(Data data)
{
	cout   
	<< data.dia / 10 << data.dia %10 << "/" // caso o dia ou mes sejam menores que 10, automaticamente concatena 0 logo antes (ex 01, 02, 03...)
	<< data.mes / 10 << data.mes %10 << "/"
	<< data.ano;

}
Data lerData()
{
	Data data;
	int n;

	cout << "\nInsira Dia: ";
	cin >> n;
	cin.ignore();
	data.dia = n;

	cout << "Insira Mês: ";
	cin >> n;
	cin.ignore();
	data.mes = n;

	cout << "Insira Ano: ";
	cin >> n;
	cin.ignore();
	data.ano = n;

	return data;
}

string dataToString(Data data)
{
	string txt = "";
	txt += to_string(data.dia / 10) + to_string(data.dia % 10) + "/"; //mantém a data com dois dígitos mesmo sendo menor que 10
	txt += to_string(data.mes / 10) + to_string(data.mes % 10) + "/";
	txt += to_string(data.ano);

	return txt;
}
int comparaDatas(Data dA, Data dB) //se a > b, retorna 1, se a < b, retorna -1, se a == b, retorna 0. Se algo der muito errado(hipotético) retorna -2
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
	cout << "\nERRO NA FUNÇÃO COMPARADATAS, O PROGRAMADOR É MUITO BURRO\n";
	return -2;
}

struct IndiceId
{
	int id;
	int pos;
};

std::string formataString(string &s, int tamanho) //para deixar a string no padrao da tabela
{
	if(s.size() > tamanho) 
	{
		s.resize(tamanho-1, ' ');
		s.append(".");
	}
	else s.resize(tamanho, ' ');
	return s;
}


struct Categoria
{
	// Categoria do Gasto realizado, como entretenimento, comida, locomoção, médico, etc
	int id;
	char descricao[40]; 
	bool excluido;
};
void printCategoria(Categoria categoria)
{
	cout << endl << categoria.id << ", " << categoria.descricao << endl << endl; 
}
void lExaustCategoriasById(Categoria *categorias, IndiceId *indice, int quant)
{
	cout
	<< "\n |------------------------------|"
	<< "\n |  Categorias - ID crescente   |"
	<< "\n |------------------------------|"
	<< "\n |  ID   | Descrição | Status   |"
	<< "\n |------------------------------|\n";

	for(int i = 0; i < quant; i++) 
	{
		Categoria categoria = categorias[indice[i].pos];
		
		string status;
		if (!categoria.excluido) status = "Ativa   ";
		else status = "Excluída";

		string descricao = categoria.descricao;
		int nDesc = 8; //tamanho maximo da descrição a ser exibida
		formataString(descricao, nDesc);
		cout   << " | "<< categoria.id <<"\t | "<< descricao <<"  | "<< status <<" |\n";
	}

	cout  
	<< " |------------------------------|\n"
	<< " | " << quant << " Registros                  |\n"
	<< " |------------------------------|\n";
	
}
void criarIndiceCategorias(Categoria *categorias, IndiceId *indice, int quant) 
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
void organizarArquivoCategorias(Categoria *categorias, IndiceId *novoIndice, int &quant)
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
Categoria lerCategoria(Categoria categoria)
{
	cout << "\nInsira a Descrição da Categoria: ";
	cin.ignore();
	gets(categoria.descricao);

	return categoria;

}
int inserirCategoria(Categoria *categorias, int &quant, Categoria add, IndiceId *indice)
{
	// retorna 0 se der certo, 1 se o id informado já estiver registrado
	const int q = quant;
	int cursor = 0;
	for (;add.id > indice[cursor].id && cursor < q; cursor++);

	if((add.id == indice[cursor].id) && (categorias[indice[cursor].pos].excluido == false))
	{
		cout << "Operação inválida: Já existe um registro com este código";
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
int bscCategoriaById(int id, Categoria *categorias, IndiceId *indice, int quant)
//retorna 0 se achar, 1 se não, 2 se achar mas estiver excluído
{
	int i = 0, f = quant;
	int cursor = (i + f) / 2;

	for 
		(;
		f >= i; 
		cursor = (i + f) / 2)
	{
		if (indice[cursor].id == id)
		{ 
			
			while (indice[cursor].id == id) 
			{
				if (!categorias[indice[cursor].pos].excluido)
				{
					Categoria categoria = categorias[indice[cursor].pos];
					cout << "\nCategoria Encontrada\n";
					printCategoria(categoria);
					return 0;
				}
				cursor++;
			}
			
			cout << "\nCategoria Não Encontrada\n\n";
			return 2;
		}
		else if(indice[cursor].id > id) f = cursor-1;
		else if(indice[cursor].id < id) i = cursor+1;

	}
	cout << "\nCategoria Não Encontrada.\n\n";
	return 1;

}
int posCategoriaById(int id, Categoria *categorias, IndiceId *indice, int quant) //retorna a posição da categoria na lista, retorna -1 se não achar.
{
	int i = 0, f = quant;
	int cursor = (i + f) / 2;

	for 
		(;
		f >= i; 
		cursor = (i + f) / 2)
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
int excCategoriaById(int id, Categoria *categorias, IndiceId *indice, int &quant)
{
	//retorna 0 se achar e excluir, 1 se não achar, 2 se achar mas estiver excluído, -1 se a exclusão for cancelada

	int i = 0, f = quant;
	int cursor = (i + f) / 2;

	for 
	(;f >= i; 
	cursor = (i + f) / 2)
	{
		if (indice[cursor].id == id)
		{
			while (indice[cursor].id == id) 
			{
				if (!categorias[indice[cursor].pos].excluido)
				{
					int pos = indice[cursor].pos;
					Categoria categoria = categorias[pos];

					cout << "\nCategoria Encontrada\n";
					printCategoria(categoria);
					cout << "\nVocê Confirma a Exclusão deste Registro? \n (Insira [S] para confirmar)";

					char conf = 0;
					cin >> conf;
					if (toupper(conf) == 'S') 
					{
						categorias[pos].excluido = true;
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
			cout << "\nCategoria Não Encontrada.\n"; 
			return 2;
		}
		else if(indice[cursor].id > id) f = cursor-1;
		else if(indice[cursor].id < id) i = cursor+1;
	}

	cout << "\nCategoria Não Encontrada.\n";
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
	cout << endl << banco.id << ", " << banco.descricao << endl << endl; 
}
void lExaustBancosById(Banco *bancos, IndiceId *indice, int quant)
{
	cout   	
	<< "\n |------------------------------------|"
	<< "\n |        Bancos - ID crescente       |"
	<< "\n |------------------------------------|"
	<< "\n |  ID   |    Descrição    | Status   |"
	<< "\n |------------------------------------|\n";

	for(int i = 0; i < quant; i++) 
	{
		Banco banco = bancos[indice[i].pos];
		
		string status;
		if (!banco.excluido) status = "Ativo   ";
		if (banco.excluido) status = "Quebrado";

		string descricao = banco.descricao;
		int nDesc = 14;
		formataString(descricao, nDesc);
		cout << " | "<< banco.id <<"\t | "<< descricao <<"  | "<< status <<" |\n";
	}
	cout
	<< " |------------------------------------|\n"
	<< " |            " << quant << " Registros             |\n"
	<< " |------------------------------------|\n";
	
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
	cout << "\nInsira a Descrição do Banco: ";
	cin.ignore();
	gets(banco.descricao);

	return banco;
}
int inserirBanco(Banco *bancos, int &quant, Banco add, IndiceId *indice)
{
	// retorna 0 se der certo, 1 se o id informado já estiver registrado e ativo 
	const int q = quant;
	int cursor = 0;
	for (;add.id > indice[cursor].id && cursor < q; cursor++);

	if((add.id == indice[cursor].id) && (bancos[indice[cursor].pos].excluido == false))
	{
		cout << "Operação inválida: Já existe um registro com este código";
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
//retorna 0 se achar, 1 se não, 2 se achar mas estiver excluído
{
	int i = 0, f = quant;
	int cursor = (i + f) / 2;

	for 
		(;
		f >= i; 
		cursor = (i + f) / 2)
	{
		if (indice[cursor].id == id)
		{ 
			
			
			while (indice[cursor].id == id) 
			{
				if (!bancos[indice[cursor].pos].excluido)
				{
					Banco banco = bancos[indice[cursor].pos];
					cout 	
					<< "\nBanco Encontrado\n"
					<< banco.id << ", " << banco.descricao << endl;
					return 0;
				}
				cursor++;
			}
			
			cout << "\nBanco Não Encontrado. (Apenas registros Excluídos)\n\n";
			return 2;
		}
		else if(indice[cursor].id > id) f = cursor-1;
		else if(indice[cursor].id < id) i = cursor+1;
	}
	cout << "\nBanco Não Encontrado.\n\n";
	return 1;

}
int posBancoById(int id, Banco *bancos, IndiceId *indice, int quant) //retorna a posição do banco na lista, retorna -1 se não achar.
{
	int i = 0, f = quant;
	int cursor = (i + f) / 2;

	for 
		(;
		f >= i; 
		cursor = (i + f) / 2)
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
	//retorna 0 se achar e excluir, 1 se não achar, 2 se achar mas ja estiver excluído, -1 se a exclusão for cancelada
	int i = 0, f = quant;
	int cursor = (i + f) / 2; 

	for 
	(;f >= i; 
	cursor = (i + f) / 2)
	{
		if (indice[cursor].id == id)
		{
			while (indice[cursor].id == id) 
			{
				if (!bancos[indice[cursor].pos].excluido)
				{
					int pos = indice[cursor].pos;
					Banco banco = bancos[pos];

					cout << "\nBanco Encontrado\n"
						<< banco.id << ", " << banco.descricao << endl;
					cout << "\nVocê Confirma a Exclusão deste Registro? \n (Insira [S] para confirmar)";

					char conf = 0;
					cin >> conf;
					if (toupper(conf) == 'S') 
					{
						bancos[pos].excluido = true;
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
			cout << "\nBanco Não Encontrado.\n"; 
			return 2;
		}
		else if(indice[cursor].id > id) f = cursor-1;
		else if(indice[cursor].id < id) i = cursor+1;
	}
	cout << "\nBanco Não Encontrado.\n";
	return 1;
}

struct Conta
{
	int id;
	int idBanco;
	char descricao[40];
	double saldo;
	bool excluido;
};

void printContacomBanco(Conta conta, Banco banco)
{
	string status;
	if (conta.excluido) status = "Cancelada";
	else status = "Ativa";

	cout
	<< "\n  ID: " << conta.id 
	<< "\n  Banco: " << banco.descricao
	<< "\n  Descrição: " << conta.descricao
	<< "\n  Saldo: " << conta.saldo
	<< "\n  Status: " << status
	<< endl << endl; 
}
void lExaustContasByIdComBanco(Conta *contas, IndiceId *indice, int quant, Banco *bancos, IndiceId *indBancos, int quantBancos)
{
	cout   		
	<< "\n -----------------------------------"
	<< "\n        Contas - ID crescente       "
	<< "\n -----------------------------------";
				
	for(int i = 0; i < quant; i++) 
	{
		Conta conta = contas[indice[i].pos];
		Banco banco;

		int posBanco = posBancoById(conta.idBanco, bancos, indBancos, quantBancos);
		if(posBanco >= 0) banco = bancos[posBanco];
		else (banco = Banco{0, "???", false});
			
		printContacomBanco(conta, banco);
	}
	cout
	<< endl 
	<< " -----------------------------------\n"
	<< "             " << quant << " Registros            \n"
	<< " -----------------------------------\n";
	
}
void lstSaldos(Conta *contas, IndiceId *indice, int quant, Banco *bancos, IndiceId *indBancos, int quantBancos)
{
	cout   
	<< "\n |-------------------------------------------|"
	<< "\n |               Listando Saldos             |"
	<< "\n |-------------------------------------------|"
	<< "\n | Conta |        Banco       |     Saldo    |"
	<< "\n |-------------------------------------------|\n";

	int cont = 0;
	double saldoTotal = 0;
	for(int i = 0; i < quant; i++) 
	{
		Conta conta = contas[indice[i].pos];
		if( !conta.excluido)
		{
			cout << fixed;
			cout << setprecision(2);

			Banco banco;
			int posBanco = posBancoById(conta.idBanco, bancos, indBancos, quantBancos);
			if(posBanco >= 0) banco = bancos[posBanco];
			else (banco = Banco{0, "???", false});

			string descBanco = banco.descricao;
			int nDescBanco = 18;
			formataString(descBanco,nDescBanco);
			
			int contZero;
			int digitos;
		
			string id = "";
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
			id += (to_string(conta.id));

			ostringstream streamSaldo; 
			streamSaldo << fixed << setprecision(2) << conta.saldo;

			string saldo = "";
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

			cout   << " |  " << id << "  | " << descBanco <<  " | " << saldo << "  |\n";

			saldoTotal += conta.saldo;
			cont++;
		}
	}

	cout  	<< " |-------------------------------------------|\n"
			<< " |                " << cont << " Registros                |\n"
			<< " |-------------------------------------------|\n"
			<< " | Saldo Final: " << saldoTotal << endl
			<< " |-------------------------------------------|\n";

}
void criarIndiceContas(Conta *contas, IndiceId *indice, int quant) 
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
void organizarArquivoContas(Conta *contas, IndiceId *novoIndice, int &quant) 
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
Conta lerConta(Conta conta)
{
	cout << "\nInsira a Descrição da Conta: ";
	cin.ignore();
	gets(conta.descricao);
	cout << "\nInsira o Saldo da Conta: ";
	cin >> conta.saldo;

	return conta;
}
int inserirConta(Conta *contas, int &quant, Conta add, IndiceId *indice)
{
	// retorna 0 se der certo, 1 se o id informado já estiver registrado
	const int q = quant;
	int cursor = 0;
	for (;add.id > indice[cursor].id && cursor < q; cursor++);

	if((add.id == indice[cursor].id) && (contas[indice[cursor].pos].excluido == false))
	{
		cout << "Operação inválida: Já existe um registro com este código";
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

int posContaById(int id, Conta *contas, IndiceId *indice, int quant) //retorna a posição da conta na lista, retorna -1 se não achar.
{
	int i = 0, f = quant;
	int cursor = (i + f) / 2;

	for 
	(;f >= i; 
	cursor = (i + f) / 2)
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
int excContaById(int id, Conta *contas, IndiceId *indice, int &quant)
{
	//retorna 0 se achar e excluir, 1 se não achar, 2 se achar mas estiver excluído, -1 se a exclusão for cancelada

	int i = 0, f = quant;
	int cursor = (i + f) / 2; // calcula a media para ficar no meio, mas para inicializar poderia ser quant/2 tbm

	for 
	(;f >= i; 
	cursor = (i + f) / 2)
	{
		if (indice[cursor].id == id)
		{
			while (indice[cursor].id == id) 
			{
				if (!contas[indice[cursor].pos].excluido)
				{
					int pos = indice[cursor].pos;
					Conta conta = contas[pos];

					cout << "\nVocê Confirma a Exclusão deste Registro? \n (Insira [S] para confirmar)";

					char conf = 0;
					cin >> conf;
					if (toupper(conf) == 'S') 
					{
						contas[pos].excluido = true;
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
			cout << "\nConta Não Encontrada.\n"; 
			return 2;
		}
		else if(indice[cursor].id > id) f = cursor-1;
		else if(indice[cursor].id < id) i = cursor+1;

	}
	cout << "\nConta Não Encontrada.\n";
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
struct IndByData
{
	Data data;
	int pos;
};

void printTransacaoCompleta(Transacao transacao, Categoria categoria, Conta conta, Banco banco)
{
	cout << fixed;
	cout << setprecision(2);

	char sinal;
	if(transacao.tipo == 'D') sinal = '-';
	else if(transacao.tipo == 'C') sinal = '+';
	else 
	{
		cout << "\n A transação tem um tipo Inválido\n";
		return;
	}	

	string status;
	if (transacao.excluido) status = "Estornada";
	else status = "Efetivada";

	cout 
	<< "\n ID: " << transacao.id 
	<< "\n Categoria: " << categoria.id << " - " << categoria.descricao
	<< "\n Conta " << conta.id << " no Banco " << banco.descricao
	<< "\n Data: " << dataToString(transacao.data)
	<< "\n Valor: R$ " << sinal << transacao.valor
	<< "\n Status: " << status << endl << endl;
	
}


void criarIndiceTransacoesData(Transacao *transacoes, IndByData *indice, int quant) 
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
			IndByData aux = indice[i];
			indice[i] = indice[j];
			indice[j] = aux;
		}
	}}
	
}
void organizarArquivoTransacoesData(Transacao *transacoes, IndByData *novoIndice, int &quant) 
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
	IndByData indice[quant];
	criarIndiceTransacoesData(transacoes, indice, quant);
	organizarArquivoTransacoesData(transacoes, indice, quant);
	float saldoTotal = 0;

	cout   
	<< "\n |-------------------------------------------|"
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
			cout << fixed;
			cout << setprecision(2);

			int contZero;
			int digitos;

			string id = "";
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
			id += to_string(transacao.id);
			
			string conta = "";
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
			conta += (to_string(transacao.idConta));
			
			string data = dataToString(transacao.data);


			ostringstream streamValor; //ostringstream = output string stream
			streamValor << fixed << setprecision(2) << transacao.valor;
			string valor = "";
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

			cout   << " | " << id << " |  " << conta << "  | " << data  << " | " << valor << " |\n";

			if(transacao.tipo == 'D') saldoTotal -= transacao.valor;
			else if(transacao.tipo == 'C') saldoTotal += transacao.valor;
			cont++;
		}
	}
	cout  
	<< " |-------------------------------------------|\n"
	<< " |                " << cont << " Registros                |\n"
	<< " |-------------------------------------------|\n"
	<< " | Saldo Final: " << saldoTotal << endl
	<< " |-------------------------------------------|\n";

	
}
Transacao lerTransacao(Transacao transacao)
{
	cout << "\nInsira o Valor da Transação: ";
	cin.ignore();
	cin >> transacao.valor;
	while(transacao.valor <= 0)
	{
		cout << "\nO valor da Transação deve ser positivo ";
		cin >> transacao.valor;
	}

	cout << "\n\nInsira o Tipo da Transação - Crédito [C] ou Débito [D] ";
	
	char tipo = 0;
	cin >> tipo;
	tipo = toupper(tipo);
	
	while(tipo != 'C' && tipo != 'D')
	{
		cout << "\nInsira um Tipo válido [C/D] ";
		cin >> tipo;
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

int inserirTransacao(Transacao *transacoes, int &quant, Transacao add, IndiceId *indId, IndByData *indData)
{
	// retorna 0 se der certo, 1 se o id informado já estiver registrado
	const int q = quant;
	int cursor = 0;
	for (;add.id > indId[cursor].id && cursor < q; cursor++);

	if((add.id == indId[cursor].id) && (transacoes[indId[cursor].pos].excluido == false))
	{
		cout << "Operação inválida: Já existe um registro com este código";
		return 1;
	}

	transacoes[q] = add; 
	IndiceId indcId = IndiceId{add.id, q};

	int reg = q;
	for (;reg > cursor; reg--) indId[reg] = indId[reg-1];
	indId[cursor] = indcId;
	
	cursor = 0;
	for (;comparaDatas(add.data, indData[cursor].data) > 0 && cursor < q; cursor++);
	IndByData indcData = IndByData{add.data, q};
	reg = q;
	for (;reg > cursor; reg--) indData[reg] =  indData[reg-1];
	indData[cursor] = indcData; 

	quant++;
	return 0;
}

int posTransacaoById(int id, Transacao *transacoes, IndiceId *indice, int quant) //retorna a posição da transação na lista, retorna -1 se não achar.
{
	int i = 0, f = quant;
	int cursor = (i + f) / 2;

	for 
	(;f >= i; 
	cursor = (i + f) / 2)
	{
		if (indice[cursor].id == id)
		{ 
			while (indice[cursor].id == id) 
			{
				if (!transacoes[indice[cursor].pos].excluido) 
				return indice[cursor].pos;
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
	//retorna 0 se achar e excluir, 1 se não achar, 2 se achar mas estiver excluído, -1 se a exclusão for cancelada

	int i = 0, f = quant;
	int cursor = (i + f) / 2; 

	for 
	(;f >= i; 
	cursor = (i + f) / 2)
	{
		if (indice[cursor].id == id)
		{
			while (indice[cursor].id == id) 
			{
				if (!transacoes[indice[cursor].pos].excluido)
				{
					int pos = indice[cursor].pos;
					Transacao transacao = transacoes[pos];
					
					cout << "\nVocê Confirma a Exclusão deste Registro? \n (Insira [S] para confirmar)";

					char conf = 0;
					cin >> conf;
					if (toupper(conf) == 'S') 
					{
						transacoes[pos].excluido = true;
						cout << "\nTransação Estornada com sucesso.\n";
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
			cout << "\nTransação Não Encontrada.\n"; 
			return 2;
		}
		else if(indice[cursor].id > id) f = cursor-1;
		else if(indice[cursor].id < id) i = cursor+1;
		
	}
	cout << "\nTransação Não Encontrada.\n";
	return 1;
}

void voltaMenu()
{
	cout << "\n\nAperte Qualquer Tecla para Voltar ao Menu\n";
	getch();
}


int main()
{
	setlocale(LC_ALL, "");

	const int MAX = 60;

	Categoria categorias[MAX];
	IndiceId indCategorias[MAX];
	int quantCategorias = 0;
	criarIndiceCategorias(categorias, indCategorias, quantCategorias);

	Banco bancos[MAX];
	IndiceId indBancos[MAX];
	int quantBancos = 0;
	criarIndiceBancos(bancos, indBancos, quantBancos);

	Conta contas[MAX];
	IndiceId indContas[MAX];
	int quantContas = 0;
	criarIndiceContas(contas, indContas, quantContas);

	const int MAXTRANSACOES = MAX*10;
	Transacao transacoes[MAXTRANSACOES];
	IndByData indTransData[MAXTRANSACOES];
	IndiceId indTransId[MAXTRANSACOES];
	int quantTransacoes = 0;
	criarIndiceTransacoesById(transacoes, indTransId, quantTransacoes);
	criarIndiceTransacoesData(transacoes, indTransData, quantTransacoes);
	organizarArquivoTransacoesById(transacoes, indTransId, quantTransacoes);
	organizarArquivoTransacoesData(transacoes, indTransData, quantTransacoes);

	bool criarInicial = true;
	if (criarInicial)
	{
		categorias[0] = Categoria{1, "Alimentação", false};
		categorias[1] = Categoria{2, "Lazer", false};
		categorias[2] = Categoria{3, "Saúde", false};
		categorias[3] = Categoria{5, "Caridade", false};
		categorias[4] = Categoria{6, "CriptoMoedas", false};
		categorias[5] = Categoria{7, "Renda Passiva", false};
		categorias[6] = Categoria{8, "Multas", true};
		categorias[7] = Categoria{9, "Bicos", true};
		quantCategorias = 8;
		criarIndiceCategorias(categorias, indCategorias, quantCategorias);


		bancos[0] = Banco{1, "SantoAndré", false};
		bancos[1] = Banco{2, "Brolesco", false};
		bancos[2] = Banco{3, "Eitaú", false};
		bancos[3] = Banco{4, "VestidoBank", false};
		bancos[4] = Banco{5, "Sijoob", false};
		bancos[5] = Banco{6, "Xinance", true};
		bancos[6] = Banco{7, "Jamerindus", true};
		quantBancos = 7;
		criarIndiceBancos(bancos, indBancos, quantBancos);


		contas[0] = Conta{1, 5, "Poupança do Seu Zé", 20221.9, false};
		contas[1] = Conta{2, 4, "Conta do Marcos", 1200.3, false};
		contas[2] = Conta{3, 3, "Conta do João", -500.04, false};
		contas[3] = Conta{4, 2, "Instituto Brasileiro de Nomes Longos", 139202.82, false};
		contas[4] = Conta{5, 1, "ONG Patas Fofas", 2601.41, false};
		contas[6] = Conta{6, 6, "Conta Laranja", 400, true};
		contas[5] = Conta{7, 5, "Conta Fantasma", 0, true};
		contas[7] = Conta{6, 6, "Carteira Cripto do Túlio", -20000, false};
		quantContas = 8;
		criarIndiceContas(contas, indContas, quantContas);
		

		transacoes[0] = Transacao{1, 1, 1, Data{25, 9, 2023}, 30.2, 'D', false}; //Seu zé almoçou
		transacoes[1] = Transacao{2, 2, 1, Data{25, 9, 2023}, 100, 'D', false};	//Depois foi no cinema com dona Jurema
		transacoes[2] = Transacao{3, 7, 1, Data{27, 9, 2023}, 352.1, 'C', false}; //Recebeu Juros da poupança

		transacoes[3] = Transacao{4, 3, 2, Data{30, 9, 2023}, 500.00, 'D', false}; //Marcos Foi ao dentista
		transacoes[4] = Transacao{5, 5, 2, Data{2, 10, 2023}, 1000.00, 'D', false}; // doou para a ong de animais
		transacoes[5] = Transacao{6, 5, 5, Data{2, 10, 2023}, 1000.00, 'C', false};	// ong de animais recebeu
		
		transacoes[6] = Transacao{7, 6, 6, Data{5, 10, 2023}, 20000, 'D', false}; 
		// Túlio gastou a poupança da vida toda em dogecoin logo antes da corretora xinance quebrar
		quantTransacoes = 7;
		
		criarIndiceTransacoesById(transacoes, indTransId, quantTransacoes);
		criarIndiceTransacoesData(transacoes, indTransData, quantTransacoes);
		organizarArquivoTransacoesById(transacoes, indTransId, quantTransacoes);
		organizarArquivoTransacoesData(transacoes, indTransData, quantTransacoes);
	}
	
	//pega a data do sistema e passa para a struct data
	time_t now = time(0);
	tm *agora = localtime(&now);
	Data hoje = Data{agora->tm_mday, 1+agora->tm_mon, 1900+agora->tm_year};

	bool run = true;
	bool constClear = false; // define se a tela será limpa ao longo do programa
	char op;

	while(run) {

	if (constClear) system("cls");

	cout   		
	<< "\n\n"
	<< "|------------------------------------------|\n"
	<< "|                " << dataToString(hoje) << "                |\n"
	<< "|------------------------------------------|\n"
	<< "|               Finance Plus               |\n"
	<< "|------------------------------------------|\n"
	<< "| DIGITE A OPERAÇÃO QUE DESEJA FAZER:      |\n"
	<< "| A - Consultar Itens                      |\n"
	<< "| B - Listar Itens                         |\n"
	<< "| C - Inserir novos Itens                  |\n"
	<< "| D - Excluir Itens                        |\n"
	<< "| E - Reorganizar Arquivos                 |\n"
	<< "| F - Manipular Transações                 |\n"
	<< "| G - Listar Saldos de todas as contas     |\n"
	<< "| X - Sair do programa                     |\n"
	<< "|------------------------------------------|\n"
	<< "\n";

	cin >> op;

	if (constClear) system("cls");

	cout << "\nVocê escolheu ";

	switch(toupper(op)) {	
	case '1':
	case 'A':
	{
		char opCon;
		cout 
		<< "Consultar Itens\n"
		<< "Que Tipo de Item você gostaria de Consultar?\n"
		<< "1 - Categoria\n"
		<< "2 - Banco\n"
		<< "3 - Conta\n";

		cin.ignore();
		cin >> opCon;
		if (constClear) system("cls");
		cout << "\nVocê escolheu Consultar ";

		switch(toupper(opCon)){
		case '1':
		{	
			cout << "uma Categoria\n";
			cout << "Insira o ID da Categoria que deseja procurar: ";
			int id;
			cin.ignore();
			cin >> id;
			bscCategoriaById(id, categorias, indCategorias, quantCategorias);
			break;
		}
		case '2':
		{	
			cout << "um Banco\n";
			cout << "Insira o ID do Banco que deseja procurar: ";
			int id;
			cin.ignore();
			cin >> id;
			bscBancoById(id, bancos, indBancos, quantBancos);
			break;
		}
		case '3':
		{	
			cout << "uma Conta\n";
			cout << "Insira o ID da Conta que deseja procurar: ";
			int id;
			cin.ignore();
			cin >> id;
			int posConta = posContaById(id, contas, indContas, quantContas);
			if(posConta != -1)
			{
				Conta conta = contas[posConta];
				Banco banco = bancos[posBancoById(conta.idBanco, bancos, indBancos, quantBancos)];
				printContacomBanco(conta, banco);
			}
			else cout << "\nConta não Encontrada\n";
			break;
		}
		default:
		cout << "Uma Opção Inválida\n";
		break;}

		break;
	}
	case '2':
	case 'B':
	{
		char opLst;
		cout 
		<< "Listar Itens\n"
		<< "Quais itens você gostaria de listar?\n"
		<< "1 - Categorias\n"
		<< "2 - Bancos\n"
		<< "3 - Contas\n";

		cin.ignore();
		cin >> opLst;
		cout << "\nVocê escolheu Listar ";

		switch(toupper(opLst)){
		case '1':
		{	
			cout << "todas as Categorias\n";
			lExaustCategoriasById(categorias, indCategorias, quantCategorias);
			break;
		}
		case '2':
		{	
			cout << "todos os Bancos\n";
			lExaustBancosById(bancos, indBancos, quantBancos);
			break;
		}
		case '3':
		{	
			cout << "todas as Contas\n";
			lExaustContasByIdComBanco(contas, indContas, quantContas, bancos, indBancos, quantBancos);
			break;
		}
		default:
		cout << "Uma Opção Inválida\n";
		break;
		}
		break;
	}
	case '3':
	case 'C':
	{
		char opIns;
		cout 
		<< "Inserir novos Itens\n"
		<< "Que Tipo de Item você gostaria de Inserir?\n"
		<< "1 - Categoria\n"
		<< "2 - Banco\n"
		<< "3 - Conta\n";

		cin >> opIns;
		cout << "\nVocê escolheu Inserir ";

		switch(toupper(opIns)){
		case '1':
		{	
			cout << "uma Categoria";
			int newId = 1;
			// procura um Id que esteja Livre para inserir nova Categoria
			while(posCategoriaById(newId, categorias, indCategorias, quantCategorias) != -1) newId++;
			Categoria categoria{newId, "", false};
			categoria = lerCategoria(categoria);
			if(inserirCategoria(categorias, quantCategorias, categoria, indCategorias) == 0)
			cout << "\nInserção feita com Sucesso! \n";
			break;
		}
		case '2':
		{
			cout << "um Banco";
			int newId = 1;
			while(posBancoById(newId, bancos, indBancos, quantBancos) != -1) newId++;
			Banco banco{newId, "", false};
			banco = lerBanco(banco);
			inserirBanco(bancos, quantBancos, banco, indBancos);
			cout << "\nInserção feita com Sucesso! \n";
			break;
		}
		case '3':
		{
			cout << "uma Conta";
			int newId = 1;
			while(posContaById(newId, contas, indContas, quantContas) != -1) newId++;
			
			int codBanco = 0;
			cout << "\nInsira o Código do Banco: ";
			cin >> codBanco;
			while(posBancoById(codBanco, bancos, indBancos, quantBancos) == -1)
			{
				cout << "\nCódigo Inválido, Insira Outro: ";
				cin >> codBanco;
			}
			Conta conta{newId, codBanco, "", 0, false};
			conta = lerConta(conta);

			printContacomBanco(conta, bancos[posBancoById(codBanco, bancos, indBancos, quantBancos)]);
			cout << "\nVocê Confirma a Inserção desta conta? \n (Insira [S] para confirmar)";
			char conf = 0;
			cin >> conf;
			if (toupper(conf) == 'S') 
			{
				if(inserirConta(contas, quantContas, conta, indContas) == 0)
				cout << "\nInserção feita com Sucesso! \n";
				break;
			}
			else {
				cout << "\nInserção Cancelada.\n";
				break;
			}
			break;
		}
		default:
		cout << "Uma Opção Inválida";
		break;
		}
		
		break;
	}
	case '4':
	case 'D':
	{
		char opExc;
		cout 
		<< "Excluir Itens\n"
		<< "Que Tipo de Item você gostaria de Excluir?\n"
		<< "1 - Categoria\n"
		<< "2 - Banco\n"
		<< "3 - Conta\n";
		cin >> opExc;
		cout << "\nVocê escolheu Excluir ";

		switch(toupper(opExc)){
		case '1':
		{	
			cout << "uma Categoria";
			cout << "\nInsira o ID da Categoria que deseja Excluir: ";
			int id;
			cin.ignore();
			cin >> id;
			excCategoriaById(id, categorias, indCategorias, quantCategorias);
			break;
		}
		case '2':
		{
			cout << "um Banco";
			cout << "\nInsira o ID do Banco que deseja Excluir: ";
			int id;
			cin.ignore();
			cin >> id;
			excBancoById(id, bancos, indBancos, quantBancos);
			break;
		}
		case '3':
		{	
			cout << "uma Conta";
			cout << "\nInsira o ID da Conta que deseja Excluir: ";
			int id;
			cin.ignore();
			cin >> id;
			while(posContaById(id, contas, indContas, quantContas) == -1)
			{
				cout << "\nCódigo Inválido, Insira Outro: ";
				cin >> id;
			}
			int posConta = posContaById(id, contas, indContas, quantContas);
			Banco banco = bancos[posBancoById(contas[posConta].idBanco, bancos, indBancos, quantBancos)];
			cout << "\nConta Encontrada\n";
			printContacomBanco(contas[posConta], banco);
			excContaById(id, contas, indContas, quantContas);

			break;
		}
		default:
		cout << "Uma Opção Inválida";
		break;
		
		}
		break;
	}
	case '5':
	case 'E':
	{
		cout 	
		<< "Reorganizar Arquivos\n\n"
		<< "ATENÇÃO!!!\n"
		<< "Esta Operação irá excluir permanentemente TODOS os registros Inativos\n"
		<< "de Categorias, Bancos, Contas e Transações\n"
		<< "\nVocê Confirma a Reorganização dos Arquivos? \n (Insira [S] para confirmar)";

		char conf = 0;
		cin >> conf;
		if (toupper(conf) == 'S') 
		{
			organizarArquivoBancos(bancos, indBancos, quantBancos);
			organizarArquivoCategorias(categorias, indCategorias, quantCategorias);
			organizarArquivoContas(contas, indContas, quantContas);
			organizarArquivoBancos(bancos, indBancos, quantBancos);
			organizarArquivoTransacoesById(transacoes, indTransId, quantTransacoes);
			organizarArquivoTransacoesData(transacoes, indTransData, quantTransacoes);
			
			cout << "\nArquivos Reorganizados com Sucesso! \n";
			break;
		}
		else {
			cout << "\nReorganização Cancelada.\n";
			break;
		}
		break;
	}
	case '6':
	case 'F':
	{
		cout 
		<< "Manipular Transações\n"
		<< "O que você gostaria de Fazer? \n"
		<< "1 - Consultar uma Transação por seu ID\n"
		<< "2 - Consultar todas as Transações efetuadas dentro de um Período\n"
		<< "3 - Consultar todas as Transações já efetuadas\n"
		<< "4 - Efetuar uma Nova Transação\n"
		<< "5 - Estornar uma Transação\n"
		<< "\n";

		char opTran;
		cin.ignore();
		cin >> opTran;
		cout << "\nVocê escolheu ";

		switch(toupper(opTran)){
		case '1':
		{	
			cout << "Consultar uma Transação por seu ID\n";
			cout << "Insira o ID da Transacao que deseja procurar: ";
			int id;
			cin.ignore();
			cin >> id;
			int posTran = posTransacaoById(id, transacoes, indTransId, quantTransacoes);
			if (posTran!= -1)
			{
				cout << "\nTransação encontrada.\n";
				Transacao transacao = transacoes[posTran];
				Categoria categoria = categorias[posCategoriaById(transacao.idCategoria, categorias, indCategorias, quantCategorias)];
				Conta conta = contas[posContaById(transacao.idConta, contas, indContas, quantContas)];
				Banco banco = bancos[posBancoById(conta.idBanco, bancos, indBancos, quantBancos)];

				printTransacaoCompleta(transacao, categoria, conta, banco);
			}
			else cout << "\nTransação não encontrada.\n";
			break;
		}
		case '2':
		{
			cout << "Consultar todas as Transações efetuadas dentro de um Período\n";
			cout << "\nInsira a Data de Início:\n";
			Data dA = lerData();

			cout << "\n\nInsira a Data Final:\n";
			Data dB = lerData();

			lExaustTransacoesPeriodo(transacoes, quantTransacoes, dA, dB);
	
			break;
		}
		case '3':
		{
			cout << "Consultar todas as Transações já efetuadas\n";
			Data dA = transacoes[indTransData[0].pos].data;
			Data dB = transacoes[indTransData[quantTransacoes-1].pos].data;
			lExaustTransacoesPeriodo(transacoes, quantTransacoes, dA, dB);

			break;
		}
		case '4':
		{
			cout << "Efetuar uma Nova Transação\n";
			int newId = 1;
			while(posTransacaoById(newId, transacoes, indTransId, quantTransacoes) != -1) newId++;

			int idCat = 0;
			cout << "\n\nInsira o Código da Categoria: ";
			cin >> idCat;
			while(posCategoriaById(idCat, categorias, indCategorias, quantCategorias) == -1)
			{
				cout << "\nCódigo Inválido, Insira Outro: ";
				cin >> idCat;
			}

			int idConta = 0;
			cout << "\n\nInsira o Código da Conta: ";
			cin >> idConta;
			while(posContaById(idConta, contas, indContas, quantContas) == -1)
			{
				cout << "\nCódigo Inválido, Insira Outro: ";
				cin >> idConta;
			}


			Transacao transacao{newId, idCat, idConta, hoje, 0, 0, false};
			transacao = lerTransacao(transacao);
			inserirTransacao(transacoes, quantTransacoes, transacao, indTransId, indTransData);
			cout << "\nTransação Efetuada com Sucesso! \n";
			
			double saldo = transacao.valor;
			if(transacao.tipo == 'D') saldo *= -1;

			contas[posContaById(idConta, contas, indContas, quantContas)].saldo += saldo;

			break;
		}
		case '5':
		{
			cout 
			<< "Estornar uma Transação\n"
			<< "\nInsira o ID da Transação que deseja Excluir: ";

			int id;
			cin.ignore();
			cin >> id;
			while(posTransacaoById(id, transacoes, indTransId, quantTransacoes) == -1)
			{
				cout << "\nCódigo Inválido, Insira Outro: ";
				cin >> id;
			}
			Transacao transacao = transacoes[posTransacaoById(id, transacoes, indTransId, quantTransacoes)];
			
			cout << "\nTransacao Encontrada\n";
			
			Categoria categoria = categorias[posCategoriaById(transacao.idCategoria, categorias, indCategorias, quantCategorias)];
			Conta conta = contas[posContaById(transacao.idConta, contas, indContas, quantContas)];
			Banco banco = bancos[posBancoById(conta.idBanco, bancos, indBancos, quantBancos)];

			printTransacaoCompleta(transacao, categoria, conta, banco);
			if(excTransacaoById(id, transacoes, indTransId, quantTransacoes) == 0)
			{
				double valor = transacao.valor;
				if (transacao.tipo == 'C') valor *= -1;
				contas[posContaById(transacao.idConta, contas, indContas, quantContas)].saldo += valor;
			};
			break;
		}
		default:
		cout << "Uma Opção Inválida";
		break;
		}
		
		break;
	}
	case '7':
	case 'G':
	{
		cout 	<< "Listar Saldos de todas as contas\n\n";
		lstSaldos(contas, indContas, quantContas, bancos, indBancos, quantBancos);
		break;
	}
	case '0':
	case 'X':
	{
		cout << "Sair do Programa. Encerrando...\n\n";
		run = false;
		return 0;
		break;
	}
	default:
	{
		cout << "Uma Opção Inválida";
		break;
	}
	}

	voltaMenu();
	}

return 0;
}