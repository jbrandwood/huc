#include <string.h>

#ifdef __HUCC__
 #ifdef NORECURSE
  #define MEMCPY_SIZE 512 //(1 << 17)
 #else
  #define MEMCPY_SIZE (STACK_SIZE / 3)
 #endif
#else
 //#if defined (STACK_SIZE)
 //#define MEMCPY_SIZE (STACK_SIZE / 3)
 //#else
 #define MEMCPY_SIZE 512 //(1 << 17)
 //#endif
#endif

void *copy (void *o, const void *i, unsigned l)
{
  return memcpy (o, i, l);
}

main ()
{
  unsigned i;
  unsigned char src[MEMCPY_SIZE];
  unsigned char dst[MEMCPY_SIZE];

  for (i = 0; i < MEMCPY_SIZE; i++)
    src[i] = (unsigned char) i,  dst[i] = 0;

  /*(void)*/ memcpy (dst, src, MEMCPY_SIZE / 128);

  for (i = 0; i < MEMCPY_SIZE / 128; i++)
    if (dst[i] != (unsigned char) i)
      abort ();

  /*(void)*/ memset (dst, 1, MEMCPY_SIZE / 128);

  for (i = 0; i < MEMCPY_SIZE / 128; i++)
    if (dst[i] != 1)
      abort ();

  /*(void)*/ memcpy (dst, src, MEMCPY_SIZE);

  for (i = 0; i < MEMCPY_SIZE; i++)
    if (dst[i] != (unsigned char) i)
      abort ();

  /*(void)*/ memset (dst, 0, MEMCPY_SIZE);

  for (i = 0; i < MEMCPY_SIZE; i++)
    if (dst[i] != 0)
      abort ();

  /*(void)*/ copy (dst, src, MEMCPY_SIZE / 128);

  for (i = 0; i < MEMCPY_SIZE / 128; i++)
    if (dst[i] != (unsigned char) i)
      abort ();

  /*(void)*/ memset (dst, 0, MEMCPY_SIZE);

  /*(void)*/ copy (dst, src, MEMCPY_SIZE);

  for (i = 0; i < MEMCPY_SIZE; i++)
    if (dst[i] != (unsigned char) i)
      abort ();

  exit (0);
}
