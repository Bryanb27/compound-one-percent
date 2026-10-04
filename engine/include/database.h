#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <stddef.h>

#include "skill.h"

sqlite3* database_open(
    const char* filename
);

void database_close(
    sqlite3* db
);

int database_initialize(
    sqlite3* db
);

int database_insert_skill(
    sqlite3* db,
    const Skill* skill
);

int database_clear_skills(
    sqlite3* db
);

static int database_save_node(
    sqlite3* db,
    const Skill* skill
);

int database_save_tree(
    sqlite3* db,
    const Skill* root
);

Skill* database_load_tree(
    sqlite3* db
);

int database_update_progress(
    sqlite3* db,
    unsigned long id,
    float progress
);

int database_update_skill(
    sqlite3* db,
    unsigned long id,
    const char* name,
    const char* description,
    float weight
);

int database_delete_skill(
    sqlite3* db,
    unsigned long id
);

size_t database_get_skills(
    sqlite3* db,
    Skill** skills,
    size_t max_count
);

Skill* database_find_skill(
    sqlite3* db,
    unsigned long id
);

int database_add_daily_progress(
    sqlite3* db,
    unsigned long id
);

int database_update_parent(
    sqlite3* db,
    unsigned long id,
    long parent_id
);

int database_update_position(
    sqlite3* db,
    unsigned long id,
    int position
);

#endif