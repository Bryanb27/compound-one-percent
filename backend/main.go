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

	fmt.Println("Starting backend...")

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

	fmt.Println("Backend ready.")

	C.bridge_database_close(database)

	fmt.Println("Listening on :8087")

	err := http.ListenAndServe(
		":8087",
		nil,
	)

	if err != nil {
		panic(err)
	}
}
