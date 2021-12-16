#include "core/util.h"
#include "utility/string.h"
#include "core/tokenizer.h"

bool_t string_is_register(char* str)
{
    for (uint32_t i = 0; i < ASM_REG_NAMES_COUNT; i++)
    {
        if (streql(str, ASM_REG_NAMES[i])) { return TRUE; }
    }
    return FALSE;
}