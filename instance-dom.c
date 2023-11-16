#include "clause-list.h"

// note: unsat for odd, sat for even
#define SIDE 7

void build(ClauseList* cl)
{
    int always_false = add_var(cl);

    int H[SIDE  ][SIDE-1]; // hori edges (var names)
    int V[SIDE-1][SIDE  ]; // vert edges (var names)

    for (int a=0; a!=SIDE; ++a) {
      for (int b=0; b!=SIDE-1; ++b) {
        H[a][b] = add_var(cl);
        V[b][a] = add_var(cl);
      }
    }

    deny(cl, always_false);

    for (int r=0; r!=SIDE; ++r) {
      for (int c=0; c!=SIDE; ++c) {

        int d0 = (    c <SIDE-1) ? H[r  ][c  ] : always_false;
        int d1 = (1<= c        ) ? H[r  ][c-1] : always_false;
        int d2 = (    r <SIDE-1) ? V[r  ][c  ] : always_false;
        int d3 = (1<= r        ) ? V[r-1][c  ] : always_false;

        assert(cl,
        make_or(cl, d0,
        make_or(cl, d1,
        make_or(cl, d2,
                    d3 ))));

        deny(cl, make_and(cl, d0, d1));
        deny(cl, make_and(cl, d0, d2));
        deny(cl, make_and(cl, d0, d3));
        deny(cl, make_and(cl, d1, d2));
        deny(cl, make_and(cl, d1, d3));
        deny(cl, make_and(cl, d2, d3));

      }
    }

}

