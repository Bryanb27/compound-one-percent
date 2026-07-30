#include "engine.h"

#include "database.h"

#include <sqlite3.h>

static sqlite3* database = NULL;

int engine_initialize(void)
{
    database = database_open(
        "../database/skills.db"
    );

    if (database == NULL)
    {
        return -1;
    }

    return database_initialize(
        database
    );
}

void engine_shutdown(void)
{
    database_close(database);

    database = NULL;
}