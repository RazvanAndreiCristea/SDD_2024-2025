#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct Produs Produs;
typedef struct NodSimplu NodSimplu;
typedef struct ListaSimplaCirculara ListaSimplaCirculara;

struct Produs
{
	int cod;
	float pret;
	char* denumire;
};

struct NodSimplu
{
	Produs info; // informatia utila
	NodSimplu* next; // pointerul de legatura next
};

struct ListaSimplaCirculara
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

Produs citireProdusDinFisier(FILE* file)
{
	char linie[256];
	Produs p = initializareProdus();

	if (fgets(linie, sizeof(linie), file))
	{
		char* token = strtok(linie, ",");
		p.cod = atoi(token);

		token = strtok(NULL, ",");
		p.pret = (float)atof(token);

		token = strtok(NULL, ",");
		p.denumire = (char*)malloc((1 + strlen(token) * sizeof(char)));
		strcpy(p.denumire, token);
	}

	return p;
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

	//nod->next = NULL;
	dezalocareProdus(nod->info);
	free(nod);
}

/* Functii pentru structura ListaSimplaCirculara */

ListaSimplaCirculara initializareListaSimplaCirculara()
{
	ListaSimplaCirculara lista = { .first = NULL, .last = NULL, .nrNoduri = 0 };
	return lista;
}

bool isEmptyList(ListaSimplaCirculara lista)
{
	return lista.first == NULL && lista.last == NULL;
}

ListaSimplaCirculara inserareNod(ListaSimplaCirculara lista, const Produs info)
{
	NodSimplu* noulNod = creareNodSimplu(info, NULL);

	if (isEmptyList(lista))
	{
		noulNod->next = noulNod;
		lista.first = noulNod;
		lista.last = noulNod;
		lista.nrNoduri = 1;

		return lista;
	}

	noulNod->next = lista.first;
	lista.last->next = noulNod;
	lista.last = noulNod;
	lista.nrNoduri++;

	return lista;
}

ListaSimplaCirculara stergereNod(ListaSimplaCirculara lista)
{
	if (isEmptyList(lista))
	{
		return lista;
	}

	if (lista.first == lista.last)
	{
		dezalocareNodSimplu(lista.first);
		lista.first = lista.last = NULL;
		lista.nrNoduri = 0;

		return lista;
	}

	NodSimplu* nodDeSters = lista.first;
	lista.first = lista.first->next;
	lista.last->next = lista.first;
	dezalocareNodSimplu(nodDeSters);
	lista.nrNoduri--;

	return lista;
}

ListaSimplaCirculara citireListaDinFisier(const char* denumireFisier)
{
	ListaSimplaCirculara lista = initializareListaSimplaCirculara();
	FILE* file = fopen(denumireFisier, "r");

	if (file == NULL)
	{
		return lista;
	}

	while (!feof(file))
	{
		Produs p = citireProdusDinFisier(file);
		lista = inserareNod(lista, p);
		dezalocareProdus(p);
	}

	fclose(file);
	return lista;
}

void afisareListaSimplaCirculara(ListaSimplaCirculara lista)
{
	if (isEmptyList(lista))
	{
		printf("Lista este goala, nu avem ce afisa!\n\n");
		return;
	}

	NodSimplu* current = lista.first;

	do
	{
		afisareNodSimplu(current);
		current = current->next;
	} while (current != lista.first);
}

void dezalocareListaSimplaCirculara(ListaSimplaCirculara* lista)
{
	if (isEmptyList(*lista))
	{
		return;
	}

	lista->last->next = NULL; // rupem circularitatea (echivalent cu a transforma lista circulara intr-una linara)
	NodSimplu* nodDeSters = NULL;

	while (lista->first != NULL)
	{
		nodDeSters = lista->first;
		lista->first = lista->first->next;
		dezalocareNodSimplu(nodDeSters);
		lista->nrNoduri--;
	}

	lista->last = NULL;
}

int main()
{
	ListaSimplaCirculara lista = citireListaDinFisier("produse.txt");
	afisareListaSimplaCirculara(lista);

	printf("======================================== Dupa eliminarea unui nod ========================================\n\n");

	lista = stergereNod(lista);
	afisareListaSimplaCirculara(lista);
	dezalocareListaSimplaCirculara(&lista);

	return 0;
}