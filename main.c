#include "stdbool.h"
#include "stdio.h"
#include "string.h"

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
};
//(int[3]*) next[nb_clauses][3];



int assignment[nb_vars];
int is_assigned[nb_vars];
int nb_assigned = 0;
int issat[nb_clauses];


void init_solve()
{
    for (int c=0; c!=nb_clauses; ++c) {
        issat[c]=0;
    }
}

void print_ass()
{
    for (int v=0; v != nb_vars; ++v) {
        printf("%c = %s\n", 'a'+v, (assignment[v]?"true":"false"));
    }
}

long int steps = 0;

int solve()
{
    if ( nb_assigned==nb_vars ) { return true; }

    int idx = nb_assigned;

    for (int val = 0; val != 2; ++val) {
        printf("--%d--%d--\n", idx, val);

        assignment[idx] = val;
        steps += 1;

        is_assigned[idx] = true;
        nb_assigned += 1;

        int found_failure=false;
        for (int c=0; c!=nb_clauses; ++c) {
            //if ( issat[c] ) { continue; }
            for (int vi=0; vi!=3; ++vi) {
                if (membership[c][vi]!=idx) { continue; }
                //if ( (val ^ is_negated[c][idx])&1 ) { // clause satisfied
                    //issat[c] = 1;
                //    break;
                //} else { // clause unsatisfied
                    if ( ! is_assigned[membership[c][0]] || ((assignment[membership[c][0]] ^ is_negated[c][0])&1) ) { continue; }
                    if ( ! is_assigned[membership[c][1]] || ((assignment[membership[c][1]] ^ is_negated[c][1])&1) ) { continue; }
                    if ( ! is_assigned[membership[c][2]] || ((assignment[membership[c][2]] ^ is_negated[c][2])&1) ) { continue; }
                    found_failure = true;
                //}
                if ( found_failure )  { break; }
            }
            //issat[c]=0;
            if ( found_failure ) { break; }
        }
        if (found_failure ) {
            nb_assigned -= 1;
            is_assigned[idx] = false;
            continue;
        }

        int rtrn = solve();  //if (solve()) { return true; }

        nb_assigned -= 1;
        is_assigned[idx] = false;

        if (rtrn) { return true; }
    }

    return false;
}


int main(char* const argv, int argc)
{
    printf("hi\n");
    int ss = solve();
    printf("solved?  %s\n", (ss?"sat!":"unsat"));
    printf("used %ld steps\n", steps);
    print_ass();
    printf("bye\n");
    return 0;
}
