#include "skill.h"

#include <stdlib.h>
#include <string.h>

Skill* skill_create(
    const char* name,
    const char* description,
    float weight
)
{
    Skill* skill = malloc(sizeof(Skill));

    if (skill == NULL)
        return NULL;

    strncpy(skill->name, name, sizeof(skill->name) - 1);
    skill->name[sizeof(skill->name) - 1] = '\0';

    strncpy(skill->description, description, sizeof(skill->description) - 1);
    skill->description[sizeof(skill->description) - 1] = '\0';

    skill->progress = 0.0f;
    skill->weight = weight;

    skill->parent = NULL;

    skill->children = NULL;
    skill->child_count = 0;

    return skill;
}

void skill_destroy(Skill* skill)
{
    if (skill == NULL)
        return;

    free(skill);
}