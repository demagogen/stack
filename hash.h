#ifndef HASH_H_
#define HASH_H_

#ifdef HASH_PROTECT
uint32_t gnu_hash(const StackElem_t* element);
bool check_hash_sum(STACK* stackInfo);
int count_hash_sum(STACK* stackInfo);

#define HASH_PROTECT

#ifdef HASH_PROTECT
#define HASH_SUM_VARIABLE(name_of_const) \
    uint32_t hash_sum = 0;               \

#define COUNT_HASH_SUM(stackInfo) \
    count_hash_sum(stackInfo);    \

#define CHECK_HASH_SUM(stack)                                \
    if (check_hash_sum(stack))                               \
    {                                                        \
        stack_dump(stackInfo);                               \
        graphic_printf(RED, BOLD, "fucking hash checker\n"); \
        assert(0 && "penetration error\n");                  \
        ASSERT(0 && "penetration error");                    \
    }                                                        \

#endif

#endif

#endif
