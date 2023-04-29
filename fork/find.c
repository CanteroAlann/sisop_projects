#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

void
buscar_contenido_en_directorio(char *buffer,
                               char *path,
                               DIR *dir,
                               char *patron,
                               char *(*contiene)(const char *cadena,
                                                 const char *pat))
{
	char *salto_de_linea = "\n";
	char *dir_act = ".";
	char *dir_sup = "..";
	struct dirent *de;
	char aux_path[PATH_MAX] = "";
	strcat(aux_path, path);
	while ((de = readdir(dir)) != NULL) {
		int tipo = de->d_type;
		char *nombre = de->d_name;
		char new_path[PATH_MAX] = "";
		if ((contiene(nombre, patron) != NULL) &&
		    (tipo == 8 || tipo == 4)) {
			strcat(buffer, aux_path);
			strcat(buffer, nombre);
			strcat(buffer, salto_de_linea);
			path = new_path;
		}
		if (tipo == 4 && (strcmp(dir_act, nombre) != 0) &&
		    (strcmp(dir_sup, nombre) != 0)) {
			int fd = openat(dirfd(dir), nombre, O_DIRECTORY);
			DIR *sub_dir = fdopendir(fd);
			strcat(path, aux_path);
			strcat(path, nombre);
			strcat(path, "/");
			buscar_contenido_en_directorio(
			        buffer, path, sub_dir, patron, contiene);
		}
		path = new_path;
	}
	closedir(dir);
	return;
}


int
main(int argc, char *argv[])
{
	if (argc < 1 || argc > 3) {
		printf("numero incorrecto de argumentos proba : ./find -i "
		       "<cadena>\n");
		printf("O la siguiente opcion : ./find <cadena> \n");
		return 1;
	}
	char *flag = "-i";
	if (argc > 2) {
		if (strcmp(argv[1], flag) != 0) {
			printf("pasaste mal el flag intenta lo siguiente: "
			       "./find -i <cadena>\n");
			return 1;
		}
	}
	char buffer[PATH_MAX] = "";
	char path[PATH_MAX] = "";
	char *patron;
	DIR *dir = opendir(".");
	char *(*contiene)(const char *, const char *);

	if (dir == NULL) {
		printf("No se pudo abrir el directorio");
		return 1;
	}
	if (strcmp(flag, argv[1]) == 0) {
		contiene = &strcasestr;
		patron = argv[2];
		buscar_contenido_en_directorio(buffer, path, dir, patron, contiene);
		printf("%s", buffer);
		return 0;
	}
	patron = argv[1];
	contiene = &strstr;
	buscar_contenido_en_directorio(buffer, path, dir, patron, contiene);
	printf("%s", buffer);
	return 0;
}
