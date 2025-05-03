#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct BST BST;
typedef struct Produs Produs;
typedef struct NodArbore NodArbore;
typedef enum TipAfisare TipAfisare;

enum TipAfisare { kPREORDINE, kINORDINE, kPOSTORDINE };

struct Produs
{
	int cod;
	float pret;
	char* denumire;
};

struct NodArbore
{
	Produs info;
	int inaltime;
	NodArbore* stanga;
	NodArbore* dreapta;
};

struct BST
{
	int nrNoduri;
	int inaltime;
	NodArbore* radacina;
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

int maxim(const int x, const int y)
{
	return (x > y) ? x : y;
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

/* Functii pentru structura NodArbore */

NodArbore* creareNodArbore(const Produs info)
{
	NodArbore* noulNod = (NodArbore*)malloc(sizeof(NodArbore));

	if (noulNod == NULL)
	{
		return NULL;
	}

	noulNod->inaltime = 1;
	noulNod->stanga = NULL;
	noulNod->dreapta = NULL;
	noulNod->info = copiazaProdus(info);

	return noulNod;
}

NodArbore* inserareNod(NodArbore* radacina, const Produs info)
{
	if (radacina == NULL)
	{
		return creareNodArbore(info);
	}

	if (info.cod < radacina->info.cod)
	{
		radacina->stanga = inserareNod(radacina->stanga, info);
	}

	if (info.cod > radacina->info.cod)
	{
		radacina->dreapta = inserareNod(radacina->dreapta, info);
	}

	int hSubarboreStang = (radacina->stanga != NULL) ? radacina->stanga->inaltime : 0;
	int hSubarboreDrept = (radacina->dreapta != NULL) ? radacina->dreapta->inaltime : 0;
	radacina->inaltime = 1 + maxim(hSubarboreStang, hSubarboreDrept);

	return radacina;
}

void afisareNodArbore(NodArbore* nod)
{
	if (nod == NULL)
	{
		return;
	}

	afisareProdus(nod->info);
}

void afisareInPreordine(NodArbore* radacina) // RSD
{
	if (radacina == NULL)
	{
		return;
	}

	afisareNodArbore(radacina);
	afisareInPreordine(radacina->stanga);
	afisareInPreordine(radacina->dreapta);
}

void afisareInInordine(NodArbore* radacina) // SRD
{
	if (radacina == NULL)
	{
		return;
	}

	afisareInInordine(radacina->stanga);
	afisareNodArbore(radacina);
	afisareInInordine(radacina->dreapta);
}

void afisareInPostordine(NodArbore* radacina) // SDR
{
	if (radacina == NULL)
	{
		return;
	}

	afisareInPostordine(radacina->stanga);
	afisareInPostordine(radacina->dreapta);
	afisareNodArbore(radacina);
}

void dezalocareNodArbore(NodArbore* nod)
{
	if (nod == NULL)
	{
		return;
	}

	dezalocareProdus(nod->info);
	free(nod);
}

void dezalocareNoduriArbore(NodArbore* radacina) // SDR
{
	if (radacina == NULL)
	{
		return;
	}

	dezalocareNoduriArbore(radacina->stanga);
	dezalocareNoduriArbore(radacina->dreapta);
	dezalocareNodArbore(radacina);

	radacina = NULL;
}

/* Functii pentru structura BST */

BST initializareArbore()
{
	BST arbore = { .radacina = NULL, .nrNoduri = 0, .inaltime = 0 };
	return arbore;
}

bool isEmptyBst(const BST arbore)
{
	return arbore.radacina == NULL;
}

BST inserareNodInArbore(BST arbore, const Produs info)
{
	arbore.radacina = inserareNod(arbore.radacina, info);
	arbore.inaltime = arbore.radacina->inaltime;
	arbore.nrNoduri++;

	return arbore;
}

BST citireArboreDinFisier(char* denumireFisier)
{
	BST arbore = initializareArbore();
	FILE* file = fopen(denumireFisier, "r");

	if (file == NULL)
	{
		return arbore;
	}

	while (!feof(file))
	{
		Produs p = citireProdusDinFisier(file);
		arbore = inserareNodInArbore(arbore, p);
		dezalocareProdus(p);
	}

	fclose(file);
	return arbore;
}

void afisareArbore(const BST arbore, TipAfisare modAfisare)
{
	if (isEmptyBst(arbore))
	{
		printf("Arborele binar de cautare este gol, nu avem ce afisa!\n\n");
		return;
	}

	switch (modAfisare)
	{
	case kPREORDINE:
		afisareInPreordine(arbore.radacina);
		break;

	case kINORDINE:
		afisareInInordine(arbore.radacina);
		break;

	case kPOSTORDINE:
		afisareInPostordine(arbore.radacina);
		break;

	default:
		printf("Cheie de afisare invalida, nu se poate afisa nimic!\n\n");
		break;
	}
}

void dezalocareArbore(BST* arbore)
{
	if (arbore == NULL || isEmptyBst(*arbore))
	{
		return;
	}

	dezalocareNoduriArbore(arbore->radacina);

	arbore->inaltime = 0;
	arbore->nrNoduri = 0;
}

/* Tema
* Sa se stearga nodul radacina din arbore si se refaca structura lui de BST
* Sa se caute si sa se afiseze elementul al carui ID este trimis ca parametru al unei functii definite de voi
*/

int main()
{
	BST arbore = citireArboreDinFisier("produse.txt");

	printf("======================================== Afisarea arborelui in preordine este mai jos ========================================\n\n");
	afisareArbore(arbore, kPREORDINE);
	printf("======================================== Afisarea arborelui in inordine este mai jos ========================================\n\n");
	afisareArbore(arbore, kINORDINE);
	printf("======================================== Afisarea arborelui in postordine este mai jos ========================================\n\n");
	afisareArbore(arbore, kPOSTORDINE);

	printf("Inaltimea arborelui binar de cautare este: %d\n", arbore.inaltime);
	printf("Numarul de nordui din arborele binar de cautare este: %d noduri\n", arbore.nrNoduri);

	dezalocareArbore(&arbore);
	return 0;
}