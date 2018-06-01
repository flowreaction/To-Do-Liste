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
	char underline[] = { 238, 238, 238,238,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238 ,238,238,238,238,238,238,238,238,238, '\0'};
	bool loop = true;
	char action[7];
	//FILE *fp = fopen("list.dat", "rb");
	//if (fp == NULL){
	//	fp = fopen("list.dat","wb");
	//}
	//start = loadList(fp);


	
	
	while (loop){
		printf("\tYour to-do-list\n");
		printf("%s\n\n\n", underline);

		printf("what do you want to do?\n");
		printf("to create a new list item type:\t\t\tnew\n");
		printf("to remove a  list item type:\t\t\tremove\n");
		printf("to print the list type:\t\t\t\tprint\n");
		printf("to save the list type:\t\t\t\tsave\n");
		printf("to load the list from a file type:\t\tload\n");
		printf("to exit program type:\t\t\t\texit\n\n");

		scanf("%6[^\n]", action);
		flashStandardInput();

		if (!strcmp(action, "new"))
			start = newListItem(start);
		else if (!strcmp(action, "remove"))
			removeItem(start);
		else if (!strcmp(action, "print"))
			printList(start);
		else if (!strcmp(action, "save"))
			saveList(start);
		else if (!strcmp(action, "load"))
			loadList(start);
		else if (!strcmp(action, "exit"))
		{
			exit(0);
		}
		
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
	return start;
}

void removeItem(toDoList *start) {

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
	while (currentItem != NULL) {
		printf("Task #%d\n", ++i);
		printf("Task:\t\t%s\n", currentItem->task);
		printf("Deadline:\t%s\n", currentItem->deadline);
		printf("Location:\t%s\n\n\n", currentItem->location);
		currentItem = currentItem->next;
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
	FILE *fp = fopen("list.bin", "rb");								//creates file
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
			fseek(fp, (sizeof(toDoList)*i), SEEK_SET);
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
