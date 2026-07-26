#ifndef SKILL_H
#define SKILL_H

#include <stddef.h>

typedef struct Skill Skill;

struct Skill
{
    char name[64];
    char description[256];

    float progress;
    float weight;

    Skill* parent;

    Skill** children;
    size_t child_count;
};

#endif