#ifndef __POLI_ASSERT__
#define __POLI_ASSERT__

#define error(rc, message) {             \
   fprintf(stderr,                       \
      "%s:%d: %s: "message"\n",          \
      __FILE__, __LINE__, __FUNCTION__); \
   if (rc) exit(rc);                     \
}

#endif
