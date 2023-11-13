typedef struct {
    int membership[/*clauses*/][3];
    int is_negated[/*clauses*/][3];
    int nb_clauses;
    int clauses_cap;

    int nb_vars;
} ClauseList;


void init_cl(ClauseList* cl, int cap);
void expand(ClauseList* cl, int new_cap);
void free_cl(ClauseList* cl);
int add_var(ClauseList* cl);
int rule_out(ClauseList* cl,
             int idxa, int vala,
             int idxb, int valb,
             int idxc, int valc );

int make_false(ClauseList* cl, int va);
int make_true (ClauseList* cl, int va);
int make_not(ClauseList* cl, int va);
int make_and(ClauseList* cl, int va, int vb);
int make_or (ClauseList* cl, int va, int vb);
int make_xor(ClauseList* cl, int va, int vb);
int make_eqv(ClauseList* cl, int va, int vb);

void ensure_add(ClauseList* cl, int len, int va, int vb, int vs)
{
    int c;
    for (int b=0; b!=len; ++b)
    {
        c = make_xor(cl ;
    }
}
