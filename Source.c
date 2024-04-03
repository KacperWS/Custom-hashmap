#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>   

typedef long long hash_t;

typedef struct hashmap{
	hash_t hash;
	struct element* list;
	struct hashmap* next;
	struct hashmap* prev;
}hashmap;

typedef struct {
	void* data;
	struct element* next;
}element;

typedef struct {
	char name[32];
	int index;
}student;

hashmap* create() {
	hashmap* test = malloc(sizeof(hashmap));
	test->list = NULL;
	test->hash = NULL;
	test->next = NULL;
	test->prev = NULL;
	return test;
}

void* findAndInsert(void* map, void* hash, int compare(void* a, void* b)) {
	hashmap* temp = (hashmap*)map;
	hash_t to_find = *((hash_t*)hash);

	while (temp != NULL) {
		int var = compare(&to_find, &temp->hash);
		switch (var) {
		case -1:
		{
			if (temp->prev == NULL) {
				hashmap* test = create();
				temp->prev = test;
				test->next = temp;
				temp = temp->prev;
				return (void*)test;
			}
			else if(temp->prev != NULL && temp->prev->hash < to_find) {
				hashmap* test = create();
				test->prev = temp->prev;
				test->next = temp;
				test->next->prev = test;
				test->prev->next = test;
				return (void*)test;
			}
			temp = temp->prev;
			break;
		}
		case 0:
		{
			return (void*)temp;
			break;
		}
		case 1:
		{
			if (temp->next == NULL) {
				hashmap* test = create();
				temp->next = test;
				test->prev = temp;
				temp = temp->next;
				return (void*)test;
			}
			else if (temp->next != NULL && temp->next->hash > to_find) {
				hashmap* test = create();
				test->prev = temp;
				test->next = temp->next;
				test->next->prev = test;
				test->prev->next = test;
				return (void*)test;
			}
			temp = temp->next;
			break;
		}
		}
	}
	return (void*)temp;
}

void push(void* map, void* data, void* hasher(void* hash), void* compare(void* a, void* b)) {
	hashmap* temp = (hashmap*)map;
	hash_t newHash = hasher(data);

	if (temp->list == NULL) {
		temp->hash = newHash;
		element* new_element = malloc(sizeof(element));
		new_element->data = data;
		new_element->next = NULL;
		temp->list = new_element;
	}
	else {
		temp = (hashmap*)findAndInsert((void*)temp, &newHash, compare);
		temp->hash = newHash;
		if (temp->list == NULL) {
			element* new_element = malloc(sizeof(element));
			new_element->data = data;
			new_element->next = NULL;
			temp->list = new_element;
		}
		else {
			element* new_element = malloc(sizeof(element));
			element* temp_list = temp->list;

			while (temp_list->next != NULL) {
				temp_list = temp_list->next;
			}

			temp_list->next = new_element;
			new_element->data = data;
			new_element->next = NULL;
		}
	}
}

void pop(void* map, void* hash) {
	hashmap* temp = (hashmap*)map;
	hash_t* toFind = (hash_t*)hash;
	while (temp != NULL) {
		int var = compare(toFind, &temp->hash);
		switch (var) {
		case -1:
		{
			temp = temp->prev;
			break;
		}
		case 0:
		{
			if (temp->list != NULL) {
				element* tempList = temp->list;
				element* prevNode = NULL;

				while (tempList->next != NULL) {
					prevNode = tempList;
					tempList = tempList->next;
				}

				free(tempList->data);
				free(tempList);

				if (prevNode != NULL) {
					prevNode->next = NULL;
				}
				else {
					temp->list = NULL;
				}
				return;
			}
			break;
		}
		case 1:
		{
			temp = temp->next;
			break;
		}
		}
	}
	printf("Fail");
}

void show(void* data, void* showResult(void* data)) {
	if (data == NULL)
		printf("Data is empty! \n");
	else
		showResult(data);
}

void removeMap(void* map) {
	hashmap* tempHead = (hashmap*)map;
	hashmap* tempNext = (hashmap*)map;
	tempNext = tempNext->prev;
	while (tempNext != NULL) {
		hashmap* tempDeleter = tempNext;
		tempNext = tempNext->prev;
		element* tempList = tempDeleter->list;
		while (tempList != NULL) {
			element* tempListDeleter = tempList;
			tempList = tempList->next;
			free(tempListDeleter->data);
			free(tempListDeleter);
		}
		free(tempDeleter);
	}
	tempNext = tempHead;
	while (tempNext != NULL) {
		hashmap* tempDeleter = tempNext;
		tempNext = tempNext->next;
		element* tempList = tempDeleter->list;
		while (tempList != NULL) {
			element* tempListDeleter = tempList;
			tempList = tempList->next;
			free(tempListDeleter->data);
			free(tempListDeleter);
		}
		free(tempDeleter);
	}
}

void* get(void* map, void* hash, void* compare(void* a, void* b), void* number) {
	hashmap* temp = (hashmap*)map;
	hash_t *to_find = (hash_t*)hash;
	while (temp != NULL) {
		int var = compare(to_find, &temp->hash);
		switch (var) {
		case -1:
		{
			temp = temp->prev;
			break;
		}
		case 0:
		{
			element* temp_list = temp->list;
			int* numb = number;
			int counter = 1;
			while (temp_list->next != NULL && counter != *numb) {
				temp_list = temp_list->next;
				counter++;
			}
			if (counter == *numb)
				return (void*)temp_list->data;
			else return NULL;
			break;
		}
		case 1:
		{
			temp = temp->next;
			break;
		}
		}
	}
	return NULL;
}

int compare(void* a, void* b) {
	hash_t* hashA = (hash_t*)a;
	hash_t* hashB = (hash_t*)b;

	if (*hashA < *hashB) {
		return -1;
	}
	else if (*hashA == *hashB) {
		return 0;
	}
	else {
		return 1;
	}
}

hash_t hash(void* variable) {
	hash_t hash = 0; int a = variable;
	a /= 2;
	for (int i = 0; i < 32; i++) {
		hash += a % 5;
	}
	return hash;
}

void showStudent(student* a) {
	printf("%s %d", a->name, a->index);
	printf("\n");
}

int main() {
	hashmap* mapa = create();
	student* table[10];
	char xd[10] = { 'a','b','c','d','e','f','g','h','i','j' };
	for (int i = 0; i < 10; i++) {
		table[i] = malloc(sizeof(student));
		table[i]->index = i;
		table[i]->name[0] = xd[i];
		table[i]->name[1] = '\0';
		table[i]->name[2] = '\0';
		push(mapa, table[i], &hash, &compare);
		hash_t tempHash = hash(table[i]); 
		int abc = 2; int* number = &abc;
		show(get(mapa, &tempHash, &compare, number), &showStudent);
	}
	hash_t tempHash = hash(table[5]);
	pop(mapa, &tempHash);
	removeMap(mapa);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	return 0;
}