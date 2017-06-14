#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define len(array) (int)(sizeof(array)/sizeof(array[0]))

#include "dynamics/dynamics.c"
#include "dynamics/types.c"
#include "dynamics/macros.c"

#define array(...) {__VA_ARGS__, {"nil"}}
#define arrayend(array, i) strcmp(any(array[i]).raw.type, "nil")

#define each(exp) _each(exp)
#define _each(itm, list) \
   (union any itm=any(list[0]), i=any(u32(0)); arrayend(list, i.u32.data); itm = any(list[++i.u32.data]))
#define in ,





void test_sizes ()
{
   printf("sizeof signed ints:\t i08:%lu\t i16:%lu\t i32:%lu\t i64:%lu\n",
      sizeof(struct i08), sizeof(struct i16), sizeof(struct i32), sizeof(struct i64));

   printf("sizeof unsigned ints:\t u08:%lu\t u16:%lu\t u32:%lu\t u64:%lu\n",
      sizeof(struct u08), sizeof(struct u16), sizeof(struct u32), sizeof(struct u64));

   printf("sizeof float types:\t f32:%lu\t f64:%lu\n",
      sizeof(struct f32), sizeof(struct f64));

   printf("sizeof other types:\t raw:%lu\t any:%lu\t dyn:%lu\n",
      sizeof(struct raw), sizeof(union any), sizeof(struct dyn));
}

int main()
{
   test_sizes();

   struct dyn mystr = string("asdasd");

   union any things[4] = array(any(i08(2)), any(i08(1)), any(i08(12)));

   for each (item in things)
      print("{} ", item);

   print("\n");

   free(mystr.data);
}
