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
* scrieti o fct pentru creare Produs
* scrieti o fct de afisare
* scrieti o fct care face o copie a produsului pe baza altui produs existent
* scrieti o fct care modifica denumirea unui produs primit ca parametru
*
* tema pentru acasa
*
* scrieti o fct de copiere a unui produs care primeste ca parametri produsul in care vreau sa copiez si sursa (fct nu returneaza nimic, si atentie sa nu aiba acelasi nume cu copiazaProdus)
* scrieti o functie care face afisarea vectorului de produse din main primit ca parametru - hint veti da si dimensiunea ca parametru
* scrieti o functie care sa returneze indexul din vector al carui produs are pretul cel mai mic, afisati cu ajutorul acestui index produsul din vector
*/

Produs creareProdus(int cod, float pret, const char* denumire)
{
	Produs p;

	p.cod = cod;
	p.pret = pret;

	if (denumire != NULL)
	{
		p.denumire = (char*)malloc((1 + strlen(denumire)) * sizeof(char));
		strcpy(p.denumire, denumire);
	}
	else
	{
		p.denumire = NULL;
	}

	return p;
}

void dezalocareProdus(Produs produs)
{
	if (produs.denumire != NULL)
	{
		free(produs.denumire);
	}
}

void afisareProdus(Produs p)
{
	printf("Codul produsului este: %d\nPretul produsului este: %.2f lei\nDenumirea produsului este: %s\n\n",
		p.cod, p.pret, p.denumire);
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

int main()
{
	Produs p = creareProdus(1, 1000, "Branza");
	afisareProdus(p);

	Produs p2 = copiazaProdus(p);
	afisareProdus(p2);

	setDenumire(&p2, "Tofu");
	afisareProdus(p2);
	afisareProdus(p);

	// sa se creeze un vector de produse care contine minimum 5 elemente

	int dim = 5;
	Produs* produse = (Produs*)malloc(dim * sizeof(Produs));

	produse[0] = creareProdus(2, 574.99f, "Televizor");
	produse[1] = creareProdus(3, 702.99f, "Fier de calcat");
	produse[2] = creareProdus(4, 1574.99f, "Frigider");
	produse[3] = creareProdus(5, 1999.99f, "Telefon");
	produse[4] = creareProdus(6, 39.99f, "Halat");

	printf("Vectorul de produse va fi afisat mai jos\n\n");

	for (int index = 0; index < dim; index++)
	{
		afisareProdus(produse[index]);
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