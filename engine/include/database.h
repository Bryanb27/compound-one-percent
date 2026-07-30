#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

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

int database_delete_skill(
    sqlite3* db,
    unsigned long id
);

#endif