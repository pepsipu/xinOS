
#define assert(cond)                                                                                                   \
    {                                                                                                                  \
        if (!(cond))                                                                                                   \
        {                                                                                                              \
            kprint("assertion \"%s\" failed.", #cond);                                                                 \
            hang();                                                                                                    \
        }                                                                                                              \
    }