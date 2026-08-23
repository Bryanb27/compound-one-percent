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

func enableCORS(
	next http.Handler,
) http.Handler {

	return http.HandlerFunc(
		func(w http.ResponseWriter, r *http.Request) {

			w.Header().Set(
				"Access-Control-Allow-Origin",
				"http://localhost:5173",
			)

			w.Header().Set(
				"Access-Control-Allow-Methods",
				"GET, POST, PUT, DELETE, OPTIONS",
			)

			w.Header().Set(
				"Access-Control-Allow-Headers",
				"Content-Type",
			)

			if r.Method == http.MethodOptions {
				w.WriteHeader(http.StatusNoContent)
				return
			}

			next.ServeHTTP(w, r)
		},
	)
}

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
		enableCORS(http.DefaultServeMux),
	)

	if err != nil {
		panic(err)
	}
}
