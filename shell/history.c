#include "history.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "defs.h"
#include "utils.h"

struct history* init_history()
{
	struct history *hist = malloc(sizeof(struct history));
	if (hist == NULL) {
		perror("Error, no se pudo inicializar el historial");
		return NULL;
	}
	hist->first=NULL;
	hist->last=NULL;
	load_history_from_file(hist);
	return hist;
}

struct history * create_aux_list(){
	struct history *hist = malloc(sizeof(struct history));
	if (hist == NULL) {
		perror("Error, no se pudo crear la lista auxiliar");
		return NULL;
	}
	hist->first=NULL;
	hist->last=NULL;
	return hist;
}

void add_command(struct history* hist, const char *command) {
	struct node *new_node = malloc(sizeof(struct node));
	if(new_node == NULL) {
		perror("Error, no se pudo agregar el comando al historial");
		return;
	}
	new_node->command = malloc(strlen(command) + 1);
	if (new_node->command == NULL) {
		perror("Error, no se pudo agregar el comando al historial");
		return;
	}
	strcpy(new_node->command, command);
	if(hist->first == NULL){
		new_node->next=NULL;
		new_node->prev=NULL;
		hist->first = new_node;
		hist->last = new_node; 
	}
	new_node->next=NULL;
	new_node->prev = hist->last;
	hist->last->next = new_node;
	hist->last=new_node;
}
/*
void destroy_history(struct history* hist) {
	struct node *aux = hist->first;
	while (aux != NULL) {
		struct history *next = aux->next;
		free(aux->command);
		free(aux);
		aux = next;
	}
	free(hist);
}
*/

void print_history(struct history* hist) {
	struct node *aux = hist->first;
	while (aux != NULL) {
		if (aux->command != NULL) {
			printf("%s", aux->command);
		}
		aux = aux->next;
	}
}

bool cargar_comandos(struct history* hist){
	struct node * aux = hist -> first;
	char path[BUFLEN];
	strcat(path,getenv("HOME"));
	strcat(path,"/.fisop_history");
	FILE* new_file = fopen(path, "a");
		if (new_file == NULL) {
			perror("Error opening file");
			return false;
		}
	while(aux != NULL){
		fprintf(new_file,"%s\n",aux->command);
		aux = aux ->next;
	}
	fclose(new_file);
	return true;
}

void load_history_from_file(struct history* hist) {
	char* env_history = getenv("HISTFILE");
	if(env_history != NULL){
		FILE *file = fopen(env_history, "a");
	}
	char path[BUFLEN]="";
	strcat(path,getenv("HOME"));
	strcat(path,"/.fisop_history");
	FILE* file = fopen(path, "a+");
	
	if (file == NULL) {
		perror("Error, no se pudo abrir el archivo");
		return;
	}
	char line[BUFLEN]="";
	int contador =0;
	while (fgets(line,BUFLEN,file) != NULL) {
		printf_debug("%s",line);
		add_command(hist, line);
		contador+=1;
	}
	fclose(file);
}
