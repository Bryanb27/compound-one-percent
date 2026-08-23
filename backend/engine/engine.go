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

type Skill struct {
	ID       uint64  `json:"id"`
	Name     string  `json:"name"`
	Progress float32 `json:"progress"`
	Children []Skill `json:"children"`
}

func CreateSkill(
	name string,
	description string,
	weight float32,
	progress float32,
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

	C.bridge_skill_set_progress(
		skill,
		C.float(progress),
	)

	result := C.bridge_database_insert_skill(
		database,
		skill,
	)

	fmt.Println(
		"Insert result:",
		int(result),
	)

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

func GetSkills() *Skill {

	database := C.bridge_database_open(
		C.CString("../database/skills.db"),
	)

	if database == nil {
		return nil
	}

	defer C.bridge_database_close(database)

	root := C.bridge_load_tree(database)

	if root == nil {
		return nil
	}

	result := convertSkill(root)

	C.bridge_destroy_skill(root)

	return &result
}

func convertSkill(
	skill *C.Skill,
) Skill {

	result := Skill{
		ID: uint64(
			C.bridge_skill_id(skill),
		),

		Name: C.GoString(
			C.bridge_skill_name(skill),
		),

		Progress: float32(
			C.bridge_skill_progress(skill),
		),
	}

	count := int(
		C.bridge_skill_child_count(skill),
	)

	for i := 0; i < count; i++ {

		child := C.bridge_skill_child(
			skill,
			C.size_t(i),
		)

		if child != nil {
			result.Children = append(
				result.Children,
				convertSkill(child),
			)
		}
	}

	return result
}

func GetAllSkills() []Skill {

	database := C.bridge_database_open(
		C.CString("../database/skills.db"),
	)

	if database == nil {
		return nil
	}

	defer C.bridge_database_close(database)

	var skills [256]*C.Skill

	count := C.bridge_get_skills(
		database,
		(**C.Skill)(&skills[0]),
		C.size_t(len(skills)),
	)

	result := make([]Skill, 0, int(count))

	for i := 0; i < int(count); i++ {

		if skills[i] == nil {
			continue
		}

		result = append(
			result,
			convertSkill(skills[i]),
		)

		C.bridge_destroy_skill(
			skills[i],
		)
	}

	return result
}
