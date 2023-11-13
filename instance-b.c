#define nb_vars        6
#define nb_clauses     8

#define _a  0
#define _b  1
#define _c  2
#define _d  3
#define _e  4
#define _f  5

/*    a -> (b or c)
 *
 * // b yields contradiction:
 *
 *    b -> (d or e)
 *    d -> f or not-a
 *    e -> f or not-a
 *    not-f
 *    not-f -> a
 *
 * // (a or b) is gauranteed
 *
 *    c -> a or b
 *    not-c -> a or b
 */

int membership[nb_clauses][3] = {
    { _a, _b, _c},

    { _b, _d, _e},
    { _d, _f, _f},
    { _e, _f, _f},
    { _f, _f, _f},
    { _f, _a, _a},

    { _c, _a, _b},
    { _c, _a, _b},
};

int is_negated[nb_clauses][3] = {
    { 1 , 0 , 0 },

    { 1 , 0 , 0 },
    { 1 , 0 , 0 },
    { 1 , 0 , 0 },
    { 1 , 1 , 1 },
    { 0 , 0 , 0 },

    { 1 , 0 , 0 },
    { 0 , 0 , 0 },
};
