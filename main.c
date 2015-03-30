#include  "ftrace.h"

#include  <stdio.h>
#include  <stdlib.h>

void    usage(char *name)
{
  printf("Usage: %s [-v] {path | -p <pid>} {--step | --breakpoint}\n", name);
  printf("-v\t: verbose mode\n");
  printf("path\t: path to executable program\n");
  printf("<pid>\t: process identifier\n");
}

int   main(int ac, char **av)
{
  t_env   *env;

  env = env_init();
  env_conf(env, av);

  if (IS_VERBOSE)
    env_print(env);

  if (info(env) == ERR)
  {
    env_destroy(env);
    return (13);
  }

  if (trace(env) == ERR)
  {
    env_destroy(env);
    return (13);
  }

  env_destroy(env);

  return (0);
}
