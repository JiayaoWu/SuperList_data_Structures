/*Jiayao Wu   Project 3*/


#include <stdio.h>
#include <stdlib.h>
#include "sl_repository.h"
#include "rand100.h"

/*make dummy_element object*/
typedef struct dummy_element {
	int *val;
	int key;
	int numOfRepeat;
	struct dummy_element *next;
	struct dummy_element *down;
} element;

/*Number of levels*/
int numOfLevels;

/*Initial dynamic head*/
element *sentinel_head;


int i, j, k, l;
/*Number of unique items*/
int size;

/*The probability of promoting an element to the next level*/
int percent;

/*for keeping track of num of steps performed so far*/
static unsigned long int steps;


/*initialize the super list*/
void Repository_init( int p) {
	percent = p;
	numOfLevels = 1;
	size = 0;
	steps = 0;
	sentinel_head = malloc(sizeof(element));
	sentinel_head->next = NULL;
	sentinel_head->down = NULL;
}


/*Insert the key(in ascending order),data if 
 * this key doesn't exit in the list and return 1
 * Update the data if this key exists and return 0
 * return -1 if there is memory issue*/
int Repository_update( int key, int data) {
	i = numOfLevels - 1;
	/*array of pointers to store elements for future use*/
	element *array[numOfLevels]; 
	element *temp = sentinel_head;
		
	/*In order to traverse to the end of the list if the needed*/
	while(temp != NULL) {
		/*in order to traverse till the end of the level if needed*/
		while(temp->next != NULL) {
			if(key > temp->next->key) {
				/*move to next element*/
				temp = temp->next;
				steps++;
			} else if(key < temp->next->key) {
				break;				
			} else if(key ==temp->next->key) {
				/*change the data*/
				*temp->next->val = data;
				return 0;
			}

		}
		/*store temp first, then decrement i*/
		array[i--] = temp;
		temp = temp->down;
		steps = steps + 2;
	}

	/*Now we have reached the bottom level
 	* reinitialize i and j to 0
 	* j: for keeping track of number of repeatness of a certain element*/
	i = 0;
	j = 0;

	/*for keeping track of which element should the pelement point down to*/
	element *tempS;
	
	/*inserting the pelement*/
	do {
		/*malloc space for the element being inserted*/
		element *pelement;
		pelement = malloc (sizeof(element));
		if(pelement==NULL) return -1;
		if(i == 0) {
			/*if at the bottom line, malloc space for val*/
			pelement->val = malloc(sizeof(int)); 
			if(pelement->val==NULL) return -1;
			*pelement->val = data;
		} else {
			/*point val to the previous inserted element's val*/
			pelement->val = tempS->val;
		}
		pelement->key = key;
		
		/*if there is no need to create new level*/
		if(i < numOfLevels) {
			pelement->next = array[i]->next;
			array[i]->next = pelement;
			steps = steps + 2;
			/*if at the bottom level*/
			if (i == 0) {
				pelement->down = NULL;
			} else { 
				/*not at the bottom level*/
				pelement->down = tempS;
				steps++;
			}
			i++;
			tempS = pelement;
			steps++;
		} else {
			/*need to create a new level*/
			/*malloc  new header each time*/		
			element *newheader;
			newheader = malloc (sizeof(element));
			if(newheader == NULL) return -1;
			/*point newheader down to the previous sentinel_head*/
			newheader->down = sentinel_head;
			sentinel_head = newheader;
			sentinel_head->next = pelement;
			pelement->down = tempS;
			pelement->next = NULL; /*since it's a new level*/
			tempS = pelement;
			steps = steps + 3;
			numOfLevels++;
			i++;
		}
		j++;
	} while (rand100() <= percent);
	/*array[0] would be the element on the bottom level*/
	array[0]->next->numOfRepeat = j;
	size++;
	return 1;
}



/**
 * Store the data into *data if key is found
 * return 1 if found, 0 otherwise
 */

void Repository_print( int print_elements ) {
	/*keep track of how many records on each level there are */
	int eachLevel[numOfLevels];
	/*Intialize every position in the eachLevel array is 0*/
	for (i = 0; i < numOfLevels; i++) {
		eachLevel[i] = 0;
	}	
	
	/*to count how many records there are on each level
 	*by traversing through the superlist: */ 	
	j = numOfLevels - 1;
	element *temp = sentinel_head;
	/*temp2 is used in order to move to the next head 
 	*if current line is done search*/ 	
	element *temp2 = sentinel_head;
	for(i = 0; i < numOfLevels; i++) {
		while (temp->next != NULL) {
			eachLevel[j]++;
			temp = temp->next;
		}
		/*move to the next level*/
		temp = temp2->down;
		temp2 = temp;
		j--;
	}

	printf("\nThe number of unique records: %d\n",  size);
	printf("The number of levels: %d\n", numOfLevels);
	for (j = numOfLevels-1; j >= 0; j--) {
		printf("The total number of records on level %d is %d\n", j + 1, eachLevel[j]);

	}
	printf("The number of next steps so far: %d\n\n", steps);


	/*print the actual content of the superlist: */
	/*first go down to the bottom level*/
	temp = sentinel_head;
	for(i = 1; i < numOfLevels; i++) {
		temp = temp->down;
	}
	/*now we have reached the bottom level*/
	if(print_elements == 1) {
		/*only print unique ones which are on the bottom level*/
		while(temp->next != NULL) {
			printf("{%d, %d}\n", temp->next->key, *temp->next->val);
			temp = temp->next;
		}
		printf("\n");
	} else if (print_elements == 2) {
		/*print the list that mimics the structur
 		*here I flipped the list from horizontal to vertical
		*see design for more details*/ 
		while(temp->next != NULL) {
			for(i = 0; i < temp->next->numOfRepeat;i++) {
				printf("<---{%d, %d} ", temp->next->key, *temp->next->val);
			}
			printf("\n");
			temp = temp->next;		
		}
	}

}



/**
 * Store the data into *data if key is found
 * return 1 if found, 0 otherwise
*/
int Repository_get( int key, int *data) {
	element *temp = sentinel_head;
	while( temp != NULL) {
		while (temp->next != NULL) {
			if (key > temp->next->key) {
				temp = temp->next;
				steps++;
			} else if (key < temp->next->key) {
				break;
			} else if (key == temp->next->key) {
				*data = *temp->next->val;
				return 1;
			}
		}	
		temp = temp->down;
		steps++;
	}
	return 0;   /*key not found*/
}



/*removes a record if such key is found and return 1.
 * return 0 if not found*/
int Repository_delete( int key ) {
	element *temp = sentinel_head;
	j = numOfLevels - 1;
	/*l is used in order to know if any element is deleted
 	* will be changed to 1 if anything is deleted*/
	l = 0;
	while (temp != NULL) {
		while (temp->next != NULL) {
			if (key > temp->next->key) {
				temp = temp->next;
				steps++;
			} else if (key < temp->next->key) {
				break;
			} else if (key == temp->next->key) {
				/*if we need to remove this whole level*/
				if(temp == sentinel_head && j != 0 && temp->next->next == NULL) {
					element *header = sentinel_head;
					element *pelement = temp->next;
					steps = steps + 2;

					temp->next = temp->next->next;
					free(pelement);
					pelement = NULL;
					sentinel_head = sentinel_head->down;
					free(header);
					header = NULL;
					numOfLevels--;
					l = 1;
				} else {		        
	
				/*remove it like in project 2*/
				element *pelement = temp->next;  /*so that I can free it later*/
				temp->next = temp->next->next;
				steps = steps + 2;
				free(pelement);
				pelement = NULL;
				l = 1;
				/*then move to the next level*/
				}
				break;
        		}	
		}
		/*go down the list*/
		temp = temp->down;
		steps++;
		j--;
	}
	if (l == 1) {
		/*this means something is deleted succesfully*/
		size--;
		return 1;
	} else {
		return 0;
	}

}


