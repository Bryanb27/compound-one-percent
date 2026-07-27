#include "skill.h"
#include "id_generator.h"

#include <stdlib.h>
#include <string.h>

Skill* skill_create(
    const char* name,
    const char* description,
    float weight
)
{
    Skill* skill = malloc(sizeof(Skill));

    if (skill == NULL)
        return NULL;

    skill->id = generate_id();
    
    strncpy(skill->name, name, sizeof(skill->name) - 1);
    skill->name[sizeof(skill->name) - 1] = '\0';

    strncpy(skill->description, description, sizeof(skill->description) - 1);
    skill->description[sizeof(skill->description) - 1] = '\0';

    skill->self_progress = 0.0f;
    skill->weight = weight;

    skill->parent = NULL;

    skill->children = NULL;
    skill->child_count = 0;

    skill->category = SKILL_CATEGORY_NONE;

    skill->status = SKILL_STATUS_NOT_STARTED;

    skill->estimated_hours = 0;

    skill->study_sessions = 0;

    return skill;
}

void skill_destroy(Skill* skill)
{
    if (skill == NULL)
        return;

    for(size_t i = 0; i < skill->child_count; i++){
        skill_destroy(skill->children[i]);
    }

    free(skill->children);

    free(skill);
}

size_t skill_child_count(const Skill* skill)
{
    if(skill == NULL){
        return 0;
    }

    return skill->child_count;
}

void skill_set_category(
    Skill* skill,
    SkillCategory category
){
    if(skill == NULL){
        return;
    }

    skill->category = category;
}

void skill_set_status(
    Skill* skill,
    SkillStatus status
){
    if(skill == NULL){
        return;
    }

    skill->status = status;
}

void skill_add_study_session(
    Skill* skill
){
    if(skill == NULL){
        return;
    }

    skill->study_sessions++;
}

static int skill_contains_child(const Skill* parent, const Skill* child){
    if(parent == NULL || child == NULL){
        return 0;
    }

    for(size_t i = 0; i < parent->child_count; i++){
        if(parent->children[i] == child){
            return 1;
        }
    }

    return 0;
}

static int skill_is_ancestor(const Skill* node, const Skill* possible_ancestor){
    const Skill* current = node;

    while(current != NULL){
        if(current == possible_ancestor){
            return 1;
        }
        current = current->parent;
    }

    return 0;
}

int skill_add_child(Skill* parent, Skill* child){
    if(parent == NULL || child == NULL){
        return -1;
    }

    if(parent == child){
        return -1;
    }

    if(child->parent != NULL){
        return -1;
    }

    if(skill_contains_child(parent, child)){
        return -1;
    }

    if(skill_is_ancestor(parent, child)){
        return -1;
    }

    Skill** new_children = realloc(
        parent->children,
        (parent->child_count + 1) * sizeof(Skill*)
    );

    if(new_children == NULL){
        return -1;
    }

    parent->children = new_children;

    parent->children[parent->child_count] = child;

    parent->child_count++;

    child->parent = parent;

    return 0;
}

int skill_remove_child(
    Skill* parent,
    Skill* child
){
    if(parent == NULL || child == NULL){
        return -1;
    }

    size_t index = parent->child_count;

    for(size_t i = 0; i < parent->child_count; i++){
        if(parent->children[i] == child){
            index = i;
            break;
        }
    }

    if(index == parent->child_count){
        return -1;
    }

    for(size_t i = index; i + 1 < parent->child_count; i++){
        parent->children[i] = parent->children[i + 1];
    }

    parent->child_count--;

    if(parent->child_count == 0){
        free(parent->children);
        parent->children = NULL;
    } else {
        Skill** resized = realloc(
            parent->children,
            parent->child_count * sizeof(Skill*)
        );

        if(resized != NULL){
            parent->children = resized;
        }
    }

    child->parent = NULL;

    return 0;
}

void skill_set_progress(
    Skill* skill,
    float progress
)
{
    if(skill == NULL){
        return;
    }

    if(progress < 0.0f){
        progress = 0.0f;
    }

    if(progress > 100.0f){
        progress = 100.0f;
    }

    skill->self_progress = progress;
}

float skill_progress(const Skill* skill)
{
    if (skill == NULL)
        return 0.0f;

    if (skill->child_count == 0)
        return skill->self_progress;

    float total_weight = 0.0f;
    float weighted_progress = 0.0f;

    for (size_t i = 0; i < skill->child_count; i++)
    {
        Skill* child = skill->children[i];

        total_weight += child->weight;

        weighted_progress +=
            skill_progress(child) * child->weight;
    }

    if (total_weight == 0.0f)
        return 0.0f;

    return weighted_progress / total_weight;
}

Skill* skill_find(
    Skill* root,
    const char* name
)
{
    if(root == NULL || name == NULL){
        return NULL;
    }

    if(strcmp(root->name, name) == 0){
        return root;
    }

    for(size_t i = 0; i < root->child_count; i++){
        Skill* result =
            skill_find(root->children[i], name);

        if(result != NULL){
            return result;
        }
    }

    return NULL;
}

const Skill* skill_find_const(
    const Skill* root,
    const char* name
)
{
    return skill_find(
        (Skill*)root,
        name
    );
}

Skill* skill_find_by_id(
    Skill* root,
    unsigned long id
)
{
    if (root == NULL)
        return NULL;

    if (root->id == id)
        return root;

    for (size_t i = 0; i < root->child_count; i++)
    {
        Skill* result =
            skill_find_by_id(
                root->children[i],
                id
            );

        if (result != NULL)
            return result;
    }

    return NULL;
}