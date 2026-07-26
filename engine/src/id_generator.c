#include <stddef.h>

static unsigned long current_id = 1;

unsigned long generate_id(void)
{
    return current_id++;
}