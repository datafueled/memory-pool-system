/* 
TEST_HEADER
 id = $Id: //info.ravenbrook.com/project/mps/master/test/conerr/52.c#2 $
 summary = reset ld again, in destroyed arena
 language = c
 link = myfmt.o testlib.o
END_HEADER
*/

#include "testlib.h"
#include "mpscamc.h"
#include "myfmt.h"

static void test(void)
{
 mps_arena_t arena;
 mps_ld_s ld;

 cdie(mps_arena_create(&arena, mps_arena_class_vm(), mmqaArenaSIZE), "create arena");

 mps_ld_reset(&ld, arena);
 comment("Reset ld."); 

 mps_arena_destroy(arena);
 comment("Destroyed arena.");

 mps_ld_reset(&ld, arena);
 comment("Reset ld again."); 

}

int main(void)
{
 easy_tramp(test);
 return 0;
}

