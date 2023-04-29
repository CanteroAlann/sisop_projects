#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int
main()
{
	int fd_1[2];
	int fd_2[2];
	int pipe_padre_hijo = pipe(fd_1);
	int pipe_hijo_padre = pipe(fd_2);

	if (pipe_padre_hijo < 0) {
		perror("Error al generar el pipe 1\n");
		exit(-1);
	}
	if (pipe_hijo_padre < 0) {
		perror("Error al generar el pipe 2\n");
		exit(-1);
	}

	printf("Hola, soy PID <%d>:\n", getpid());
	printf("- primer pipe me devuelve : [%d,%d]\n", fd_1[0], fd_1[1]);
	printf("- segundo pipe me devuelve : [%d,%d]\n", fd_2[0], fd_2[1]);

	int i = fork();

	if (i < 0) {
		perror("Error en el fork");
		exit(-1);
	}

	if (i == 0) {
		close(fd_1[1]);
		close(fd_2[0]);
		int v;
		read(fd_1[0], &v, sizeof(v));
		printf("Donde fork me devuelve %d:\n", i);
		printf("- getpid me devuelve: %d\n", getpid());
		printf("- getppid me devuelve: %d\n", getppid());
		printf("- recibo valor %d via fd=%d\n", v, fd_1[0]);
		printf("- reenvio valor en fd=%d y termino\n", fd_2[1]);
		write(fd_2[1], &v, sizeof(v));
	} else {
		close(fd_1[0]);
		close(fd_2[1]);
		printf("Donde fork me devuelve %d:\n", i);
		printf("- getpid me devuelve: %d\n", getpid());
		printf("- getppid me devuelve: %d\n", getppid());
		srand(time(0));
		int n = rand();
		printf("- random me devuelve: %d\n", n);
		printf("- envio valor %d a traves de fd=%d\n", n, fd_1[1]);
		write(fd_1[1], &n, sizeof(n));
		int p;
		read(fd_2[0], &p, sizeof(p));
		printf("Hola, de nuevo PID <%d>:\n", getpid());
		printf("- recibi valor <%d> via fd=%d\n", p, fd_2[0]);
		wait(NULL);
	}
}