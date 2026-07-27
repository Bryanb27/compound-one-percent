#ifndef STORAGE_H
#define STORAGE_H

#include "skill.h"

int skill_save(
    const Skill* root,
    const char* filename
);

Skill* skill_load(
    const char* filename
);

#endif