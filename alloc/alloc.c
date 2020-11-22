#include "alloc.h"

char *get_freespace(int size);               // find freespace
char *mem;                                   // memory
unsigned int metatable[PAGESIZE / MINALLOC]; // 4096 / 8 = 512

int init_alloc()
{
  int i;
  for (i = 0; i < PAGESIZE / MINALLOC; i++)
    metatable[i] = 0;
  mem = mmap(0, PAGESIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,
             -1, 0);
  return mem == MAP_FAILED ? -1 : 0;
}

int cleanup()
{
  int i;
  for (i = 0; i < PAGESIZE / MINALLOC; i++)
    metatable[i] = 0;
  return munmap(mem, PAGESIZE);
}

char *alloc(int size)
{
  if (size % MINALLOC != 0)
    return NULL;
  return get_freespace(size);
}

void dealloc(char *ptr)
{
  int i;
  int offset = (ptr - mem) / MINALLOC;

  for (i = 1; i <= metatable[offset]; ++i)
    metatable[offset + (i - 1)] = 0;
  return;
}

char *get_freespace(int size)
{
  unsigned int *cur = metatable;
  int freesize = 0;
  int i = 0;

  while (1)
  {
    if (*cur != 0)
    {
      cur += *cur;
      freesize = 0;
      continue;
    }
    cur++;
    freesize++;
    if (freesize == size / MINALLOC || cur - metatable == PAGESIZE / MINALLOC)
      break;
  }
  if (freesize != (size / MINALLOC))
    return NULL;
  i = (cur - metatable) - (size / MINALLOC);
  if (i == PAGESIZE / MINALLOC)
    return NULL;
  metatable[i] = size / MINALLOC;
  return mem + i * MINALLOC;
}
