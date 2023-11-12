#include "bool.h"
#include "stdio.h"
#include "string.h"

int const nb_vars;
int const nb_clauses;

int membership[nb_clauses][3];
bool is_negated[nb_clauses][3];
//(int[3]*) next[nb_clauses][3];

int assignment[nb_vars];
bool is_assigned[nb_vars];
int nb_assigned = 0;
bool issat[nb_clauses];

void init_solve()
{
    for (int c=0; c!=nb_clauses; ++c) {
        issat[nb_clauses]=0;
    }
}

bool solve()
{
    if ( ! nb_assigned ) { return true; }

    int idx = nb_assigned;

    for (int val = 0; val != 2; ++val) {
        assignment[idx] = val;

        is_assigned[idx] = true;
        //nb_assigned += 1;

        for (int c=0; c!=nb_clauses; ++c) {
            if (issat[c]) { continue; }
            for (int vi=0; vi!=3; ++vi) {
                if (membership[c][vi]!=idx) { continue; }
                if ( (val ^ is_negated[c][vi])&1 ) { // clause satisfied
                    issat[c] = 1;
                } else { // clause unsatisfied
                    if ( ! is_assigned[membership[c][0]] || ((val ^ is_negated[c][0])&1) ) { continue; }
                    if ( ! is_assigned[membership[c][1]] || ((val ^ is_negated[c][1])&1) ) { continue; }
                    if ( ! is_assigned[membership[c][2]] || ((val ^ is_negated[c][2])&1) ) { continue; }
                }
            }
            issat[nb_clauses]=0;
        }


        if solve() { return true; }

        //nb_assigned -= 1;
        is_assigned[idx] = false;
    }

    return false;
}


int main(char* const argv, int argc)
{
    printf("hi\n");
    bool ss = solve();
    printf("solved=%s\n", (ss?"sat!":"unsat"));
    printf("bye\n");
    return 0;
}
