package main

/*
#cgo CFLAGS: -I../engine/include
#cgo LDFLAGS: -L../engine/build -lcompound -lsqlite3

#include "bridge.h"
*/
import "C"

import (
	"fmt"
	"net/http"

	"compound/backend/routes"
)

func main() {

	routes.RegisterRoutes()

	fmt.Println("Starting C engine test...")

	database := C.bridge_database_open(
		C.CString("../database/skills.db"),
	)

	if database == nil {
		fmt.Println("Database failed.")
		return
	}

	fmt.Println("Database opened.")

	if C.bridge_database_initialize(database) != 0 {
		fmt.Println("Database initialization failed.")

		C.bridge_database_close(database)

		return
	}

	fmt.Println("Database initialized.")

	root := C.bridge_load_tree(
		database,
	)

	/*if root == nil {
		fmt.Println("Failed to load tree.")

		C.bridge_database_close(database)

		return
	}*/

	fmt.Println("Tree loaded!")

	fmt.Printf(
		"Root: %s\n",
		C.GoString(
			C.bridge_skill_name(root),
		),
	)

	fmt.Printf(
		"Progress: %.2f%%\n",
		float32(
			C.bridge_skill_progress(root),
		),
	)

	fmt.Println("Tree:")

	C.bridge_print_tree(root)

	C.bridge_destroy_skill(root)

	C.bridge_database_close(database)

	fmt.Println("C engine test finished.")

	fmt.Println("Listening on :8087")

	err := http.ListenAndServe(
		":8087",
		nil,
	)

	if err != nil {
		panic(err)
	}
}
