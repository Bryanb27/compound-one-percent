#ifndef SKILL_H
#define SKILL_H

#include <stddef.h>

typedef struct Skill Skill;

struct Skill
{
    unsigned long id;
    
    char name[64];
    char description[256];

    float self_progress;
    float weight;

    Skill* parent;

    Skill** children;
    size_t child_count;
};

Skill* skill_create(
    const char* name,
    const char* description,
    float weight
);

void skill_destroy(Skill* skill);

int skill_add_child(Skill* parent, Skill* child);

size_t skill_child_count(const Skill* skill);

float skill_progress(const Skill* skill);

void skill_set_progress(
    Skill* skill,
    float progress
);

Skill* skill_find(
    Skill* root,
    const char* name
);

const Skill* skill_find_const(
    const Skill* root,
    const char* name
);

Skill* skill_find_by_id(
    Skill* root,
    unsigned long id
);

#endif