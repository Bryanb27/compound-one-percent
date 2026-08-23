#ifndef BRIDGE_H
#define BRIDGE_H

#include "skill.h"
#include "database.h"
#include <sqlite3.h>


void engine_hello(void);

Skill* bridge_create_skill(
    const char* name,
    const char* description,
    float weight
);

unsigned long bridge_skill_id(
    Skill* skill
);

const char* bridge_skill_name(
    Skill* skill
);

float bridge_skill_progress(
    Skill* skill
);

void bridge_destroy_skill(
    Skill* skill
);

Skill* bridge_load_tree(
    sqlite3* db
);

sqlite3* bridge_database_open(
    const char* filename
);

void bridge_database_close(
    sqlite3* db
);

int bridge_database_initialize(
    sqlite3* db
);

void bridge_print_tree(
    Skill* skill
);

void bridge_test_database(
    sqlite3* db
);

int bridge_database_insert_skill(
    sqlite3* db,
    Skill* skill
);

int bridge_database_update_progress(
    sqlite3* db,
    unsigned long id,
    float progress
);

int bridge_database_delete_skill(
    sqlite3* db,
    unsigned long id
);

void bridge_add_study_session(
    Skill* skill
);

size_t bridge_skill_child_count(
    Skill* skill
);

Skill* bridge_skill_child(
    Skill* skill,
    size_t index
);

size_t bridge_get_skills(
    sqlite3* db,
    Skill** skills,
    size_t max_count
);

void bridge_skill_set_progress(
    Skill* skill,
    float progress
);

#endif