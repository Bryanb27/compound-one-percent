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

    printf("Program started\n");

    Skill* result =
    skill_find(backend, "SQL");

    printf("Search finished\n");

    if (result != NULL)
    {
        printf("%s\n", result->name);
    }

    Skill* missing = skill_find(backend, "Rust");

    if (missing == NULL)
    {
        printf("Rust not found\n");
    }

    printf(
        "Backend Progress: %.2f%%\n",
        skill_progress(backend)
    );

    printf(
        "C# ID: %lu\n",
        csharp->id
    );

    result = skill_find_by_id(
        backend,
        csharp->id
    );

    printf(
        "Found: %s\n",
        result->name
    );

    skill_set_category(
        csharp,
        SKILL_CATEGORY_LANGUAGE
    );

    skill_add_study_session(csharp);

    skill_add_study_session(csharp);

    printf(
        "Sessions: %u\n",
        csharp->study_sessions
    );

    printf("Children: %zu\n",
    skill_child_count(backend));

    skill_remove_child(backend, sql);

    printf("Children: %zu\n",
        skill_child_count(backend));

    Skill* found = skill_find(backend, "SQL");

    if (found == NULL)
    {
        printf("SQL removed successfully.\n");
    }

    int test = skill_save(
        backend,
        "skills.txt"
        );

    if(test == 0){
        printf("Saved succesfully.\n");
    } else {
        printf("Error while creating file.\n");
    }

    sqlite3* db = database_open(
        "skills.db"
    );

    if (db == NULL)
    {
        printf("Database failed.\n");
        return 1;
    }

    if (database_initialize(db) == 0)
    {
        printf(
            "Database initialized.\n"
        );
    }
    else
    {
        printf(
            "Database initialization failed.\n"
        );
    }

    database_clear_skills(db);

    database_save_tree(
        db,
        backend
    );

    backend = database_load_tree(db);

    printf(
        "%s\n",
        backend->name
    );

    printf(
        "Children: %zu\n",
        backend->child_count
    );

    database_update_progress(
        db,
        sql->id,
        90.0f
    );

    database_delete_skill(
        db,
        csharp->id
    );


    database_close(db);
    skill_destroy(backend);

    return 0;
}