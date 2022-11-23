//
//  main.c
//  WorkerThread
//
//  Created by srujan chilukunuri on 20/11/22.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "tpool.h"

static const size_t num_threads = 4;

int jobs = 1;

struct work_args
{
  size_t job_id;
  char *args[3];
};

void worker(void *arg)
{
  struct work_args *job_args = arg;

  //    char *t = strcat(job_args->args[0]," -l");

  //    system(t);

  if (fork() == 0)
  {
    printf("%s %s \n", job_args->args[0], job_args->args[1]);
    if (freopen(strcat((char *)job_args->job_id, ".out"), "w+", stdout) == NULL)
    {
      perror("freopen() failed");
    }

    execvp(job_args->args[0], job_args->args);
    fclose(stdout);
  }
  else
  {
    wait(NULL);
  }
}

void print_menu(tpool_t *tm, int *vals)
{
  bool stop = false;
  while (stop == false)
  {
    printf("%d\n", stop);
    printf("Enter command>");
    char *line = NULL;

    size_t len = 0;
    ssize_t lineSize = 0;
    lineSize = getline(&line, &len, stdin);
    line[lineSize - 1] = '\0';

    char *command[3];
    int i = 0;
    char *token;
    token = strtok(line, " ");

    while (token != NULL)
    {
      command[i++] = token;
      token = strtok(NULL, " ");
    }

    if (strcmp(command[0], "submit") == 0)
    {
      struct work_args *args = calloc(1, sizeof(*args));
      args->job_id = jobs++;
      args->args[0] = command[1];
      args->args[1] = command[2];
      args->args[2] = NULL;
      tpool_add_work(tm, worker, args);
      printf("submit\n");
      jobs++;
    }

    if (strcmp(command[0], "showjobs") == 0)
    {
      print_jobs();
    }

    if (strcmp(command[0], "exit") == 0)
    {
      printf("e\n");
      stop = true;
    }
  }
}

int main(int argc, char **argv)

{

  tpool_t *tm;
  int *vals;

  tm = tpool_create(num_threads);
  vals = calloc(num_items, sizeof(*vals));

  print_menu(tm, vals);
  tpool_wait(tm);
  free(vals);

  tpool_destroy(tm);

  return 0;
}