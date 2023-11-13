#define nb_vars        6
#define nb_clauses     10

#define _a  0
#define _b  1
#define _c  2
#define _d  3
#define _e  4
#define _f  5

/*      a <-> not-b
 *      b <-> not-c
 *      c <-> not-d
 *      d <-> not-e
 *      e <-> not-a
 */

int membership[nb_clauses][3] = {
    { _a, _b, _b},
    { _a, _b, _b},
    { _b, _c, _c},
    { _b, _c, _c},
    { _c, _d, _d},
    { _c, _d, _d},
    { _d, _e, _e},
    { _d, _e, _e},
    { _e, _a, _a},
    { _e, _a, _a},
};

int is_negated[nb_clauses][3] = {
    { 1 , 1 , 1 },
    { 0 , 0 , 0 },

    { 1 , 1 , 1 },
    { 0 , 0 , 0 },

    { 1 , 1 , 1 },
    { 0 , 0 , 0 },

    { 1 , 1 , 1 },
    { 0 , 0 , 0 },

    { 1 , 1 , 1 },
    { 0 , 0 , 0 },
};
