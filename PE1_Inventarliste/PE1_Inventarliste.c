#define _CRT_SECURE_NO_WARNINGS
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

//Konstanten
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
		setbuf(stdout, 0);
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

//Funktion zur Überprüfung ob ein Artikel (item_id) vorhanden ist
//Wenn dies der Fall ist erhält serial_number die entsprechende Position im Array
unsigned item_in_stock(unsigned item_id, int* serial_id) {
	for (int i = 0; i < stock_size; i++) {
		if (item_id == stock[i].id) {
			*serial_id = i;
			return 1;
		}
	}
	return 0;
}

//Aufgabe 1
void enter_desired_items(unsigned int* id, unsigned int* nr) {
	setbuf(stdout, 0);
	printf("\nBitte geben Sie die ID des Artikels ein: ");
	scanf("%i", id);
	setbuf(stdout, 0);
	printf("\nBitte geben Sie die gewuenschte Anzahl ein: ");
	scanf("%i", nr);
}

void remove_items_from_stock(unsigned nr, int serial_id) {
	if (nr <= stock[serial_id].quantity) {
		stock[serial_id].quantity = stock[serial_id].quantity - nr;
		setbuf(stdout, 0);
		printf("\nDie Buchung war erfolgreich");
		setbuf(stdout, 0);
		printf("\nPosition %s: rack%i/pos %i", stock[serial_id].model, stock[serial_id].rack, stock[serial_id].position);
	}
	else {
		setbuf(stdout, 0);
		printf("\nZu wenig Artikel auf Lager");
	}
}

//Aufgabe 2
void add_items_to_stock(unsigned id, unsigned nr, int serial_id) {
	// storage to preliminarily store the information of an item
	char manufacturer[16]; char model[16];
	unsigned rack; unsigned position;
	if (serial_id != (-1)) {
		stock[serial_id].quantity = stock[serial_id].quantity + nr;
		setbuf(stdout, 0);
		printf("\nDer Artikel wurde erfolgreich dem Bestand hinzugefuegt");
	}
	else {
		stock_size++;
		setbuf(stdout, 0);
		printf("Der Artikel wurde nicht gefunden. Neu hinzufuegen:\nHersteller Name: ");
		scanf("%s", manufacturer);
		setbuf(stdout, 0);
		printf("Modell: ");
		scanf("%s", model);
		setbuf(stdout, 0);
		printf("Regal: ");
		scanf("%i", &rack);
		setbuf(stdout, 0);
		printf("Position: ");
		scanf("%i", &position);

		stock[stock_size - 1].id = id;
		strcpy(stock[stock_size - 1].manufacturer, manufacturer);
		strcpy(stock[stock_size - 1].model, model);
		stock[stock_size - 1].rack = rack;
		stock[stock_size - 1].position = position;
		stock[stock_size - 1].quantity = nr;
	}
}

//Aufgabe 3
/* compare function for items which orders them by ID */
int order_items_by_id(const void* i1, const void* i2, int position)
{
	// first convert pointers to correct type
	struct item_s* item1 = (struct item_s*)i1;
	struct item_s* item2 = (struct item_s*)i2;
	struct item_s buf;
	if (item1->id > item2->id) {
		buf = *item1;
		stock[position] = *item2;
		stock[position + 1] = buf;
	}
	else return 0;
}

void print_stock()
{
	for (unsigned i = 0; i < stock_size; i++) {
		setbuf(stdout, 0);
		printf("%04i %s %s - rack %i/pos %i - #items: %i\n",
			stock[i].id,
			stock[i].manufacturer,
			stock[i].model,
			stock[i].rack,
			stock[i].position,
			stock[i].quantity);
	}
}

// Aufgabe 4
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

//Main-Function
int main()
{
	// print program header
	printf("\n\n");
	setbuf(stdout, 0);
	printf("HAUPTMENU\n");
	printf("---------\n\n");

	// read intial stock from files
	read_items_from_file("lager.txt");

	//
	do {
		// read the name to be searched
		setbuf(stdout, 0);
		printf("\n\nWas wollen Sie tun?\n");
		setbuf(stdout, 0);
		printf("(1) Entnahme von Elementen\n");
		setbuf(stdout, 0);
		printf("(2) Hinzufuegen von Elementen\n");
		setbuf(stdout, 0);
		printf("(3) aktuellen Lagerbestand speichern\n");
		setbuf(stdout, 0);
		printf("(4) Inventarliste ausgeben\n");
		setbuf(stdout, 0);
		printf("(0) Programm beenden\n");
		setbuf(stdout, 0);
		printf("\tIhre Wahl: ");

		unsigned  choice;
		scanf("%d", &choice);
		while (getchar() != '\n');

		// menu items
		unsigned desired_id, desired_quant;
		//serial_id = Fortlaufende Nummer -> Position im Array. Hat serial_id den Wert -1 ist der Artikel nicht in der Liste vorhanden.
		int serial_id;
		switch (choice)
		{
			//Entnahme eines Artikels
		case 1:
			serial_id = (-1);
			while (1) {
				enter_desired_items(&desired_id, &desired_quant);
				if (item_in_stock(desired_id, &serial_id)) {
					remove_items_from_stock(desired_quant, serial_id);
					break;
				}
				else {
					setbuf(stdout, 0);
					printf("\nDer Artikel wurde nicht gefunden. Bitte Eingabe wiederholen");
				}
			}
			break;
		case 2:
			//Hinzufügen eines Artikels
			serial_id = (-1);
			enter_desired_items(&desired_id, &desired_quant);
			item_in_stock(desired_id, &serial_id);
			add_items_to_stock(desired_id, desired_quant, serial_id);
			break;
		case 3:
			save_items_to_file("lager.txt");
			break;
		case 4:
			//Sortierung des Inventars nach ID und Ausgabe
			for (int n = stock_size - 1; n > 0; n--) {
				for (int i = 0; i < n; i++) order_items_by_id(&stock[i], &stock[i + 1], i);
			}
			print_stock();
			break;
		case 0:
			return 0;
		default:
			setbuf(stdout, 0);
			printf("Ungueltige Eingabe.\n");
		}
	} while (1);
}