#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>


void
proceso_hijo(int fd)
{
	int primo;
	read(fd, &primo, sizeof(primo));
	printf("primo %d\n", primo);
	int fds[2];
	int piper = pipe(fds);

	if (piper < 0) {
		perror("Fallo el pipe\n");
		return;
	}
	int var;
	bool crear_nuevo_proceso = true;
	while (read(fd, &var, sizeof(var))) {
		if (var % primo) {
			write(fds[1], &var, sizeof(var));
			if (crear_nuevo_proceso) {
				int i = fork();
				crear_nuevo_proceso = false;
				if (i == 0) {
					close(fds[1]);
					close(fd);
					proceso_hijo(fds[0]);
					close(fds[0]);
					return;

				} else {
					close(fds[0]);
				}
			}
		}
	}
	close(fds[1]);
	close(fd);
	wait(NULL);
}

int
main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("numero incorrecto de argumentos , proba lo siguiente "
		       "./primes 2 \n");
		return 1;
	}
	int base = 0;
	char *str[2];
	int n = strtol(argv[1], str, base);

	if (n < 2 || n > 10000) {
		printf("ingresa un numero entre 2 y 10000\n");
		return 1;
	}
	if (n == 2) {
		printf("primo %d\n", n);
		return 0;
	}
	int fd[2];
	int contador = 2;
	int limite = n;
	int pipe_ini = pipe(fd);

	if (pipe_ini < 0) {
		perror("Fallo en el pipe\n");
		return -1;
	}

	int i = fork();

	if (i < 0) {
		perror("Fallo el fork\n");
		return -1;
	}

	if (i == 0) {
		close(fd[1]);
		proceso_hijo(fd[0]);
		close(fd[0]);
		wait(NULL);
	} else {
		close(fd[0]);
		while (contador <= limite) {
			write(fd[1], &contador, sizeof(contador));
			contador += 1;
		}
		close(fd[1]);
		wait(NULL);
	}
	return 0;
}