#include <stdio.h>
#include <iostream>

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
};

struct Categoria_Gasto
{
    // Categoria do Gasto realizado, como entretenimento, comida, locomo��o, m�dico, etc
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
    char tipo; //D�bito ou Cr�dito
};

