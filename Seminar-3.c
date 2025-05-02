#include <ctype.h>
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

char* trim(char* sir)
{
	if (sir == NULL)
	{
		return sir;
	}

	char* start = sir;

	while (isspace(*start))
	{
		start++;
	}

	char* final = sir + strlen(sir) - 1;

	while (final > sir && isspace(*final))
	{
		final--;
	}

	sir = start;
	*(final + 1) = '\0';

	return sir;
}

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

Produs citireProdusDinFisier(FILE* f)
{
	char linie[256];
	Produs produs = initializareProdus();

	if (fgets(linie, sizeof(linie), f))
	{
		char* token = trim(strtok(linie, ","));
		produs.cod = atoi(token);

		token = trim(strtok(NULL, ","));
		produs.pret = (float)atof(token);

		token = trim(strtok(NULL, ","));
		produs.denumire = (char*)malloc((1 + strlen(token) * sizeof(char)));
		strcpy(produs.denumire, token);
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
		afisareNodSimplu(aux);
		aux = aux->next;
	}

	// echivalent dar cu bucla for

	/*for (NodSimplu* aux = lista.first; aux != NULL; aux = aux->next)
	{
		afisareNodSimplu(aux);
	}*/
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

ListaSimpla inserareNodLaInceput(ListaSimpla lista, const Produs info)
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

	noulNod->next = lista.first;
	lista.first = noulNod;
	lista.nrNoduri++;

	return lista;
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

ListaSimpla inserareNodDupaFirst(ListaSimpla lista, const Produs info)
{
	if (isEmptyList(lista))
	{
		lista = inserareNodLaInceput(lista, info);
		return lista;
	}

	if (lista.first == lista.last)
	{
		lista = inserareNodLaFinal(lista, info);
		return lista;
	}

	NodSimplu* noulNod = creareNodSimplu(info, lista.first->next);

	if (noulNod == NULL)
	{
		return lista;
	}

	lista.first->next = noulNod;
	lista.nrNoduri++;

	return lista;
}

ListaSimpla inserareNodInainteDeLast(ListaSimpla lista, const Produs info)
{
	if (isEmptyList(lista))
	{
		lista = inserareNodLaInceput(lista, info);
		return lista;
	}

	if (lista.first == lista.last)
	{
		lista = inserareNodLaFinal(lista, info);
		return lista;
	}

	NodSimplu* noulNod = creareNodSimplu(info, lista.last);

	if (noulNod == NULL)
	{
		return lista;
	}

	NodSimplu* aux = lista.first;

	while (aux->next != lista.last)
	{
		aux = aux->next;
	}

	aux->next = noulNod;
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

ListaSimpla stergereNodLaFinal(ListaSimpla lista)
{
	if (isEmptyList(lista))
	{
		return lista;
	}

	if (lista.first == lista.last)
	{
		dezalocareNodSimplu(lista.first);
		lista.first = NULL;
		lista.last = NULL;
		lista.nrNoduri = 0;

		return lista;
	}

	NodSimplu* aux = lista.first;

	while (aux->next != lista.last)
	{
		aux = aux->next;
	}

	NodSimplu* nodDeSters = lista.last;
	aux->next = NULL;
	lista.last = aux;
	dezalocareNodSimplu(nodDeSters);
	lista.nrNoduri--;

	return lista;
}

ListaSimpla stergereNodInainteDeLast(ListaSimpla lista)
{
	if (isEmptyList(lista))
	{
		return lista;
	}

	if (lista.first == lista.last)
	{
		dezalocareNodSimplu(lista.first);
		lista.first = NULL;
		lista.last = NULL;
		lista.nrNoduri = 0;

		return lista;
	}

	if (lista.first->next == lista.last) // mai avem fix 2 noduri in lista
	{
		lista = stergereNodLaInceput(lista);
		return lista;
	}

	NodSimplu* aux = lista.first;

	while (aux->next->next != lista.last)
	{
		aux = aux->next;
	}

	NodSimplu* nodDeSters = aux->next;
	aux->next = lista.last;
	dezalocareNodSimplu(nodDeSters);
	lista.nrNoduri--;

	return lista;
}

ListaSimpla inserareNodOriunde(ListaSimpla lista, const Produs info, const int index)
{
	if (index < 0 || index > lista.nrNoduri) // index invalid (nu putem insera in lista)
	{
		return lista;
	}

	if (index == 0) // inserare inainte de first
	{
		lista = inserareNodLaInceput(lista, info);
		return lista;
	}

	if (index == lista.nrNoduri) // inserare dupa last
	{
		lista = inserareNodLaFinal(lista, info);
		return lista;
	}

	NodSimplu* aux = lista.first;

	for (int i = 0; i < index - 1; i++)
	{
		aux = aux->next;
	}

	NodSimplu* noulNod = creareNodSimplu(info, aux->next);

	if (noulNod == NULL)
	{
		return lista;
	}

	aux->next = noulNod;
	lista.nrNoduri++;

	return lista;
}

ListaSimpla citireListaDinFisier(const char* fisier)
{
	ListaSimpla lista = initializareListaSimpla();
	FILE* f = fopen(fisier, "r");

	if (f == NULL)
	{
		return lista;
	}

	while(!feof(f))
	{
		Produs p = citireProdusDinFisier(f);
		lista = inserareNodLaFinal(lista, p);
		dezalocareProdus(p);
	}

	fclose(f);
	return lista;
}

void inversareListaSimpla(ListaSimpla* lista)
{
	if (isEmptyList(*lista))
	{
		return;
	}

	if (lista->first == lista->last) // daca lista are doar un nod nu o inversam
	{
		return;
	}

	NodSimplu* prev = NULL;
	NodSimplu* next = NULL;
	NodSimplu* current = lista->first;

	while (current != NULL)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}

	lista->last = lista->first;
	lista->first = prev;
}

int main()
{
	ListaSimpla lista = citireListaDinFisier("produse.txt");

	printf("======================================= Dupa citirea listei din fisier =======================================\n\n");
	afisareListaSimpla(lista);

	Produs p1 = creareProdus(12, 55.5f, "Mouse");
	Produs p2 = creareProdus(8, 5.5f, "Ciocolata");
	Produs p3 = creareProdus(5, 25.0f, "Bere");
	Produs p4 = creareProdus(19, 10.0f, "Salam");
	Produs p5 = creareProdus(1, 2.5f, "Paine");
	Produs p6 = creareProdus(11, 89.99f, "Camera Web");
	Produs p7 = creareProdus(20, 7.5f, "Lapte");
	Produs p8 = creareProdus(22, 6.5f, "Pateu");

	printf("======================================= Dupa inserarea la final =======================================\n\n");

	lista = inserareNodLaFinal(lista, p1);
	lista = inserareNodLaFinal(lista, p2);
	lista = inserareNodLaFinal(lista, p3);
	lista = inserareNodLaFinal(lista, p4);

	afisareListaSimpla(lista);

	printf("======================================= Dupa inserarea la inceput =======================================\n\n");

	lista = inserareNodLaInceput(lista, p6);
	afisareListaSimpla(lista);

	printf("======================================= Inserarea unui nod dupa first =======================================\n\n");

	lista = inserareNodDupaFirst(lista, p7);
	afisareListaSimpla(lista);

	printf("======================================= Inserarea unui nod inainte de last =======================================\n\n");

	lista = inserareNodInainteDeLast(lista, p8);
	afisareListaSimpla(lista);

	printf("======================================= Dupa inserarea oriunde =======================================\n\n");

	lista = inserareNodOriunde(lista, p5, 3);
	afisareListaSimpla(lista);

	printf("======================================= Dupa inversarea listei =======================================\n\n");

	inversareListaSimpla(&lista);
	afisareListaSimpla(lista);

	printf("======================================= Dupa stergerea la inceput =======================================\n\n");

	lista = stergereNodLaInceput(lista);
	afisareListaSimpla(lista);

	printf("======================================= Dupa stergerea la final =======================================\n\n");

	lista = stergereNodLaFinal(lista);
	afisareListaSimpla(lista);

	printf("======================================= Dupa stergerea inainte de last =======================================\n\n");

	lista = stergereNodInainteDeLast(lista);
	afisareListaSimpla(lista);
	dezalocareListaSimpla(&lista);

	dezalocareProdus(p1);
	dezalocareProdus(p2);
	dezalocareProdus(p3);
	dezalocareProdus(p4);
	dezalocareProdus(p5);
	dezalocareProdus(p6);
	dezalocareProdus(p7);
	dezalocareProdus(p8);

	return 0;
}