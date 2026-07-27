#include <stdio.h>

#include "skill.h"

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

    skill_destroy(backend);

    return 0;
}