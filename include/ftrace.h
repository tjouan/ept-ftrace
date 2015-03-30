#ifndef   _FTRACE_H_
#define   _FTRACE_H_

#include  <elf.h>

/* DEFINES */

#define   MODE_UNSET  0
#define   MODE_FORK 1
#define   MODE_ATTACH 2

#define   TR_MODE_UNSET 0
#define   TR_MODE_STEP  1
#define   TR_MODE_BREAK 2

#define   VERBOSE_OFF 0
#define   VERBOSE_ON  1
#define   VERBOSE_DEF VERBOSE_OFF
#define   IS_VERBOSE  env->verb

#define   ARG_HELP  "-h"
#define   ARG_VERB  "-v"
#define   ARG_PID   "-p"
#define   ARG_STEP  "--step"
#define   ARG_BREAK "--breakpoint"

#define   OK    1
#define   ERR   0

#define   STAB_UNDEF  0
#define   STAB_TYPE 1
#define   STAB_FUNC 2
#define   STAB_PARAM  3

#define   STAB_TOK_SEP    ':'
#define   STAB_TOK_TYPE   't'
#define   STAB_TOK_FUNC   'F'
#define   STAB_TOK_PARAM  'p'
#define   STAB_TOK_VAR    '('

/* STRUCTURES */

struct    s_list
{
  int     idx;
  void    *ptr;
  struct s_list *next;
};
typedef struct s_list t_list;

struct    s_sym
{
  char    *name;
  int     value;
  int     alt_val;
};
typedef struct s_sym t_sym;

struct    s_stab
{
  char    *type;
  char    *name;
  char    *ret;
  char    *end_ptr;
  int     st_cnt;
  struct s_stab *ln_type;
  t_list  *ln_param;
  t_list  *ln_vars;
};
typedef struct s_stab t_stab;

struct    s_systab
{
  char    *ret;
  char    *name;
  char    *args;
  char    *line;
};
typedef struct s_systab t_systab;

struct    s_env
{
  int     mode;
  int     tr_mode;
  char    *path;
  int     pid;
  char    *pid_str;
  int     verb;
  int     file_size;
  void    *file_data;
  Elf32_Ehdr *elf_header;
  t_list  *elf_sections;
  char    *elf_shstrtab;
  Elf32_Sym *elf_symtab;
  int     elf_symtab_size;
  char    *elf_strtab;
  char    *elf_stabs;
  int     elf_stabs_size;
  t_list  *elf_syms;
  t_list  *st_type;
  t_list  *st_func;
  t_list  *st_param;
  t_list  *st_var;
};
typedef struct s_env t_env;


/* PROTOTYPES */

/* main.c */
void    usage(char *name);

/* env.c */
t_env   *env_init();
void    env_print(t_env *env);
void    env_conf(t_env *env, char **av);
void    env_check(t_env *env, char **av);
void    env_destroy(t_env *env);

/* std.c */
void    *xmalloc(int size);
char    *xstrdup(char *s);
int     xopen(char *path);
int     file_size(char *path);
void    *file_mmap(int fd, int size);
int     xclose(int fd, char *path, int size);

/* trace.c */
int     trace(t_env *env);
int     trace_break(t_env *env);
int     trace_step(t_env *env);
int     trace_sym_init(t_env *env);
void    trace_print(t_env *env, t_sym *sym);
void    trace_print_params(t_env *env, t_stab *f);
int     trace_print_sys(t_env *env, t_sym *sym);

/* info.c */
int     info(t_env *env);
void    info_abort(t_env *env, int fd);

/* elf.c */
int   elf_header_set(t_env *env);
int   elf_header_check(t_env *env);
int   elf_sections_set(t_env *env);
int   elf_shstrtab_set(t_env *env);
int   elf_symtab_set(t_env *env);
int   elf_strtab_set(t_env *env);
Elf32_Shdr *elf_sections_get(t_env *env, char *name);
int   elf_stabs_set(t_env *env);
int   elf_sym_set(t_env *env);
int   elf_parse(t_env *env);

/* list.c */
int   list_add(t_list **list, void *data);
void  *list_get(t_list *list, int idx);
int   list_count(t_list *list);
void  list_destroy(t_list *list);

/* stabs.c */
int     stabs_parse(t_env *env);
char    *stabs_get_line(t_env *env);
t_stab  *stabs_parse_line(t_env *env, char *line);
t_stab  *stabs_new();
void    stabs_print(t_stab *s);
char    *stabs_line_name(t_env *env, char *line);
char    *stabs_line_type(t_env *env, char *line);
char    *stabs_line_end_ptr(t_env *env, char *line);
int     stabs_refer(t_env *env, t_stab *s);
t_stab  *stabs_refer_type(t_env *env, t_stab *s);
t_stab  *stabs_list_get_type(t_list *list, char *type);
int     stabs_is_struct(t_env *env, char *line);
int     stabs_parse_struct(t_env *env, char *line);
char    *stabs_struct_next(t_env *env, char *line, char *new_ptr);
char    *stabs_struct_start(t_env *env, char *line);
t_list  **stabs_store_list(t_env *env, char *s);
int     stabs_store_struct(t_env *env, char *elem);
t_stab  *stabs_last_func(t_env *env);

#endif
