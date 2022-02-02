#define DIVIDE_ROUNDNEAREST(numer, denom) (                                             \
    /* NB: `!=` acts as a logical XOR operator */                                       \
    /* See: https://stackoverflow.com/a/1596681/4561887 */                              \
    ((numer) < 0) != ((denom) < 0) ?                                                    \
    /* numer OR denom, but NOT both, is negative, so do this: */                        \
    ((numer) - ((denom)/2)) / (denom) :                                                 \
    /* numer AND denom are either *both positive* OR *both negative*, so do this: */    \
    ((numer) + ((denom)/2)) / (denom)                                                   \
)
