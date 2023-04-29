#ifndef NARGS
#define NARGS 4
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define LENGHT 100
#define ARG_LENGHT NARGS + 2

void
destruir_paquete(char **paquete)
{
	int contador = 1;
	while (paquete[contador] != NULL) {
		free(paquete[contador]);
		contador += 1;
	}
	free(paquete);
}


char **
crear_paquete(char **contenido, int largo, char *comando)
{
	char **paquete = (char **) malloc((largo + 1) * sizeof(char *));
	if (paquete == NULL) {
		return NULL;
	}
	int i;
	paquete[0] = comando;
	for (i = 1; i < largo; i++) {
		size_t lenght = strlen(contenido[i]) + 1;
		paquete[i] = (char *) malloc(lenght * sizeof(char));
		if (paquete[i] == NULL) {
			destruir_paquete(paquete);
			return NULL;
		}
		strcpy(paquete[i], contenido[i]);
	}
	paquete[i] = NULL;
	return paquete;
}

void
destruir_contenedor(char ***contenedor)
{
	int contador = 0;
	while (contenedor[contador] != NULL) {
		destruir_paquete(contenedor[contador]);
		contador += 1;
	}
	free(contenedor);
}

void
limpiar_buffer(char **buffer)
{
	for (int i = 0; i < ARG_LENGHT; i++) {
		buffer[i] = NULL;
	}
}

void
ejecutar_comando(char **argumentos)
{
	int i = fork();

	if (i < 0) {
		perror("Error en el fork");
		exit(-1);
	}

	if (i == 0) {
		execvp(argumentos[0], argumentos);
	} else {
		wait(NULL);
	}
}

int
main(int argc, char *argv[])
{
	if (argc > 2) {
		printf("Pasaste demasiados argumentos intenta lo siguiente : "
		       "./xargs <argumento>\n");
		return 1;
	}
	char ***contenedor_paquetes = (char ***) malloc(LENGHT * sizeof(char **));

	if (contenedor_paquetes == NULL) {
		return -1;
	}


	char *comando = argv[1];
	char *paquete[NARGS + 2];

	int cantidad_paquetes = 0;
	int filas_leidas = 1;
	char nueva_linea = '\n';
	char *line;
	size_t bufsize = 0;
	while (getline(&line, &bufsize, stdin) != EOF) {
		char *barra_n = index(line, nueva_linea);
		*barra_n = '\0';
		size_t lenght = strlen(line) + 1;
		paquete[filas_leidas] = (char *) malloc(lenght * sizeof(char));

		if (paquete[filas_leidas] == NULL) {
			destruir_contenedor(contenedor_paquetes);
			return (-1);
		}
		strcpy(paquete[filas_leidas], line);
		filas_leidas += 1;
		if (filas_leidas == NARGS + 1) {
			char **new_pack =
			        crear_paquete(paquete, filas_leidas, comando);
			if (new_pack == NULL) {
				destruir_contenedor(contenedor_paquetes);
				return (-1);
			}
			contenedor_paquetes[cantidad_paquetes] = new_pack;
			filas_leidas = 1;
			cantidad_paquetes += 1;
			limpiar_buffer(paquete);
		}
	}
	char **new_pack = crear_paquete(paquete, filas_leidas, comando);
	contenedor_paquetes[cantidad_paquetes] = new_pack;
	contenedor_paquetes[cantidad_paquetes + 1] = NULL;
	int contador = 0;
	while (contenedor_paquetes[contador]) {
		ejecutar_comando(contenedor_paquetes[contador]);
		contador += 1;
	}
	destruir_contenedor(contenedor_paquetes);
}
