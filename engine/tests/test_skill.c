#include <stdio.h>

#include "skill.h"

int main(void)
{
    Skill* skill = skill_create(
        "Backend",
        "Backend Development",
        100.0f
    );

    if (skill == NULL)
    {
        printf("Creation failed\n");
        return 1;
    }

    printf("Name: %s\n", skill->name);
    printf("Description: %s\n", skill->description);
    printf("Progress: %.1f\n", skill->progress);

    skill_destroy(skill);

    return 0;
}