#include <stdio.h>

#include "skill.h"
#include "database.h"

int main(void)
{
    Skill* backend = skill_create(
        "Backend",
        "",
        100
    );

    Skill* csharp = skill_create(
        "C#",
        "",
        70
    );

    Skill* sql = skill_create(
        "SQL",
        "",
        30
    );

    skill_set_progress(csharp, 80);
    skill_set_progress(sql, 50);

    skill_add_child(backend, csharp);
    skill_add_child(backend, sql);

    printf("Skill tree created.\n");

    sqlite3* db = database_open(
        "../database/skills.db"
    );

    if (db == NULL)
    {
        printf("Database failed.\n");
        skill_destroy(backend);
        return 1;
    }

    printf("Database opened.\n");

    if (database_initialize(db) != 0)
    {
        printf("Database initialization failed.\n");

        database_close(db);
        skill_destroy(backend);

        return 1;
    }

    printf("Database initialized.\n");

    database_clear_skills(db);

    database_save_tree(
        db,
        backend
    );

    printf("Tree saved.\n");

    skill_destroy(backend);

    backend = database_load_tree(db);

    if (backend == NULL)
    {
        printf("Tree failed to load.\n");

        database_close(db);

        return 1;
    }

    printf("Tree loaded.\n");

    printf(
        "Root: %s\n",
        backend->name
    );

    printf(
        "Children: %zu\n",
        backend->child_count
    );

    database_close(db);

    skill_destroy(backend);

    return 0;
}