#ifndef STDBOOL_H
#define STDBOOL_H

#ifndef __cplusplus

/* Define bool, true, and false for C */
#define bool _Bool
#define true 1
#define false 0

#else

/* In C++, bool, true, and false are already defined as keywords. */
/* Do not redefine them. */
#define _Bool bool

#endif

#define __bool_true_false_are_defined 1

#endif /* STDBOOL_H */