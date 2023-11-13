#define nb_vars        5
#define nb_clauses     9
#define _a  0
#define _b  1
#define _c  2
#define _d  3
#define _e  4

int membership[nb_clauses][3] = {
    { _a, _b, _b},
    { _b, _c, _c},
    { _c, _d, _d},

    { _d, _e, _e},
    { _d, _e, _e},

    { _d, _b, _b},
    { _d, _b, _b},

    { _b, _b, _b},
    { _e, _e, _e},
};
int is_negated[nb_clauses][3] = {
    // TODO: make actual ternary rather than 2sat problem!
    { 0 , 1 , 1 }, // b -> a
    { 0 , 1 , 1 }, // c -> b
    { 1 , 1 , 1 }, // d -> notc

    { 1 , 1 , 1 }, // d,e different
    { 0 , 0 , 0 },

    { 1 , 0 , 0 }, // d,b same
    { 0 , 1 , 1 },

    { 1 , 1 , 1 }, // b false
    { 1 , 1 , 1 }, // turn ON or OFF for unsat or sat
    //{ 0 , 0 , 0 }, // turn ON or OFF for unsat or sat
};
