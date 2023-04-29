#include <ctype.h>
#include "builtin.h"
#include "utils.h"
extern int status;


int view_history(int num);

// returns true if the 'exit' call
// should be performed
//
// (It must not be called from here)
int
exit_shell(char *cmd)
{
	if (strcmp(cmd, "exit") == 0) {
		return 1;
	}
	return 0;
}

// returns true if "chdir" was performed
//  this means that if 'cmd' contains:
// 	1. $ cd directory (change to 'directory')
// 	2. $ cd (change to $HOME)
//  it has to be executed and then return true
//
//  Remember to update the 'prompt' with the
//  	new directory.
//
// Examples:
//  1. cmd = ['c','d', ' ', '/', 'b', 'i', 'n', '\0']
//  2. cmd = ['c','d', '\0']
int
cd(char *cmd)
{
	if (strcmp(cmd, "cd") == 0) {
		char *home = getenv("HOME");
		int change_dir = chdir(home);
		if (change_dir < 0) {
			printf_debug("Error, no se pudo cambiar de "
			             "directorio");
			status = 1;
			return 0;
		}
		snprintf(prompt,
		         sizeof prompt,
		         "(%s)",
		         home);  // actualizo el prompt
		return 1;
	}
	if (strlen(cmd) > 3 && cmd[0] == 'c' && cmd[1] == 'd' && cmd[2] == ' ') {
		char *dir = split_line(cmd, SPACE);
		int new_dir = chdir(dir);
		if (new_dir < 0) {
			printf_debug("Error, no se pudo cambiar de "
			             "directorio");
			status = 1;
			return 0;
		}
		char *buf = NULL;
		snprintf(prompt,
		         sizeof prompt,
		         "(%s)",
		         getcwd(buf, FNAMESIZE));  // actualizo prompt
		return 1;
	}
	return 0;
}

// returns true if 'pwd' was invoked
// in the command line
//
// (It has to be executed here and then
// 	return true)
int
pwd(char *cmd)
{
	// Your code here
	if (strcmp(cmd, "pwd") == 0) {
		char actual_dir[FNAMESIZE];
		if (getcwd(actual_dir, sizeof(actual_dir)) != NULL) {
			printf_debug("%s\n", actual_dir);
			return 1;
		}
	}
	return 0;
}


// returns true if `history` was invoked
// in the command line
//
// (It has to be executed here and then
// 	return true)

int
history(char *cmd)
{
	//save cmd in list of commands

	if (strcmp(cmd, "history") == 0) {
		return view_history(0);
	} else {
		char* contains_command = strstr(cmd, "history");
		if (contains_command != NULL) {
			char* number = split_line(cmd, SPACE);
			int i = 0;
			while (number[i] != '\0') {
				if (isdigit(number[i]) == 0) {
					printf_debug("No es un numero\n");
					status = 1;
					return 1;
				}
				i++;
			}
			int num = atoi(number);
			return view_history(num);
		}
		return 0;
	}
}
int
view_history(int num)
{
	//Recorremos la lista historial e imprimimos

	return 1;
}
