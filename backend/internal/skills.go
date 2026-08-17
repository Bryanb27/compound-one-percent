package internal

/*
#cgo CFLAGS: -I../../engine/include
#cgo LDFLAGS: -L../../engine/build -lcompound -lsqlite3

#include "bridge.h"
*/
import "C"

import "fmt"

type Skill struct {
	ID       uint64  `json:"id"`
	Name     string  `json:"name"`
	Progress float32 `json:"progress"`
}

func GetSkills() []Skill {

	return []Skill{
		{
			ID:       1,
			Name:     "Backend",
			Progress: 71,
		},
		{
			ID:       2,
			Name:     "C#",
			Progress: 80,
		},
		{
			ID:       3,
			Name:     "SQL",
			Progress: 50,
		},
	}
}

func GetSkill(id uint64) *Skill {

	skills := GetSkills()

	for _, skill := range skills {

		if skill.ID == id {
			return &skill
		}
	}

	return nil
}

func GetSkillss() {

	fmt.Println("GetSkills called from Go")

	database := C.bridge_database_open(
		C.CString("../database/skills.db"),
	)

	if database == nil {
		fmt.Println("Database failed")
		return
	}

	C.bridge_test_database(database)

	C.bridge_database_close(database)
}
