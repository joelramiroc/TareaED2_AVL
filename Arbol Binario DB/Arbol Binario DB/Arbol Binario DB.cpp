// Arbol Binario DB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<string>
#include<iostream>
#include<cstring>
#include"Funciones.h"

using namespace std;
int main()
{

	Funciones* funciones = new Funciones();
	bool continuar = true;
	int opcion;
	do {
		cout << "ARBOL BALANCEADO CON ALMACENAMIENTO SECUNDARIO" << endl;
		cout << "Que desea hacer:" << endl;
		cout << "1. Agregar registro" << endl;
		cout << "2. Eliminar registro" << endl;
		cout << "3. Buscar registro" << endl;
		cout << "4. Ver registros" << endl;
		cout << "5. Salir" << endl;
		cin >> opcion;
		switch (opcion)
		{		
		case 1:
		{
			funciones->InsertarNodo();
			funciones->IsMoveTheFather();
		}
		break;
		
		case 2:
		{
			funciones->Eliminar();
		}
		break;
		
		case 3:
		{
			funciones->Buscar();
		}
		break;
		case 4:
		{
			int type;
			cout << "Seleccione un tipo de impresion:" << endl;
			cout << "1. In Order" << endl;
			cout << "2. Pre Order" << endl;
			cout << "3. Pos Order" << endl;
			cin >> type;
			funciones->ImprimirNodos(type);
		}
		break;

		case 5:
		{
			continuar = false;
		}
		break;

		default:
			break;
		}

	} while (continuar);
    return 0;
}

