/* 
TEST_HEADER
 id = $Id: //info.ravenbrook.com/project/mps/master/test/conerr/9.c#2 $
 summary = create a format in a destroyed arena
 language = c
 link = testlib.o
END_HEADER
*/

#include "testlib.h"
#include "mpscmv.h"

static void zilch(void)
{
}


static mps_addr_t myskip(mps_addr_t object)
{
 return object;
}

static void test(void)
{
 mps_arena_t arena;
 mps_fmt_t format;
 mps_fmt_A_s fmtA;


 cdie(mps_arena_create(&arena, mps_arena_class_vm(), mmqaArenaSIZE), "create arena");

 fmtA.align = (mps_align_t) 1;
 fmtA.scan  = &zilch;
 fmtA.skip  = &myskip;
 fmtA.copy  = &zilch;
 fmtA.fwd   = &zilch;
 fmtA.isfwd = &zilch;
 fmtA.pad   = &zilch;

 mps_arena_destroy(arena);
 comment("Destroy arena.");

 cdie(
  mps_fmt_create_A(&format, arena, &fmtA), 
  "create format");

}

int main(void)
{
 easy_tramp(test);
 return 0;
}
