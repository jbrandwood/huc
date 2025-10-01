/* Copyright (C) 2003  Free Software Foundation.

   Test strcpy optimizations don't evaluate side-effects twice.
      
   Written by Jakub Jelinek, June 23, 2003.  */

#include <string.h>

//typedef __SIZE_TYPE__ size_t;
#define size_t unsigned int

extern void abort (void);
extern void exit (int);

size_t
test1 (char *s, size_t i)
{
  strcpy (s, "foobarbaz" + i++);
  return i;
}

size_t
check2 (void)
{
  static size_t r;// = 5;
  if (r != 0)//5)
    abort ();
  return ++r;
}

void
test2 (char *s)
{
  strcpy (s, "foobarbaz" + check2 ());
}

int
main (void)
{
  char buf[10];
  if (test1 (buf, 7) != 8 || memcmp (buf, "az", 3))
    abort ();
  test2 (buf);
#ifdef wix
  if (memcmp (buf, "baz", 4))
    abort ();
#endif
  exit (0);
}
