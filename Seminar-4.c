#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct Produs Produs;
typedef struct NodDublu NodDublu;
typedef struct ListaDubla ListaDubla;

struct Produs
{
	int cod;
	float pret;
	char* denumire;
};

struct NodDublu
{
	Produs info; // informatia utila
	NodDublu* next; // pointerul de legatura next
	NodDublu* prev; // pointerul de legatura prev
};

struct ListaDubla
{
	int nrNoduri; // echivalent cu numarul de elemente din lista
	NodDublu* first;
	NodDublu* last;
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

/* Functii pentru structura NodDublu */

NodDublu* creareNodDublu(const Produs info, NodDublu* next, NodDublu* prev)
{
	NodDublu* noulNod = (NodDublu*)malloc(sizeof(NodDublu));

	if (noulNod == NULL)
	{
		return noulNod;
	}

	noulNod->next = next;
	noulNod->prev = prev;
	noulNod->info = copiazaProdus(info);

	return noulNod;
}

void dezalocareNodDublu(NodDublu* nod)
{
	if (nod == NULL)
	{
		return;
	}

	nod->next = NULL;
	nod->prev = NULL;
	dezalocareProdus(nod->info);
	free(nod);
}

void afisareNodDublu(NodDublu* nod)
{
	if (nod == NULL)
	{
		return;
	}

	afisareProdus(nod->info);
}

ListaDubla initializareListaDubla()
{
	ListaDubla lista = { .first = NULL, .last = NULL, .nrNoduri = 0 };
	return lista;
}

bool isEmptyList(const ListaDubla lista)
{
	return lista.first == NULL && lista.last == NULL;
}

ListaDubla inserareNodLaInceput(ListaDubla lista, const Produs info)
{
	NodDublu* noulNod = creareNodDublu(info, NULL, NULL);

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

	lista.first->prev = noulNod;
	noulNod->next = lista.first;
	lista.first = noulNod;
	lista.nrNoduri++;

	return lista;
}

ListaDubla inserareNodLaSfarsit(ListaDubla lista, const Produs info)
{
	NodDublu* noulNod = creareNodDublu(info, NULL, NULL);

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
	noulNod->prev = lista.last;
	lista.last = noulNod;
	lista.nrNoduri++;

	return lista;
}

ListaDubla stergereNodLaInceput(ListaDubla lista)
{
	if (isEmptyList(lista))
	{
		return lista;
	}

	if (lista.first == lista.last)
	{
		dezalocareNodDublu(lista.first);
		lista.first = NULL;
		lista.last = NULL;
		lista.nrNoduri = 0;

		return lista;
	}

	NodDublu* nodDeSters = lista.first;
	lista.first = lista.first->next;
	lista.first->prev = NULL;
	dezalocareNodDublu(nodDeSters);
	lista.nrNoduri--;

	return lista;
}

ListaDubla stergereNodLaSfarsit(ListaDubla lista)
{
	if (isEmptyList(lista))
	{
		return lista;
	}

	if (lista.first == lista.last)
	{
		dezalocareNodDublu(lista.first);
		lista.first = NULL;
		lista.last = NULL;
		lista.nrNoduri = 0;

		return lista;
	}

	NodDublu* nodDeSters = lista.last;
	lista.last = lista.last->prev;
	lista.last->next = NULL;
	dezalocareNodDublu(nodDeSters);
	lista.nrNoduri--;

	return lista;
}

ListaDubla citireListaDinFisier(const char* denumireFisier)
{
	ListaDubla lista = initializareListaDubla();
	FILE* file = fopen(denumireFisier, "r");

	if (file == NULL)
	{
		return lista;
	}

	while (!feof(file))
	{
		Produs p = citireProdusDinFisier(file);
		lista = inserareNodLaInceput(lista, p);
		dezalocareProdus(p);
	}

	fclose(file); // trebuie sa inchidem fisierul dupa ce nu ne mai folosim de el
	return lista;
}

void afisareListaDubla(const ListaDubla lista)
{
	if (isEmptyList(lista))
	{
		printf("Lista este goala, nu avem ce afisa!\n\n");
		return;
	}

	NodDublu* current = lista.first;

	while (current != NULL)
	{
		afisareNodDublu(current);
		current = current->next;
	}
}

void afisareListaDublaInvers(const ListaDubla lista)
{
	if (isEmptyList(lista))
	{
		printf("Lista este goala, nu avem ce afisa!\n\n");
		return;
	}

	NodDublu* current = lista.last;

	while (current != NULL)
	{
		afisareNodDublu(current);
		current = current->prev;
	}
}

void dezalocareListaDubla(ListaDubla* lista)
{
	if (lista == NULL || isEmptyList(*lista))
	{
		return;
	}

	NodDublu* aux = NULL;

	while (lista->first != NULL)
	{
		aux = lista->first;
		lista->first = lista->first->next;
		dezalocareNodDublu(aux);
		lista->nrNoduri--;
	}

	lista->last = NULL;
}

int main()
{
	ListaDubla lista = citireListaDinFisier("produse.txt");

	afisareListaDubla(lista);
	printf("================================================== Dupa afisarea invers ==================================================\n\n");
	afisareListaDublaInvers(lista);

	Produs p1 = creareProdus(888, 100, "Incaltaminte sport");
	Produs p2 = creareProdus(2000, 55.65, "Camasa albastra");

	lista = inserareNodLaSfarsit(lista, p1);
	lista = inserareNodLaInceput(lista, p2);

	printf("\n\n================================================== Dupa inserari ==================================================\n\n");

	afisareListaDubla(lista);
	printf("\n\n================================================== Dupa afisarea invers ==================================================\n\n");
	afisareListaDublaInvers(lista);

	printf("\n\n================================================== Dupa stergerea la inceput ==================================================\n\n");

	lista = stergereNodLaInceput(lista);

	afisareListaDubla(lista);
	printf("================================================== Dupa afisarea invers ==================================================\n\n");
	afisareListaDublaInvers(lista);

	printf("\n\n================================================== Dupa stergerea la sfarsit ==================================================\n\n");

	lista = stergereNodLaSfarsit(lista);

	afisareListaDubla(lista);
	printf("================================================== Dupa afisarea invers ==================================================\n\n");
	afisareListaDublaInvers(lista);

	dezalocareListaDubla(&lista);
	dezalocareProdus(p1);
	dezalocareProdus(p2);

	return 0;
}