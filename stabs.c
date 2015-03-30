#include  "ftrace.h"

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

int   stabs_parse(t_env *env)
{
  char    *line;
  char    *p;
  t_list  **list;
  t_stab  *stab;
  t_stab  *func;

  if (!env->elf_stabs)
    return (ERR);

  for (line = stabs_get_line(env); line; line = stabs_get_line(env))
  {
    /* printf("+ %s\n", line); */
    if ((p = strchr(line, STAB_TOK_SEP)) == NULL)
      continue;
    p++;
    if (stabs_is_struct(env, line))
    {
      if (stabs_parse_struct(env, line) == ERR)
        return (ERR);
      continue;
    }
    if ((list = stabs_store_list(env, line)) == NULL)
    {
      free(line);
      continue;
    }
    if ((stab = stabs_parse_line(env, line)) == NULL)
    {
      free(line);
      return (ERR);
    }
    if (stabs_refer(env, stab) == ERR)
    {
      free(line);
      return (ERR);
    }
    if (*p == STAB_TOK_PARAM)
    {
      func = stabs_last_func(env);
      list_add(&(func->ln_param), stab);
    }
    list_add(list, stab);
    /* stabs_print(stab); */
    free(line);
  }

  return (OK);
}

t_list    **stabs_store_list(t_env *env, char *s)
{
  char    *p;
  t_list        **list;

  if (!s)
    return (NULL);
  if ((p = strchr(s, STAB_TOK_SEP)) == NULL)
    return (NULL);
  p++;
  if (!*p)
    return (NULL);
  if (*p == STAB_TOK_TYPE)
    list = &(env->st_type);
  else if (*p == STAB_TOK_FUNC)
    list = &(env->st_func);
  else if (*p == STAB_TOK_PARAM)
    list = &(env->st_param);
  else if (*p == STAB_TOK_VAR)
    list = &(env->st_var);
  else
    return (NULL);

  return (list);
}

int   stabs_is_struct(t_env *env, char *line)
{
  char    *p;

  if (!line)
    return (0);
  if ((p = strchr(line, STAB_TOK_SEP)) == NULL)
    return (0);
  if ((p = strchr(p, '=')) == NULL)
    return (0);
  p++;
  if (*p && *p == 's')
    return (1);
  return (0);
}

int   stabs_parse_struct(t_env *env, char *line)
{
  char    *elem;

  elem = stabs_struct_next(env, line, stabs_struct_start(env, line));
  for (; elem; elem = stabs_struct_next(env, line, 0))
  {
    if (!elem)
      return (OK);

    if (!strcmp(elem, "?"))
    {
      free(line);
      line = stabs_get_line(env);
      /* printf("- %s\n", line); */
      elem = stabs_struct_next(env, line, line);
    }

    stabs_store_struct(env, elem);
    free(elem);
  }

  return (OK);
}

int   stabs_store_struct(t_env *env, char *elem)
{
  t_list        **list;
  t_stab  *stab;

  if ((list = stabs_store_list(env, elem)) == NULL)
    return (ERR);
  if ((stab = stabs_parse_line(env, elem)) == NULL)
    return (ERR);
  if (stabs_refer(env, stab) == ERR)
    return (ERR);
  list_add(list, stab);
  /* stabs_print(stab); */

  return (OK);
}

char    *stabs_struct_start(t_env *env, char *line)
{
  char    *ptr;

  if (!line)
    return (NULL);

  if ((ptr = strchr(line, STAB_TOK_SEP)) == NULL)
    return (NULL);
  if ((ptr = strchr(ptr, '=')) == NULL)
    return (NULL);
  if (!*++ptr)
    return (NULL);
  while (*++ptr)
    if (*ptr < '0' || *ptr > '9')
      break;

  return (ptr);
}

char    *stabs_struct_next(t_env *env, char *line, char *new_ptr)
{
  static char *ptr = 0;
  char    *start;
  char    *ret;

  if (new_ptr)
    ptr = new_ptr;

  start = ptr;
  if (!start)
    return (NULL);
  if (*start == ';')
    return (NULL);
  if (*start == '?')
    return (xstrdup("?"));
  if ((ptr = strchr(start, ';')) == NULL)
    return (NULL);
  *ptr = 0;
  ret = xstrdup(start);
  *ptr = ';';
  ptr++;
  return (ret);
}

char    *stabs_get_line(t_env *env)
{
  static char *ptr = 0;
  char    *ret;

  if (ptr >= env->elf_stabs + env->elf_stabs_size)
    return (NULL);

  if (!ptr)
    ptr = env->elf_stabs;

  ret = ptr;

  while (*ptr++)
    ;

  return (xstrdup(ret));
}

t_stab    *stabs_parse_line(t_env *env, char *line)
{
  t_stab  *new;

  new = stabs_new();
  if ((new->name = stabs_line_name(env, line)) == NULL)
    return (NULL);
  if ((new->type = stabs_line_type(env, line)) == NULL)
    return (NULL);
  new->end_ptr = stabs_line_end_ptr(env, line);

  return (new);
}

int   stabs_refer(t_env *env, t_stab *s)
{
  s->ln_type = stabs_refer_type(env, s);
  /*
     if (s->ln_type)
     printf("ln: (%s) -> (%s)\n", s->type, s->ln_type->type);
  */

  return (OK);
}

t_stab    *stabs_refer_type(t_env *env, t_stab *s)
{
  t_stab  *find;

  if (!s || !s->type)
    return (NULL);

  if (s->end_ptr)
  {
    if ((find = stabs_list_get_type(env->st_type, s->end_ptr)) != NULL)
    {
      s->st_cnt = 1;
      return (find);
    }

    if ((find = stabs_list_get_type(env->st_func, s->end_ptr)) == NULL)
      if ((find = stabs_list_get_type(env->st_param, s->end_ptr)) == NULL)
        if ((find = stabs_list_get_type(env->st_var, s->end_ptr)) == NULL)
          ;

    if (find && find->ln_type)
    {
      s->st_cnt = find->st_cnt + 1;
      return (find->ln_type);
    }
  }

  if ((find = stabs_list_get_type(env->st_type, s->type)) != NULL)
    return (find);

  if ((find = stabs_list_get_type(env->st_func, s->type)) == NULL)
    if ((find = stabs_list_get_type(env->st_param, s->type)) == NULL)
      if ((find = stabs_list_get_type(env->st_var, s->type)) == NULL)
        ;

  if (find && find->ln_type)
  {
    s->st_cnt = find->st_cnt;
    return (find->ln_type);
  }

  return (NULL);
}

t_stab    *stabs_refer_type_alt(t_env *env, t_stab *s)
{
  static int  st = 0;
  static t_stab *ref = 0;
  t_stab  *find;

  if (!s || !s->type)
  {
    st = 0;
    ref = 0;
    return (NULL);
  }

  if (!ref)
    ref = s;

  if ((find = stabs_list_get_type(env->st_type, s->type)) != NULL)
  {
    ref->st_cnt = st;
    st = 0;
    ref = 0;
    return (find);
  }

  if (!s->end_ptr)
  {
    st = 0;
    ref = 0;
    return (NULL);
  }

  st++;

  if ((find = stabs_list_get_type(env->st_type, s->end_ptr)) != NULL)
  {
    ref->st_cnt = st;
    st = 0;
    ref = 0;
    return (find);
  }

  if ((find = stabs_list_get_type(env->st_func, s->end_ptr)) != NULL)
    return (stabs_refer_type(env, find));
  if ((find = stabs_list_get_type(env->st_param, s->end_ptr)) != NULL)
    return (stabs_refer_type(env, find));
  if ((find = stabs_list_get_type(env->st_var, s->end_ptr)) != NULL)
    return (stabs_refer_type(env, find));

  st = 0;
  ref = 0;

  return (NULL);
}

t_stab    *stabs_list_get_type(t_list *list, char *type)
{
  int   i;
  t_stab  *s;

  if (!list || !type)
    return (NULL);

  for (i = 0; i < list_count(list); i++)
  {
    s = list_get(list, i);
    if (!strcmp(s->type, type))
      return (s);
  }

  return (NULL);
}

char    *stabs_line_name(t_env *env, char *line)
{
  char    *p;
  char    *ret;

  if ((p = strchr(line, STAB_TOK_SEP)) == NULL)
    return (NULL);
  *p = 0;
  ret = xstrdup(line);
  *p = STAB_TOK_SEP;

  return (ret);
}

char    *stabs_line_type(t_env *env, char *line)
{
  char    *p;
  char    *start;
  char    *ret;

  if ((p = strchr(line, STAB_TOK_SEP)) == NULL)
    return (NULL);
  if ((p = strchr(p, '(')) == NULL)
    return (NULL);
  start = p + 1;
  if ((p = strchr(start, ')')) == NULL)
    return (NULL);
  *p = 0;
  ret = xstrdup(start);
  *p = ')';

  return (ret);
}

char    *stabs_line_end_ptr(t_env *env, char *line)
{
  char    *p;
  char    *p_star;
  char    *start;
  char    *ret;

  if ((p = strchr(line, STAB_TOK_SEP)) == NULL)
    return (NULL);
  if ((p = strchr(p, '=')) == NULL)
    return (NULL);
  if ((p_star = strchr(p, '*')) != NULL)
    p = p_star + 1;
  if ((p = strchr(p, '(')) == NULL)
    return (NULL);
  start = p + 1;
  if ((p = strchr(start, ')')) == NULL)
    return (NULL);
  *p = 0;
  ret = xstrdup(start);
  *p = ')';

  return (ret);
}

t_stab    *stabs_new()
{
  t_stab  *new;

  new = xmalloc(sizeof(*new));
  new->type = NULL;
  new->name = NULL;
  new->end_ptr = NULL;
  new->st_cnt = 0;
  new->ln_type = NULL;
  new->ln_param = NULL;
  new->ln_vars = NULL;

  return (new);
}

void    stabs_print(t_stab *s)
{
  printf("ps: [%s] [%s] [%s]\n", s->type, s->name, s->end_ptr);
}

t_stab    *stabs_last_func(t_env *env)
{
  return (list_get(env->st_func, list_count(env->st_func) - 1));
}
