#include "clause-list.h"

/*      a <-> not-b
 *      b <-> not-c
 *
 *      c  -> not-d or f
 *      c <-  not-d
 *
 *      d <-> not-e
 *      e <-> not-a
 *
 *      f -> not-b
 */

void build(ClauseList* cl)
{
    int a = add_var(cl);
    int b = add_var(cl);
    int c = add_var(cl);
    int d = add_var(cl);
    int e = add_var(cl);
    int f = add_var(cl);

    wire(cl, a, make_not(cl, b));
    wire(cl, b, make_not(cl, c));

    assert(cl, make_implies(cl, c, make_or(cl, make_not(cl, d), f)));
    assert(cl, make_implies(cl, make_not(cl, d), c));

    wire(cl, d, make_not(cl, e));
    wire(cl, e, make_not(cl, a));

    assert(cl, make_implies(cl, f, make_not(cl, b)));
}

