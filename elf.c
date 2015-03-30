#include  "ftrace.h"

#include  <stdlib.h>
#include  <stdio.h>

#ifdef    __linux__
#include  <string.h>
#endif

int   elf_parse(t_env *env)
{
  if (elf_header_set(env) == ERR)
    return (ERR);
  if (elf_sections_set(env) == ERR)
    return (ERR);
  if (elf_shstrtab_set(env) == ERR)
    return (ERR);
  if (elf_symtab_set(env) == ERR)
    return (ERR);
  if (elf_strtab_set(env) == ERR)
    return (ERR);
  if (elf_stabs_set(env) == ERR)
    printf("missing stabs\n");
  if (elf_sym_set(env) == ERR)
    return (ERR);

  return (OK);
}

int   elf_header_set(t_env *env)
{
  env->elf_header = xmalloc(sizeof(*(env->elf_header)));
  memcpy(env->elf_header, env->file_data, sizeof(*(env->elf_header)));
  return (OK);
}

int   elf_header_check(t_env *env)
{
  Elf32_Ehdr  *h;

  h = env->elf_header;

  if (! h)
    return (ERR);

  if ((h->e_ident[EI_MAG0] != ELFMAG0) || (h->e_ident[EI_MAG1] != ELFMAG1) ||
      (h->e_ident[EI_MAG2] != ELFMAG2) || (h->e_ident[EI_MAG3] != ELFMAG3))
    return (ERR);
#ifndef   __linux__
  if ((h->e_machine != EM_386) && (h->e_machine != EM_486))
    return (ERR);
#endif
  if (h->e_type != ET_EXEC)
    return (ERR);
  if (h->e_version != EV_CURRENT)
    return (ERR);

  return (OK);
}

int   elf_sections_set(t_env *env)
{
  Elf32_Shdr  *s;
  int         i;
  void        *start;
  int         step;

  if (! env->elf_header)
    return (ERR);
  if (env->elf_header->e_shnum == 0)
    return (ERR);

  start = env->file_data + env->elf_header->e_shoff;
  step = env->elf_header->e_shentsize;
  for (i = 0; i < env->elf_header->e_shnum; i++)
  {
    s = xmalloc(step);
    memcpy(s, start + step * i, step);
    list_add(&(env->elf_sections), (void *) s);
  }

  return (OK);
}

int   elf_shstrtab_set(t_env *env)
{
  Elf32_Shdr  *s;
  void        *start;

  if (! env->elf_header)
    return (ERR);
  if (env->elf_header->e_shstrndx == 0)
    return (ERR);

  s = list_get(env->elf_sections, env->elf_header->e_shstrndx);
  env->elf_shstrtab = xmalloc(s->sh_size);
  start = env->file_data + s->sh_offset;
  memcpy(env->elf_shstrtab, start, s->sh_size);

  return (OK);
}

int   elf_symtab_set(t_env *env)
{
  Elf32_Shdr  *tmp_sect;

  if (! env->elf_sections)
    return (ERR);

  if ((tmp_sect = elf_sections_get(env, ".symtab")) == ERR)
    return (ERR);

  env->elf_symtab_size = tmp_sect->sh_size;
  env->elf_symtab = xmalloc(tmp_sect->sh_size);
  memcpy(env->elf_symtab, env->file_data + tmp_sect->sh_offset, tmp_sect->sh_size);

  return (OK);
}

int   elf_strtab_set(t_env *env)
{
  Elf32_Shdr  *tmp_sect;

  if (! env->elf_sections)
    return (ERR);

  if ((tmp_sect = elf_sections_get(env, ".strtab")) == ERR)
    return (ERR);

  env->elf_strtab = xmalloc(tmp_sect->sh_size);
  memcpy(env->elf_strtab, env->file_data + tmp_sect->sh_offset, tmp_sect->sh_size);

  return (OK);
}

int   elf_stabs_set(t_env *env)
{
  Elf32_Shdr  *tmp_sect;

  if (! env->elf_sections)
    return (ERR);

  if ((tmp_sect = elf_sections_get(env, ".stabstr")) == ERR)
    return (ERR);

  env->elf_stabs_size = tmp_sect->sh_size;
  env->elf_stabs = xmalloc(tmp_sect->sh_size);
  memcpy(env->elf_stabs, env->file_data + tmp_sect->sh_offset, tmp_sect->sh_size);

  return (OK);
}

int   elf_sym_set(t_env *env)
{
  Elf32_Sym *symtab;
  Elf32_Sym *tmp;
  int       i;
  t_sym     *sym;
  char      *to_dup;

  if (! env->elf_symtab)
    return (ERR);

  symtab = env->elf_symtab;
  for (i = 0; i < (env->elf_symtab_size / sizeof(Elf32_Sym)); i++)
  {
    tmp = &symtab[i];
    if (ELF32_ST_TYPE(tmp->st_info) != STT_FUNC)
      continue;
    sym = xmalloc(sizeof(*sym));
    to_dup = env->elf_strtab + tmp->st_name;
    sym->name = xstrdup(to_dup);
    sym->value = tmp->st_value;
    list_add(&(env->elf_syms), (void *) sym);
  }

  return (OK);
}

Elf32_Shdr  *elf_sections_get(t_env *env, char *name)
{
  int         i;
  Elf32_Shdr  *tmp;

  for (i = 0; i < list_count(env->elf_sections); i++)
  {
    if ((tmp = list_get(env->elf_sections, i)) == NULL)
      return (ERR);
    if (! strcmp(&env->elf_shstrtab[tmp->sh_name], name))
      return (tmp);
  }

  return (NULL);
}
