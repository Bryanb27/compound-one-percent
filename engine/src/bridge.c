#include <stdio.h>

#include "bridge.h"
#include "database.h"

void engine_hello(void)
{
    printf("Hello from C Engine!\n");
}

Skill* bridge_create_skill(
    const char* name,
    const char* description,
    float weight
)
{
    return skill_create(
        name,
        description,
        weight
    );
}

unsigned long bridge_skill_id(
    Skill* skill
)
{
    if (skill == NULL)
        return 0;

    return skill->id;
}

const char* bridge_skill_name(
    Skill* skill
)
{
    if (skill == NULL)
        return NULL;

    return skill->name;
}

float bridge_skill_progress(
    Skill* skill
)
{
    if (skill == NULL)
        return 0.0f;

    return skill->self_progress;
}

void bridge_destroy_skill(
    Skill* skill
)
{
    skill_destroy(skill);
}

Skill* bridge_load_tree(
    sqlite3* db
)
{
    return database_load_tree(db);
}

sqlite3* bridge_database_open(
    const char* filename
)
{
    return database_open(filename);
}

void bridge_database_close(
    sqlite3* db
)
{
    database_close(db);
}

int bridge_database_initialize(
    sqlite3* db
)
{
    return database_initialize(db);
}

void bridge_print_tree(
    Skill* skill
)
{
    if (skill == NULL)
        return;

    printf(
        "%lu|%s|%.2f\n",
        skill->id,
        skill->name,
        skill->self_progress
    );

    for (size_t i = 0; i < skill->child_count; i++)
    {
        bridge_print_tree(
            skill->children[i]
        );
    }
}

void bridge_test_database(
    sqlite3* db
)
{
    if (db == NULL)
    {
        printf("No database.\n");
        return;
    }

    printf("C received database.\n");
}

int bridge_database_insert_skill(
    sqlite3* db,
    Skill* skill
)
{
    if (db == NULL || skill == NULL)
        return -1;

    return database_insert_skill(
        db,
        skill
    );
}

int bridge_database_update_progress(
    sqlite3* db,
    unsigned long id,
    float progress
)
{
    if (db == NULL)
        return -1;

    return database_update_progress(
        db,
        id,
        progress
    );
}

int bridge_database_delete_skill(
    sqlite3* db,
    unsigned long id
)
{
    if (db == NULL)
        return -1;

    return database_delete_skill(
        db,
        id
    );
}

void bridge_add_study_session(
    Skill* skill
)
{
    if (skill == NULL)
        return;

    skill_add_study_session(skill);
}

size_t bridge_skill_child_count(
    Skill* skill
)
{
    if (skill == NULL)
        return 0;

    return skill->child_count;
}

Skill* bridge_skill_child(
    Skill* skill,
    size_t index
)
{
    if (skill == NULL)
        return NULL;

    if (index >= skill->child_count)
        return NULL;

    return skill->children[index];
}

size_t bridge_get_skills(
    sqlite3* db,
    Skill** skills,
    size_t max_count
)
{
    return database_get_skills(
        db,
        skills,
        max_count
    );
}

void bridge_skill_set_progress(
    Skill* skill,
    float progress
)
{
    skill_set_progress(
        skill,
        progress
    );
}

void bridge_add_child(
    Skill* parent,
    Skill* child
)
{
    if (parent == NULL || child == NULL)
        return;

    skill_add_child(
        parent,
        child
    );
}
Skill* bridge_find_skill_by_id(
    Skill* root,
    unsigned long id
)
{
    if (root == NULL)
        return NULL;

    return skill_find_by_id(
        root,
        id
    );
}

Skill* bridge_database_find_skill(
    sqlite3* db,
    unsigned long id
)
{
    return database_find_skill(
        db,
        id
    );
}

long bridge_skill_parent_id(
    Skill* skill
)
{
    if (skill == NULL)
        return -1;

    if (skill->parent == NULL)
        return -1;

    return skill->parent->id;
}

int bridge_database_update_skill(
    sqlite3* db,
    unsigned long id,
    const char* name,
    const char* description,
    float weight
)
{
    if (db == NULL)
        return -1;

    return database_update_skill(
        db,
        id,
        name,
        description,
        weight
    );
}

int bridge_database_add_daily_progress(
    sqlite3* db,
    unsigned long id
)
{
    if (db == NULL)
        return -1;

    return database_add_daily_progress(
        db,
        id
    );
}

float bridge_skill_calculate_progress(
    Skill* skill
)
{
    return skill_calculate_progress(skill);
}

int bridge_database_update_parent(
    sqlite3* db,
    unsigned long id,
    long parent_id
)
{
    if (db == NULL)
        return -1;

    return database_update_parent(
        db,
        id,
        parent_id
    );
}

int bridge_database_update_position(
    sqlite3* db,
    unsigned long id,
    int position
)
{
    if (db == NULL)
        return -1;

    return database_update_position(
        db,
        id,
        position
    );
}