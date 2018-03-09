#pragma once

#include<iostream>
using namespace std;
struct Item
{
	int Codigo=-1;
	char Nombre[30];
	char Depto[15];
	int HijoDerecho=-1;
	int HijoIzquierdo=-1;
};

class ItemInMemory
{
public:
	ItemInMemory()
	{
		this->codigo = -1;
		this->hijoIzquierdo = nullptr;
		this->hijoDerecho = nullptr;
		this->Nombre = "";
		this->Departamento = "";
		this->alturaIzquierda = 0;
		this->alturaDerecha = 0;
	}
	ItemInMemory* hijoDerecho, *hijoIzquierdo;
	string Nombre, Departamento;
	int codigo,alturaIzquierda,alturaDerecha;
};