/* 
TEST_HEADER
 id = $Id: //info.ravenbrook.com/project/mps/master/test/argerr/0.c#2 $
 summary = create an arena with a NULL arena_t
 language = c
 link = testlib.o
END_HEADER
*/

#include "testlib.h"
#include "arg.h"

static void test(void)
{
 adie(mps_arena_create(NULL, mps_arena_class_vm(), mmqaArenaSIZE),
      "Create arena");
}

int main(void)
{
 easy_tramp(test);
 return 0;
}
