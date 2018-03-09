#include "stdafx.h"
#include "Funciones.h"

bool Funciones::Eliminar()
{
	int codigo;
	cout << "Ingrese codigo del registro a eliminar:" << endl;
	cin >> codigo;
	ItemInMemory* posR = this->Busqueda(codigo, this->Raiz);
	if (posR != nullptr)
	{
		if(posR->hijoDerecho == nullptr && posR->hijoIzquierdo == nullptr)
		{
			this->EliminarSolo(this->Raiz,posR);
		}
		else if (posR->hijoDerecho != nullptr && posR->hijoIzquierdo != nullptr)
		{
			this->temporalFather = this->Raiz;
			this->EliminarAmbosHijos(&this->Raiz, posR);
		}
		else
		{
			this->EliminarUnHijo(this->Raiz,posR);
		}
		this->IsMoveTheFather();
	}
	else
	{
		cout << "No existe ese registro" << endl;
	}
	return false;
}
void Funciones::IsMoveTheFather()
{
	Item leido;
	this->leyendo.close();
	this->leyendo.open(this->ArchiveName, ios::in | ios::out);
	if (!escribiendo->is_open())
		this->escribiendo->open(this->ArchiveName, ios::in | ios::out | ios::binary);

	int apuntadorInicio;
	this->leyendo.seekg(0,ios::beg);
	this->leyendo.read(reinterpret_cast<char*>(&apuntadorInicio), sizeof(int));
	this->leyendo.seekg(apuntadorInicio*sizeof(Item)+4);
	this->leyendo.read(reinterpret_cast<char*>(&leido), sizeof(Item));

	if (this->Raiz == nullptr)
	{
		this->escribiendo->seekp(0, ios::beg);
		newRaiz = -1;
		this->escribiendo->write(reinterpret_cast<char*>(&this->newRaiz), sizeof(int));
	}
	else if (leido.Codigo!=this->Raiz->codigo)
	{
		this->escribiendo->seekp(0, ios::beg);
		this->escribiendo->write(reinterpret_cast<char*>(&this->newRaiz), sizeof(int));
	}
	this->leyendo.close();
	this->escribiendo->close();
}
bool Funciones::EliminarAmbosHijosArchivo(ItemInMemory * father, ItemInMemory * eliminado, ItemInMemory * newInEliminado)
{
	this->leyendo.close();
	this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
	if (!this->escribiendo->is_open())
	{
		this->escribiendo->open(this->ArchiveName, ios::in | ios::out | ios::binary);
	}

	int principal;
	this->leyendo.seekg(0, ios::beg);
	this->leyendo.read(reinterpret_cast<char*>(&principal), sizeof(int));
	this->leyendo.close();

	int posEliminado, posFather, posNewEliminado, posSinHijo;
	Item fatherA, eliminadoA, newEliminadoA, sinHijoa,temporal;
	temporal.Codigo = -1;
	temporal.HijoDerecho = -1;
	temporal.HijoIzquierdo = -1;
	ItemInMemory* last = this->posLastDerecho(eliminado->hijoIzquierdo);

	posEliminado = this->BuscarenArchivo(eliminado->codigo, principal);
	posNewEliminado = this->BuscarenArchivo(newInEliminado->codigo, posEliminado);
	posFather = this->BuscarenArchivo(father->codigo, principal);

	if (this->Raiz == eliminado)
	{
		int temporal= 0;
		this->escribiendo->seekp(0, ios::beg);
		this->escribiendo->write(reinterpret_cast<char*>(&posNewEliminado), sizeof(int));
	}
	else
	{
		this->leyendo.close();
		this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
		this->leyendo.seekg(posFather*sizeof(Item) + 4);
		this->leyendo.read(reinterpret_cast<char*>(&fatherA), sizeof(Item));
		if (fatherA.HijoDerecho == posEliminado)
		{
			fatherA.HijoDerecho = posNewEliminado;
		}
		else
		{
			fatherA.HijoIzquierdo = posNewEliminado;
		}
		this->escribiendo->seekp(posFather * sizeof(Item) + 4);
		this->escribiendo->write(reinterpret_cast<char*>(&fatherA), sizeof(Item));
	}
	this->leyendo.close();
	this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
	this->leyendo.seekg(posNewEliminado * sizeof(Item) + 4);
	this->leyendo.read(reinterpret_cast<char*>(&newEliminadoA), sizeof(Item));
	this->leyendo.seekg(posEliminado * sizeof(Item) + 4);
	this->leyendo.read(reinterpret_cast<char*>(&eliminadoA), sizeof(Item));
	this->leyendo.seekg(posNewEliminado * sizeof(Item) + 4);
	this->leyendo.read(reinterpret_cast<char*>(&newEliminadoA), sizeof(Item));
	newEliminadoA.HijoDerecho = eliminadoA.HijoDerecho;
	newEliminadoA.HijoIzquierdo = eliminadoA.HijoIzquierdo;
	this->escribiendo->seekp(posEliminado * sizeof(Item) + 4);
	this->escribiendo->write(reinterpret_cast<char*>(&newEliminadoA), sizeof(Item));
	this->escribiendo->seekp(posNewEliminado * sizeof(Item) + 4);
	this->escribiendo->write(reinterpret_cast<char*>(&temporal), sizeof(Item));
	if (last != nullptr)
	{
		this->leyendo.close();
		posSinHijo = this->BuscarenArchivo(last->codigo, principal);
		this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
		this->leyendo.seekg(posSinHijo * sizeof(Item) + 4);
		this->leyendo.read(reinterpret_cast<char*>(&sinHijoa), sizeof(Item));
		sinHijoa.HijoDerecho = -1;
		this->escribiendo->seekp(posSinHijo * sizeof(Item) + 4);
		this->escribiendo->write(reinterpret_cast<char*>(&sinHijoa), sizeof(Item));
	}
	this->leyendo.close();
	this->escribiendo->close();
	return true;
}
ItemInMemory * Funciones::posLastDerecho(ItemInMemory * father)
{
	if(father == nullptr)
	{ 
		return nullptr;
	}
	else if (father->hijoDerecho == nullptr)
	{
		return nullptr;
	}
	else if (father->hijoDerecho->hijoDerecho == nullptr)
	{
		return father;
	}
	else
	{
		return this->posLastDerecho(father->hijoDerecho);
	}
	return nullptr;
}
bool Funciones::EliminarSoloArchivo(ItemInMemory * father, ItemInMemory * eliminado)
{
	if (!this->escribiendo->is_open())
	{
		this->escribiendo->open(this->ArchiveName, ios::in | ios::binary);
	}

	Item eliminadoA,fatherA;
	eliminadoA.Codigo = -1;
	eliminadoA.HijoDerecho = -1;
	eliminadoA.HijoIzquierdo = -1;

	if (father==nullptr)
	{
		int principal=-1;
		this->escribiendo->seekp(0, ios::beg);
		this->escribiendo->write(reinterpret_cast<char*>(&principal), sizeof(int));
		this->escribiendo->seekp(4);
		this->escribiendo->write(reinterpret_cast<char*>(&eliminadoA), sizeof(Item));
		this->escribiendo->close();
		return true;
	}
	else
	{
		int principal;
		this->leyendo.close();
		this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
		this->leyendo.seekg(0, ios::beg);
		this->leyendo.read(reinterpret_cast<char*>(&principal), sizeof(int));
		this->leyendo.close();

		int posEliminado = this->BuscarenArchivo(eliminado->codigo, principal);
		int posFather = this->BuscarenArchivo(father->codigo, principal);
		this->leyendo.close();
		this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
		this->leyendo.seekg(posFather * sizeof(Item) + 4);
		this->leyendo.read(reinterpret_cast<char*>(&fatherA), sizeof(Item));
		

		if (fatherA.HijoDerecho == posEliminado)
		{
			fatherA.HijoDerecho = -1;
		}
		else
		{
			fatherA.HijoIzquierdo = -1;
		}
		this->escribiendo->seekp(posFather * sizeof(Item) + 4);
		this->escribiendo->write(reinterpret_cast<char*>(&fatherA), sizeof(Item));
		this->escribiendo->seekp(posEliminado * sizeof(Item) + 4);
		this->escribiendo->write(reinterpret_cast<char*>(&eliminadoA), sizeof(Item));
		this->leyendo.close();
		this->escribiendo->close();
		return true;
	}

	return false;
}
bool Funciones::EliminarUnHijo(ItemInMemory * father, ItemInMemory * eliminado, ItemInMemory * newInEliminado)
{
	Item fatherA, eliminadoA, newInEliminadoA;
	eliminadoA.Codigo = -1;
	eliminadoA.HijoDerecho = -1;
	eliminadoA.HijoIzquierdo = -1;
	int principal;
	this->leyendo.close();
	this->leyendo.open(this->ArchiveName,ios::in | ios::binary);
	if (!this->escribiendo->is_open())
	{
		this->escribiendo->open(this->ArchiveName, ios::in | ios::out | ios::binary);
	}
	this->leyendo.seekg(0, ios::beg);
	this->leyendo.read(reinterpret_cast<char*>(&principal), sizeof(int));
	this->leyendo.close();
	int newPosNewInEliminado = this->BuscarenArchivo(newInEliminado->codigo,principal);
	int newPosEliminado = this->BuscarenArchivo(eliminado->codigo, principal);

	if (father == nullptr)
	{
		this->escribiendo->seekp(0, ios::beg);
		this->escribiendo->write(reinterpret_cast<char*>(&newPosNewInEliminado), sizeof(int));
		this->escribiendo->seekp(newPosEliminado * sizeof(Item) + 4);
		this->escribiendo->write(reinterpret_cast<char*>(&eliminadoA), sizeof(Item));
		this->leyendo.close();
		this->escribiendo->close();
		return true;
	}
	else
	{
		int newPosFather = this->BuscarenArchivo(father->codigo, principal);
		this->leyendo.close();
		this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
		this->leyendo.seekg(newPosFather * sizeof(Item) + 4);
		this->leyendo.read(reinterpret_cast<char*>(&fatherA), sizeof(Item));
		
		if(fatherA.HijoDerecho == newPosEliminado)
		{
			fatherA.HijoDerecho = newPosNewInEliminado;
		}
		else
		{
			fatherA.HijoIzquierdo = newPosNewInEliminado;
		}
		this->escribiendo->seekp(newPosFather * sizeof(Item) + 4);
		this->escribiendo->write(reinterpret_cast<char*>(&fatherA), sizeof(Item));
		this->escribiendo->seekp(newPosEliminado * sizeof(Item) + 4);
		this->escribiendo->write(reinterpret_cast<char*>(&eliminadoA), sizeof(Item));
		this->escribiendo->close();
		this->leyendo.close();
	}
	return false;
}
ItemInMemory * Funciones::TraerUltimo(ItemInMemory * subRaiz)
{
	if (subRaiz->hijoDerecho == nullptr)
	{
		subRaiz->hijoDerecho = nullptr;
		ItemInMemory* nuevo = new ItemInMemory();
		nuevo->Nombre = subRaiz->Nombre;
		nuevo->Departamento = subRaiz->Departamento;
		nuevo->alturaDerecha = subRaiz->alturaDerecha;
		nuevo->alturaIzquierda = subRaiz->alturaIzquierda;
		nuevo->codigo = subRaiz->codigo;
		nuevo->hijoDerecho = subRaiz->hijoDerecho;
		nuevo->hijoIzquierdo = subRaiz->hijoIzquierdo;
		subRaiz = nullptr;
		return nuevo;
	}
	else
	{
		ItemInMemory* retornar =  this->TraerUltimo(subRaiz->hijoDerecho);
		if (subRaiz->hijoDerecho!=nullptr)
		{
			subRaiz->alturaDerecha = this->Max(subRaiz->hijoDerecho->alturaDerecha, subRaiz->hijoDerecho->alturaIzquierda);
		}
		else
		{
			subRaiz->alturaDerecha = 0;
		}
		this->Balancear(&subRaiz);
		return retornar;
	}
	return nullptr;
}
bool Funciones::EliminarUnHijo(ItemInMemory * father, ItemInMemory * eliminado)
{
	if (father == nullptr)
	{
		return false;
	}
	else if (this->Raiz == eliminado)
	{
		if (father->hijoDerecho != nullptr)
		{
			this->EliminarUnHijo(nullptr,father,father->hijoDerecho);
			this->Raiz = father->hijoDerecho;
			return true;
		}
		else
		{
			this->EliminarUnHijo(nullptr, father, father->hijoIzquierdo);
			this->Raiz = father->hijoIzquierdo;
			return true;
		}
	}
	else if (father->hijoDerecho == eliminado || father->hijoIzquierdo == eliminado)
	{
		if (father->hijoDerecho == eliminado)
		{
			if (eliminado->hijoDerecho != nullptr)
			{
				this->EliminarUnHijo(father, eliminado, eliminado->hijoDerecho);
				father->hijoDerecho = eliminado->hijoDerecho;
			}
			else
			{
				this->EliminarUnHijo(father, eliminado, eliminado->hijoIzquierdo);
				father->hijoDerecho = eliminado->hijoIzquierdo;
			}
			
			father->alturaDerecha = this->Max(father->hijoDerecho->alturaDerecha, father->hijoDerecho->alturaIzquierda);
		}
		else
		{
			if (eliminado->hijoDerecho != nullptr)
			{
				this->EliminarUnHijo(father, eliminado, eliminado->hijoDerecho);
				father->hijoIzquierdo = eliminado->hijoDerecho;
			}
			else
			{
				this->EliminarUnHijo(father, eliminado, eliminado->hijoIzquierdo);
				father->hijoIzquierdo = eliminado->hijoIzquierdo;
			}
			father->alturaIzquierda = this->Max(father->hijoIzquierdo->alturaDerecha, father->hijoIzquierdo->alturaIzquierda);
		}

		this->Balancear(&father);
	}
	else if (father->codigo > eliminado->codigo)
	{
		this->EliminarUnHijo(father->hijoIzquierdo, eliminado);
		this->Balancear(&father);
		return true;
	}
	else
	{
		this->EliminarUnHijo(father->hijoDerecho, eliminado);
		this->Balancear(&father);
		return true;
	}
	return false;
}
bool Funciones::EliminarSolo(ItemInMemory * father, ItemInMemory * eliminado)
{
	if (father == nullptr)
	{
		return false;
	}
	else if (this->Raiz == eliminado && this->Raiz == father)
	{
		this->EliminarSoloArchivo(nullptr,this->Raiz);
		this->Raiz = nullptr;
		return true;
	}
	else if (father->hijoDerecho == eliminado  || father->hijoIzquierdo == eliminado)
	{
		if (father->hijoDerecho == eliminado)
		{
			this->EliminarSoloArchivo(father, father->hijoDerecho);
			father->hijoDerecho = nullptr;
			father->alturaDerecha = 0;
		}
		else
		{
			this->EliminarSoloArchivo(father, father->hijoIzquierdo);
			father->hijoIzquierdo = nullptr;
			father->alturaIzquierda = 0;
		}

		return true;
	}
	else if (father->codigo > eliminado->codigo)
	{
		this->EliminarSolo(father->hijoIzquierdo,eliminado);
		this->Balancear(&father);
		return true;
	}
	else
	{
		this->EliminarSolo(father->hijoDerecho, eliminado);
		this->Balancear(&father);
		return true;
	}
	return false;
}
bool Funciones::EliminarAmbosHijos(ItemInMemory ** subRaiz, ItemInMemory * eliminado)
{
	if (*subRaiz == nullptr)
	{
		return false;
	}
	else if ((*subRaiz) == eliminado)
	{
		ItemInMemory* temporal = this->TraerUltimo((*subRaiz)->hijoIzquierdo);
		temporal->hijoDerecho = eliminado->hijoDerecho;
		temporal->hijoIzquierdo = eliminado->hijoIzquierdo;
		temporal->alturaDerecha = eliminado->alturaDerecha;
		temporal->alturaIzquierda = eliminado->alturaIzquierda;
		*subRaiz = temporal;
		this->Balancear(&temporal);
	}
	else if ((*subRaiz)->codigo > eliminado->codigo)
	{
		this->temporalFather = *subRaiz;
		this->EliminarAmbosHijos(&(*subRaiz)->hijoIzquierdo, eliminado);
		(*subRaiz)->alturaIzquierda = this->Max((*subRaiz)->hijoIzquierdo->alturaDerecha, (*subRaiz)->hijoIzquierdo->alturaIzquierda);
		this->Balancear(subRaiz);
	}
	else
	{
		this->temporalFather = *subRaiz;
		this->EliminarAmbosHijos(&(*subRaiz)->hijoDerecho, eliminado);
		(*subRaiz)->alturaDerecha = this->Max((*subRaiz)->hijoDerecho->alturaDerecha, (*subRaiz)->hijoDerecho->alturaIzquierda);
		this->Balancear(subRaiz);
	}
	return false;
}
bool Funciones::escribirLaRaiz()
{
	if (!this->escribiendo->is_open())
		this->escribiendo->open(this->ArchiveName, ios::in | ios::out | ios::binary);
	Item nuevo;
	nuevo.Codigo = this->Raiz->codigo;
	strcpy_s(nuevo.Nombre, this->Raiz->Nombre.c_str());
	strcpy_s(nuevo.Depto, this->Raiz->Departamento.c_str());
	int a = 0;
	this->escribiendo->seekp(0, ios::beg);
	this->escribiendo->write(reinterpret_cast<char*>(&a), sizeof(int));
	this->escribiendo->write(reinterpret_cast<char*>(&nuevo), sizeof(Item));
	this->escribiendo->close();
	return true;
}
bool Funciones::EscribirEnArchivo(ItemInMemory * father, ItemInMemory * nuevo, int hijo)
{
	this->leyendo.close();
	this->leyendo.open(this->ArchiveName,ios::in | ios::binary);
	this->leyendo.seekg(0, ios::end);
	int newPos = this->leyendo.tellg();
	Item Thefather, Thenuevo;
	if(!this->escribiendo->is_open())
		this->escribiendo->open(this->ArchiveName, ios::in | ios::out | ios::binary);

	if (father != nullptr)
	{
		int principal;
		this->leyendo.seekg(0);
		this->leyendo.read(reinterpret_cast<char*>(&principal), sizeof(int));
		int Posfather = this->BuscarenArchivo(father->codigo, principal);
		this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
		this->leyendo.seekg(Posfather * sizeof(Item) + 4);
		this->leyendo.read(reinterpret_cast<char*>(&Thefather), sizeof(Item));
		int test = sizeof(Item);
		int newPosToItem = (newPos - 4) / sizeof(Item);
		if (hijo == 1)
		{
			Thefather.HijoIzquierdo = newPosToItem;
		}
		else if (hijo == 2)
		{
			Thefather.HijoDerecho = newPosToItem;
		}
		this->escribiendo->seekp(Posfather * sizeof(Item) + 4);
		this->escribiendo->write(reinterpret_cast<char*>(&Thefather), sizeof(Item));
	}
	else 
	{
		this->escribiendo->seekp(0, ios::beg);
		int posI = 0;
		this->escribiendo->write(reinterpret_cast<char*>(&posI), sizeof(int));
	}

	this->escribiendo->seekp(0, ios::end);
	Thenuevo.Codigo = nuevo->codigo;
	strcpy_s(Thenuevo.Nombre, nuevo->Nombre.c_str());
	strcpy_s(Thenuevo.Depto, nuevo->Departamento.c_str());
	this->escribiendo->write(reinterpret_cast<char*>(&Thenuevo), sizeof(Item));
	this->escribiendo->close();
	return true;
}
bool Funciones::InsertarItem(ItemInMemory * nuevo,ItemInMemory** subRaiz, ItemInMemory* father)
{
	if (*subRaiz == nullptr) {
		*subRaiz = nuevo;
		if (this->Raiz == nuevo)
			this->escribirLaRaiz();
		return 0;
	}
	int max;

	if ((*subRaiz)->codigo == nuevo->codigo)
		return -1;

	if ((*subRaiz)->codigo > nuevo->codigo)
	{
		InsertarItem(nuevo, &((*subRaiz)->hijoIzquierdo), (*subRaiz));
		max = this->Max((*subRaiz)->hijoIzquierdo->alturaIzquierda, (*subRaiz)->hijoIzquierdo->alturaDerecha) + 1;
		(*subRaiz)->alturaIzquierda = max;
		if ((*subRaiz)->hijoIzquierdo == nuevo)
			this->EscribirEnArchivo(*subRaiz, nuevo,1);
		this->fatherToMoveInArchive = father;
		Balancear(subRaiz);
		max = this->Max((*subRaiz)->alturaIzquierda, (*subRaiz)->alturaDerecha);
		return max;
	}
	else if ((*subRaiz)->codigo < nuevo->codigo)
	{
		InsertarItem(nuevo, &((*subRaiz)->hijoDerecho), (*subRaiz));
		max = this->Max((*subRaiz)->hijoDerecho->alturaIzquierda, (*subRaiz)->hijoDerecho->alturaDerecha) + 1;
		(*subRaiz)->alturaDerecha = max;
		if ((*subRaiz)->hijoDerecho == nuevo)
			this->EscribirEnArchivo(*subRaiz, nuevo, 2);
		this->fatherToMoveInArchive = father;
		Balancear(subRaiz);
		max = this->Max((*subRaiz)->alturaIzquierda, (*subRaiz)->alturaDerecha);
		return max;
	}
	else 
	{
		return -1;
	}
}
bool Funciones::CargarArchivo()
{
	this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
	if(!this->leyendo)
		return false;
	int posA;
	leyendo.seekg(0, ios::beg);
	leyendo.read(reinterpret_cast<char*>(&posA), sizeof(int));
	if (posA == -1)
		return false;
	this->leyendo.close();
	this->Recuperar(posA);
	return true;
}
bool Funciones::InsertarItemDesdeArchivo(ItemInMemory * nuevo, ItemInMemory ** subRaiz)
{
	if (*subRaiz == nullptr) {
		*subRaiz = nuevo;
		return 0;
	}
	int max;

	if ((*subRaiz)->codigo == nuevo->codigo)
		return -1;

	if ((*subRaiz)->codigo > nuevo->codigo)
	{
		InsertarItemDesdeArchivo(nuevo, &((*subRaiz)->hijoIzquierdo));
		max = this->Max((*subRaiz)->hijoIzquierdo->alturaIzquierda, (*subRaiz)->hijoIzquierdo->alturaDerecha) + 1;
		(*subRaiz)->alturaIzquierda = max;
		max = this->Max((*subRaiz)->alturaIzquierda, (*subRaiz)->alturaDerecha);
		return max;
	}
	else if ((*subRaiz)->codigo < nuevo->codigo)
	{
		InsertarItemDesdeArchivo(nuevo, &((*subRaiz)->hijoDerecho));
		max = this->Max((*subRaiz)->hijoDerecho->alturaIzquierda, (*subRaiz)->hijoDerecho->alturaDerecha) + 1;
		(*subRaiz)->alturaDerecha = max;
		max = this->Max((*subRaiz)->alturaIzquierda, (*subRaiz)->alturaDerecha);
		return max;
	}
	else {
		return -1;
	}
}
bool Funciones::EscribiendoEnArchivo(ItemInMemory ** raiz)
{
	if (&raiz == nullptr)
	{
		return false;
	}
	escribir.open(this->ArchiveName, ios::in | ios::out | ios::binary);
	Item fatherNuevo;
	int numberOfRegister;
	int father;
	this->escribir.seekp(0, ios::end);
	father = escribir.tellp();
	escribir.seekp(father);
	escribir.write(reinterpret_cast<char*>(&fatherNuevo), sizeof(Item));
	escribir.close();
	fatherNuevo.Codigo = (*raiz)->codigo;
	strcpy_s(fatherNuevo.Nombre, ((*raiz)->Nombre).c_str());
	strcpy_s(fatherNuevo.Depto, ((*raiz)->Departamento).c_str());

	if ((*raiz)->hijoIzquierdo != nullptr)
	{
		this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
		this->leyendo.seekg(0, ios::end);
		numberOfRegister = this->leyendo.tellg();
		this->leyendo.close();
		numberOfRegister -= 4;
		numberOfRegister = numberOfRegister / sizeof(Item);
		fatherNuevo.HijoIzquierdo = numberOfRegister;
		this->EscribiendoEnArchivo(&(*raiz)->hijoIzquierdo);
	}

	if ((*raiz)->hijoDerecho != nullptr)
	{
		this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
		this->leyendo.seekg(0, ios::end);
		numberOfRegister = this->leyendo.tellg();
		this->leyendo.close();
		numberOfRegister -= 4;
		numberOfRegister = numberOfRegister / sizeof(Item);
		fatherNuevo.HijoDerecho = numberOfRegister;
		this->EscribiendoEnArchivo(&(*raiz)->hijoDerecho);
	}

	escribir.open(this->ArchiveName, ios::in | ios::out | ios::binary);
	escribir.seekp(father);
	escribir.write(reinterpret_cast<char*>(&fatherNuevo), sizeof(Item));	
	escribir.close();
	return true;
}
void Funciones::ImprimirNodos(int type)
{
	if (this->Raiz == nullptr)
		return;

	if (type == 1)
	{
		this->InOrder(this->Raiz);
	}
	else if (type == 2)
	{
		this->PreOrder(this->Raiz);
	}
	else if (type == 3)
	{
		this->PosOrder(this->Raiz);
	}
}
Funciones::Funciones()
{
	this->newRaiz = 0;
	this->fatherToMoveInArchive = nullptr;
	this->ArchiveName = "joel4.dat";
	this->Raiz = nullptr;
	this->escribiendo = new ofstream(this->ArchiveName, ios::in | ios::out | ios::binary);
	if(!escribiendo->is_open())
		this->escribiendo->open(this->ArchiveName);	
	this->escribiendo->close();
	this->CargarArchivo();
}
ItemInMemory * Funciones::Buscar()
{
	int codigo;
	cout << "Ingrese codigo del registro que busca:" << endl;
	cin >> codigo;
	return this->Busqueda(codigo, this->Raiz);
}
ItemInMemory* Funciones::Busqueda(int codigo,ItemInMemory* raiz)
{
	if (raiz==nullptr)
	{
		return nullptr;
	}
	else if (raiz->codigo==codigo)
	{
		return raiz;
	}
	else if (raiz->codigo>codigo)
	{
		return this->Busqueda(codigo, raiz->hijoIzquierdo);
	}
	else
	{
		return this->Busqueda(codigo, raiz->hijoDerecho);
	}
	return nullptr;
}
int Funciones::InsertarNodo()
{
	ItemInMemory* nuevo= new ItemInMemory();
	cout << "Ingrese nombre:" << endl;
	cin >> nuevo->Nombre;
	cout << "Ingrese Departamento:" << endl;
	cin >> nuevo->Departamento;
	cout << "Ingrese Codigo:" << endl;
	cin >> nuevo->codigo;
	
	if (this->Busqueda(nuevo->codigo,this->Raiz) == nullptr)
	{
		return this->InsertarItem(nuevo,&this->Raiz,nullptr);
	}
	else
	{
		cout << "Ya existe un registro con ese codigo" << endl;
	}
	return false;
}
void Funciones::InOrder(ItemInMemory * raiz)
{
	if (raiz == nullptr)
	{
		return;
	}
	this->InOrder(raiz->hijoIzquierdo);
	cout << "Codigo: " << endl;
	cout << raiz->codigo << endl;
	cout << "Nombre: " << endl;
	cout << raiz->Nombre << endl;
	cout << "Departamento: " << endl;
	cout << raiz->Departamento << endl;
	cout << "==========================================" << endl;
	this->InOrder(raiz->hijoDerecho);
}
void Funciones::PreOrder(ItemInMemory * raiz)
{
	if (raiz == nullptr)
	{
		return;
	}
	cout << "Codigo: " << endl;
	cout << raiz->codigo << endl;
	cout << "Nombre: " << endl;
	cout << raiz->Nombre << endl;
	cout << "Departamento: " << endl;
	cout << raiz->Departamento << endl;
	cout << "==========================================" << endl;
	this->PreOrder(raiz->hijoIzquierdo);
	this->PreOrder(raiz->hijoDerecho);
}
void Funciones::PosOrder(ItemInMemory * raiz)
{
	if (raiz == nullptr)
	{
		return;
	}
	this->PosOrder(raiz->hijoIzquierdo);
	this->PosOrder(raiz->hijoDerecho);
	cout << "Codigo: " << endl;
	cout << raiz->codigo << endl;
	cout << "Nombre: " << endl;
	cout << raiz->Nombre << endl;
	cout << "Departamento: " << endl;
	cout << raiz->Departamento << endl;
	cout << "==========================================" << endl;
}
int Funciones::diferencia(int n1, int n2)
{
	int total = 0;
	if (n1>n2)
		total = n1 - n2;
	if (n2>n1)
		total = n2 - n1;
	return total;
}
void Funciones::GiroIzquierda(ItemInMemory ** subRaiz)
{
	ItemInMemory* _hijoIzquierdo = (*subRaiz)->hijoIzquierdo;
	(*subRaiz)->hijoIzquierdo = _hijoIzquierdo->hijoDerecho;
	_hijoIzquierdo->hijoDerecho = *subRaiz;
	*subRaiz = _hijoIzquierdo;
	ActualizarAlturas(subRaiz);
	this->ActualizarApuntadoresEnArchivo((*subRaiz)->hijoDerecho, (*subRaiz)->hijoDerecho->hijoDerecho, (*subRaiz)->hijoDerecho->hijoIzquierdo, (*subRaiz), (*subRaiz)->hijoDerecho, (*subRaiz)->hijoIzquierdo,2);

}
void Funciones::GiroDerecha(ItemInMemory ** subRaiz)
{
	ItemInMemory* _hijoDerecho = (*subRaiz)->hijoDerecho;
	(*subRaiz)->hijoDerecho = _hijoDerecho->hijoIzquierdo;
	_hijoDerecho->hijoIzquierdo = *subRaiz;
	*subRaiz = _hijoDerecho;
	ActualizarAlturas(subRaiz);
	this->ActualizarApuntadoresEnArchivo((*subRaiz)->hijoIzquierdo, (*subRaiz)->hijoIzquierdo->hijoDerecho, (*subRaiz)->hijoIzquierdo->hijoIzquierdo, (*subRaiz), (*subRaiz)->hijoDerecho, (*subRaiz)->hijoIzquierdo,1);
}
void Funciones::DobleGiroIzquierda(ItemInMemory ** subRaiz)
{
	GiroDerecha(&(*subRaiz)->hijoIzquierdo);
	GiroIzquierda(subRaiz);
}
void Funciones::DobleGiroDerecha(ItemInMemory ** subRaiz)
{
	GiroIzquierda(&(*subRaiz)->hijoDerecho);
	GiroDerecha(subRaiz);
}
int Funciones::Max(int primero, int segundo)
{
	if (primero>segundo)
		return primero;
	return  segundo;
}
void Funciones::ActualizarAlturas(ItemInMemory ** subRaiz)
{
	ItemInMemory* _hizoDer = (*subRaiz)->hijoDerecho;
	ItemInMemory* _hizoIzq = (*subRaiz)->hijoIzquierdo;
	bool der = false;
	bool izq = false;
	if (_hizoDer == nullptr)
	{
		(*subRaiz)->hijoDerecho = 0;
		der = true;
	}
	if (_hizoIzq == nullptr)
	{
		izq = true;
		(*subRaiz)->alturaIzquierda = 0;
	}
	if (!der) {
		if (_hizoDer->hijoIzquierdo == nullptr) {
			_hizoDer->alturaIzquierda = 0;
		}
		else {
			_hizoDer->alturaIzquierda =
				Max(_hizoDer->hijoIzquierdo->alturaIzquierda, _hizoDer->hijoIzquierdo->alturaDerecha) + 1;
		}

		if (_hizoDer->hijoDerecho == nullptr) {
			_hizoDer->alturaDerecha = 0;
		}
		else {
			_hizoDer->alturaDerecha =
				Max(_hizoDer->hijoDerecho->alturaIzquierda, _hizoDer->hijoDerecho->alturaDerecha) + 1;
		}
	}
	if (!izq) {
		if (_hizoIzq->hijoIzquierdo == nullptr) {
			_hizoIzq->alturaIzquierda = 0;
		}
		else {
			_hizoIzq->alturaIzquierda =
				Max(_hizoIzq->hijoIzquierdo->alturaIzquierda, _hizoIzq->hijoIzquierdo->alturaDerecha) + 1;
		}

		if (_hizoIzq->hijoDerecho == nullptr) {
			_hizoIzq->alturaDerecha = 0;
		}
		else {
			_hizoIzq->alturaDerecha = Max(_hizoIzq->hijoDerecho->alturaIzquierda,
				_hizoIzq->hijoDerecho->alturaDerecha + 1);
		}
	}
	if (!izq)
		(*subRaiz)->alturaIzquierda = Max(_hizoIzq->alturaIzquierda, _hizoIzq->alturaDerecha) + 1;
	if (!der)
		(*subRaiz)->alturaDerecha = Max(_hizoDer->alturaIzquierda, _hizoDer->alturaDerecha) + 1;
}
void Funciones::Balancear(ItemInMemory ** subRaiz)
{
	if (diferencia((*subRaiz)->alturaIzquierda, (*subRaiz)->alturaDerecha)<2)
		return;
	if ((*subRaiz)->alturaDerecha>(*subRaiz)->alturaIzquierda)
	{
		if ((*subRaiz)->hijoDerecho->alturaDerecha > (*subRaiz)->hijoDerecho->alturaIzquierda)
		{
			GiroDerecha(subRaiz);
		}
		else
		{
			DobleGiroDerecha(subRaiz);
		}
	}
	else
	{
		if ((*subRaiz)->hijoIzquierdo->alturaIzquierda > (*subRaiz)->hijoIzquierdo->alturaDerecha)
		{
			GiroIzquierda(subRaiz);
		}
		else
		{
			DobleGiroIzquierda(subRaiz);
		}
	}
}
int Funciones::BuscarenArchivo(int codigo, int posA)
{
	Item leido;
	this->leyendo.close();
	this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
	int posLeer = posA * sizeof(Item) + 4;
	this->leyendo.seekg(posLeer);
	this->leyendo.read(reinterpret_cast<char*>(&leido), sizeof(Item));
	this->leyendo.close();
	if (leido.Codigo == -1)
	{
		return -1;
	}

	if (leido.Codigo == codigo)
	{
		return posA;
	}

	if (leido.Codigo > codigo)
	{
		return this->BuscarenArchivo(codigo, leido.HijoIzquierdo);
	}

	else if (leido.Codigo < codigo)
	{
		return this->BuscarenArchivo(codigo, leido.HijoDerecho);
	}

	return -1;
}
bool Funciones::ActualizarApuntadoresEnArchivo(ItemInMemory * father, ItemInMemory * hijoD, ItemInMemory * hijoI, ItemInMemory * father2, ItemInMemory * hijoD2, ItemInMemory * hijoI2, int rotation)
{
	this->leyendo.close();
	this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
	int principal, posFather, posD = -1, posI = -1;
	int posFather2, posD2 = -1, posI2 = -1;
	int posFather3, posD3 = -1, posI3 = -1;
	Item Ifather, Ifather2,principalFather;

	this->leyendo.seekg(0, ios::beg);
	this->leyendo.read(reinterpret_cast<char*>(&principal), sizeof(int));
	this->leyendo.close();

	posFather = this->BuscarenArchivo(father->codigo, principal);
	posFather2 = this->BuscarenArchivo(father2->codigo, principal);
	this->newRaiz = posFather2;
	if(this->fatherToMoveInArchive!=nullptr)
		posFather3 = this->BuscarenArchivo(this->fatherToMoveInArchive->codigo, principal);
	this->leyendo.open(this->ArchiveName, ios::in | ios::binary);
	
	this->leyendo.seekg(posFather * sizeof(Item) + 4);
	this->leyendo.read(reinterpret_cast<char*>(&Ifather), sizeof(Item));
	this->leyendo.seekg(posFather2 * sizeof(Item) + 4);
	this->leyendo.read(reinterpret_cast<char*>(&Ifather2), sizeof(Item));
	if (this->fatherToMoveInArchive != nullptr)
	{
		this->leyendo.seekg(posFather3 * sizeof(Item) + 4);
		this->leyendo.read(reinterpret_cast<char*>(&principalFather), sizeof(Item));
	}
	this->leyendo.close();

	if (hijoD != nullptr)
		posD = this->BuscarenArchivo(hijoD->codigo, principal);

	Ifather.HijoDerecho = posD;

	if (hijoI != nullptr)
		posI = this->BuscarenArchivo(hijoI->codigo, principal);

	Ifather.HijoIzquierdo = posI;


	if (hijoD2 != nullptr)
		posD2 = this->BuscarenArchivo(hijoD2->codigo, principal);

	Ifather2.HijoDerecho = posD2;

	if (hijoI2 != nullptr)
		posI2 = this->BuscarenArchivo(hijoI2->codigo, principal);

	Ifather2.HijoIzquierdo = posI2;

	if (this->fatherToMoveInArchive != nullptr)
	{

		if (rotation == 1)
		{
			principalFather.HijoDerecho = posFather2;
			posD3 = this->BuscarenArchivo(father2->codigo, principal);
			principalFather.HijoDerecho = posD3;
		}
		else
		{
			principalFather.HijoIzquierdo = posFather2;
			posI3 = this->BuscarenArchivo(father2->codigo, principal);
			principalFather.HijoIzquierdo = posI3;
		}
	}


	if(!this->escribiendo->is_open())
		this->escribiendo->open(this->ArchiveName, ios::in | ios::out | ios::binary);
	this->escribiendo->seekp(posFather * sizeof(Item) + 4);
	this->escribiendo->write(reinterpret_cast<char*>(&Ifather), sizeof(Item));
	this->escribiendo->seekp(posFather2 * sizeof(Item) + 4);
	this->escribiendo->write(reinterpret_cast<char*>(&Ifather2), sizeof(Item));

	if (this->fatherToMoveInArchive != nullptr)
	{
		this->escribiendo->seekp(posFather3 * sizeof(Item) + 4);
		this->escribiendo->write(reinterpret_cast<char*>(&principalFather), sizeof(Item));
	}
	this->escribiendo->close();
	return true;
}
bool Funciones::GuardarEnArchivo(string archiveName)
{
	this->escribiendo = new ofstream(archiveName, ios::in | ios::out | ios::binary);
	if (!escribiendo->is_open())
	{
		escribiendo->open(archiveName);
	}
	int f = 0;
	escribiendo->seekp(0, ios::beg);
	escribiendo->write(reinterpret_cast<char*>(&f), sizeof(int));
	this->ArchiveName = archiveName;
	escribiendo->close();
	this->EscribiendoEnArchivo(&this->Raiz);
	return false;
}
bool Funciones::CargarArchivo(string archiveName)
{
	this->ArchiveName = archiveName;
	this->leyendo.open(archiveName, ios::in | ios::binary);
	if (leyendo)
	{
		int raiz = 0;
		leyendo.seekg(0,ios::beg);
		leyendo.read(reinterpret_cast<char*>(&raiz), sizeof(int));
		leyendo.close();
		return this->Recuperar(raiz);
	}
	else
	{
		cout << "No se encontro el archivo" << endl;
	}
	return false;
}
bool Funciones::Recuperar(int posActual)
{
	if (posActual==-1)
	{
		return false;
	}

	leyendo.open(this->ArchiveName);
	Item leido;
	int posA = posActual * sizeof(Item) + 4;
	leyendo.seekg(posA);
	leyendo.read(reinterpret_cast<char*>(&leido), sizeof(Item));
	leyendo.close();

	if (leido.Codigo==-1)
	{
		return false;
	}

	ItemInMemory* nuevo = new ItemInMemory();
	nuevo->codigo = leido.Codigo;
	nuevo->Departamento = leido.Depto;
	nuevo->Nombre.assign(leido.Nombre);

	this->InsertarItemDesdeArchivo(nuevo, &this->Raiz);

	this->Recuperar(leido.HijoIzquierdo);

	this->Recuperar(leido.HijoDerecho);
	return true;
}