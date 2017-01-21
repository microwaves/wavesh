#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WAVESH_RL_BUFSIZE 1024
#define WAVESH_TOK_BUFSIZE 64
#define WAVESH_TOK_DELIM " \t\r\n\a"

char **wavesh_split_line(char *line)
{
  int bufsize = WAVESH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "wavesh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, WAVESH_TOK_DELIM);

  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += WAVESH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));

      if (!tokens) {
        fprintf(stderr, "wavesh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, WAVESH_TOK_DELIM);
  }

  tokens[position] = NULL;
  return tokens;
}

// Implement using getline() later.
char *wavesh_read_line(void)
{
  int bufsize = WAVESH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "wavesh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();

    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }

    position++;

    if (position >= bufsize) {
      bufsize += WAVESH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);

      if (!buffer) {
        fprintf(stderr, "wavesh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

void wavesh_init(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("wavesh-0.1$ ");
    line = wavesh_read_line();
    args = wavesh_split_line(line);
    status = wavesh_execute(args);
    free(line);
    free(args);
  } while(status);
}

int main(int argc, char **argv)
{
  // Load config files.

  wavesh_init();
  return EXIT_SUCCESS;
}
