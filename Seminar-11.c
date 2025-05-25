#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct Oras Oras;
typedef struct Graf Graf;
typedef struct NodAdiacenta NodAdiacenta;
typedef struct ListaAdiacenta ListaAdiacenta;

struct Oras
{
	int id;
	char* denumire;
};

struct NodAdiacenta
{
	int idVecin;
	NodAdiacenta* next;
};

struct ListaAdiacenta
{
	Oras info;
	NodAdiacenta* vecini;
};

struct Graf
{
	int nrOrase;
	ListaAdiacenta* liste; // vector de liste
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

/* Functii pentru structura Oras */

Oras initializareOras()
{
	Oras Oras = { .id = 0, .denumire = NULL };
	return Oras;
}

Oras citireOrasDinFisier(FILE* f)
{
	char linie[256];
	Oras oras = initializareOras();

	if (fgets(linie, sizeof(linie), f))
	{
		char* token = trim(strtok(linie, ","));
		oras.id = atoi(token);

		token = trim(strtok(NULL, ","));
		oras.denumire = (char*)malloc((1 + strlen(token) * sizeof(char)));
		strcpy(oras.denumire, token);
	}

	return oras;
}

Oras copiazaOras(const Oras original)
{
	Oras copie = initializareOras();

	copie.id = original.id;

	if (original.denumire != NULL)
	{
		copie.denumire = (char*)malloc((1 + strlen(original.denumire)) * sizeof(char));
		strcpy(copie.denumire, original.denumire);
	}
	else
	{
		copie.denumire = NULL;
	}

	return copie;
}

void dezalocareOras(Oras* oras)
{
	if (oras->denumire != NULL)
	{
		free(oras->denumire);
		oras->denumire = NULL;
	}
}

void afisareOras(const Oras oras)
{
	printf("Codul Orasului este: %d\nDenumirea orasului este: %s\n\n",
		oras.id, oras.denumire);
}

/* Functii pentru structura NodAdiacenta */

NodAdiacenta* creareNod(const int idVecin)
{
	NodAdiacenta* noulNod = (NodAdiacenta*)malloc(sizeof(NodAdiacenta));

	if (noulNod == NULL)
	{
		return noulNod;
	}

	noulNod->next = NULL;
	noulNod->idVecin = idVecin;

	return noulNod;
}

void dezalocareNod(NodAdiacenta* nod)
{
	if (nod == NULL)
	{
		return;
	}

	free(nod);
}

/* Functii pentru structura ListaAdiacenta */

ListaAdiacenta initializareLista()
{
	ListaAdiacenta lista = { .info = initializareOras(), .vecini = NULL };
	return lista;
}

ListaAdiacenta adaugareVecin(ListaAdiacenta lista, const int idVecin)
{
	NodAdiacenta* noulNod = creareNod(idVecin);

	if (noulNod == NULL)
	{
		return lista;
	}

	noulNod->next = lista.vecini;
	lista.vecini = noulNod;

	return lista;
}

void afisareListaAdiacenta(const ListaAdiacenta* lista, ListaAdiacenta* toateListele)
{
	if (lista == NULL || lista->vecini == NULL)
	{
		return;
	}

	afisareOras(lista->info);

	NodAdiacenta* current = lista->vecini;

	printf("Vecinii directi pentru orasul %s sunt: ", lista->info.denumire);

	while (current != NULL)
	{
		printf("-> %s", toateListele[current->idVecin].info.denumire);
		current = current->next;
	}
}

void dezalocareLista(ListaAdiacenta* lista)
{
	if (lista == NULL)
	{
		return;
	}

	dezalocareOras(&lista->info);

	NodAdiacenta* aux = NULL;

	while (lista->vecini != NULL)
	{
		aux = lista->vecini;
		lista->vecini = lista->vecini->next;
		dezalocareNod(aux);
	}
}

/* Functii pentru structura Graf */

Graf initializareGraf()
{
	Graf graf = { .nrOrase = 0, .liste = NULL };
	return graf;
}

Graf citireGrafDinFisier(const char* denumireFisier)
{
	Graf graf = initializareGraf();
	FILE* file = fopen(denumireFisier, "r");

	if (file == NULL)
	{
		return graf;
	}

	int dimensiune = 0;
	fscanf(file, "%d", &dimensiune);

	int character;
	while ((character = fgetc(file)) != '\n' && character != EOF);

	graf.nrOrase = dimensiune;
	graf.liste = (ListaAdiacenta*)malloc(dimensiune * sizeof(ListaAdiacenta));

	for (int i = 0; i < dimensiune; i++)
	{
		Oras oras = citireOrasDinFisier(file);
		graf.liste[i] = initializareLista();
		graf.liste[i].info = copiazaOras(oras);
		dezalocareOras(&oras);
	}

	fclose(file);
	return graf;
}

Graf adaugareMuchie(Graf graf, const int sursa, const int destinatie)
{
	if (sursa >= graf.nrOrase || destinatie >= graf.nrOrase)
	{
		return graf;
	}

	graf.liste[sursa] = adaugareVecin(graf.liste[sursa], destinatie);
	graf.liste[destinatie] = adaugareVecin(graf.liste[destinatie], sursa);

	return graf;
}

void afisareGraf(const Graf graf)
{
	if (graf.liste == NULL)
	{
		return;
	}

	for (int i = 0; i < graf.nrOrase; i++)
	{
		afisareListaAdiacenta(&graf.liste[i], graf.liste);
		printf("\n\n");
	}
}

void dezalocareGraf(Graf* graf)
{
	if (graf == NULL || graf->liste == NULL)
	{
		return;
	}

	for (int i = 0; i < graf->nrOrase; i++)
	{
		dezalocareLista(&graf->liste[i]);
	}

	free(graf->liste);
	graf->liste = NULL;
}

int main()
{
	Graf graf = citireGrafDinFisier("orase.txt");

	graf = adaugareMuchie(graf, 0, 1);
	graf = adaugareMuchie(graf, 0, 2);
	graf = adaugareMuchie(graf, 0, 3);
	graf = adaugareMuchie(graf, 4, 5);
	graf = adaugareMuchie(graf, 3, 7);
	graf = adaugareMuchie(graf, 6, 0);
	graf = adaugareMuchie(graf, 4, 2);
	graf = adaugareMuchie(graf, 1, 6);

	afisareGraf(graf);
	dezalocareGraf(&graf);

	return 0;
}