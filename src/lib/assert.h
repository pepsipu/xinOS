#include <sys/cpu.h>

// asserts should be *explicit*. when a user sees an assertion failure, they should know exactly what happened.
#define assert(cond)                                                                                                   \
    {                                                                                                                  \
        if (!__builtin_expect((cond), 1))                                                                              \
        {                                                                                                              \
            kprint("assert \"%s\" at \"%s:%s:%d\" failed", #cond, __FILE__, __func__, __LINE__);                       \
            hang();                                                                                                    \
        }                                                                                                              \
    }