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
		"GET /skills",
		handlers.GetSkillsHandler,
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
		"PUT /skills/{id}",
		handlers.UpdateSkillHandler,
	)

	http.HandleFunc(
		"POST /skills/{id}/daily",
		handlers.DailyProgressHandler,
	)

	http.HandleFunc(
		"DELETE /skills/{id}",
		handlers.DeleteSkillHandler,
	)

	http.HandleFunc(
		"PUT /skills/{id}/parent",
		handlers.UpdateSkillParentHandler,
	)

	http.HandleFunc(
		"PUT /skills/{id}/position",
		handlers.UpdateSkillPositionHandler,
	)
}
