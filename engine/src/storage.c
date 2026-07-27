#include <stdio.h>

#include "storage.h"
#include "skill.h"

static void skill_save_node(
    FILE* file,
    const Skill* skill
)
{
    long parent_id = -1;

    if (skill->parent != NULL)
        parent_id = (long)skill->parent->id;

    fprintf(
        file,
        "%lu|%ld|%s|%s|%.2f|%.2f|%d\n",
        skill->id,
        parent_id,
        skill->name,
        skill->description,
        skill->weight,
        skill->self_progress,
        skill->category
    );

    for (size_t i = 0; i < skill->child_count; i++)
    {
        skill_save_node(
            file,
            skill->children[i]
        );
    }
}

int skill_save(
    const Skill* root,
    const char* filename
)
{
    if (root == NULL || filename == NULL)
        return -1;

    FILE* file = fopen(filename, "w");

    if (file == NULL)
        return -1;

    skill_save_node(file, root);

    fclose(file);

    return 0;
}