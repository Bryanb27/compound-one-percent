package routes

import (
	"net/http"

	"compound/backend/handlers"
)

func RegisterRoutes() {
	http.HandleFunc(
		"/health",
		handlers.HealthHandler,
	)

	http.HandleFunc(
		"/skills",
		handlers.GetSkillsHandler,
	)

	http.HandleFunc(
		"/skills/create",
		handlers.CreateSkillHandler,
	)
}
