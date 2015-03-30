#include  "ftrace.h"

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <sys/ptrace.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <unistd.h>

t_sym   *trace_sym_find(t_env *env, struct reg *regs);

int   trace(t_env *env)
{
  char    *lav[2];

  if (env->mode == MODE_FORK)
  {
    if ((env->pid = fork()) == -1)
    {
      perror("fork");
      return (ERR);
    }
    if (! env->pid)
    {
      if (ptrace(PT_TRACE_ME, 0, 0, 0) != 0)
        perror("ptrace");
      lav[0] = xstrdup(env->path);
      lav[1] = NULL;
      if (execvp(env->path, lav) == -1)
        perror("execvp");
    }
  }
  else if (env->mode == MODE_ATTACH)
    ptrace(PT_ATTACH, env->pid, 0, 0);
  else
    return (ERR);

  if (env->tr_mode == TR_MODE_BREAK)
    return (trace_break(env));
  else if (env->tr_mode == TR_MODE_STEP)
    return (trace_step(env));

  return (ERR);
}

int   trace_break(t_env *env)
{
  int         ret_code;
  struct reg  regs;
  int         tmp;
  t_sym       *sym;
  int         data;

  wait(&ret_code);
  if (trace_sym_init(env) == ERR)
    return (ERR);
  ptrace(PT_CONTINUE, env->pid, (caddr_t) 1, 0);
  wait(&ret_code);
  while (! WIFEXITED(ret_code) || WIFSIGNALED(ret_code))
  {
    tmp = ptrace(PT_GETREGS, env->pid, (caddr_t) &regs, 0);
    regs.r_rip -= 1;
    tmp = ptrace(PT_SETREGS, env->pid, (caddr_t) &regs, 0);
    if ((sym = trace_sym_find(env, &regs)) != NULL)
    {
      if (trace_print_sys(env, sym) != OK)
        trace_print(env, sym);
      tmp = ptrace(PT_WRITE_I, env->pid, (caddr_t) sym->value, sym->alt_val);
      tmp = ptrace(PT_STEP, env->pid, (caddr_t) 1, 0);
      wait(&ret_code);
      if (WIFEXITED(ret_code) || WIFSIGNALED(ret_code))
        break;
      data = sym->alt_val & 0xffffff00;
      data |= 0xcc;
      tmp = ptrace(PT_WRITE_I, env->pid, (caddr_t) sym->value, data);
    }
    else
    {
      tmp = ptrace(PT_STEP, env->pid, (caddr_t) 1, 0);
      wait(&ret_code);
      if (WIFEXITED(ret_code) || WIFSIGNALED(ret_code))
        break;
    }
    tmp = ptrace(PT_CONTINUE, env->pid, (caddr_t) 1, 0);
    wait(&ret_code);
  }

  return (OK);
}

int   trace_step(t_env *env)
{
  int         ret_code;
  struct reg  regs;
  t_sym       *sym;

  wait(&ret_code);
  while (! WIFEXITED(ret_code))
  {
    if (ptrace (PT_GETREGS, env->pid, (caddr_t) &regs, 0) != 0)
    {
      perror ("ptrace");
      return (ERR);
    }
    if ((sym = trace_sym_find(env, &regs)) != NULL)
      if (trace_print_sys(env, sym) != OK)
        trace_print(env, sym);
    if (ptrace(PT_STEP, env->pid, (caddr_t) 1, 0) != 0)
    {
      perror("ptrace");
      return (ERR);
    }
    wait(&ret_code);
  }

  return (OK);
}

void    trace_print(t_env *env, t_sym *sym)
{
  int     i;
  int     cnt;
  t_stab  *f;

  for (i = 0; i < list_count(env->st_func); i++)
  {
    if ((f = list_get(env->st_func, i)) == NULL)
      return;
    if (strcmp(f->name, sym->name))
      continue;
    printf("stab> ");
    if (f->ln_type)
      printf("%s ", f->ln_type->name);
    else
      printf("? ");
    for (cnt = 0; cnt < f->st_cnt; cnt++)
      printf("*");
    printf("%s(", f->name);
    trace_print_params(env, f);
    printf(");\n");
    return;
  }
  printf(" sym> %s\n", sym->name);
}

void    trace_print_params(t_env *env, t_stab *f)
{
  int     i;
  t_stab  *p;
  t_stab  *type;
  int     cnt;

  for (i = 0; i < list_count(f->ln_param); i++)
  {
    if ((p = list_get(f->ln_param, i)) == NULL)
      return;
    type = p->ln_type;
    if (! type)
      printf("? ");
    else
      printf("%s ", type->name);
    for (cnt = 0; cnt < p->st_cnt; cnt++)
      printf("*");
    printf("%s", p->name);
    if (i < (list_count(f->ln_param) - 1))
      printf(", ");
  }
}

int   trace_sym_init(t_env *env)
{
  int     i;
  t_sym   *sym;
  int     tmp;

  for (i = 0; i < list_count(env->elf_syms); i++)
  {
    if ((sym = list_get(env->elf_syms, i)) == NULL)
      return (ERR);
    tmp = ptrace(PT_READ_I, env->pid, (caddr_t) sym->value, 0);
    sym->alt_val = tmp;
    tmp &= 0xffffff00;
    tmp |= 0xcc;
    ptrace(PT_WRITE_I, env->pid, (caddr_t) sym->value, tmp);
  }

  return (OK);
}

t_sym   *trace_sym_find(t_env *env, struct reg *regs)
{
  int   i;
  t_sym *sym;

  if (! regs)
    return (NULL);

  for (i = 0; i < list_count(env->elf_syms); i++)
  {
    if ((sym = list_get(env->elf_syms, i)) == NULL)
      return (NULL);
    if (sym->value == regs->r_rip)
      return (sym);
  }

  return (NULL);
}
