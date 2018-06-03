/*
Aufgabe		
Autor		Florian Bopp
Datum		

Kurzbeschreibung:	
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

typedef enum bool{ false, true } bool;

typedef struct toDoList
{
	char task[99];							//Payload
	char deadline[99];
	char location[99];
	struct toDoList *next;				//Pointer to next list item
} toDoList;								//typedef synonym

/******************functionsprototypes**********************/
toDoList* newListItem(toDoList *start);
void removeItem(toDoList *start);
void clearList(toDoList *start);
void printList(toDoList *start);
void saveList(toDoList *start);
toDoList* loadList(toDoList *start);
toDoList *readNextItemFromFile(toDoList *start, FILE *fp);
void flashStandardInput(void);
void newline(void);


/******************Main function********************/
int main(void) {

	toDoList *start = NULL;
	char underline[] = { 238, 238, 238,238,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238, '\0'};
	bool loop = true;
	char action[7];	


	printf("\tYour to-do-list\n");
	printf("\t%s\n\n\n", underline);

	while (loop){
		printf("Enter one of the following commands.\n\n");
		printf("\tto create a new list item enter:\t\tnew\n");
		printf("\tto remove a list item enter:\t\t\tremove\n");
		printf("\tto clear the list enter:\t\t\tclear\n");
		printf("\tto print the list enter:\t\t\tprint\n");
		printf("\tto save the list enter:\t\t\t\tsave\n");
		printf("\tto load the list from a file enter:\t\tload\n");
		printf("\tto exit program enter:\t\t\t\texit\n\n");

		scanf("%6[^\n]", action);
		flashStandardInput();
		newline();
		if ((strncmp(action, "new", 3) != 0)  && (strncmp(action, "remove", 6) != 0) && (strncmp(action, "print", 5) != 0) && (strncmp(action, "save", 4) != 0) && (strncmp(action, "load", 4) != 0) && (strncmp(action, "clear", 5) != 0) && (strncmp(action, "exit", 4) != 0))
			printf("Enter a valid command.\n\n\n\n");
		else if (!strncmp(action, "new", 3))
			start = newListItem(start);
		else if (!strncmp(action, "remove", 6))
			removeItem(start);
		else if (!strncmp(action, "print", 5))
			printList(start);
		else if (!strncmp(action, "save", 4))
			saveList(start);
		else if (!strncmp(action, "load", 4))
			start = loadList(start);
		else if (!strncmp(action, "clear", 5))
			clearList(start);
		else if (!strncmp(action, "exit", 4))
			exit(0);
	}
	return 0;
}

/******************Functions*******************/
toDoList* newListItem(toDoList *start){
	toDoList *currentItem;
	char task[99] = { 0 };
	char deadline[99] = { 0 };
	char location[99] = { 0 };
	printf("Enter the task: ");
	scanf("%[^\n]s", task);
	flashStandardInput();
	newline();
	printf("Enter the deadline: ");
	scanf("%[^\n]s", deadline);
	flashStandardInput();
	newline();
	printf("Enter the location: ");
	scanf("%[^\n]s", location);
	flashStandardInput();
	newline();
	currentItem = (toDoList*)malloc(sizeof(toDoList));
	if (currentItem == NULL)	{
		fprintf(stderr, "Not enough RAM space");
		exit(-1);
	}
	strcpy(currentItem->task, task);
	strcpy(currentItem->deadline, deadline);
	strcpy(currentItem->location, location);
	currentItem->next = start;
	start = currentItem;
	printf("List Item %s entered...", currentItem->task);
	return start;
}

void removeItem(toDoList *start) {
	if (start == NULL)
		printf("There are no items on this list.");
	else
	{
		toDoList *currentItem = start;
		toDoList *deleteItem = NULL;
		char strPosition[3] = { 0 };
		short position = 0;
		printf("Enter number of item you wnat to remove: ");
		scanf("%2[0-9]", strPosition);
		flashStandardInput();
		newline();
		position = atoi(strPosition);
		for (int i = 1; i < position - 1; i++)
			currentItem = currentItem->next;				//stepping to item which is next to the item which is to be removed.
		deleteItem = currentItem->next;						//deleteItem becomes the item which is to be removed
		currentItem->next = deleteItem->next;				//next pointer of current item pointing to the item after deleteItem, thus deleteItem is not connected to previous it
		deleteItem->next = NULL;							//deleteItem is completly seperate from list
		free(deleteItem);									//deleteItem is removed	
	}
}

void clearList(toDoList *start) {
	toDoList *freeItem = start;
	toDoList *tempItem = NULL;
	while (freeItem != NULL){
		tempItem = freeItem->next;
		free(freeItem);
		freeItem = tempItem;
	}
}

void printList(toDoList *start) {
	toDoList *currentItem = start;
	int i = 0;
	if (currentItem == NULL)
		printf("The list is empty.\n\n\n");
	else{
		while (currentItem != NULL) {
			printf("Task #%d\n", ++i);
			printf("Task:\t\t%s\n", currentItem->task);
			printf("Deadline:\t%s\n", currentItem->deadline);
			printf("Location:\t%s\n\n\n", currentItem->location);
			currentItem = currentItem->next;
		}
	}
}

void saveList(toDoList *start) {
	FILE *fp = fopen("list.bin", "wb");								//creates file
	if (fp == NULL) {
		fprintf(stderr, "ERROR OPENING FILE\n");					
		return;
	}else{
		toDoList *currentItem = start;								//current item is set to start
		toDoList *tempNext = NULL;									//tempNext needed to save null pointer to file
		while (currentItem != NULL)	{							
			tempNext = currentItem->next;							
			currentItem->next = NULL;
			fseek(fp, 0, SEEK_END);									//cursor set to end of file
			fwrite(currentItem, sizeof(toDoList), 1, fp);			//current item with next pointed to null gets written to file
			currentItem->next = tempNext;							//next pointer gets original value back to keep integrety of list
			tempNext = NULL;											
			currentItem = currentItem->next;						//pointer set to next item
		}
		fclose(fp);													//closing file			
		fp = NULL;						
	}
}

toDoList *readNextItemFromFile(toDoList *start, FILE *fp) {
	int value;
	if (start == NULL)
	{
		start = malloc(sizeof(toDoList));
		value = fread(start, sizeof(toDoList), 1, fp);
		start->next = NULL;
	} else {
		toDoList *newItem = malloc(sizeof(toDoList));
		value = fread(newItem, sizeof(toDoList), 1, fp);
		newItem->next = start;
		start = newItem;
	}
	return start;
}

toDoList* loadList(toDoList *start) {
	FILE *fp = fopen("list.bin", "rb");								//opens file
	if (fp == NULL) {
		fprintf(stderr, "ERROR OPENING FILE\n");
		return NULL;
	}else {
		clearList(start);
		start = NULL;

		fseek(fp, 0, SEEK_END);
		long fileSize = ftell(fp);
		rewind(fp);

		int numOfItems = (int)(fileSize / (sizeof(toDoList)));
		for (int i = 0; i < numOfItems; i++)
		{
			fseek(fp, (sizeof(toDoList) * i), SEEK_SET);
			start = readNextItemFromFile(start, fp);
		}
	}
	return start;
}

void flashStandardInput(void)
{
	int intCharacter;
	while ((intCharacter = getchar()) != '\n' && intCharacter != EOF);
}

void newline(void) {
	printf("\n");
}


/********************Sichere Abfrage*******************
do {
	printf("Bitte Dimension der Matrix eingeben (1, 2 oder 3 Dimensionen): ");
	check = scanf("%lf", &dimensionen);
	flashStandardInput();
	newline();
} while (check == 0 || dimensionen < 1 || dimensionen > 3 || dimensionen != (int)dimensionen); //sichere Abfrage
*/
