#include "defs.h"
#include "types.h"
#include "readline.h"
#include "runcmd.h"
#include "history.h"


static struct history * 
init_histfile()
{
	char* env_history = getenv("HISTFILE");
	if (env_history == NULL) {
		char* home_temp = getenv("HOME");
		char home_copy[BUFLEN];
		strcpy(home_copy, home_temp);
		char* path = strcat(home_copy, "/.fisop_history");
		FILE* new_file = fopen(path, "a");
		if (new_file == NULL) {
			perror("Error opening file");
		}
		fclose(new_file);
	}
	return init_history();
}


char prompt[PRMTLEN] = { 0 };

// runs a shell command
static void
run_shell()
{
	struct history * hist = NULL;
	struct history * lista_aux = NULL;

	#ifndef SHELL_NO_INTERACTIVE
    hist =init_histfile();
	lista_aux = create_aux_list();//esta lista guarda los comandos de la cesion actual 
    #endif

	char *cmd;
	while ((cmd = read_line(prompt)) != NULL) {
		if (hist != NULL && lista_aux != NULL){
			add_command(hist, cmd);
		    add_command(lista_aux,cmd);
		}
		if (run_cmd(cmd) == EXIT_SHELL){
			if(lista_aux != NULL){
				cargar_comandos(lista_aux);
				print_history(hist);
			}
			return;
		}
	}
}

// initializes the shell
// with the "HOME" directory
static void
init_shell()
{
	char buf[BUFLEN] = { 0 };
	char *home = getenv("HOME");

	if (chdir(home) < 0) {
		snprintf(buf, sizeof buf, "cannot cd to %s ", home);
		perror(buf);
	} else {
		snprintf(prompt, sizeof prompt, "(%s)", home);
	}
}

int
main(void)
{
	init_shell();
	run_shell();
	return 0;
}
