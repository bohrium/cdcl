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
    //{ 0 , 0 , 0 }, // turn ON or OFF for unsat or sat
};
//(int[3]*) next[nb_clauses][3];

int assignment[nb_vars];
int is_assigned[nb_vars];

int assign_stack[nb_vars]; // rank to variable name
#define TAE 0
#define TSF 1
int assign_type[nb_vars];  // vi -> (TAE trial&error, TSF 2sat fill)
int nb_assigned = 0;

int issat[nb_clauses];

void init_solve()
{
    for (int c=0; c!=nb_clauses; ++c) {
        issat[c]=0;
    }
    //for (int v=0; v!=nb_vars; ++v) {
    //    assign_stack[v]=0;
    //}
    nb_assigned=0;
}

void print_ass()
{
    for (int v=0; v != nb_vars; ++v) {
        printf("%c = %s\n", 'a'+v, (assignment[v]?"true":"false"));
    }
}

long int steps = 0;

#define GET_TERM(c,vi) ((assignment[membership[c][vi]] ^ is_negated[c][vi])&1)

// fill while looking for contradictions induced or already present
int fill_2sat()
{
    while ( 1 ) {
        int found_forced_clause = false;

        int val;
        int idx;
        int c;
        for (c=0; c!=nb_clauses; ++c) {
            int nb_false = 0;
            int nb_true  = 0;
            int which_unassigned=-1;
            int found_forced_var = false;
            for (int vi=0; vi!=3; ++vi) {
                int ass = is_assigned[membership[c][vi]];
                nb_false += (ass && ! GET_TERM(c,vi)) ? 1 : 0;
                nb_true  += (ass && GET_TERM(c,vi)) ? 1 : 0;
                if ( ! ass ) {
                    if ( which_unassigned==-1 ) { which_unassigned = vi; found_forced_var = true; }
                    else if ( membership[c][vi]!=membership[c][which_unassigned] ) { found_forced_var = false; }
                }
            }
            if ( nb_false == 3 ) { return false; } // found contradiction
            found_forced_var = found_forced_var && (! nb_true);

            val = is_negated[c][which_unassigned] ? 0 : 1; /* TODO: assumes (p or not-p) does not appear! */
            idx = membership[c][which_unassigned];
            if ( found_forced_var ) { found_forced_clause = true; break; }
        }
        //
        // ----
        if ( nb_assigned==nb_vars ) { break; } // exit here rather than at loop body boundary because still need false=3 check
        if ( ! found_forced_clause ) { break; }
        // ----
        //
        if ( is_assigned[idx] ) {
            if ( assignment[idx] != val ) {
                return false; // CONTRADICTION!
            } else {
                // TODO: UNEXPECTED
            }
        }

        assignment[idx] = val;
        printf(" ? --%d--%d--\n", idx, val);
        assign_type[idx] = TSF;
        is_assigned[idx] = true;
        assign_stack[nb_assigned] = idx;
        nb_assigned += 1;

    }
    return true;
}

int solve();
int solve_wrap();

int solve_wrap()
{
    //TODO: assign forceds!
    int nb_ass_before = nb_assigned;
    int rtrn = fill_2sat();
    rtrn = rtrn && solve(); // short circuit
    for (int idx=nb_ass_before; idx!=nb_assigned; ++idx) {
        is_assigned[assign_stack[idx]] = false;
    }
    nb_assigned = nb_ass_before;
    return rtrn&1;
}

int solve()
{
    if ( nb_assigned==nb_vars ) { return true; }

        // TODO: move ordering here!
    int idx;
    for (idx=0; ; ++idx) {
        if ( ! is_assigned[idx] ) { break; }
        if (idx==nb_vars) { /* FIXME */ }
    }

    for (int val = 0; val != 2; ++val) {
        printf("--%d--%d--\n", idx, val);

        assignment[idx] = val;
        assign_type[idx] = TAE;
        is_assigned[idx] = true;
        assign_stack[nb_assigned] = val;
        nb_assigned += 1;

        steps += 1;

        // TODO: CDCL here!
        //
        // if chain >= size 3 (w -- x -- y -- z, searched over all
        // 2*2*2 for xyz)
        // TODO : CDCL!
        // like, if A--B--C--D--E--X--Y--Z with c current level
        // then want (A!=a || B!=b || C!=c || D!=d || E!=e)
        // equiv
        // (A!=a || B!=b || P)
        // ( ! P || C!=c || Q )
        // ( ! Q || D!=d || E!=e )
        // TODO: splaying?
        // TODO: heuristic clauses / var values ?
        // (hcs: used with 2sat exploration to order which variables and
        // varvalues to try?)

        int rtrn = false;
        //if ( ! found_failure && solve_wrap() ) {
        if ( solve_wrap() ) {
            rtrn = true;
        }

        nb_assigned -= 1;
        is_assigned[idx] = false;
        if (rtrn) { return true; }
    }

    return false;
}


int main(char* const argv, int argc)
{
    printf("hi\n");
    init_solve();
    int ss = solve();
    printf("solved?  %s\n", (ss?"sat!":"unsat"));
    printf("used %ld steps\n", steps);
    print_ass();
    printf("bye\n");
    return 0;
}
