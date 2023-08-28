#include <stdio.h>
#include <iostream>

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
};

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

