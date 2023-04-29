#include "defs.h"
#include "readline.h"
#include <termios.h>
#include <ctype.h>



static char buffer[BUFLEN];

struct termios saved_attributes;
static void reset_input_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}
static void set_input_mode(void)
{
	struct termios tattr;

	/* Make sure stdin is a terminal. */
	if (!isatty(STDIN_FILENO)) {
		fprintf(stderr, "Not a terminal.\n");
		exit(EXIT_FAILURE);
	}

	/* Save the terminal attributes so we can restore them later. */
	tcgetattr(STDIN_FILENO, &saved_attributes);

	/* Set the funny terminal modes. */
	tcgetattr(STDIN_FILENO, &tattr);
	/* Clear ICANON and ECHO. We'll do a manual echo! */
	tattr.c_lflag &= ~(ICANON | ECHO);
	/* Read one char at a time */
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}
// reads a line from the standard input
// and prints the prompt
char *
read_line(const char *prompt)
{
	int i = 0, c = 0;

#ifndef SHELL_NO_INTERACTIVE
	fprintf(stdout, "%s %s %s\n", COLOR_RED, prompt, COLOR_RESET);
	fprintf(stdout, "%s", "$ ");
#endif

	memset(buffer, 0, BUFLEN);

	set_input_mode();
	c = getchar();

	while (c) {
		if (c == END_LINE) {
			write(STDOUT_FILENO, &c, 1);
			break;
		}
		if (c == CHAR_EOF) {
			return NULL; // ctrl + d
		}
		if (c == CHAR_DEL) {
			// backspace
			if (i == 0) {
				printf("inicio de la linea !!!!");
				continue;
			}
			write(STDOUT_FILENO, "\b \b", 3);
			buffer[i--] = '\0';
		}
		if (c == CHAR_ESC) {
			char esc_seq;
			read(STDIN_FILENO, &esc_seq, 1);
			if (esc_seq != '[') {
				continue;
			}
			read(STDIN_FILENO, &esc_seq, 1);
			if (esc_seq == 'A') {
				printf("flechita para arriba \n");
			}
			if (esc_seq == 'B') {
				printf("flechita para abajo \n");
			}
			if (esc_seq == 'C') {
				printf("flechita para la derecha \n");
			}
			if (esc_seq == 'D') {
				printf("flechita para la izquierda \n");
			}
		}
		if (isprint(c)) { // si es visible
			write(STDOUT_FILENO, &c, 1);
			buffer[i++] = c;
		}
		c = getchar();
	}
	buffer[i] = END_STRING;
	reset_input_mode();
	return buffer;
}
