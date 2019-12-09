//
//  stock.c
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* structure to store all information about items */

struct item_s {
	// each item has a unique id to clearly identify it
	unsigned id;
	// manufacturer and model
	char manufacturer[16];
	char model[16];
	// where the item is stored: rack and position within rack
	unsigned rack;
	unsigned position;
	// number of items in stock
	unsigned quantity;
};

#define MAX_NR_ITEMS 10000
/* all items in stock */
struct item_s stock[MAX_NR_ITEMS];
/* number of items in stock */
unsigned stock_size = 0;

/* read at most max_nr items from file "filename" and write them to array items
 * the number of items actually read is returned */

void read_items_from_file(char* filename)
{
	// storage to preliminarily store the information of an item
	unsigned id; char manufacturer[16]; char model[16];
	unsigned rack; unsigned position; unsigned quantity;
	// number of tokens read from current input line
	size_t len = 0;

	// open the input file for reading
	FILE* fp = fopen(filename, "r");
	if (fp == NULL)
	{
		// if file could not be opened (wrong path, not readable, ...)
		// output a short message and immediately exit
		char resp;
		printf("Eingabedatei kann nicht geoeffnet werden.\n");
		scanf("%c", &resp);
		exit(EXIT_FAILURE);
	}

	// the following loop reads a new item in every iteration
	// until the end of the file (or an invalid entry appears)
	len = fscanf(fp, "%i %s %s %i %i %i", &id, manufacturer, model, &rack, &position, &quantity);

	// as long as there is space in the array and a correct line is read
	while ((stock_size < MAX_NR_ITEMS) && (len == 6))
	{
		// printf("Lese Element: %i\n", id);
		stock[stock_size].id = id;
		strcpy(stock[stock_size].manufacturer, manufacturer);
		strcpy(stock[stock_size].model, model);
		stock[stock_size].rack = rack;
		stock[stock_size].position = position;
		stock[stock_size].quantity = quantity;
		stock_size++;
		// reading next item
		len = fscanf(fp, "%i %s %s %i %i %i", &id, manufacturer, model, &rack, &position, &quantity);
	}

	// finally close the input file
	fclose(fp);
}

/* compare function for items which orders them by ID */

int order_items_by_id(const void* i1, const void* i2)
{
	// first convert pointers to correct type
	struct item_s* item1 = (struct item_s*)i1;
	struct item_s* item2 = (struct item_s*)i2;
	if (item1->id > item2->id) return 1;
	else if (item1->id < item2->id) return -1;
	else return 0;
}

/* function that checks whether some item is in stock */

unsigned item_in_stock(unsigned item_id) {
	for (int i = 0; i < stock_size; i++) {
		if (item_id == stock[i].id) return 1;
	}
	return 0;
}

// --- part 1

void enter_desired_items(unsigned int* id, unsigned int* nr) {
	setbuf(stdout, 0);
	printf("\nGeben Sie die ID des Artikels ein: ");
	scanf("%i", id);
	setbuf(stdout, 0);
	printf("\nGeben Sie die gewuenschte Anzahl ein: ");
	scanf("%i", nr);
}

void remove_items_from_stock(unsigned id, unsigned nr) {
	if (nr <= stock[id].quantity) {
		stock[id].quantity = stock[id].quantity - nr;
		setbuf(stdout, 0);
		printf("\nDie Buchung war erfolgreich");
		setbuf(stdout, 0);
		printf("\nPosition %s: rack%i/pos %i", stock[id].model, stock[id].rack, stock[id].position);
	}
	else {
		setbuf(stdout, 0);
		printf("\nZu wenig Artikel auf Lager");
	}
}

// --- part 2

struct item_s enter_new_item(unsigned id);
void add_items_to_stock(unsigned id, unsigned nr);

// --- part 3 (preliminary solution)

void print_stock()
{
	for (unsigned i = 0; i < stock_size; i++)
		printf("%04i %s %s - rack %i/pos %i - #items: %i\n",
			stock[i].id,
			stock[i].manufacturer,
			stock[i].model,
			stock[i].rack,
			stock[i].position,
			stock[i].quantity);
}

// --- part 4

void save_items_to_file(char* filename) {
	FILE* fp = fopen(filename, "w");
	for (int i = 0; i < stock_size; i++) {
		fprintf(fp, "%i %s %s %i %i %i\n",
			stock[i].id,
			stock[i].manufacturer,
			stock[i].model,
			stock[i].rack,
			stock[i].position,
			stock[i].quantity);
	}
	fclose(fp);
}

/*
 * main function: entry of point
 */

int main()
{
	// print program header
	printf("\n\n");
	printf("HAUPTMENU\n");
	printf("---------\n\n");

	// read intial stock from files
	read_items_from_file("lager.txt");

	//
	do {
		// read the name to be searched
		printf("\n\nWas wollen Sie tun?\n");
		printf("(1) Entnahme von Elementen\n");
		printf("(2) Hinzufuegen von Elementen\n");
		printf("(3) aktuellen Lagerbestand speichern\n");
		printf("(4) Inventarliste ausgeben\n");
		printf("(0) Programm beenden\n");
		printf("\tIhre Wahl: ");

		unsigned  choice;
		scanf("%d", &choice);
		while (getchar() != '\n');

		// menu items
		switch (choice)
		{
		case 1:
			// TODO
			remove_items_from_stock();
			break;
		case 2:
			// TODO
			enter_desired_items();
			break;
		case 3:
			save_items_to_file("lager.txt");
			break;
		case 4:
			print_stock();
			break;
		case 0:
			return 0;
		default:
			printf("Ungueltige Eingabe.\n");
		}
	} while (1);
}