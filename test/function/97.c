/* 
TEST_HEADER
 id = $Id: //info.ravenbrook.com/project/mps/master/test/function/97.c#6 $
 summary = test of mps_arena_formatted_objects_walk
 language = c
 link = testlib.o rankfmt.o
END_HEADER

 some kinds of errors that could occur in the walker:

 I. miss out a reachable object
 II. miss out an unreachable object
 III. extra walk over a reachable object
 IV. extra walk over an unreachable object
 V. step on something which isn't a valid object (or pad)
 VI. assertion failure in the mps
 VII. might not pass parameters correctly

 II is very hard to detect. The others we can catch with assertions, by
 repeatedly tracing the graph ourselves, time-stamping objects, and then
 calling the walker and time-stamping them again. Objects found with
 out-of-date timestamps must have been missed on the previous trace/walk.
 So when the walker finds an object not found on a previous trace, that's
 an unreachable object; when it finds an object it's already found that's
 III or IV. When the tracer finds an object with an out-of-date timestamp,
 the walker must have missed it -- that's I.
*/

#include "testlib.h"
#include "mpscamc.h"
#include "mpscawl.h"
#include "mpsclo.h"
#include "rankfmt.h"

#define MAGICSIZE (342)
#define MAGICPOINT ((mycell *) 214208)

#define genCOUNT (3)

static mps_gen_param_s testChain[genCOUNT] = {
  { 6000, 0.90 }, { 8000, 0.65 }, { 16000, 0.50 } };

void *stackpointer;
long int appcount;
long int apppadcount;

int oldstamp, newstamp;

mps_arena_t arena;
mps_pool_t poolamc, poollo, poolawl;
mps_thr_t thread;
mps_root_t root, root1;

 mps_chain_t chain;
mps_fmt_t format;
mps_ap_t apamc, aplo, apawl;


static void tracegraph(mycell *obj)
{
 int i;

 if (obj == NULL) {
  return;
 }

 asserts((obj->tag & 3) == MCdata, "odd check at %p (%p)", obj, obj->tag);

 if (obj->data.checkedflag == oldstamp) {
  obj->data.checkedflag = newstamp;
  if (obj->data.assoc != NULL) {
   tracegraph(obj->data.assoc);
  }
  for (i=0; i<(obj->data.numrefs); i++) {
   if (obj->data.ref[i].addr != NULL) {
    tracegraph(obj->data.ref[i].addr);
   }
  }
 } else {
  asserts(obj->data.checkedflag == newstamp,
         "I. trace on old object at %p", obj);
 }
}


static void stepper(mps_addr_t addr, mps_fmt_t fmt, mps_pool_t pool,
                    void *V, size_t S)
{
 mycell *a;

 asserts((mycell *) V == MAGICPOINT, "VII. Void * didn't get passed!");
 asserts(S == MAGICSIZE, "VII. Size didn't get passed!");
 asserts(fmt == format, "VII. Format didn't get passed!");
 a = addr;
 asserts(((a->tag) & 0x3) == MCdata || ((a->tag) & 0x3) == MCpad,
         "V. step onto bad object at %p", addr);
 if (((a->tag) & 0x3) == MCdata) {
  appcount += 1;
  asserts(a->data.checkedflag != newstamp,
          "III/IV. step on object again at %p", a);
  commentif(a->data.checkedflag != oldstamp,
          "*. step on unreachable object at %p", a);
  a->data.checkedflag = newstamp;
 } else {
  apppadcount +=1;
 }
}

static void test(void)
{
/* a is a table of exact roots
   b    a table of ambiguous roots
   f    a table of non-roots
*/

 mycell *a[4], *b[4], *f[4];
 mps_addr_t addr;
 int i, j, k;

 cdie(mps_arena_create(&arena, mps_arena_class_vm(), mmqaArenaSIZE), "create arena");

 cdie(mps_thread_reg(&thread, arena), "register thread");

 addr = &a[0];
 cdie(
  mps_root_create_table(&root, arena, mps_rank_exact(), 0, addr, 4),
  "create a root table");

 addr = &b[0];
 cdie(
  mps_root_create_table(&root1, arena, mps_rank_ambig(), 0, addr, 4),
  "create b root table");

 cdie(
  mps_fmt_create_A(&format, arena, &fmtA),
  "create format");

 cdie(mps_chain_create(&chain, arena, genCOUNT, testChain), "chain_create");

 cdie(
  mps_pool_create(&poolamc, arena, mps_class_amc(), format, chain),
  "create pool");

 cdie(
  mps_pool_create(&poollo, arena, mps_class_lo(), format),
  "create pool");

 cdie(
  mps_pool_create(&poolawl, arena, mps_class_awl(), format, getassociated),
  "create pool");

 cdie(
  mps_ap_create(&apamc, poolamc, mps_rank_exact()),
  "create ap");

 cdie(
  mps_ap_create(&aplo, poollo, mps_rank_exact()),
  "create ap");

 cdie(
  mps_ap_create(&apawl, poolawl, mps_rank_exact()),
  "create ap");

 newstamp = 0;

 for (i=0; i<4; i++) {
  addr = &a[i];
  die(allocrdumb(addr, aplo, 64, mps_rank_exact()), "alloc failed");
  addr = &b[i];
  die(allocrone(addr, apawl, 5, mps_rank_exact()), "alloc failed");
  b[i]->data.ref[0].addr = a[i];
  addr = &a[i];
  die(allocrone(addr, apamc, 5, mps_rank_exact()), "alloc failed");
  a[i]->data.ref[0].addr = b[i];
 }


 for (j=0; j<100; j++) {

  comment("%i of 100", j);

  for (i=0; i<10000; i++) {
   k = ranint(4);
   addr = &a[k];
   die(allocrdumb(addr, aplo, 64, mps_rank_exact()), "alloc failed");
   k = ranint(4);
   addr = &b[k];
   die(allocrone(addr, apawl, 5, mps_rank_exact()), "alloc failed");
   b[k]->data.ref[0].addr = a[ranint(4)];
   b[k]->data.ref[1].addr = b[ranint(4)];
   addr = &a[k];
   die(allocrone(addr, apamc, 5, mps_rank_exact()), "alloc failed");
   f[k] = a[k]->data.ref[2].addr;
   a[k]->data.ref[2].addr = b[ranint(4)];
  }

  comment("walking...");

  mps_arena_park(arena);
  mps_arena_collect(arena);

  oldstamp = newstamp;
  newstamp += 1;
  mps_arena_formatted_objects_walk(arena, stepper,
                                   (void *) MAGICPOINT, MAGICSIZE);
  mps_arena_release(arena);

  comment("tracing...");

  oldstamp = newstamp;
  newstamp += 1;
  tracegraph((mycell *) exfmt_root);
  tracegraph(a[0]);
  tracegraph(a[1]);
  tracegraph(a[2]);
  tracegraph(a[3]);
  tracegraph(b[0]);
  tracegraph(b[1]);
  tracegraph(b[2]);
  tracegraph(b[3]);
  
  comment("ok");
 }

 mps_ap_destroy(apamc);
 mps_ap_destroy(aplo);
 mps_ap_destroy(apawl);
 comment("Destroyed aps.");

 mps_pool_destroy(poolamc);
 mps_pool_destroy(poollo);
 mps_pool_destroy(poolawl);
 comment("Destroyed pools.");

 mps_fmt_destroy(format);
 comment("Destroyed format.");

 mps_chain_destroy(chain);
 comment("Destroyed chain.");

 mps_root_destroy(root);
 mps_root_destroy(root1);
 comment("Destroyed roots.");

 mps_thread_dereg(thread);
 comment("Deregistered thread.");

 mps_arena_destroy(arena);
 comment("Destroyed arena.");
}

int main(void)
{
 void *m;
 stackpointer=&m; /* hack to get stack pointer */

 easy_tramp(test);
 pass();
 return 0;
}
