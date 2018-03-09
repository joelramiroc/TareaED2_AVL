
#pragma once
#include"Structs.h"
#include<fstream>
#include<cstring>
#include<string>

class Funciones
{
private:
	bool EliminarAmbosHijosArchivo(ItemInMemory* father, ItemInMemory* eliminado, ItemInMemory* newInEliminado);
	ItemInMemory* posLastDerecho(ItemInMemory* father);
	bool EliminarSoloArchivo(ItemInMemory* father, ItemInMemory* eliminado);
	bool EliminarUnHijo(ItemInMemory* father, ItemInMemory* eliminado,ItemInMemory* newInEliminado);
	ItemInMemory* TraerUltimo(ItemInMemory ** subRaiz);
	bool EliminarUnHijo(ItemInMemory* father, ItemInMemory* eliminado);
	bool EliminarSolo(ItemInMemory* father, ItemInMemory* eliminado);
	bool EliminarAmbosHijos(ItemInMemory ** subRaiz, ItemInMemory* eliminado, ItemInMemory * fatherN);
	int newRaiz;
	ItemInMemory* fatherToMoveInArchive,*temporalFather;
	bool escribirLaRaiz();
	bool EscribirEnArchivo(ItemInMemory* father, ItemInMemory* nuevo, int hijo);
	ItemInMemory* Raiz;
	string ArchiveName;
	string nombre, departamento;
	int codigo;
	bool InsertarItem(ItemInMemory* nuevo, ItemInMemory** raiz, ItemInMemory* father);
	bool CargarArchivo();
	bool InsertarItemDesdeArchivo(ItemInMemory* nuevo, ItemInMemory** raiz);
	bool EscribiendoEnArchivo(ItemInMemory **raiz);
	ifstream leyendo;
	ofstream escribir;
	void InOrder(ItemInMemory* raiz);
	void PreOrder(ItemInMemory* raiz);
	void PosOrder(ItemInMemory* raiz);
	ofstream* escribiendo;
	ItemInMemory* Busqueda(int codigo, ItemInMemory* raiz);
	int diferencia(int altHiz,int altD);
	void GiroIzquierda(ItemInMemory** subRaiz);
	void GiroDerecha(ItemInMemory** subRaiz);
	void DobleGiroIzquierda(ItemInMemory** subRaiz);
	void DobleGiroDerecha(ItemInMemory** subRaiz);
	int Max(int primero, int segundo);
	void ActualizarAlturas(ItemInMemory **subRaiz);
	void Balancear(ItemInMemory** subRaiz);
	int BuscarenArchivo(int codigo, int posA);
	bool ActualizarApuntadoresEnArchivo(ItemInMemory* father, ItemInMemory* hijoD, ItemInMemory* hijoI, ItemInMemory* father2, ItemInMemory* hijoD2, ItemInMemory* hijoI2, int rotation);
public:
	bool Eliminar();
	void IsMoveTheFather();
	void ImprimirNodos(int type);
	Funciones();
	ItemInMemory*  Buscar();
	int InsertarNodo();
	bool GuardarEnArchivo(string archiveName);
	bool CargarArchivo(string archiveName);
	bool Recuperar(int posActual);
};

