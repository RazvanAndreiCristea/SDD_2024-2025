#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct Produs Produs;
typedef struct NodSimplu NodSimplu;
typedef struct ListaSimpla ListaSimpla;

struct Produs
{
	int cod;
	float pret;
	char* denumire;
};

struct NodSimplu
{
	Produs info; // informatia utila
	NodSimplu* next; // pointerul de legatura cu alt nod
};

struct ListaSimpla
{
	int nrNoduri; // echivalent cu numarul de elemente din lista
	NodSimplu* first;
	NodSimplu* last;
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

/* Functii pentru structura NodSimplu */

NodSimplu* creareNodSimplu(const Produs info, NodSimplu* next)
{
	NodSimplu* noulNod = (NodSimplu*)malloc(sizeof(NodSimplu));

	if (noulNod == NULL)
	{
		return noulNod;
	}

	noulNod->next = next;
	noulNod->info = copiazaProdus(info);

	return noulNod;
}

void afisareNodSimplu(NodSimplu* nod)
{
	if (nod == NULL)
	{
		return;
	}

	afisareProdus(nod->info);
}

void dezalocareNodSimplu(NodSimplu* nod)
{
	if (nod == NULL)
	{
		return;
	}

	dezalocareProdus(nod->info);
	free(nod);
}

/* Functii pentru structura ListaSimpla */

ListaSimpla initializareListaSimpla()
{
	ListaSimpla lista = { .first = NULL, .last = NULL, .nrNoduri = 0 };
	return lista;
}

bool isEmptyList(const ListaSimpla lista)
{
	return lista.first == NULL && lista.last == NULL;
}

void afisareListaSimpla(const ListaSimpla lista)
{
	if (isEmptyList(lista))
	{
		printf("Lista este goala nu avem ce afisa!\n\n");
		return;
	}

	NodSimplu* aux = lista.first;

	while (aux != NULL)
	{
		afisareProdus(aux->info);
		aux = aux->next;
	}
}

void dezalocareListaSimpla(ListaSimpla* lista)
{
	if (lista == NULL || isEmptyList(*lista))
	{
		return;
	}

	while (lista->first != NULL)
	{
		NodSimplu* aux = lista->first;
		lista->first = aux->next;
		dezalocareNodSimplu(aux);
		lista->nrNoduri--;
	}

	lista->last = NULL;
}

ListaSimpla inserareNodLaFinal(ListaSimpla lista, const Produs info)
{
	NodSimplu* noulNod = creareNodSimplu(info, NULL);

	if (noulNod == NULL)
	{
		return lista;
	}

	if (isEmptyList(lista))
	{
		lista.first = noulNod;
		lista.last = noulNod;
		lista.nrNoduri = 1;

		return lista;
	}

	lista.last->next = noulNod;
	lista.last = noulNod;
	lista.nrNoduri++;

	return lista;
}

ListaSimpla stergereNodLaInceput(ListaSimpla lista)
{
	if (isEmptyList(lista))
	{
		return lista;
	}

	if (lista.first == lista.last)
	{
		dezalocareNodSimplu(lista.first); // sau dezalocareNodSimplu(lista.last); // sunt corecte ambele doar in aceasta situatie
		lista.first = NULL;
		lista.last = NULL;
		lista.nrNoduri = 0;

		return lista;
	}

	NodSimplu* aux = lista.first;
	lista.first = lista.first->next;
	dezalocareNodSimplu(aux);
	lista.nrNoduri--;

	return lista;
}

/*
* sa se scrie o functie care insereaza un nod inainte de first
* sa se scrie o functie care insereaza un nod dupa first
* sa se scrie o functie care insereaza un nod inainte de last
* sa se scrie o functie care sterge ultimul nod (last)
* sa se scrie o functie care sterge nodul de dinainte de last
*/

int main()
{
	ListaSimpla lista = initializareListaSimpla();

	Produs p1 = creareProdus(12, 55.5f, "Mouse");
	Produs p2 = creareProdus(8, 5.5f, "Ciocolata");
	Produs p3 = creareProdus(5, 25.0f, "Bere");
	Produs p4 = creareProdus(19, 10.0f, "Salam");

	afisareListaSimpla(lista);

	printf("======================================= Dupa inserarea la final =======================================\n\n");

	lista = inserareNodLaFinal(lista, p1);
	lista = inserareNodLaFinal(lista, p2);
	lista = inserareNodLaFinal(lista, p3);
	lista = inserareNodLaFinal(lista, p4);

	afisareListaSimpla(lista);

	printf("======================================= Dupa stergerea la inceput =======================================\n\n");

	lista = stergereNodLaInceput(lista);
	lista = stergereNodLaInceput(lista);
	lista = stergereNodLaInceput(lista);
	lista = stergereNodLaInceput(lista);

	afisareListaSimpla(lista);
	dezalocareListaSimpla(&lista);

	dezalocareProdus(p1);
	dezalocareProdus(p2);
	dezalocareProdus(p3);
	dezalocareProdus(p4);

	return 0;
}