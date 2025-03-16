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

Produs creareProdus(const int cod, const float pret, const char* denumire)
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

bool vectorIsEmpty(const Vector v)
{
	return v.elemente == NULL && v.dimensiune <= 0;
}

void dezalocareVector(Vector* v)
{
	if (vectorIsEmpty(*v))
	{
		return;
	}

	for (int i = 0; i < v->dimensiune; i++)
	{
		dezalocareProdus(v->elemente[i]);
	}

	free(v->elemente);

	v->dimensiune = 0;
	v->elemente = NULL;
}

void afisareVector(const Vector v)
{
	if (vectorIsEmpty(v))
	{
		printf("Vectorul este gol nu avem ce afisa!\n\n");
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
	if (vectorIsEmpty(v))
	{
		v.elemente = (Produs*)malloc(sizeof(Produs));
		v.elemente[0] = copiazaProdus(produs);
		v.dimensiune = 1;

		return v;
	}

	Produs* aux = v.elemente;
	int dimensiune = v.dimensiune;

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

Vector pushBack(Vector v, const Produs produs)
{
	if (vectorIsEmpty(v))
	{
		v.elemente = (Produs*)malloc(sizeof(Produs));
		v.elemente[0] = copiazaProdus(produs);
		v.dimensiune = 1;

		return v;
	}

	Produs* aux = v.elemente;
	int dimensiune = v.dimensiune;

	v.elemente = (Produs*)malloc((dimensiune + 1) * sizeof(Produs));
	v.elemente[dimensiune] = copiazaProdus(produs);

	for (int i = 0; i < dimensiune; i++)
	{
		v.elemente[i] = aux[i];
	}

	v.dimensiune++;
	free(aux);

	return v;
}

Vector popFront(Vector v)
{
	if (vectorIsEmpty(v))
	{
		return v;
	}

	if (v.elemente != NULL && v.dimensiune == 1)
	{
		dezalocareProdus(v.elemente[0]);
		free(v.elemente);

		v.dimensiune = 0;
		v.elemente = NULL;

		return v;
	}

	Produs* aux = v.elemente;
	int dimensiune = v.dimensiune;

	v.elemente = (Produs*)malloc((dimensiune - 1) * sizeof(Produs));

	for (int i = 1; i < dimensiune; i++)
	{
		v.elemente[i - 1] = aux[i];
	}

	v.dimensiune--;
	dezalocareProdus(aux[0]);
	free(aux);

	return v;
}

Vector popBack(Vector v)
{
	if (vectorIsEmpty(v))
	{
		return v;
	}

	if (v.elemente != NULL && v.dimensiune == 1)
	{
		dezalocareProdus(v.elemente[0]);
		free(v.elemente);

		v.dimensiune = 0;
		v.elemente = NULL;

		return v;
	}

	Produs* aux = v.elemente;
	int dimensiune = v.dimensiune;

	v.elemente = (Produs*)malloc((dimensiune - 1) * sizeof(Produs));

	for (int i = 0; i < dimensiune - 1; i++)
	{
		v.elemente[i] = aux[i];
	}

	v.dimensiune--;
	dezalocareProdus(aux[dimensiune - 1]);
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

	Produs p1 = creareProdus(5, 5.5f, "Costita");

	v = pushFront(v, p1);
	afisareVector(v);

	printf("=========================================== Dupa inserarea la sfarsit ===========================================\n\n");

	Produs p2 = creareProdus(6, 13.5f, "Hrean");

	v = pushBack(v, p2);
	afisareVector(v);

	printf("=========================================== Dupa eliminarea la inceput ===========================================\n\n");

	v = popFront(v);
	afisareVector(v);

	printf("=========================================== Dupa eliminarea la sfarsit ===========================================\n\n");

	v = popBack(v);
	afisareVector(v);

	dezalocareProdus(p1);
	dezalocareProdus(p2);
	dezalocareVector(&v);

	return 0;
}