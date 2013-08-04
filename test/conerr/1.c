/* 
TEST_HEADER
 id = $Id: //info.ravenbrook.com/project/mps/master/test/conerr/1.c#2 $
 summary = destroy an arena without creating it
 language = c
 link = testlib.o
END_HEADER
*/

#include "testlib.h"

static void test(void)
{
 mps_arena_t arena;

 mps_arena_destroy(arena);
 comment("Destroy arena.");
}

int main(void)
{
 easy_tramp(test);
 return 0;
}
