#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct Produs Produs;
typedef struct Vector Vector;

struct Produs
{
	int cod;
	float pret;
	char* denumire;
};

struct Vector
{
	int dimensiune;
	Produs* elemente;
};

/* Functii pentru structura Produs */

Produs initializareProdus()
{
	Produs produs = { .cod = 0, .denumire = NULL, .pret = 0.0f };
	return produs;
}

Produs creareProdus(int cod, float pret, const char* denumire)
{
	Produs produs;

	produs.cod = cod;
	produs.pret = pret;

	if (denumire != NULL)
	{
		produs.denumire = (char*)malloc((1 + strlen(denumire)) * sizeof(char));
		strcpy(produs.denumire, denumire);
	}
	else
	{
		produs.denumire = NULL;
	}

	return produs;
}

Produs copiazaProdus(const Produs produs)
{
	Produs copie;

	copie.cod = produs.cod;
	copie.pret = produs.pret;

	if (produs.denumire != NULL)
	{
		copie.denumire = (char*)malloc((1 + strlen(produs.denumire)) * sizeof(char));
		strcpy(copie.denumire, produs.denumire);
	}
	else
	{
		copie.denumire = NULL;
	}

	return copie;
}

void dezalocareProdus(Produs produs)
{
	if (produs.denumire != NULL)
	{
		free(produs.denumire);
	}
}

void afisareProdus(const Produs produs)
{
	printf("Codul produsului este: %d\nPretul produsului este: %.2f lei\nDenumirea produsului este: %s\n\n",
		produs.cod, produs.pret, produs.denumire);
}

/* Functii pentru structura Vector */

Vector initializareVector()
{
	Vector v = { .dimensiune = 0, .elemente = NULL };
	return v;
}

Vector creareVector(const int dimensiune, Produs* elemente)
{
	if (dimensiune <= 0 || elemente == NULL)
	{
		return initializareVector();
	}

	Vector v;

	v.dimensiune = dimensiune;
	v.elemente = (Produs*)malloc(dimensiune * sizeof(Produs));

	for (int i = 0; i < dimensiune; i++)
	{
		v.elemente[i] = copiazaProdus(elemente[i]);
	}

	return v;
}

void dezalocareVector(Vector v)
{
	if (v.elemente != NULL)
	{
		for (int i = 0; i < v.dimensiune; i++)
		{
			dezalocareProdus(v.elemente[i]);
		}

		free(v.elemente);
		v.elemente = NULL;
	}

	v.dimensiune = 0;
}

bool vectorIsEmpty(const Vector v)
{
	return v.elemente == NULL || v.dimensiune <= 0;
}

void afisareVector(const Vector v)
{
	if (vectorIsEmpty(v))
	{
		return;
	}

	for (int i = 0; i < v.dimensiune; i++)
	{
		afisareProdus(v.elemente[i]);
	}
}

/*
* scrieti o functie care face inserarea unui element in vector la inceput -> done
* scrieti o functie care face inserarea unui element in vector la final - tema (pushBack)
* scrieti o functie care face stergerea primului element din vector - incercati (popFront)
* scrieti o functie care face stergerea ultimului element din vector - incercati (popBack)
*/

Vector pushFront(Vector v, const Produs produs)
{
	if (v.elemente == NULL)
	{
		v.elemente = (Produs*)malloc(sizeof(Produs));
		v.elemente[0] = copiazaProdus(produs);
		v.dimensiune++;

		return v;
	}

	int dimensiune = v.dimensiune;
	Produs* aux = v.elemente;

	v.elemente = (Produs*)malloc((dimensiune + 1) * sizeof(Produs));
	v.elemente[0] = copiazaProdus(produs);

	for (int i = 0; i < dimensiune; i++)
	{
		v.elemente[i + 1] = aux[i];
	}

	v.dimensiune++;
	free(aux);

	return v;
}

int main()
{
	int nrElemente = 4;
	Produs produse[4] =
	{
		creareProdus(1, 10.0f, "Garnier"),
		creareProdus(2, 19.99f, "Cascaval"),
		creareProdus(3, 99.99f, "Tastatura"),
		creareProdus(4, 2000.0f, "Nokia")
	};

	Vector v = creareVector(nrElemente, produse);
	afisareVector(v);

	printf("=========================================== Dupa inserarea la inceput ===========================================\n\n");

	Produs p = creareProdus(5, 5.5f, "Costita");

	v = pushFront(v, p);
	afisareVector(v);

	dezalocareProdus(p);
	dezalocareVector(v);

	return 0;
}