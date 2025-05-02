#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct Stiva Stiva;
typedef struct Produs Produs;
typedef struct NodStiva NodStiva;

struct Produs
{
	int cod;
	float pret;
	char* denumire;
};

struct NodStiva
{
	Produs info;
	NodStiva* next;
};

struct Stiva
{
	int dimensiune;
	NodStiva* varf;
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

	if (fgets(linie, sizeof(linie), f)) // citim linia complet din fisier ca string
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

/* Functii pentru structura NodStiva */

NodStiva* creareNodStiva(const Produs info, NodStiva* next)
{
	NodStiva* noulNod = (NodStiva*)malloc(sizeof(NodStiva));

	if (noulNod == NULL)
	{
		return noulNod;
	}

	noulNod->next = next;
	noulNod->info = copiazaProdus(info);

	return noulNod;
}

void afisareNodStiva(NodStiva* nod)
{
	if (nod == NULL)
	{
		return;
	}

	afisareProdus(nod->info);
}

void dezalocareNodStiva(NodStiva* nod)
{
	if (nod == NULL)
	{
		return;
	}

	nod->next = NULL;
	dezalocareProdus(nod->info);
	free(nod);
}

/* Functii pentru structura Stiva */

Stiva initializareStiva()
{
	Stiva stiva = { .varf = NULL, .dimensiune = 0 };
	return stiva;
}

bool isEmptyStack(Stiva stiva)
{
	return stiva.varf == NULL;
}

Stiva copiereStiva(Stiva* original)
{
	Stiva copie = initializareStiva();

	if (isEmptyStack(*original))
	{
		return copie;
	}

	NodStiva* varfNou = creareNodStiva(original->varf->info, NULL);

	if (varfNou == NULL)
	{
		return copie;
	}

	copie.varf = varfNou;
	copie.dimensiune = 1;

	NodStiva* current = copie.varf;
	NodStiva* aux = original->varf->next;

	while (aux != NULL)
	{
		NodStiva* noulNod = creareNodStiva(aux->info, NULL);
		current->next = noulNod;
		current = current->next;
		copie.dimensiune++;
		aux = aux->next;
	}

	return copie;
}

Stiva push(Stiva stiva, const Produs info)
{
	NodStiva* noulNod = creareNodStiva(info, NULL);

	if (noulNod == NULL)
	{
		return stiva;
	}

	if (isEmptyStack(stiva))
	{
		stiva.varf = noulNod;
		stiva.dimensiune = 1;

		return stiva;
	}

	noulNod->next = stiva.varf;
	stiva.varf = noulNod;
	stiva.dimensiune++;

	return stiva;
}

Stiva pop(Stiva stiva)
{
	if (isEmptyStack(stiva))
	{
		return stiva;
	}

	NodStiva* nodDeSters = stiva.varf;
	stiva.varf = stiva.varf->next;
	afisareProdus(nodDeSters->info);
	dezalocareNodStiva(nodDeSters);
	stiva.dimensiune--;

	return stiva;
}

Stiva citireStivaDinFisier(char* denumireFisier)
{
	Stiva stiva = initializareStiva();
	FILE* file = fopen(denumireFisier, "r");

	if (file == NULL)
	{
		return stiva;
	}

	while(!feof(file))
	{
		Produs p = citireProdusDinFisier(file);
		stiva = push(stiva, p);
		dezalocareProdus(p);
	}

	fclose(file);
	return stiva;
}

void afisareStiva(const Stiva stiva)
{
	if (isEmptyStack(stiva))
	{
		printf("Stiva este goala!\n\n");
		return;
	}

	Stiva copie = copiereStiva(&stiva);

	while (!isEmptyStack(copie))
	{
		copie = pop(copie);
	}
}

void dezalocareStiva(Stiva* stiva)
{
	if (isEmptyStack(*stiva))
	{
		return;
	}

	NodStiva* nodDeSters = NULL;

	while (stiva->varf != NULL)
	{
		nodDeSters = stiva->varf;
		stiva->varf = stiva->varf->next;
		dezalocareNodStiva(nodDeSters);
		stiva->dimensiune--;
	}
}

Produs* ToVector(const Stiva stiva)
{
	if (isEmptyStack(stiva))
	{
		return NULL;
	}

	Produs* produse = (Produs*)malloc(stiva.dimensiune * sizeof(Produs));

	if (produse == NULL)
	{
		return produse;
	}

	int index = 0;
	Stiva copie = copiereStiva(&stiva);
	NodStiva* aux = NULL;

	while (!isEmptyStack(copie))
	{
		aux = copie.varf;
		produse[index] = copiazaProdus(aux->info);
		copie = pop(copie);
		index++;
	}

	return produse;
}

int main()
{
	Stiva stiva = citireStivaDinFisier("produse.txt");
	afisareStiva(stiva);

	int dim = stiva.dimensiune;
	Produs* produse = ToVector(stiva);

	printf("======================================= Dupa operatia de pop =======================================\n\n");

	stiva = pop(stiva);

	printf("======================================= Afisare dupa operatia de pop =======================================\n\n");

	afisareStiva(stiva);
	dezalocareStiva(&stiva);

	printf("======================================= Conversia stivei in vector =======================================\n\n");

	for (int i = 0; i < dim; i++)
	{
		afisareProdus(produse[i]);
	}

	for (int i = 0; i < dim; i++)
	{
		produse[i].pret += produse[i].pret * 0.2;
	}

	printf("\n\n======================================= Preturi majorate cu 20 de procente =======================================\n\n");

	for (int i = 0; i < dim; i++)
	{
		afisareProdus(produse[i]);
	}

	for (int i = 0; i < dim; i++)
	{
		dezalocareProdus(produse[i]);
	}

	free(produse);

	return 0;
}