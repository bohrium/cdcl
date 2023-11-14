#define MAKE_TAGGED_UNION2(name, t0, n0, t1, n1)

typedef struct {
  char which;
  t0 _0_##name;
  t1 _1_##name;
} name;

name n0(t0 x)
{
    name rtrn;
    rtrn.which = 0;
    rtrn._0_##name = x;
    return rtrn;
}

name n1(t1 x)
{
    name rtrn;
    rtrn.which = 1;
    rtrn._1_##name = x;
    return rtrn;
}

int is_##n0(name y)
{
    return (y.which==0) ? 1 : 0;
}
int is_##n1(name y)
{
    return (y.which==1) ? 1 : 0;
}
