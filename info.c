#include  "ftrace.h"

#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>

void    info_abort(t_env *env, int fd)
{
  xclose(fd, env->file_data, env->file_size);
  printf("Error: cannot parse ELF file.\n");
  exit(EXIT_FAILURE);
}

int   info(t_env *env)
{
  int   fd;

  fd = xopen(env->path);
  env->file_size = file_size(env->path);
  env->file_data = file_mmap(fd, env->file_size);

  elf_header_set(env);
  if (elf_header_check(env) == ERR)
    info_abort(env, fd);
  if (elf_parse(env) == ERR)
    info_abort(env, fd);

  if (env->elf_stabs)
  {
    if (stabs_parse(env) == ERR)
      info_abort(env, fd);
  }

  xclose(fd, env->file_data, env->file_size);

  return (OK);
}
