#include  "ftrace.h"

#include  <fcntl.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <sys/mman.h>
#include  <sys/stat.h>
#include  <unistd.h>

void    *xmalloc(int size)
{
  void    *p;

  if (! (p = calloc(1, size)))
  {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  return (p);
}

char    *xstrdup(char *s)
{
  char  *ret;
  int   i;

  ret = xmalloc(strlen(s) + 1);
  for (i = 0; s[i]; i++)
    ret[i] = s[i];
  ret[i] = '\0';

  return (ret);
}

int   xopen(char *path)
{
  int   fd;

  if ((fd = open(path, O_RDONLY)) == -1)
  {
    perror("open");
    exit(EXIT_FAILURE);
  }

  return (fd);
}

int   xclose(int fd, char *path, int size)
{
  if (close(fd) == -1)
  {
    perror("close");
    exit(EXIT_FAILURE);
  }

  if (munmap(path, (size_t) size) == -1)
  {
    perror("munmap");
    exit(EXIT_FAILURE);
  }

  return (OK);
}

int   file_size(char *path)
{
  struct stat info;

  if (stat(path, &info) != 0)
  {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  return (info.st_size);
}

void    *file_mmap(int fd, int size)
{
  void    *ret;

  if ((ret = mmap(0, (size_t) size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
  {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  return (ret);
}
