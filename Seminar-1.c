#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning (disable : 4996)
#pragma warning (disable : 6031)

typedef struct Produs Produs;

struct Produs
{
	int cod;
	float pret;
	char* denumire;
};

/*
* scrieti o functie pentru creare Produs
* scrieti o functie de afisare a unui produs
* scrieti o functie care face o copie profunda a unui produs pe baza altui produs existent
* scrieti o functie care modifica denumirea unui produs primit ca parametru
*
* tema pentru acasa
*
* scrieti o functie de copiere a unui produs care primeste ca parametri produsul in care vreau sa copiez si sursa (fct nu returneaza nimic, si atentie sa nu aiba acelasi nume cu copiazaProdus)
* scrieti o functie care face afisarea vectorului de produse din main primit ca parametru - hint veti da si dimensiunea ca parametru
* scrieti o functie care sa returneze indexul din vector al carui produs are pretul cel mai mic, afisati cu ajutorul acestui index produsul din vector
*/

Produs initializareProdus()
{
	Produs produs = { .cod = 0, .denumire = NULL, .pret = 0.0f };
	return produs;
}

Produs creareProdus(int cod, float pret, const char* denumire)
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

void copiazaProdus2(Produs* destinatie, const Produs sursa)
{
	if (destinatie == NULL)
	{
		return;
	}

	if (destinatie->denumire != NULL)
	{
		free(destinatie->denumire);
	}

	destinatie->cod = sursa.cod;
	destinatie->pret = sursa.pret;

	if (sursa.denumire != NULL)
	{
		destinatie->denumire = (char*)malloc((1 + strlen(sursa.denumire)) * sizeof(char));
		strcpy(destinatie->denumire, sursa.denumire);
	}
	else
	{
		destinatie->denumire = NULL;
	}
}

void setDenumire(Produs* produs, const char* denumireNoua)
{
	if (produs->denumire != NULL)
	{
		free(produs->denumire);
	}

	if (denumireNoua != NULL)
	{
		produs->denumire = (char*)malloc((1 + strlen(denumireNoua)) * sizeof(char));
		strcpy(produs->denumire, denumireNoua);
	}
	else
	{
		produs->denumire = NULL;
	}
}

void afisareVector(Produs* produse, const int dimensiune)
{
	if (produse == NULL || dimensiune <= 0)
	{
		return;
	}

	for (int i = 0; i < dimensiune; i++)
	{
		afisareProdus(produse[i]);
	}
}

int getPozitieProdusPretMinim(Produs* produse, const int dimensiune)
{
	if (produse == NULL || dimensiune <= 0)
	{
		return -1;
	}

	int index = -1; // plecam de la premisa ca nu exista un produs care sa aiba un pret mai mic ca valoarea din pretMinim
	float pretMinim = produse[0].pret;

	for (int i = 1; i < dimensiune; i++)
	{
		if (pretMinim > produse[i].pret)
		{
			index = i;
			pretMinim = produse[i].pret;
		}
	}

	return index;
}

int main()
{
	Produs p = creareProdus(1, 75.5f, "Branza");
	afisareProdus(p);

	Produs p2 = copiazaProdus(p);
	afisareProdus(p2);

	setDenumire(&p2, "Tofu");
	afisareProdus(p2);
	afisareProdus(p);

	Produs p3 = initializareProdus();

	copiazaProdus2(&p3, p2);
	setDenumire(&p3, "Baterie reincarcabila");
	afisareProdus(p2);
	afisareProdus(p3);

	// sa se creeze un vector de produse care contine minimum 5 elemente

	int dim = 5;
	Produs* produse = (Produs*)malloc(dim * sizeof(Produs));

	produse[0] = creareProdus(2, 574.99f, "Televizor");
	produse[1] = creareProdus(3, 702.99f, "Fier de calcat");
	produse[2] = creareProdus(4, 1574.99f, "Frigider");
	produse[3] = creareProdus(5, 1999.99f, "Telefon");
	produse[4] = creareProdus(6, 39.99f, "Halat");

	printf("Vectorul de produse va fi afisat mai jos\n\n");

	afisareVector(produse, dim);

	int indexMinim = getPozitieProdusPretMinim(produse, dim);

	if (indexMinim != -1)
	{
		afisareProdus(produse[indexMinim]);
	}
	else
	{
		printf("Nu exista un produs care sa aiba un pret mai mic decat preturile produselor din vector");
	}

	for (int index = 0; index < dim; index++)
	{
		dezalocareProdus(produse[index]);
	}

	free(produse);
	dezalocareProdus(p);
	dezalocareProdus(p2);

	return 0;
}