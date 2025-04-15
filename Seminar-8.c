#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct Coada Coada;
typedef struct Produs Produs;
typedef struct NodCoada NodCoada;

struct Produs
{
	int cod;
	float pret;
	char* denumire;
};

struct NodCoada
{
	Produs info;
	NodCoada* next;
};

struct Coada
{
	int dimensiune;
	NodCoada* front;
	NodCoada* rear;
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

/* Functii pentru structura NodCoada */

NodCoada* creareNodCoada(const Produs info, NodCoada* next)
{
	NodCoada* noulNod = (NodCoada*)malloc(sizeof(NodCoada));

	if (noulNod == NULL)
	{
		return noulNod;
	}

	noulNod->next = next;
	noulNod->info = copiazaProdus(info);

	return noulNod;
}

void afisareNodCoada(NodCoada* nod)
{
	if (nod == NULL)
	{
		return;
	}

	afisareProdus(nod->info);
}

void dezalocareNodCoada(NodCoada* nod)
{
	if (nod == NULL)
	{
		return;
	}

	nod->next = NULL;
	dezalocareProdus(nod->info);
	free(nod);
}

/* Functii pentru structura Coada */

Coada initializareCoada()
{
	Coada coada = { .dimensiune = 0, .front = NULL, .rear = NULL };
	return coada;
}

bool isEmptyQueue(const Coada coada)
{
	return coada.front == NULL && coada.rear == NULL;
}

Coada enQueue(Coada coada, const Produs info)
{
	NodCoada* noulNod = creareNodCoada(info, NULL);

	if (noulNod == NULL)
	{
		return coada;
	}

	if (isEmptyQueue(coada))
	{
		coada.front = coada.rear = noulNod;
		coada.dimensiune = 1;

		return coada;
	}

	coada.rear->next = noulNod;
	coada.rear = noulNod;
	coada.dimensiune++;

	return coada;
}

Coada deQueue(Coada coada)
{
	if (isEmptyQueue(coada))
	{
		return coada;
	}

	if (coada.front == coada.rear)
	{
		dezalocareNodCoada(coada.front);
		coada.front = coada.rear = NULL;
		coada.dimensiune = 0;

		return coada;
	}

	NodCoada* nodDeSters = coada.front;
	coada.front = coada.front->next;
	dezalocareNodCoada(nodDeSters);
	coada.dimensiune--;

	return coada;
}

Coada copiereCoada(Coada* original)
{
	Coada copie = initializareCoada();

	if (isEmptyQueue(*original))
	{
		return copie;
	}

	NodCoada* aux = original->front;

	while (aux != NULL)
	{
		copie = enQueue(copie, aux->info);
		aux = aux->next;
	}

	return copie;
}

Coada citireCoadaDinFisier(char* denumireFisier)
{
	Coada coada = initializareCoada();
	FILE* file = fopen(denumireFisier, "r");

	if (file == NULL)
	{
		return coada;
	}

	while (!feof(file))
	{
		Produs p = citireProdusDinFisier(file);
		coada = enQueue(coada, p);
		dezalocareProdus(p);
	}

	fclose(file);

	return coada;
}

void afisareCoada(const Coada coada)
{
	if (isEmptyQueue(coada))
	{
		printf("Coada este goala!\n\n");
		return;
	}

	Coada copie = copiereCoada(&coada);

	while (!isEmptyQueue(copie))
	{
		afisareNodCoada(copie.front);
		copie = deQueue(copie);
	}
}

void dezalocareCoada(Coada* coada)
{
	if (coada == NULL || isEmptyQueue(*coada))
	{
		return;
	}

	NodCoada* aux = NULL;

	while (coada->front != NULL)
	{
		aux = coada->front;
		coada->front = coada->front->next;
		dezalocareNodCoada(aux);
		coada->dimensiune--;
	}

	coada->rear = NULL;
}

int main()
{
	Coada coada = citireCoadaDinFisier("produse.txt");

	printf("====================================== Dupa citirea din fisier ======================================\n\n");
	afisareCoada(coada);

	printf("====================================== Dupa un deQueue ======================================\n\n");

	coada = deQueue(coada);
	afisareCoada(coada);

	printf("====================================== Dupa un enQueue ======================================\n\n");

	Produs produs = creareProdus(222, 50, "Cub rubic");
	coada = enQueue(coada, produs);
	afisareCoada(coada);

	dezalocareCoada(&coada);
	dezalocareProdus(produs);

	return 0;
}