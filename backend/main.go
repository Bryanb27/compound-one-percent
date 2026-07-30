package main

import (
	"fmt"
	"net/http"

	"compound/backend/routes"
)

func main() {

	routes.RegisterRoutes()

	fmt.Println(
		"Listening on :8087",
	)

	err := http.ListenAndServe(
		":8087",
		nil,
	)

	if err != nil {
		panic(err)
	}
}
