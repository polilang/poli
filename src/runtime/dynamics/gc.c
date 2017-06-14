#ifndef __POLI_GARBAGE__
#define __POLI_GARBAGE__

#include <stdlib.h>

#define STACK_MAX 256

#include "dynamics.c"

void assert(int condition, const char* message) {
  if (!condition) {
    printf("%s\n", message);
    exit(1);
  }
}


typedef struct Object {
   unsigned char marked;

  /* The next object in the linked list of heap allocated objects. */
   struct Object* next;
   struct dyn value;
} Object;

typedef struct VM {
  Object* stack[STACK_MAX];
  int stackSize;

  /* The first object in the linked list of all objects on the heap. */
  Object* firstObject;

  /* The total number of currently allocated objects. */
  int numObjects;

  /* The number of objects required to trigger a GC. */
  int maxObjects;
} VM;




VM* newVM() {
  VM* vm = malloc(sizeof(VM));
  vm->stackSize = 0;
  vm->firstObject = NULL;
  vm->numObjects = 0;
  vm->maxObjects = 8;
  return vm;
}



void mark(Object* object) {
  /* If already marked, we're done. Check this first to avoid recursing
     on cycles in the object graph. */
  if (object->marked) return;

  object->marked = 1;
}

void markAll(VM* vm)
{
  for (int i = 0; i < vm->stackSize; i++) {
    mark(vm->stack[i]);
  }
}

void sweep(VM* vm)
{
  Object** object = &vm->firstObject;
  while (*object) {
    if (!(*object)->marked) {
      /* This object wasn't reached, so remove it from the list and free it. */
      Object* unreached = *object;

      *object = unreached->next;
      free(unreached);

      vm->numObjects--;
    } else {
      /* This object was reached, so unmark it (for the next GC) and move on to
       the next. */
      (*object)->marked = 0;
      object = &(*object)->next;
    }
  }
}



void gc(VM* vm) {
  int numObjects = vm->numObjects;

  markAll(vm);
  sweep(vm);

  vm->maxObjects = vm->numObjects * 2;

  printf("Collected %d objects, %d remaining.\n", numObjects - vm->numObjects,
         vm->numObjects);
}



Object* newObject(VM* vm) {
  if (vm->numObjects == vm->maxObjects) gc(vm);

  Object* object = malloc(sizeof(Object));
  object->next = vm->firstObject;
  vm->firstObject = object;
  object->marked = 0;

  vm->numObjects++;

  return object;
}

void push(VM* vm, struct dyn obj) {
  Object* object = newObject(vm);
  object->value = obj;

  assert(vm->stackSize < STACK_MAX, "Stack overflow!");
  vm->stack[vm->stackSize++] = object;
}

Object* pop(VM* vm) {
  assert(vm->stackSize > 0, "Stack underflow!");
  return vm->stack[--vm->stackSize];
}



void freeVM(VM *vm) {
  vm->stackSize = 0;
  gc(vm);
  free(vm);
}

#endif
