#include "ealloc.h"

char *get_freespace(int size);                   // find freespace
int heap_top;                                    // heap top
unsigned int metatable[PAGESIZE / MINALLOC * 4]; // 4096 / 256 * 4 = 64
char *mem[4];                                    // memory

void init_alloc()
{
  int i;
  heap_top = 0;
  for (i = 0; i < PAGESIZE / MINALLOC * 4; i++)
    metatable[i] = 0;
}

void cleanup()
{
  int i;
  heap_top = 0;
  for (i = 0; i < PAGESIZE / MINALLOC * 4; i++)
    metatable[i] = 0;
}

char *alloc(int size)
{
  char *result;

  if (size % MINALLOC != 0 || size > PAGESIZE)
    return NULL;

  result = get_freespace(size);

  if (result == NULL && heap_top < PAGESIZE * 4)
  {
    mem[heap_top / PAGESIZE] = mmap(0, PAGESIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,
                                    -1, 0);
    heap_top += PAGESIZE;
    result = get_freespace(size);
  }
  return result;
}

void dealloc(char *ptr)
{
  int i;
  int offset;

  for (i = 0; i < heap_top / PAGESIZE; ++i)
  {
    offset = (ptr - mem[i]) / MINALLOC;
    if (offset >= 0 && offset < PAGESIZE)
    {
      offset += PAGESIZE / MINALLOC * i;
      break;
    }
  }

  for (i = 1; i <= metatable[offset]; ++i)
    metatable[offset + (i - 1)] = 0;
  return;
}

char *get_freespace(int size)
{
  unsigned int *cur = metatable;
  int freesize = 0;
  int i = 0;

  if (heap_top == 0)
    return NULL;

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
    if (freesize == size / MINALLOC || cur - metatable == heap_top / MINALLOC)
      break;
  }
  if (freesize != (size / MINALLOC))
    return NULL;
  i = (cur - metatable) - (size / MINALLOC);
  if (i == heap_top / MINALLOC)
    return NULL;
  metatable[i] = size / MINALLOC;
  return mem[i / (PAGESIZE / MINALLOC)] + (i % (PAGESIZE / MINALLOC)) * MINALLOC;
}
