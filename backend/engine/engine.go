package engine

/*
#cgo CFLAGS: -I../../engine/include
#cgo LDFLAGS: -L../../engine/build -lcompound -lsqlite3

#include <stdlib.h>
#include "bridge.h"
*/
import "C"

import (
	"fmt"
	"unsafe"
)

func CreateSkill(
	name string,
	description string,
	weight float32,
) bool {

	fmt.Println("Creating skill...")

	cName := C.CString(name)
	cDescription := C.CString(description)

	defer C.free(unsafe.Pointer(cName))
	defer C.free(unsafe.Pointer(cDescription))

	database := C.bridge_database_open(
		C.CString("../database/skills.db"),
	)

	if database == nil {
		fmt.Println("Failed to open database")
		return false
	}

	defer C.bridge_database_close(database)

	fmt.Println("Database opened")

	if C.bridge_database_initialize(database) != 0 {
		fmt.Println("Failed to initialize database")
		return false
	}

	fmt.Println("Database initialized")

	skill := C.bridge_create_skill(
		cName,
		cDescription,
		C.float(weight),
	)

	if skill == nil {
		fmt.Println("Failed to create C skill")
		return false
	}

	defer C.bridge_destroy_skill(skill)

	fmt.Println("C skill created")

	result := C.bridge_database_insert_skill(
		database,
		skill,
	)

	fmt.Println("Insert result:", int(result))

	return result == 0
}

func UpdateProgress(
	id uint64,
	progress float32,
) bool {

	database := C.bridge_database_open(
		C.CString("../database/skills.db"),
	)

	if database == nil {
		return false
	}

	defer C.bridge_database_close(database)

	result := C.bridge_database_update_progress(
		database,
		C.ulong(id),
		C.float(progress),
	)

	return result == 0
}

func DeleteSkill(id uint64) bool {

	database := C.bridge_database_open(
		C.CString("../database/skills.db"),
	)

	if database == nil {
		return false
	}

	defer C.bridge_database_close(database)

	result := C.bridge_database_delete_skill(
		database,
		C.ulong(id),
	)

	return result == 0
}
