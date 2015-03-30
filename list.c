#include  "ftrace.h"

#include  <stdlib.h>

int   list_add(t_list **list, void *data)
{
  t_list  *tmp;
  t_list  *new;

  new = xmalloc(sizeof(*new));
  new->ptr = data;
  new->next = NULL;

  if (!*list)
  {
    new->idx = 0;
    *list = new;
    return (0);
  }

  for (tmp = *list; tmp->next; tmp = tmp->next)
    ;
  new->idx = tmp->idx + 1;
  tmp->next = new;

  return (new->idx);
}

void    *list_get(t_list *list, int idx)
{
  t_list  *tmp;

  if (!list)
    return (NULL);

  if (idx >= list_count(list))
    return (NULL);

  for (tmp = list; tmp; tmp = tmp->next)
    if (tmp->idx == idx)
      return (tmp->ptr);

  return (NULL);
}

int   list_count(t_list *list)
{
  t_list  *tmp;
  int   i;

  for (tmp = list, i = 0; tmp; i++)
    tmp = tmp->next;

  return (i);
}

void    list_destroy(t_list *list)
{
  t_list  *tmp;
  t_list  *todel = 0;

  if (!list)
    return;

  for (tmp = list; tmp; tmp = tmp->next)
  {
    if (todel)
      free(todel);
    todel = tmp;
  }
  free(todel);
}
