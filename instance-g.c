#include "clause-list.h"

/*      a <-> not-b
 *      b <-> not-c
 *
 *      c <-> not-d
 *
 *      d <-> not-e
 *      e <-> not-a
 */

void build(ClauseList* cl)
{
    int a = add_var(cl);
    int b = add_var(cl);
    int c = add_var(cl);
    int d = add_var(cl);
    int e = add_var(cl);

    wire(cl, a, make_not(cl, b));
    wire(cl, b, make_not(cl, c));
    wire(cl, c, make_not(cl, d));
    wire(cl, d, make_not(cl, e));
    wire(cl, e, make_not(cl, a));
}

