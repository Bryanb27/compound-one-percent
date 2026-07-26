#include <stdio.h>

#include "skill.h"

int main(void)
{
    Skill* backend = skill_create(
        "Backend",
        "",
        100
    );

    Skill* csharp = skill_create(
        "C#",
        "",
        70
    );

    Skill* sql = skill_create(
        "SQL",
        "",
        30
    );

    skill_set_progress(csharp, 80);
    skill_set_progress(sql, 50);

    skill_add_child(backend, csharp);
    skill_add_child(backend, sql);

    printf(
        "Backend Progress: %.2f%%\n",
        skill_progress(backend)
    );

    skill_destroy(backend);

    return 0;
}