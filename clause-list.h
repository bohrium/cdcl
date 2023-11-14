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

int assert(ClauseList* cl, int v);
int deny  (ClauseList* cl, int v);
int wire  (ClauseList* cl, int va, int vb);

int make_not(ClauseList* cl, int va);
int make_and(ClauseList* cl, int va, int vb);
int make_or (ClauseList* cl, int va, int vb);
int make_xor(ClauseList* cl, int va, int vb);
int make_eqv(ClauseList* cl, int va, int vb);
int make_implies(ClauseList* cl, int va, int vb);


/* if car_in==-1 , then interpret as "no carry-in bit" */
/* if car_out==-1, then interpret as "don't care if overflow" */
/* numbers va, vb, vs assumed to occupy `len` many contiguous vars */
void ensure_add(ClauseList* cl, int len,
                int car_in, int va, int vb, int vs, int car_out);

