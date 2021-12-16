#include "utility/types.h"
#include "utility/debug.h"
#include "utility/string.h"
#include "core/tokenizer.h"

int main(int argc, char** argv)
{
    debug_init();

    tokenizer_t tokenizer = tokenizer_load(argv[1]);
    if (!tokenizer_run(&tokenizer)) { return 0xFF; }

    tokenizer_dispose(&tokenizer);
    return 0;
}
