#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct Heap Heap;
typedef struct Produs Produs;

struct Produs
{
	int cod;
	float pret;
	char* denumire;
};

struct Heap
{
	int dimensiune;
	Produs* elemente;
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

/* Functii structura Heap */

Heap initializareHeap()
{
	Heap heap = { .dimensiune = 0, .elemente = NULL };
	return heap;
}

bool isEmptyHeap(const Heap heap)
{
	return heap.elemente == NULL;
}

int getPozitieParinte(const int pozitieCurenta)
{
	if (pozitieCurenta == 0)
	{
		return 0;
	}

	return (pozitieCurenta - 1) / 2;
}

int getPozitieFiuStang(const int pozitieCurenta)
{
	return 2 * pozitieCurenta + 1;
}

int getPozitieFiuDrept(const int pozitieCurenta)
{
	return 2 * pozitieCurenta + 2;
}

void interschimbare(Produs* elemente, const int i, const int j)
{
	Produs aux = elemente[i];
	elemente[i] = elemente[j];
	elemente[j] = aux;
}

void minHeapify(Heap* heap, const int pozitieStart)
{
	int pozElementMinim = pozitieStart;
	int pozFiuStang = getPozitieFiuStang(pozitieStart);
	int pozFiuDrept = getPozitieFiuDrept(pozitieStart);

	if (pozFiuStang < heap->dimensiune && heap->elemente[pozFiuStang].pret < heap->elemente[pozElementMinim].pret)
	{
		pozElementMinim = pozFiuStang;
	}

	if (pozFiuDrept < heap->dimensiune && heap->elemente[pozFiuDrept].pret < heap->elemente[pozElementMinim].pret)
	{
		pozElementMinim = pozFiuDrept;
	}

	if (pozElementMinim != pozitieStart)
	{
		interschimbare(heap->elemente, pozitieStart, pozElementMinim);
		minHeapify(heap, pozElementMinim); // refiltram pentru a asigura ca intregul vector va fi aranjat sub forma de heap
	}
}

Heap citireHeapDinFisier(const char* denumireFisier)
{
	Heap heap = initializareHeap();
	FILE* file = fopen(denumireFisier, "r");

	if (file == NULL)
	{
		return heap;
	}

	int dimensiune = 0;
	fscanf(file, "%d", &dimensiune);

	int character;
	while ((character = fgetc(file)) != '\n' && character != EOF); // consumam restul de linie pana ajungem la urmatoarea
	heap.elemente = (Produs*)malloc(dimensiune * sizeof(Produs));

	if (heap.elemente == NULL)
	{
		return heap;
	}

	int index = 0;
	heap.dimensiune = dimensiune;

	while (!feof(file) && index < dimensiune)
	{
		Produs p = citireProdusDinFisier(file);
		heap.elemente[index++] = copiazaProdus(p);
		dezalocareProdus(p);
	}

	fclose(file);

	for (int i = getPozitieParinte(dimensiune - 1); i >= 0; i--)
	{
		minHeapify(&heap, i);
	}

	return heap;
}

Produs* extragereProdusCuPretMinim(Heap* heap)
{
	if (heap == NULL || isEmptyHeap(*heap) || heap->dimensiune < 1)
	{
		return NULL;
	}

	interschimbare(heap->elemente, 0, heap->dimensiune - 1); // interschimbam valorile radacinii cu al ultimei frunze
	Produs* produsMinim = &heap->elemente[heap->dimensiune - 1]; // retinem valoarea minima care acum este in locul frunzei
	heap->dimensiune--; // ascundem casuta pe care se afla ultimul element (este o stergere logica)
	minHeapify(heap, 0); // refiltram pentru a readuce vectorul in forma de heap

	return produsMinim;
}

void afisareHeap(const Heap heap)
{
	if (isEmptyHeap(heap))
	{
		printf("Heap-ul este gol nu avem ce afisa!\n\n");
		return;
	}

	for (int i = 0; i < heap.dimensiune; i++)
	{
		afisareProdus(heap.elemente[i]);
	}
}

void dezalocareHeap(Heap* heap)
{
	if (heap == NULL || isEmptyHeap(*heap))
	{
		return;
	}

	for (int i = 0; i < heap->dimensiune; i++)
	{
		dezalocareProdus(heap->elemente[i]);
	}

	free(heap->elemente);
	heap->elemente = NULL;
	heap->dimensiune = 0;
}

/*
* Tema Heap
* sa se scrie functia care filtreaza vectorul astfel incat sa se obtina un max heap (maxHeapify)
* sa se scrie o functie care insereaza un element in heap si ii pastreaza structura interna de heap
*/

int main()
{
	Heap heap = citireHeapDinFisier("produse.txt");
	printf("========================================== Dupa filtrare ==========================================\n\n");
	afisareHeap(heap);

	printf("========================================== Extragerea produsului cu pretul minim ==========================================\n\n");
	Produs* produsMinim = extragereProdusCuPretMinim(&heap);
	afisareProdus(*produsMinim);

	printf("========================================== Dupa extragerea produsului cu pret minim ==========================================\n\n");
	afisareHeap(heap);

	dezalocareProdus(*produsMinim);
	dezalocareHeap(&heap);

	return 0;
}