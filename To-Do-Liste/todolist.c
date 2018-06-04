/*
task		To-Do-List
author		Florian Bopp
date		4.6.18

summary:	This program gives the user a way to save and load his todo list from or to a file.
					It enables the user to add items to the list or remove them, once an item  
					can be crossed off, of the list.
					This is acomplished using a singly-linked list and fwrite/fread functions.
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
toDoList* removeItem(toDoList *start);
toDoList* clearList(toDoList *start);
void printList(toDoList *start);
void saveList(toDoList *start);
toDoList* loadList(toDoList *start);
toDoList *readNextItemFromFile(toDoList *start, FILE *fp);
void printTitle(void);
void flashStandardInput(void);
void newline(void);


/******************Main function********************/
int main(void) {

	/************initialization**************/
	toDoList *start = NULL;													//only pointer needed in main()										
	bool loop = true;														//loop variable to keep loop running forever until exited
	char action[7];															//input string variable for user to give commands


	printTitle();

	while (loop){
		printf("Enter one of the following commands.\n\n");
		printf("\tto create a new list item enter:\t\tnew\n");
		printf("\tto remove a list item enter:\t\t\tremove\n");
		printf("\tto clear the list enter:\t\t\tclear\n");
		printf("\tto print the list enter:\t\t\tprint\n");
		printf("\tto save the list enter:\t\t\t\tsave\n");
		printf("\tto load the list from a file enter:\t\tload\n");
		printf("\tto exit and safe program enter:\t\t\texit\n");
		printf("\tto exit without saving program enter:\t\tescape\n\n");


		scanf("%6[^\n]", action);
		flashStandardInput();
		newline();
		system("cls");											
		printTitle();
		if ((strcmp(action, "new") != 0) && (strcmp(action, "remove") != 0) &&		//checking if command which is put in is allowed
			(strcmp(action, "print") != 0) && (strcmp(action, "save") != 0) && 
			(strcmp(action, "load") != 0) && (strcmp(action, "clear") != 0) && 
			(strcmp(action, "exit") != 0) && (strcmp(action, "escape") != 0))
			printf("Enter a valid command.\n\n\n\n");
		else if (!strcmp(action, "new"))											//checking which command was entered to start according function
			start = newListItem(start);
		else if (!strcmp(action, "remove"))
			start = removeItem(start);
		else if (!strcmp(action, "print"))
			printList(start);
		else if (!strcmp(action, "save"))
			saveList(start);
		else if (!strcmp(action, "load"))
			start = loadList(start);
		else if (!strcmp(action, "clear"))
			start = clearList(start);
		else if (!strcmp(action, "exit"))
			exit(0);
		else if (!strcmp(action, "escape"))
			return 0;
	}
	return 0;																		//only needed for coding style since it is never reached
}

/******************Functions*******************/
toDoList* newListItem(toDoList *start){														
	toDoList *currentItem;														//pointing to current item
	char task[99] = { 0 };														//strings for payload
	char deadline[99] = { 0 };
	char location[99] = { 0 };
	printf("Enter the task: ");													//user enters payload information
	scanf("%98[^\n]s", task);
	flashStandardInput();
	newline();
	printf("Enter the deadline: ");
	scanf("%98[^\n]s", deadline);
	flashStandardInput();
	newline();
	printf("Enter the location: ");
	scanf("%98[^\n]s", location);
	flashStandardInput();
	newline();
	currentItem = (toDoList*)malloc(sizeof(toDoList));							//currentItem receives address to allocated memory of the struct size
	if (currentItem == NULL)	{												//if not enough memory
		fprintf(stderr, "Not enough RAM space");
		exit(-1);
	}
	strcpy(currentItem->task, task);											//copying strings to the currentItem
	strcpy(currentItem->deadline, deadline);
	strcpy(currentItem->location, location);
	currentItem->next = start;													//currentItem.next now pointing to the previous start
	start = currentItem;														//start is now pointing to the currentItem, meaning currentItem is the new start of the list			
	printf("List Item '%s' has been entered...\n\n", currentItem->task);
	return start;																//start pointer is returned to main()
}

toDoList* removeItem(toDoList *start) {
	if (start == NULL){
		printf("There are no items on this list.");
		return NULL;
	}
	else
	{
		printList(start);												//printing current list to view the coresponing list item numbers
		toDoList *currentItem = start;
		toDoList *deleteItem = NULL;
		char strPosition[3] = { 0 };
		short position = 0;
		printf("Enter number of item you wnat to remove: ");
		scanf("%2[0-9]", strPosition);
		flashStandardInput();
		newline();
		position = atoi(strPosition);
		if (position == 1) {												//if item to be deletet is the first item
			deleteItem = currentItem;									//set first item to delete and current to next
			currentItem = deleteItem->next;								//and current to next
			start = currentItem;										//new start to current
			deleteItem->next = NULL;									//deleteItem is completly seperate from list
			free(deleteItem);											//deleteItem is removed	
			printf("Item removed...\nthis is the current list:\n\n");
			printList(start);
			return start;
		}
		else if (position > 1) {
			int i;
			for (i = 1; (i < position - 1) && (currentItem->next != NULL); i++)
				currentItem = currentItem->next;						//stepping to item which is next to the item which is to be removed.
			if (currentItem->next == NULL) {							//if position was higher then number of items in list 
				printf("No such item as #%i.\n\n", position);
				return start;
			}
			deleteItem = currentItem->next;								//deleteItem becomes the item which is to be removed
			currentItem->next = deleteItem->next;						//next pointer of current item pointing to the item after deleteItem, thus deleteItem is not connected to previous it
			deleteItem->next = NULL;									//deleteItem is completly seperate from list
			free(deleteItem);											//deleteItem is removed
			printf("Item removed...\nthis is the current list:\n\n");
			printList(start);
			return start;
		}
		else {
			printf("no item was removed.\n\n");
			return start;
		}
	}
}

toDoList* clearList(toDoList *start) {						
	toDoList *freeItem = start;
	toDoList *tempItem = NULL;
	while (freeItem != NULL){											//as long as the pointer is pointing to a structure and not NULL
		tempItem = freeItem->next;										//the pointer to the next item is saved
		free(freeItem);													//free the allocated memory of the pointer
		freeItem = tempItem;											//freeItem now pointing to the next on the list
	}
	printf("Your list is now empty.\n\n");
	return NULL;
}

void printList(toDoList *start) {
	toDoList *currentItem = start;
	int i = 0;
	if (currentItem == NULL)
		printf("The list is empty.\n\n\n");
	else{
		while (currentItem != NULL) {										//while pointing to struct
			printf("Task #%d\n", ++i);
			printf("Task:\t\t%s\n", currentItem->task);
			printf("Deadline:\t%s\n", currentItem->deadline);
			printf("Location:\t%s\n\n\n", currentItem->location);
			currentItem = currentItem->next;								//now pointing to next list item
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
		printf("List has been saved successfully.\n\n");
	}
}

toDoList *readNextItemFromFile(toDoList *start, FILE *fp) {				
	int value;														//file for debugging purposes (should always equal to 1
	if (start == NULL) {											//if list is empty and the start pointer is pointing no no element 
		start = malloc(sizeof(toDoList));							//start receives pointer to allocated memory of the struct size
		value = fread(start, sizeof(toDoList), 1, fp);				//memory allocated to start pointer gets filled with bits of struct size from file
		start->next = NULL;											//next pointer pointing to NULL
	} else {														//if list is not empty
		toDoList *newItem = malloc(sizeof(toDoList));				//new item receives pointer to allocated memory of the struct size
		value = fread(newItem, sizeof(toDoList), 1, fp);			//memory allocated to newItem pointer gets filled with bits of struct size from file
		newItem->next = start;										//newItem next now pointing to the previos start
		start = newItem;											//start pointing to newItem which is the new start
	}
	return start;													//start needs to be returned otherwise the list is lost.
}

toDoList* loadList(toDoList *start) {
	FILE *fp = fopen("list.bin", "rb");								//opens file in read mode
	if (fp == NULL) {
		fprintf(stderr, "ERROR OPENING FILE\n");
		return NULL;
	}else {
		start = clearList(start);									//clearing list so no memory is wasted when overwritten with list from file
		system("cls");
		printTitle();
		fseek(fp, 0, SEEK_END);										//set cursor to end of file
		long fileSize = ftell(fp);									//save cursor position to filesize (cursor position = number of bytes)
		rewind(fp);													//cursor back to beginning

		int numOfItems = (int)(fileSize / (sizeof(toDoList)));		//calculate number of structs that are saved in file. filesize divided by size of one struct equals number of structs
		for (int i = 0; i < numOfItems; i++){						//for the number of structs
			fseek(fp, (sizeof(toDoList) * i), SEEK_SET);			//set cursor to beginning of new struct with each iteration
			start = readNextItemFromFile(start, fp);				//start = return value of function
		}
	}
	rewind(fp);
	fclose(fp);
	printf("This has been loaded:\n\n");
	printList(start);
	return start;
}

void printTitle(void) {
	char underline[] = { 238, 238, 238,238,238 ,
		238 ,238 ,238 ,238 ,238 ,238 ,238 ,238 ,
		238 ,238, '\0' };
	printf("\tYour to-do-list\n");
	printf("\t%s\n\n\n", underline);
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
