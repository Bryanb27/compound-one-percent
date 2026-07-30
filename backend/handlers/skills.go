package handlers

import (
	"encoding/json"
	"net/http"
)

type SkillResponse struct {
	ID       uint64  `json:"id"`
	Name     string  `json:"name"`
	Progress float32 `json:"progress"`
}

type CreateSkillRequest struct {
	Name        string  `json:"name"`
	Description string  `json:"description"`
	Weight      float32 `json:"weight"`
}

func GetSkillsHandler(
	w http.ResponseWriter,
	r *http.Request,
) {
	skills := []SkillResponse{
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

	w.Header().Set(
		"Content-Type",
		"application/json",
	)

	json.NewEncoder(w).Encode(skills)
}

func CreateSkillHandler(
	w http.ResponseWriter,
	r *http.Request,
) {
	if r.Method != http.MethodPost {
		http.Error(
			w,
			"Method not allowed",
			http.StatusMethodNotAllowed,
		)
		return
	}

	var request CreateSkillRequest

	err := json.NewDecoder(
		r.Body,
	).Decode(&request)

	if err != nil {
		http.Error(
			w,
			"Invalid JSON",
			http.StatusBadRequest,
		)
		return
	}

	w.Header().Set(
		"Content-Type",
		"application/json",
	)

	json.NewEncoder(w).Encode(request)
}
