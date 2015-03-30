#include  "ftrace.h"

#include  <stdio.h>
#include  <stdlib.h>

#ifdef    __linux__
#include  <string.h>
#endif

t_env   *env_init()
{
  t_env   *env;

  env = xmalloc(sizeof(*env));
  env->mode = MODE_UNSET;
  env->tr_mode = TR_MODE_UNSET;
  env->path = NULL;
  env->pid = 0;
  env->pid_str = NULL;
  env->verb = VERBOSE_DEF;
  env->file_size = 0;
  env->file_data = NULL;
  env->elf_header = NULL;
  env->elf_sections = NULL;
  env->elf_shstrtab = NULL;
  env->elf_symtab = NULL;
  env->elf_symtab_size = 0;
  env->elf_strtab = NULL;
  env->elf_stabs = NULL;
  env->elf_stabs_size = 0;
  env->elf_syms = NULL;
  env->st_type = NULL;
  env->st_func = NULL;
  env->st_param = NULL;
  env->st_var = NULL;

  return (env);
}

void    env_print(t_env *env)
{
  printf("ENV:\n");
  printf("  mode: %d\n", env->mode);
  printf("  tr_mode: %d\n", env->tr_mode);
  printf("  path: %s\n", env->path);
  printf("  pid: %d\n", env->pid);
  printf("  verb: %d\n", env->verb);
}

void    env_check(t_env *env, char **av)
{
  if (((! env->path) && (! env->pid)) || (env->path && env->pid))
  {
    usage(av[0]);
    exit(EXIT_FAILURE);
  }

  if (env->pid)
    env->mode = MODE_ATTACH;
  if (env->path)
    env->mode = MODE_FORK;

  if (env->tr_mode == TR_MODE_UNSET)
    env->tr_mode = TR_MODE_STEP;

  if (env->mode == MODE_ATTACH)
  {
    env->path = xmalloc(strlen("/proc/") + strlen(env->pid_str) + strlen("/file") + 1);
    memcpy(env->path, "/proc/", strlen("/proc/"));
    memcpy(env->path + strlen("/proc/"), env->pid_str, strlen(env->pid_str));
    memcpy(env->path + strlen("/proc/") + strlen(env->pid_str), "/file", strlen("/file"));
    env->path[strlen("/proc/") + strlen(env->pid_str) + strlen("/file")] = 0;
  }
}

void    env_conf(t_env *env, char **av)
{
  int   i;

  for (i = 1; av[i]; i++)
  {
    if (! strcmp(av[i], ARG_HELP))
    {
      usage(av[0]);
      exit(EXIT_SUCCESS);
    }
    else if (! strcmp(av[i], ARG_VERB))
      env->verb = VERBOSE_ON;
    else if (! strcmp(av[i], ARG_STEP))
      env->tr_mode = TR_MODE_STEP;
    else if (! strcmp(av[i], ARG_BREAK))
      env->tr_mode = TR_MODE_BREAK;
    else if (! strcmp(av[i], ARG_PID))
    {
      if (av[i + 1])
      {
        env->pid_str = av[i + 1];
        env->pid = atoi(env->pid_str);
        i++;
      }
      else
      {
        usage(av[0]);
        exit(EXIT_FAILURE);
      }
    }
    else
      env->path = av[i];
  }

  env_check(env, av);
}

void    env_destroy(t_env *env)
{
  if (env->pid_str)
    free(env->pid_str);
  if (env->elf_header)
    free(env->elf_header);
  if (env->elf_sections)
    free(env->elf_sections);
  if (env->elf_shstrtab)
    free(env->elf_shstrtab);
  if (env->elf_symtab)
    free(env->elf_symtab);
  if (env->elf_strtab)
    free(env->elf_strtab);
  if (env->elf_stabs)
    free(env->elf_stabs);
  if (env->elf_syms)
    free(env->elf_syms);
  if (env->st_type)
    list_destroy(env->st_type);
  if (env->st_func)
    list_destroy(env->st_func);
  if (env->st_param)
    list_destroy(env->st_param);
  if (env->st_var)
    list_destroy(env->st_var);
}
