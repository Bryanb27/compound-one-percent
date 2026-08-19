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
		"GET /skills/{id}",
		handlers.GetSkillHandler,
	)

	http.HandleFunc(
		"POST /skills/create",
		handlers.CreateSkillHandler,
	)

	http.HandleFunc(
		"PUT /skills/{id}/progress",
		handlers.UpdateProgressHandler,
	)

	http.HandleFunc(
		"DELETE /skills/{id}",
		handlers.DeleteSkillHandler,
	)
}
