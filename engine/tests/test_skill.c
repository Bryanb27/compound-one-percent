#include <stdio.h>

#include "skill.h"

int main(void)
{
    Skill* sql = skill_create(
        "SQL",
        "Database Language",
        30.0f
    );
    
    Skill* backend = skill_create("Backend", "", 100.0f);

    printf("%d\n", skill_add_child(backend, backend));

    Skill* csharp = skill_create("C#", "", 50.0f);

    skill_add_child(backend, csharp);

    printf("%d\n", skill_add_child(backend, csharp));

    skill_add_child(backend, csharp);
    skill_add_child(backend, sql);

    printf("Backend has %zu children\n",
        skill_child_count(backend));

    skill_destroy(backend);

    return 0;
}