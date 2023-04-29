#ifndef SHELL_HISTORY_H
#define SHELL_HISTORY_H

#include <stdbool.h>

struct node {
	char *command;
	struct node *next;
	struct node *prev;
};

struct history{
	struct node * first;
	struct node * last;
};

struct history* init_history();
struct history* create_aux_list();
void add_command(struct history* hist, const char *command);
void destroy_history(struct history* hist);
void print_history(struct history* hist);
void load_history_from_file(struct history* hist);
bool cargar_comandos(struct history* hist);
#endif  // SHELL_HISTORY_H
