/* 
TEST_HEADER
 id = $Id: //info.ravenbrook.com/project/mps/master/test/function/0.c#2 $
 summary = test that the mps header file is accepted by the compiler
 language = c
 link = testlib.o
END_HEADER
*/

#include "mps.h"
#include "testlib.h"

int main(void)
{
 pass();
 return 0;
}

