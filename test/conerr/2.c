/* 
TEST_HEADER
 id = $Id: //info.ravenbrook.com/project/mps/master/test/conerr/2.c#2 $
 summary = destroy an arena which isn't an arena
 language = c
 link = testlib.o
END_HEADER
*/

#include "testlib.h"

static void test(void)
{
 mps_arena_t arena;

 arena = malloc(64);
 mps_arena_destroy(arena);
 comment("Destroy arena.");
}

int main(void)
{
 easy_tramp(test);
 return 0;
}
