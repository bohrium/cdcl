#include "clause-list.h"

// note: unsat for odd, sat for even
#define L 1

void build(ClauseList* cl)
{
    int a[L]; for (int l=0; l!=L; ++l) { a[l] = add_var(cl); printf("  a %d\n", a[l]); }
    int b[L]; for (int l=0; l!=L; ++l) { b[l] = add_var(cl); printf("  b %d\n", b[l]); }
    int s[L]; for (int l=0; l!=L; ++l) { s[l] = add_var(cl); printf("  s %d\n", s[l]); }

    ensure_add(cl, L, -1, a[0], b[0], s[0], -1);

    int vals[L] = {1,};
    for (int l=0; l!=L; ++l) {
        if (vals[l]) { assert(cl, s[l]); }
        else         { deny  (cl, s[l]); }
    }
}

