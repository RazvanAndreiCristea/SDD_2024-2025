#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct Produs Produs;
typedef struct NodSimplu NodSimplu;
typedef struct HashTable HashTable;
typedef struct ListaSimpla ListaSimpla;

struct Produs
{
	int cod;
	float pret;
	char* denumire;
};

struct NodSimplu
{
	Produs info;
	NodSimplu* next;
};

struct ListaSimpla
{
	int nrNoduri;
	NodSimplu* first;
	NodSimplu* last;
};

struct HashTable
{
	int dimensiune;
	ListaSimpla* vector; // vector de liste simple
};

char* trim(char* sir)
{
	if (sir == NULL)
	{
		return sir;
	}

	while (isspace(*sir))
	{
		sir++;
	}

	char* end = sir + (strlen(sir)) - 1;

	while (end > sir && isspace(*end))
	{
		end--;
	}

	*(end + 1) = '\0';

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

	if (fgets(linie, sizeof(linie), f)) // citim linia complet din fisier ca string
	{
		char* token = strtok(linie, ",");
		produs.cod = atoi(token);

		token = strtok(NULL, ",");
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

ListaSimpla initializareLista()
{
	ListaSimpla lista = { .first = NULL, .last = NULL, .nrNoduri = 0 };
	return lista;
}

bool isEmptyList(const ListaSimpla lista)
{
	return lista.first == NULL && lista.last == NULL;
}

ListaSimpla inserareNod(ListaSimpla lista, const Produs info)
{
	NodSimplu* noulNod = creareNodSimplu(info, NULL);

	if (noulNod == NULL)
	{
		return lista;
	}

	if (isEmptyList(lista))
	{
		lista.first = lista.last = noulNod;
		lista.nrNoduri = 1;

		return lista;
	}

	lista.last->next = noulNod;
	lista.last = noulNod;
	lista.nrNoduri++;

	return lista;
}

void afisareListaSimpla(const ListaSimpla lista)
{
	if (isEmptyList(lista))
	{
		printf("Lista este goala!\n\n");
		return;
	}

	NodSimplu* aux = lista.first;

	while (aux != NULL)
	{
		afisareNodSimplu(aux);
		aux = aux->next;
	}
}

void dezalocareListaSimpla(ListaSimpla* lista)
{
	if (lista == NULL || isEmptyList(*lista))
	{
		return;
	}

	NodSimplu* aux = NULL;

	while (lista->first != NULL)
	{
		aux = lista->first;
		lista->first = aux->next;
		dezalocareNodSimplu(aux);
		lista->nrNoduri--;
	}

	lista->last = NULL;
}

/* Functii pentru structura HashTable */

int getHashCode(const int dimensiune, const char* denumireProdus)
{
	int hashCode = strlen(denumireProdus) % dimensiune;
	return hashCode;
}

HashTable initializareHashTable()
{
	HashTable tabela = { .dimensiune = 0, .vector = NULL };
	return tabela;
}

bool isEmptyHashTable(const HashTable tabela)
{
	return tabela.vector == NULL;
}

HashTable inserareProdus(HashTable tabela, const Produs produs)
{
	if (isEmptyHashTable(tabela))
	{
		return tabela;
	}

	int index = getHashCode(tabela.dimensiune, produs.denumire);
	tabela.vector[index] = inserareNod(tabela.vector[index], produs);

	return tabela;
}

HashTable citireTabelaDinFisier(const int dimensiune, const char* numeFisier)
{
	HashTable tabela = initializareHashTable();

	tabela.dimensiune = dimensiune;
	tabela.vector = (ListaSimpla*)malloc(dimensiune * sizeof(ListaSimpla));

	for (int i = 0; i < tabela.dimensiune; i++)
	{
		tabela.vector[i] = initializareLista();
	}

	FILE* file = fopen(numeFisier, "r");

	while (!feof(file))
	{
		Produs p = citireProdusDinFisier(file);
		tabela = inserareProdus(tabela, p);
		dezalocareProdus(p);
	}

	fclose(file);

	return tabela;
}

void afisareTabela(const HashTable tabela)
{
	if (isEmptyHashTable(tabela))
	{
		printf("Tabela este goala\n\n");
		return;
	}

	for (int i = 0; i < tabela.dimensiune; i++)
	{
		if (isEmptyList(tabela.vector[i]))
		{
			continue;
		}

		printf("Lista de pe pozitia %d este afisata mai jos.\n\n", i + 1);
		afisareListaSimpla(tabela.vector[i]);
	}
}

void dezalocareHashTable(HashTable* tabela)
{
	if (tabela == NULL || isEmptyHashTable(*tabela))
	{
		return;
	}

	for (int i = 0; i < tabela->dimensiune; i++)
	{
		dezalocareListaSimpla(&tabela->vector[i]);
	}

	free(tabela->vector);

	tabela->vector = NULL;
	tabela->dimensiune = 0;
}

int main()
{
	HashTable tabela = citireTabelaDinFisier(5, "produse.txt");
	afisareTabela(tabela);
	dezalocareHashTable(&tabela);

	return 0;
}