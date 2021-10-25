#include "card.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> // int isatty(int fd);

 

typedef struct card CARD;
typedef struct _index_record 
{
	char *name;
	long offset;
} NmRD;

void remove_all_chars(char* str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}


// Code borrowed from: https://stackoverflow.com/a/61518604        	
char* toLower(char* s) {
  for(char *p=s; *p; p++) *p=tolower(*p);
  return s;
}
//l -> index of first of the array
//r -> index of last element f the array(total number of elements with the array)-1 [0->499]500
//x-> user input(data being searching) arr[l---r]

//Code for binarySearch function taken from https://www.geeksforgeeks.org/binary-search/
//Modified for fitting the homework's requirement.
int binarySearch(NmRD **arr, int r, char *x) 
{ 
	int l = 0;
	
	char *k1 = malloc(200); //=  toLower(arr[m]->name);
	char *k2 = malloc(200);
	while (l <= r) { 
        	int m = l + (r - l) / 2;
        	// =  toLower(x);
        	
        	

        	strcpy(k1, arr[m]->name);
        	strcpy(k2, x);
        	
        	
        	// Check if x is present at mid //strcmp (index_reader[m]->name, "common")==0
        	if (strncmp(toLower(k1), toLower(k2),strlen(x)-1) == 0){//-1 because last one is a newlin
        		free(k1);
        		free(k2);
            		return m;
            	} 
        	// If x greater, ignore left half 
        	else if (strncmp(toLower(k1), toLower(k2), strlen(x)-1) < 0) 
            		l = m + 1;
        	// If x is smaller, ignore right half 
        	else
            		r = m - 1;
            		 
	} 
	// if we reach here, then element was not present
	free(k1);
        free(k2);
	return -1;
}

void display_to_screen( CARD* pointers_to_cards, char *name)
{
	int line_len = 52;
	int indent;
	const char *the_Rarity[] = { "common", "uncommon", "rare", "mythic" };
	char *dash = "----------------------------------------------------";
	//printing everyting on terminal line by line, ident is the number of spaces.
	indent = line_len -(strlen (name) + strlen (pointers_to_cards -> cost));
	//Since all the name starts with an uppercase
	//For continuty making all usertyped names to uppercase.
	//if (name[0] >= 97 && name[0] <= 122){ 
	//	name[0] = name[0] -32;
	//}
	printf( "%s%*s%s\n",name, indent,"", pointers_to_cards -> cost);
		
	indent = line_len -(strlen (pointers_to_cards -> type) + strlen (the_Rarity[pointers_to_cards -> rarity]));
	printf("%s%*s%s\n", pointers_to_cards-> type, indent,"", the_Rarity[pointers_to_cards -> rarity]);
		
	printf("%s\n", dash);
		
	printf("%s\n", pointers_to_cards -> text);
	printf("%s\n", dash);
		
	indent = line_len -(strlen (pointers_to_cards-> stats));
	printf("%*s%s\n",indent, "", pointers_to_cards -> stats);
	printf("\n");
	//printf("\n");

} 


void index_to_card(long offset, char *name)
{
	//cards are chosen from cards_bin with respect to offset and *name the user provided
	FILE *cards_bin = fopen("cards.bin", "rb");

	CARD *cards = malloc(sizeof(CARD));
	fseek( cards_bin, offset, SEEK_SET);
	
	fread(&cards->id, sizeof(unsigned int), 1, cards_bin);
	
	int length_of_cost;
	fread(&length_of_cost, sizeof(int), 1, cards_bin);
	cards->cost = malloc(sizeof(char) * (length_of_cost + 1));
	fread(cards->cost, sizeof(char), length_of_cost, cards_bin);
	cards->cost[length_of_cost] = 00;
	
	fread(&cards->converted_cost, sizeof(int), 1, cards_bin);
	
	int length_of_type = 0;
	fread(&length_of_type, sizeof(int), 1, cards_bin);
	cards->type = malloc(sizeof(char) * (length_of_type + 1));
	fread(cards->type, sizeof(char), length_of_type, cards_bin);
	cards->type[length_of_type] = '\0';
	
	
	int length_of_text;
	fread(&length_of_text, sizeof(int), 1, cards_bin);
	cards->text = malloc(sizeof(char) * (length_of_text + 1));
	fread(cards->text, sizeof(char), length_of_text, cards_bin);
	cards->text[length_of_text] = 00;
	
	int length_of_stats;
	fread(&length_of_stats, sizeof(int), 1, cards_bin);
	cards->stats = malloc(sizeof(char) * (length_of_stats + 1));
	fread(cards->stats, sizeof(char), length_of_stats, cards_bin);
	cards->stats[length_of_stats] = 00;
	
	fread(&cards->rarity, sizeof(int), 1, cards_bin);
	
	display_to_screen(cards, name);
	free(cards->cost);
	free(cards->type);
	free(cards->text);
	free(cards->stats);
	free(cards);
	fclose(cards_bin);
   
} 

int main(void) {
	//char *checkInput;
	FILE *index_bin = fopen("index.bin", "rb");

	if (index_bin == NULL){
		fprintf(stderr, "./search: cannot open(\"%s\"): No such file or directory\n", "index.bin");
		//free(pointers_to_cards[0]);
		//free(pointers_to_cards);
	    	exit(1);
	} 
	int no_of_cards;
	

	
	NmRD **index_reader = malloc(sizeof(NmRD*));
	
	fseek(index_bin, 0, SEEK_END);
	
	long end_flag =  ftell(index_bin);
	fseek(index_bin, 0, SEEK_SET);
	int counter =0;

	int i;
	while(ftell(index_bin) != end_flag){
		
	//}
		i = counter;
	//for (int i = 0; i < no_of_cards; ++i) {
		index_reader[i] = malloc(sizeof(NmRD));
		
		int length_of_name;
		//name and offset likns to the cards_bin
		fread(&length_of_name, sizeof(int), 1, index_bin);
		index_reader[i]->name = malloc(sizeof(char) * (length_of_name + 1));
		fread(index_reader[i]->name, sizeof(char), length_of_name, index_bin);
		index_reader[i]->name[length_of_name] = '\0';
		
		fread(&index_reader[i]->offset, sizeof(long), 1, index_bin);
		counter++;
		index_reader = realloc(index_reader, (counter+1)*sizeof(NmRD*));
	
	}
	//fclose(end_flag);
	fclose(index_bin);
	no_of_cards = counter;
	char *userin = malloc(100);

	while (1) {
		printf(">> ");
		//int isatty(int fd);

		if(!fgets(userin, 100, stdin)){
			break;
		}
		//int isatty(userin);
		
		if (isatty(0) == 0){
			printf("%s", userin);		
		
		}
		
		
		
		if (userin[0] == 'q' && userin[1] == '\n') break;
		
		
		int val = binarySearch(index_reader,no_of_cards , userin); 
		//printf("%d", val); This 
		if (val == -1){
		remove_all_chars(userin, '\n');
		printf("./search: '%s' not found!\n", userin);//./search: 'Does not exist' not found!
		}
		else{
			index_to_card( index_reader[val]->offset, index_reader[val]->name);
		}
		
    	}
	
	for (int in =0; in< no_of_cards; in++){
		free(index_reader[in]->name);
		free(index_reader[in]);
	}
	free(index_reader);
	free(userin);
	return 0;
}