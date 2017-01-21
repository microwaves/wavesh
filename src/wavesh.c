#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define WAVESH_TOK_BUFSIZE 64
#define WAVESH_TOK_DELIM " \t\r\n\a"

int wavesh_cd(char **args);
int wavesh_help(char **args);
int wavesh_exit(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &wavesh_cd,
  &wavesh_help,
  &wavesh_exit
};

int wavesh_num_builtins()
{
  return sizeof(builtin_str) / sizeof(char *);
}

int wavesh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "wavesh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("wavesh");
    }
  }

  return 1;
}

int wavesh_help(char **args)
{
  int i;

  printf("°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸ THE WAVE SHELL! °º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°`°º¤ø,¸\n");
  printf("Type ur desired program name and arguments, and hit enter. SIT BACK AND RELAX!\n");
  printf("\n");
  printf("                   (  )/\n");
  printf("                    )(/\n");
  printf(" ________________  ( /)\n");
  printf("()__)____________)))))\n");
  printf("\n");
  printf("The following are built in commands:\n");

  for (i = 0; i < wavesh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Be smart, use the man command for information on other programs.\n");

  return 1;
}

int wavesh_exit(char **args)
{
  return 0;
}

int wavesh_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();

  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("wavesh");
    }

    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("wavesh");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int wavesh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    return 1;
  }

  for (i = 0; i < wavesh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return wavesh_launch(args);
}

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

char *wavesh_read_line(void)
{
  char *line = NULL;
  size_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
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
