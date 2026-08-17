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