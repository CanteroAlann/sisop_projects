#ifndef READLINE_H
#define READLINE_H
#define CHAR_EOF '\004'
#define CHAR_DEL 127
#define CHAR_ESC '\033'


char *read_line(const char *prompt);

#endif  // READLINE_H
