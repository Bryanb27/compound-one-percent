#include "database.h"

#include <stdio.h>

typedef struct
{
    unsigned long id;

    long parent_id;

    Skill* skill;

} LoadedSkill;

sqlite3* database_open(
    const char* filename
)
{
    sqlite3* db = NULL;

    int result = sqlite3_open(
        filename,
        &db
    );

    if (result != SQLITE_OK)
    {
        printf(
            "Could not open database.\n"
        );

        sqlite3_close(db);

        return NULL;
    }

    return db;
}

void database_close(
    sqlite3* db
)
{
    if (db != NULL)
    {
        sqlite3_close(db);
    }
}

int database_initialize(
    sqlite3* db
)
{
    if (db == NULL)
        return -1;

    const char* sql =
        "CREATE TABLE IF NOT EXISTS skills ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "parent_id INTEGER,"
        "name TEXT NOT NULL,"
        "description TEXT,"
        "progress REAL DEFAULT 0,"
        "weight REAL DEFAULT 100,"
        "category INTEGER DEFAULT 0,"
        "status INTEGER DEFAULT 0,"
        "study_sessions INTEGER DEFAULT 0,"
        "last_practiced TEXT"
        ");";

    char* error = NULL;

    int result = sqlite3_exec(
        db,
        sql,
        NULL,
        NULL,
        &error
    );

    if (result != SQLITE_OK)
    {
        printf(
            "SQLite Error: %s\n",
            error
        );

        sqlite3_free(error);

        return -1;
    }

    return 0;
}

int database_insert_skill(
    sqlite3* db,
    const Skill* skill
)
{
    if (db == NULL || skill == NULL)
        return -1;

    const char* sql =
        "INSERT INTO skills ("
        "parent_id,"
        "name,"
        "description,"
        "progress,"
        "weight,"
        "category,"
        "status,"
        "study_sessions"
        ") VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* statement = NULL;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            NULL
        ) != SQLITE_OK)
    {
        return -1;
    }

    long parent_id = -1;

    if (skill->parent != NULL)
    {
        parent_id = skill->parent->id;
    }

    sqlite3_bind_int64(
        statement,
        1,
        parent_id
    );

    sqlite3_bind_text(
        statement,
        2,
        skill->name,
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement,
        3,
        skill->description,
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_double(
        statement,
        4,
        skill->self_progress
    );

    sqlite3_bind_double(
        statement,
        5,
        skill->weight
    );

    sqlite3_bind_int(
        statement,
        6,
        skill->category
    );

    sqlite3_bind_int(
        statement,
        7,
        skill->status
    );

    sqlite3_bind_int(
        statement,
        8,
        skill->study_sessions
    );

    int result = sqlite3_step(statement);

    if (result != SQLITE_DONE)
    {
        printf(
            "SQLite insert error: %s\n",
            sqlite3_errmsg(db)
        );

        sqlite3_finalize(statement);

        return -1;
    }

    sqlite3_finalize(statement);

    return 0;
}

int database_clear_skills(
    sqlite3* db
)
{
    if (db == NULL)
        return -1;

    const char* sql =
        "DELETE FROM skills;";

    char* error = NULL;

    int result = sqlite3_exec(
        db,
        sql,
        NULL,
        NULL,
        &error
    );

    if (result != SQLITE_OK)
    {
        printf(
            "SQLite Error: %s\n",
            error
        );

        sqlite3_free(error);

        return -1;
    }

    return 0;
}

static int database_save_node(
    sqlite3* db,
    const Skill* skill
)
{
    if (skill == NULL)
        return 0;

    if (database_insert_skill(db, skill) != 0)
        return -1;

    for (size_t i = 0; i < skill->child_count; i++)
    {
        if (database_save_node(
                db,
                skill->children[i]
            ) != 0)
        {
            return -1;
        }
    }

    return 0;
}

int database_save_tree(
    sqlite3* db,
    const Skill* root
)
{
    if (db == NULL || root == NULL)
        return -1;

    char* error = NULL;

    if (sqlite3_exec(
            db,
            "BEGIN TRANSACTION;",
            NULL,
            NULL,
            &error
        ) != SQLITE_OK)
    {
        printf(
            "SQLite Error: %s\n",
            error
        );

        sqlite3_free(error);

        return -1;
    }

    int result = database_save_node(
        db,
        root
    );

    if (result == 0)
    {
        sqlite3_exec(
            db,
            "COMMIT;",
            NULL,
            NULL,
            NULL
        );
    }
    else
    {
        sqlite3_exec(
            db,
            "ROLLBACK;",
            NULL,
            NULL,
            NULL
        );
    }

    return result;
}

int database_update_progress(
    sqlite3* db,
    unsigned long id,
    float progress
)
{
    if (db == NULL)
        return -1;

    const char* sql =
        "UPDATE skills "
        "SET progress = ? "
        "WHERE id = ?;";

    sqlite3_stmt* statement = NULL;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            NULL
        ) != SQLITE_OK)
    {
        return -1;
    }

    sqlite3_bind_double(
        statement,
        1,
        progress
    );

    sqlite3_bind_int64(
        statement,
        2,
        id
    );

    int result = sqlite3_step(statement);

    sqlite3_finalize(statement);

    return result == SQLITE_DONE ? 0 : -1;
}

int database_update_skill(
    sqlite3* db,
    unsigned long id,
    const char* name,
    const char* description,
    float weight
)
{
    if (db == NULL || name == NULL || description == NULL)
        return -1;

    const char* sql =
        "UPDATE skills "
        "SET name = ?, "
        "description = ?, "
        "weight = ? "
        "WHERE id = ?;";

    sqlite3_stmt* statement = NULL;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            NULL
        ) != SQLITE_OK)
    {
        return -1;
    }

    sqlite3_bind_text(
        statement,
        1,
        name,
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement,
        2,
        description,
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_double(
        statement,
        3,
        weight
    );

    sqlite3_bind_int64(
        statement,
        4,
        id
    );

    int result = sqlite3_step(statement);

    sqlite3_finalize(statement);

    if (result != SQLITE_DONE)
        return -1;

    return 0;
}

int database_delete_skill(
    sqlite3* db,
    unsigned long id
)
{
    if (db == NULL)
        return -1;

    const char* sql =
        "WITH RECURSIVE descendants(id) AS ("
        "    SELECT id FROM skills WHERE id = ? "
        "    UNION ALL "
        "    SELECT skills.id "
        "    FROM skills "
        "    JOIN descendants "
        "    ON skills.parent_id = descendants.id"
        ") "
        "DELETE FROM skills "
        "WHERE id IN (SELECT id FROM descendants);";

    sqlite3_stmt* statement = NULL;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            NULL
        ) != SQLITE_OK)
    {
        return -1;
    }

    sqlite3_bind_int64(
        statement,
        1,
        id
    );

    int result = sqlite3_step(statement);

    sqlite3_finalize(statement);

    if (result != SQLITE_DONE)
        return -1;

    return 0;
}

Skill* database_load_tree(
    sqlite3* db
)
{
    if (db == NULL)
        return NULL;

    const char* sql =
        "SELECT "
        "id,"
        "parent_id,"
        "name,"
        "description,"
        "progress,"
        "weight,"
        "category,"
        "status,"
        "study_sessions "
        "FROM skills;";
    
    sqlite3_stmt* statement = NULL;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            NULL
        ) != SQLITE_OK)
    {
        return NULL;
    }

    LoadedSkill loaded[256];

    size_t loaded_count = 0;

    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        unsigned long id =
            sqlite3_column_int64(statement, 0);

        const char* name =
            (const char*)sqlite3_column_text(statement, 2);

        const char* description =
            (const char*)sqlite3_column_text(statement, 3);

        float weight =
            sqlite3_column_double(statement, 5);

        Skill* skill = skill_create(
            name,
            description,
            weight
        );

        skill->id = id;

        skill_set_progress(
            skill,
            sqlite3_column_double(statement, 4)
        );

        skill_set_category(
            skill,
            (SkillCategory)
            sqlite3_column_int(statement, 6)
        );

        skill->status =
            sqlite3_column_int(statement, 7);

        skill->study_sessions =
            sqlite3_column_int(statement, 8);

        printf(
            "Loaded: %s\n",
            skill->name
        );

        loaded[loaded_count].id = id;

        loaded[loaded_count].parent_id =
            sqlite3_column_int64(
                statement,
                1
            );

        loaded[loaded_count].skill = skill;

        loaded_count++;

    }
    
    sqlite3_finalize(statement);

    Skill* root = NULL;

    for(size_t i = 0; i < loaded_count; i++)
    {
        if(loaded[i].parent_id == -1)
        {
            root = loaded[i].skill;
            break;
        }
    }

    for(size_t i = 0; i < loaded_count; i++)
    {
        if(loaded[i].parent_id == -1)
            continue;

        for(size_t j = 0; j < loaded_count; j++)
        {
            if(loaded[j].id == loaded[i].parent_id)
            {
                skill_add_child(
                    loaded[j].skill,
                    loaded[i].skill
                );

                break;
            }
        }
    }

    return root;
}

size_t database_get_skills(
    sqlite3* db,
    Skill** skills,
    size_t max_count
)
{
    if (db == NULL || skills == NULL)
        return 0;

    const char* sql =
        "SELECT "
        "id,"
        "parent_id,"
        "name,"
        "description,"
        "progress,"
        "weight,"
        "category,"
        "status,"
        "study_sessions "
        "FROM skills;";

    sqlite3_stmt* statement = NULL;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            NULL
        ) != SQLITE_OK)
    {
        return 0;
    }

    unsigned long parent_ids[256];

    size_t count = 0;

    while (
        sqlite3_step(statement) == SQLITE_ROW &&
        count < max_count
    )
    {
        unsigned long id =
            sqlite3_column_int64(statement, 0);

        parent_ids[count] =
            sqlite3_column_int64(statement, 1);

        const char* name =
            (const char*)sqlite3_column_text(
                statement,
                2
            );

        const char* description =
            (const char*)sqlite3_column_text(
                statement,
                3
            );

        float progress =
            sqlite3_column_double(
                statement,
                4
            );

        float weight =
            sqlite3_column_double(
                statement,
                5
            );

        Skill* skill = skill_create(
            name,
            description,
            weight
        );

        if (skill == NULL)
            continue;

        skill->id = id;

        skill_set_progress(
            skill,
            progress
        );

        skill_set_category(
            skill,
            (SkillCategory)
            sqlite3_column_int(statement, 6)
        );

        skill->status =
            sqlite3_column_int(statement, 7);

        skill->study_sessions =
            sqlite3_column_int(statement, 8);

        skills[count] = skill;

        count++;
    }

    sqlite3_finalize(statement);

    /*
     * Build parent -> child relationships.
     */
    for (size_t i = 0; i < count; i++)
    {
        if (parent_ids[i] == -1)
            continue;

        for (size_t j = 0; j < count; j++)
        {
            if (skills[j]->id == parent_ids[i])
            {
                skill_add_child(
                    skills[j],
                    skills[i]
                );

                break;
            }
        }
    }

    return count;
}

Skill* database_find_skill(
    sqlite3* db,
    unsigned long id
)
{
    if (db == NULL)
        return NULL;

    const char* sql =
        "SELECT "
        "id,"
        "name,"
        "description,"
        "progress,"
        "weight,"
        "category,"
        "status,"
        "study_sessions "
        "FROM skills "
        "WHERE id = ?;";

    sqlite3_stmt* statement = NULL;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            NULL
        ) != SQLITE_OK)
    {
        return NULL;
    }

    sqlite3_bind_int64(
        statement,
        1,
        id
    );

    Skill* skill = NULL;

    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        const char* name =
            (const char*)sqlite3_column_text(
                statement,
                1
            );

        const char* description =
            (const char*)sqlite3_column_text(
                statement,
                2
            );

        float weight =
            sqlite3_column_double(
                statement,
                4
            );

        skill = skill_create(
            name,
            description,
            weight
        );

        if (skill != NULL)
        {
            skill->id =
                sqlite3_column_int64(
                    statement,
                    0
                );

            skill_set_progress(
                skill,
                sqlite3_column_double(
                    statement,
                    3
                )
            );

            skill_set_category(
                skill,
                (SkillCategory)
                sqlite3_column_int(
                    statement,
                    5
                )
            );

            skill->status =
                sqlite3_column_int(
                    statement,
                    6
                );

            skill->study_sessions =
                sqlite3_column_int(
                    statement,
                    7
                );
        }
    }

    sqlite3_finalize(statement);

    return skill;
}

int database_add_daily_progress(
    sqlite3* db,
    unsigned long id
)
{
    if (db == NULL)
        return -1;

    const char* sql =
        "UPDATE skills "
        "SET progress = MIN(progress + 1, 100), "
        "study_sessions = study_sessions + 1, "
        "last_practiced = date('now') "
        "WHERE id = ? "
        "AND (last_practiced IS NULL "
        "OR last_practiced != date('now'));";

    sqlite3_stmt* statement = NULL;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            NULL
        ) != SQLITE_OK)
    {
        return -1;
    }

    sqlite3_bind_int64(
        statement,
        1,
        id
    );

    int result = sqlite3_step(statement);

    if (result != SQLITE_DONE)
    {
        sqlite3_finalize(statement);
        return -1;
    }

    int changed = sqlite3_changes(db);

    sqlite3_finalize(statement);

    return changed;
}

int database_update_parent(
    sqlite3* db,
    unsigned long id,
    long parent_id
)
{
    if (db == NULL)
        return -1;

    const char* sql =
        "UPDATE skills "
        "SET parent_id = ? "
        "WHERE id = ?;";

    sqlite3_stmt* statement = NULL;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            NULL
        ) != SQLITE_OK)
    {
        return -1;
    }

    sqlite3_bind_int64(
        statement,
        1,
        parent_id
    );

    sqlite3_bind_int64(
        statement,
        2,
        id
    );

    int result = sqlite3_step(statement);

    sqlite3_finalize(statement);

    if (result != SQLITE_DONE)
        return -1;

    return 0;
}

int database_update_position(
    sqlite3* db,
    unsigned long id,
    int position
)
{
    if (db == NULL)
        return -1;

    const char* sql =
        "UPDATE skills "
        "SET position = ? "
        "WHERE id = ?;";

    sqlite3_stmt* statement = NULL;

    if (sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            NULL
        ) != SQLITE_OK)
    {
        return -1;
    }

    sqlite3_bind_int(
        statement,
        1,
        position
    );

    sqlite3_bind_int64(
        statement,
        2,
        id
    );

    int result = sqlite3_step(statement);

    sqlite3_finalize(statement);

    return result == SQLITE_DONE ? 0 : -1;
}