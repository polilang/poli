/*
   evolbug - 2017, MIT license
   vector/dynamic array implementation
*/



#ifndef EVOLBUG_VECTOR
#define EVOLBUG_VECTOR



#include "strace/strace.h"
#include "except/except.h"
#include "garbage/gm.h"



extern const Exception VECTOR_INIT;
extern const Exception VECTOR_EMPTY;
extern const Exception VECTOR_INDEX;



unsigned v_size (void* vector);
void* v_new ();
void v_insert(unsigned index, unsigned esize, unsigned ecount, char **vector, char *e);
void v_remove(unsigned index, unsigned esize, char **vector);



#define vector(type) type*


#define v_new() st_call(v_new())


#define v_insert(index, vector, ...) ({\
   st_push(v_insert(index, vector, ##__VA_ARGS__));\
   assert(gm_ismanaged(((unsigned*)vector - 1)), VECTOR_INIT, #vector);\
   assert(index <= v_size(vector), VECTOR_INDEX, #vector, #index);\
   __typeof__(vector[0]) temp[] = {__VA_ARGS__};\
   v_insert(index, sizeof(vector[0]), sizeof(temp)/sizeof(vector[0]), (char**)&vector, (char*)temp);\
   st_pop();})


#define v_push(vector, ...) ({\
   st_push(v_push(vector, ##__VA_ARGS__));\
   v_insert(v_size(vector), vector, ##__VA_ARGS__);\
   st_pop();})


#define v_remove(vector, index) ({\
   st_push(v_remove(vector, index));\
   assert(gm_ismanaged(((unsigned*)vector - 1)), VECTOR_INIT, #vector);\
   assert(v_size(vector), VECTOR_EMPTY, #vector);\
   assert(index < v_size(vector), VECTOR_INDEX, #vector, #index);\
   v_remove(index, sizeof(vector[0]), (char**)&vector);\
   st_pop();})


#define v_pop(vector) ({\
   st_push(v_pop(vector));\
   assert(v_size(vector), VECTOR_EMPTY, #vector);\
   __typeof__(vector[0]) temp = vector[v_size(vector) - 1];\
   v_remove(vector, v_size(vector) - 1);\
   st_pop();\
   temp;})


#define v_free(vector) ({\
   st_push(v_free(vector));\
   assert(gm_ismanaged(((unsigned*)vector - 1)), VECTOR_INIT, #vector);\
   gfree(((unsigned*)vector - 1));\
   st_pop();})


#endif
