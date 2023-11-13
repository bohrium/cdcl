#include "stdlib.h"
#include "clause-list.h"

void init_cl(ClauseList* cl, int cap)
{
    cl->nb_vars = 0;
    cl->nb_clauses = 0;

    cl->membership = malloc(sizeof(int[3]) * cap);
    cl->is_negated = malloc(sizeof(int[3]) * cap);
    cl->clauses_cap = cap;
}

void expand(ClauseList* cl, int new_cap)
{
    ClauseList old = cl;
    cl->membership = malloc(sizeof(int[3]) * new_cap);
    cl->is_negated = malloc(sizeof(int[3]) * new_cap);
    for (int c=0; c!=cl->nb_clauses; ++c) {
        cl->membership[c] = old->membership[c];
        cl->is_negated[c] = old->is_negated[c];
    }
    cl->cap = new_cap;
    free(old->membership);
    free(old->is_negated);
}

void free_cl(ClauseList* cl)
{
    free(cl->membership);
    free(cl->is_negated);
    cl->nb_vars=0;
    cl->nb_clauses=0;
    cl->clauses_cap=0;
}

int add_var(ClauseList* cl)
{
    cl->nb_vars += 1;
    return cl->nb_vars-1;
}

int rule_out(ClauseList* cl,
             int idxa, int vala,
             int idxb, int valb,
             int idxc, int valc )
{
    if ( cl->nb_clauses == cl->clauses_cap ) { expand(cl, cl->cap*2 + 1); }

    int nc = cl->nb_clauses;
    cl->is_negated[nc][0] = vala; cl->membership[nc][0] = idxa;
    cl->is_negated[nc][1] = valb; cl->membership[nc][1] = idxb;
    cl->is_negated[nc][2] = valc; cl->membership[nc][2] = idxc;
    cl->nb_clauses += 1;
}

int make_not(ClauseList* cl, int va)
{
    int v = add_var(cl);
    rule_out(cl, v,0, va,0, va,0);
    rule_out(cl, v,1, va,1, va,1);
    return v;
}

int make_and(ClauseList* cl, int va, int vb)
{
    int v = add_var(cl);
    rule_out(cl, v,1, va,0, vb,0);
    rule_out(cl, v,1, va,0, vb,1);
    rule_out(cl, v,1, va,1, vb,0);
    rule_out(cl, v,0, va,1, vb,1);
    return v;
}

int make_or (ClauseList* cl, int va, int vb)
{
    int v = add_var(cl);
    rule_out(cl, v,1, va,0, vb,0);
    rule_out(cl, v,0, va,0, vb,1);
    rule_out(cl, v,0, va,1, vb,0);
    rule_out(cl, v,0, va,1, vb,1);
    return v;
}

int make_xor(ClauseList* cl, int va, int vb)
{
    int v = add_var(cl);
    rule_out(cl, v,1, va,0, vb,0);
    rule_out(cl, v,0, va,0, vb,1);
    rule_out(cl, v,0, va,1, vb,0);
    rule_out(cl, v,1, va,1, vb,1);
    return v;
}

int make_eqv(ClauseList* cl, int va, int vb)
{
    int v = add_var(cl);
    rule_out(cl, v,0, va,0, vb,0);
    rule_out(cl, v,1, va,0, vb,1);
    rule_out(cl, v,1, va,1, vb,0);
    rule_out(cl, v,0, va,1, vb,1);
    return v;
}

int ensure_add(ClauseList* cl, int len, int va, int vb, vs)
{

}
