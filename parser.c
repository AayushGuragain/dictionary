#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "card.h"
#include <errno.h>

typedef struct tracking_device {
	char *name;
	long track;
} tracking_device;
typedef struct card CARD;
char *comma_seperator(char *buf, char *tmp);
char *text_breaker(char *tmp);
char *text_breaker2(char *tmp);
void output_to_file(CARD **pointers_to_cards[]);
void sort(int n, tracking_device** pointers_to_cards);
void parserout_index( int line, tracking_device** tracker);
void parserout_card( CARD** pointers_to_cards, int line, tracking_device** tracker);
int duplicate_killer(int n, CARD** pointers_to_cards);


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("ERROR!\n");
        return -1;
    }
    //Pointers of Pointer for storing all the cards
    CARD **pointers_to_cards = malloc(sizeof(CARD*));
    pointers_to_cards[0] = malloc(sizeof(CARD));
    
    FILE *csvdata = fopen(argv[1], "r");
    

    if (csvdata == NULL){
	fprintf(stderr, "./parser: cannot open(\"%s\"): No such file or directory\n", argv[1]);
	free(pointers_to_cards[0]);
	free(pointers_to_cards);
    	exit(1);
    	
    } 
    
  
    char *input = malloc (1024); //Allocating memeory for file line reader
    char *buf; // pointer to move within the line
    char *tmp = calloc(1, 1024);//For storing individual pieces of CARD
    fgets(input, 1024, csvdata);
    int line = 0;
    int perm_line;

    int hold_id;
    int flag_restore;
    
    while ( fgets(input, 1024, csvdata))
    {  
    	flag_restore = 0;
        buf = input;
        buf = comma_seperator(buf,tmp);
        pointers_to_cards[line] -> id = atoi(tmp);
        
        hold_id = pointers_to_cards[line] -> id;

        buf = comma_seperator(buf,tmp);
        pointers_to_cards[line]-> name = strdup(tmp);
        //here it checks for duplicates
        int k = duplicate_killer(line, pointers_to_cards);
        if (k != -2) 
        {
        	free(pointers_to_cards[line] -> name);
        	if ( pointers_to_cards[k] -> id < pointers_to_cards[line] -> id) {
        	        pointers_to_cards[k] -> id = hold_id;
		        free(pointers_to_cards[k] -> cost);
		        free(pointers_to_cards[k] -> type);
		        free(pointers_to_cards[k] -> text);
		        free(pointers_to_cards[k] -> stats);
		        perm_line = line;
		        line = k;
		        flag_restore = 1;
            	}
        	
        else {
            	continue;
        }
        	//If k is replaced by line, it ocasianally works, but at times removes the one with the higher id.
        }
        buf = comma_seperator(buf,tmp);
        pointers_to_cards[line] -> cost = strdup(tmp);
        buf = comma_seperator(buf,tmp);
        pointers_to_cards[line] -> converted_cost = atoi(tmp);
        buf = comma_seperator(buf,tmp);
        pointers_to_cards[line] -> type = strdup(tmp);
        buf = comma_seperator(buf,tmp);
        tmp = text_breaker2(tmp);
        tmp = text_breaker(tmp);
        pointers_to_cards[line] -> text = strdup(tmp);
        buf = comma_seperator(buf,tmp);
        pointers_to_cards[line] -> stats = strdup(tmp);
        buf = comma_seperator(buf,tmp);
        if ( strcmp (tmp, "common")==0){
        	pointers_to_cards[line] -> rarity = 0;
        }
        else if (strcmp (tmp, "uncommon")==0){
        	pointers_to_cards[line] -> rarity = 1;
        }
        else if (strcmp (tmp, "rare")==0){
        	pointers_to_cards[line] -> rarity = 2;
        }
        else {
        	pointers_to_cards[line] -> rarity = 3;
        }
        
        if (flag_restore == 1)
        {
        	line = perm_line;
        }
        else{
            line++;
            pointers_to_cards = realloc(pointers_to_cards, (line+1) * sizeof(CARD*));
            pointers_to_cards [line] = malloc (sizeof(CARD));
        }
    }
    	//tracking_device tracker[line];
    	
    	
    	//tracking_device ** tracker = malloc ();
    	
    	
    	tracking_device **tracker = malloc(sizeof(tracking_device*) * line);
	

	parserout_card(pointers_to_cards,line, tracker);//unsorted entries into cards.bin
	parserout_index(line, tracker);//sorted entries into index.bin

    
    //parserout(pointers_to_cards, line);
	for (int i =0; i< line; i++){
    		free(pointers_to_cards[i] -> name);
		free(pointers_to_cards[i] -> cost);
		free(pointers_to_cards[i] -> type);
		free(pointers_to_cards[i] -> text);
		free(pointers_to_cards[i] -> stats);
		free(pointers_to_cards[i]);
		free(tracker[i]->name);
		free(tracker[i]);
	}
	free(pointers_to_cards[line]);
	free(pointers_to_cards);
	free(input);
	free(tmp);
	free(tracker);
	fclose(csvdata);
    return 0;
}
char * comma_seperator(char *buf,char *tmp)
{
    if (*buf == 34){//34 is ascii value for "
        buf++;
        int flag =0;
        char *track;
        track = buf;
        int counter = 0;
        while(1){
        	if ( (*track == 34) && ((*(track+1) == 44) || (*(track+1) == 10) )) {
        	//44 is ascii value for ,
                if ((*(track+2) == '\n')){
                    flag =1;
                }
                strncpy(tmp, buf, counter);
                tmp[counter] = '\0';
                //printf("%s\n\n", tmp); Checkpoint
                break; 
        	} 
            
            else {
                track ++;
                counter++;
        	}
        }
        //strcpy(tmp, strsep(&buf, "\""));
        buf = counter + buf;
        if (flag){
            return (buf+1);
        }
        else{
             return (buf + 2);
        }						
       
    } 
    else { 
        strcpy(tmp, strsep(&buf, ","));
        return buf;
    }
}
char *text_breaker(char *tmp) 
{
    char new_tmp [1024];
    int a = 0, b = 0;
    do
    {
    //if / and n are NOT one after another, it does nothing.
    if (!(tmp[a]==92 && tmp[a+1] == 110)){
        new_tmp[b] = tmp[a];
        b++;
        }
    else {
    //else / and n are one after another, it adds a newline, ascci value is 10.
        new_tmp[b] = 10;
        b++;
        a++;
        }
        a++;
        new_tmp[b+1] = 00;//This is the newline at the end of the text
    } while (tmp[a] != 00);
    tmp[a] = 00;
	strcpy(tmp, new_tmp);
    return tmp;
}

char *text_breaker2(char *tmp) 
{
    char new_tmp [1024];
    int a = 0, b = 0;
    do
    {
    //if " and " are NOT one after another, it does nothing.
    if (!(tmp[a]==34 && tmp[a+1] == 34)){
        new_tmp[b] = tmp[a];
        b++;
        }
    else {
    //else " and " are one after another, it adds a newline, ascci value is 10.
        new_tmp[b] = 34;
        b++;
        a++;
        }
        a++;
        new_tmp[b+1] = 00;//This is the newline at the end of the text
    } while (tmp[a] != 00);
    tmp[a] = 00;
	strcpy(tmp, new_tmp);
    return tmp;
}

void sort(int n, tracking_device** pointers_to_cards)
{ 

    int i, j; 
    tracking_device *t; 
    for (i = 0; i < n; i++) { 
        for (j = i+1; j < n; j++) {
        //if cards name is not in alphabetical order, puts in alphabetical order.
           if (strcmp( pointers_to_cards[j] -> name, pointers_to_cards[i] -> name) < 0){ 
                t = pointers_to_cards[i];
                pointers_to_cards[i] = pointers_to_cards[j];
                pointers_to_cards[j] = t;
            } 
        } 
    } 
}
int duplicate_killer(int n, CARD** pointers_to_cards)
{
    int i;
    int length1, length2;
    length1 = strlen(pointers_to_cards[n] -> name);
    for (i = 0; i < n; i++) { 
        length2 = strlen(pointers_to_cards[i] -> name);
        if (length1 == length2){
            if (strncmp(pointers_to_cards[i] -> name, pointers_to_cards[n] -> name, length2) == 0){
            		return i;
            } 
        }
    }
    return -2; 
}


void parserout_card( CARD** pointers_to_cards, int line, tracking_device** tracker)
{
	
	FILE* out2 = fopen("cards.bin", "wb");
	if( out2 == NULL) {
		printf ("ERROR\n");
	}
	unsigned int length;
	
	for (int i = 0; i < line;i ++){
	
	
		tracker[i] = malloc(sizeof(tracking_device));
		tracker[i]->name = strdup(pointers_to_cards[i] -> name);
		tracker[i]->track = ftell(out2);
		
		fwrite(&pointers_to_cards[i]->id, sizeof(int), 1, out2);
		
		length = strlen (pointers_to_cards[i] -> cost);
		fwrite(&length, sizeof(int), 1, out2);
		fwrite(pointers_to_cards[i]->cost, sizeof(char), length, out2);
		
		fwrite(&pointers_to_cards[i]->converted_cost, sizeof(int), 1, out2);
		
		length = strlen (pointers_to_cards[i] -> type);
		fwrite(&length, sizeof(int), 1, out2);
		fwrite(pointers_to_cards[i]->type, sizeof(char), length, out2);
		
		length = strlen (pointers_to_cards[i] -> text);
		fwrite(&length, sizeof(int), 1, out2);
		fwrite(pointers_to_cards[i]->text, sizeof(char), length, out2);
		
		length = strlen (pointers_to_cards[i] -> stats);
		fwrite(&length, sizeof(int), 1, out2);
		fwrite(pointers_to_cards[i]->stats, sizeof(char), length, out2);
		
		fwrite(&pointers_to_cards[i]->rarity, sizeof(int), 1, out2);
	
	}
	fclose(out2);

}


void parserout_index( int line, tracking_device** tracker)
{
	FILE* out = fopen("index.bin", "wb");
	if(out == NULL) {
		printf ("ERROR\n");
	}
	unsigned int length;
	long tracker2;
	sort(line, tracker);
	for (int i = 0; i < line;i ++){
		tracker2 = tracker[i]->track;
		length = strlen (tracker[i] -> name);
		fwrite(&length, sizeof(unsigned int), 1, out);
		fwrite(tracker[i]->name, sizeof(char), length, out);
		fwrite(&tracker2, sizeof(long), 1, out);
		
	
	}
	fclose(out);

}


