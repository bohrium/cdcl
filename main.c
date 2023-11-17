#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "clause-list.h"

//#include "instance-b-new.c"
#include "instance-dom.c"
//#include "instance-add.c"

#define CDCL    true
#define VERBOSE true
#define V_LIM   12

#define GET_TERM(c,vi) (                    \
        (  A->assignment[cl->membership[c][vi]] \
         ^ cl->is_negated[c][vi]             \
        )                                   \
        &1)

//#define TAE 0
//#define TSF 1
//int* assign_type;  // vi -> (TAE trial&error, TSF 2sat fill)
// int issat[nb_clauses];

typedef struct {
    int* is_assigned;   // varname to bool
    int* assignment;    // varname to bool, defined when is_assigned
    int* assign_stack;  // search rootiness to varname
    int nb_assigned;
    //
    int C;
} assign_t;

// `steps` measures branch explorations
long int steps = 0;


void init_assign(assign_t* A, ClauseList const* cl)
{
    A->C = 1.2 * cl->nb_vars + 1;
    A->is_assigned = malloc(sizeof(int) * A->C);

    for (int c=0; c!=cl->nb_vars; ++c) { // slayed correctness bug here!
      A->is_assigned[c]=0;              // (problem was I forgot to initialize.
    }                                   // i forgot to initialize because
                                        // i hadn't fixed in my mind that
                                        // `is_assigned` takes as indices
                                        // actual var names (instead of
                                        // birthday) yet ---unlike
                                        // `assignment` ---must still be
                                        // valid even when nb_assigned=0;

    A->assignment  = malloc(sizeof(int) * A->C);
    A->assign_stack= malloc(sizeof(int) * A->C);
    A->nb_assigned=0;
}
void shallow_copy_assign(assign_t* src, assign_t* dst) {
    dst->C = src->C;

    dst->is_assigned  = src->is_assigned ;
    dst->assignment   = src->assignment  ;
    dst->assign_stack = src->assign_stack;
    dst->nb_assigned  = src->nb_assigned ;
}
void copy_assign(assign_t const* src, assign_t* dst)
{
    // don't copy Cs

    for (int idx=0; idx!=src->C; ++idx) {
      dst->is_assigned[idx] = src->is_assigned[idx];
      dst->assignment[idx] = src->assignment[idx];
    }
    for (int idx=src->C; idx!=dst->C; ++idx) {
      dst->is_assigned[idx] = 0;
    }
    for (int rk=0; rk!=src->C; ++rk) {
      dst->assign_stack[rk] = src->assign_stack[rk];
    }
    dst->nb_assigned=src->nb_assigned;
}
void free_assign(assign_t* A)
{
    free(A->is_assigned );
    free(A->assignment  );
    free(A->assign_stack);
    A->nb_assigned=0;
    A->C=0;
}

void print_ass(assign_t* A, ClauseList* cl)
{
    for (int v=0; v != cl->nb_vars; ++v) {
        // print varname
        printf("%c", 'a'+(v%26));
        if (v/26) { printf("%d", v/26); }

        // print predicate
        printf(" = %s\n", (A->assignment[v]?"true":"false"));

        if (V_LIM<=v) { return; }
    }
}

void print_cl(ClauseList* cl)
{
    for (int c=0; c != cl->nb_clauses; ++c) {
        for (int vi=0; vi!=3; ++vi) {
          int v = cl->membership[c][vi];
          printf(cl->is_negated[c][vi] ? "not " : "");
          printf("%c", 'a'+(v%26));
          if (v/26) { printf("%d", v/26); }
          printf(" ");
        }
        printf("\n");

        if (V_LIM<=c) { return; }
    }
}



// fill while looking for contradictions induced or already present
int fill_2sat(assign_t* A, ClauseList* cl)
{
    while ( 1 ) {
        int found_forced_clause = false;

        int val;
        int idx;
        int c;
        for (c=0; c!=cl->nb_clauses; ++c) {
            int nb_false = 0;
            int nb_true  = 0;
            int which_unassigned=-1;
            int found_forced_var = false;
            for (int vi=0; vi!=3; ++vi) {
                int ass = A->is_assigned[cl->membership[c][vi]];
                nb_false += (ass && ! GET_TERM(c,vi)) ? 1 : 0;
                nb_true  += (ass && GET_TERM(c,vi)) ? 1 : 0;
                if ( ! ass ) {
                    if ( which_unassigned==-1 ) { which_unassigned = vi; found_forced_var = true; }
                    else if ( cl->membership[c][vi]!=cl->membership[c][which_unassigned] ) { found_forced_var = false; }
                }
            }
            if ( nb_false == 3 ) { return false; } // found contradiction
            found_forced_var = found_forced_var && (! nb_true);

            val = cl->is_negated[c][which_unassigned] ? 0 : 1; /* TODO: assumes (p or not-p) does not appear! */
            idx = cl->membership[c][which_unassigned];
            if ( found_forced_var ) { found_forced_clause = true; break; }
        }
        //
        // ----
        if ( A->nb_assigned==cl->nb_vars ) { break; } // exit here rather than at loop body boundary because still need false=3 check
        if ( ! found_forced_clause ) { break; }
        // ----
        //
        if ( A->is_assigned[idx] ) {
            if ( A->assignment[idx] != val ) {
                return false; // CONTRADICTION!
            } else {
                // TODO: UNEXPECTED
            }
        }

        if (VERBOSE) { printf(" ? --%d--%d--\n", idx, val); }
        A->is_assigned[idx] = true;
        A->assignment[idx] = val;
        //assign_type[idx] = TSF;
        A->assign_stack[A->nb_assigned] = idx;
      // TODO: for cdcl and non-cdcl efficiency, don't add forceds to ass stack!
        A->nb_assigned += 1;
    }
    return true;
}

int solve(assign_t* A, ClauseList* cl);
int solve_wrap(assign_t* A, ClauseList* cl);

int solve_wrap(assign_t* A, ClauseList* cl)
{
    // assigns forceds!
    int nb_ass_before = A->nb_assigned;
    int rtrn = fill_2sat(A, cl);

    rtrn = rtrn && solve(A, cl); // short circuit

    // there was sign typo here
    for (int rk=nb_ass_before; rk!=A->nb_assigned; ++rk) {
        A->is_assigned[A->assign_stack[rk]] = false;
    }
    // TODO: for cdcl and non-cdcl efficiency, don't add forceds to ass stack!

    A->nb_assigned = nb_ass_before;
    return rtrn&1;
}

void add_constraint(assign_t* A, ClauseList* cl)
{
    int na = A->nb_assigned;
    // TODO: try and name different conditions for when add constraint
    if (na < 3) { return; }

    int or_chain = add_var(cl);
    deny(cl, or_chain);

    //int nb_vars_old = cl->nb_vars;
    for (int rk=0; rk!=na; ++rk) {
        // TODO: don't include forced variables here!
        int idx = A->assign_stack[rk];
        int val = A->assignment[idx];
        // TODO: clean up low level construction
        if (val) { or_chain = make_implies(cl, idx, or_chain); }
        else     { or_chain = make_or     (cl, idx, or_chain); }
    }
    // TODO: assert or_chain?!
    // (***) TODO: mystery of why commenting the following line seems not to
    // drastically slow down program --- yet commenting out ((***) below) does
    //assert(cl, or_chain);

    //int nb_vars_diff = cl->nb_vars - nb_vars_old;

    // if ( ! nb_vars_diff ) { continue; }
    if ( cl->nb_vars <= A->C ) { return; }
    // expands A to keep up with number of vars:
    assign_t B;
    init_assign(&B, cl);
    copy_assign(A, &B);
    free_assign(A);
    shallow_copy_assign(&B, A);
}

int solve(assign_t* A, ClauseList* cl)
{
    if ( A->nb_assigned==cl->nb_vars ) { return true; }

        // TODO: move ordering here!
    int idx;
    for (idx=0; ; ++idx) {
        if ( ! A->is_assigned[idx] ) { break; }
        if (idx==cl->nb_vars) { /* FIXME */ }
    }

    // TODO: val ordering
    for (int val = 0; val != 2; ++val) {
        if ( idx<5 || VERBOSE) { printf("--%d--%d--\n", idx, val); }

        steps += 1;

        A->is_assigned[idx] = true;
        A->assignment[idx] = val;
        A->assign_stack[A->nb_assigned] = idx; // was typo here
        A->nb_assigned += 1;

        int rtrn = solve_wrap(A, cl);

        if ( CDCL && ! rtrn ) {
          // found conflict! ... now TODO: do CDCL

          // (***) TODO: mystery of why commenting out (***) above seems not to
          // drastically slow down program --- yet, as expected, commenting out
          // the line below DOES:
          add_constraint(A, cl);

          // TODO: splaying?
          // TODO: heuristic clauses / var values ?
          // (hcs: used with 2sat exploration to order which variables and
          // varvalues to try?)
        }

        A->nb_assigned -= 1;
        A->is_assigned[idx] = false;
        if (rtrn) { return true; }

    }

    return false;
}


int main(char* const argv, int argc)
{
    ClauseList cl;
    assign_t A;

    printf("hi\n");
    init_cl(&cl);
    build(&cl);
    init_assign(&A, &cl);
    printf("%d clauses on %d variables\n", cl.nb_clauses, cl.nb_vars);
    print_cl(&cl);

    int ss = solve(&A, &cl);
    printf("solved?  %s\n", (ss?"sat!":"unsat"));
    printf("used %ld branch explorations\n", steps);
    print_ass(&A, &cl);
    printf("%d clauses on %d variables\n", cl.nb_clauses, cl.nb_vars);

    free_assign(&A);
    free_cl(&cl);
    printf("bye\n");
    return 0;
}
