int   trace_print_sys(t_env *env, t_sym *sym)
{
  int   i;

  for (i = 0; systab[i].name; i++)
  {
    if (! strcmp(systab[i].name, sym->name))
    {
      printf(" sys> %s;\n", systab[i].line);
      return (OK);
    }
  }

  return (ERR);
}
