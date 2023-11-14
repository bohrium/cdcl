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

void build(ClauseList* cl)
{
    int a = add_var(cl);
    int b = add_var(cl);
    int c = add_var(cl);
    int d = add_var(cl);
    int e = add_var(cl);
    int f = add_var(cl);

    ifthen(cl, a, make_or(cl, b, c));

    ifthen(cl, b, make_or(cl, d, e));
    ifthen(cl, d, make_or(cl, f, make_not(cl, a)));
    ifthen(cl, e, make_or(cl, f, make_not(cl, a)));
    deny(cl, f);
    ifthen(cl, make_not(cl, f), a);

    ifthen(cl, c, make_or(cl, a, b));
    ifthen(cl, make_not(cl, c), make_or(cl, a, b));
}
