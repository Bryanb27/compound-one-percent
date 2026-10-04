package handlers

import (
	"C"
	"compound/backend/engine"
	"compound/backend/internal"
	"encoding/json"
	"fmt"
	"net/http"
)
import "strconv"

type SkillResponse struct {
	ID       uint64  `json:"id"`
	Name     string  `json:"name"`
	Progress float32 `json:"progress"`
}

type CreateSkillRequest struct {
	Name        string  `json:"name"`
	Description string  `json:"description"`
	Weight      float32 `json:"weight"`
	Progress    float32 `json:"progress"`
	ParentID    uint64  `json:"parentId"`
}

type UpdateProgressRequest struct {
	Progress float32 `json:"progress"`
}

type UpdateSkillRequest struct {
	Name        string  `json:"name"`
	Description string  `json:"description"`
	Weight      float32 `json:"weight"`
}

type UpdateParentRequest struct {
	ParentID int64 `json:"parentId"`
}

type UpdatePositionRequest struct {
	Position int `json:"position"`
}

func GetSkillHandler(
	w http.ResponseWriter,
	r *http.Request,
) {
	idString := r.PathValue("id")

	var id uint64

	_, err := fmt.Sscanf(
		idString,
		"%d",
		&id,
	)

	if err != nil {
		http.Error(
			w,
			"Invalid skill ID",
			http.StatusBadRequest,
		)

		return
	}

	skill := internal.GetSkill(id)

	if skill == nil {
		http.Error(
			w,
			"Skill not found",
			http.StatusNotFound,
		)

		return
	}

	w.Header().Set(
		"Content-Type",
		"application/json",
	)

	json.NewEncoder(w).Encode(skill)
}

func GetSkillsHandler(
	w http.ResponseWriter,
	r *http.Request,
) {
	skills := engine.GetAllSkills()

	if skills == nil {
		http.Error(
			w,
			"Failed to load skills",
			http.StatusInternalServerError,
		)

		return
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

	created := engine.CreateSkill(
		request.Name,
		request.Description,
		request.Weight,
		request.Progress,
		request.ParentID,
	)

	if !created {
		http.Error(
			w,
			"Failed to create skill",
			http.StatusInternalServerError,
		)
		return
	}

	w.Header().Set(
		"Content-Type",
		"application/json",
	)

	json.NewEncoder(w).Encode(request)
}

func UpdateProgressHandler(
	w http.ResponseWriter,
	r *http.Request,
) {
	idString := r.PathValue("id")

	var id uint64

	_, err := fmt.Sscanf(
		idString,
		"%d",
		&id,
	)

	if err != nil {
		http.Error(
			w,
			"Invalid skill ID",
			http.StatusBadRequest,
		)
		return
	}

	var request UpdateProgressRequest

	err = json.NewDecoder(
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

	if request.Progress < 0 ||
		request.Progress > 100 {
		http.Error(
			w,
			"Progress must be between 0 and 100",
			http.StatusBadRequest,
		)
		return
	}

	if !engine.UpdateProgress(
		id,
		request.Progress,
	) {
		http.Error(
			w,
			"Failed to update progress",
			http.StatusInternalServerError,
		)
		return
	}

	w.WriteHeader(
		http.StatusNoContent,
	)
}

func UpdateSkillHandler(
	w http.ResponseWriter,
	r *http.Request,
) {
	id, err := strconv.ParseUint(
		r.PathValue("id"),
		10,
		64,
	)

	if err != nil {
		http.Error(
			w,
			"Invalid skill ID",
			http.StatusBadRequest,
		)
		return
	}

	var request UpdateSkillRequest

	err = json.NewDecoder(
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

	success := engine.UpdateSkill(
		id,
		request.Name,
		request.Description,
		request.Weight,
	)

	if !success {
		http.Error(
			w,
			"Failed to update skill",
			http.StatusInternalServerError,
		)
		return
	}

	w.Header().Set(
		"Content-Type",
		"application/json",
	)

	json.NewEncoder(w).Encode(request)
}

func DeleteSkillHandler(
	w http.ResponseWriter,
	r *http.Request,
) {
	idString := r.PathValue("id")

	var id uint64

	_, err := fmt.Sscanf(
		idString,
		"%d",
		&id,
	)

	if err != nil {
		http.Error(
			w,
			"Invalid skill ID",
			http.StatusBadRequest,
		)
		return
	}

	if !engine.DeleteSkill(id) {
		http.Error(
			w,
			"Failed to delete skill",
			http.StatusInternalServerError,
		)
		return
	}

	w.WriteHeader(
		http.StatusNoContent,
	)
}

func DailyProgressHandler(
	w http.ResponseWriter,
	r *http.Request,
) {
	idString := r.PathValue("id")

	id, err := strconv.ParseUint(
		idString,
		10,
		64,
	)

	if err != nil {
		http.Error(
			w,
			"Invalid skill ID",
			http.StatusBadRequest,
		)
		return
	}

	success := engine.AddDailyProgress(id)

	if !success {
		http.Error(
			w,
			"Already practiced today or failed",
			http.StatusConflict,
		)
		return
	}

	w.Header().Set(
		"Content-Type",
		"application/json",
	)

	json.NewEncoder(w).Encode(
		map[string]interface{}{
			"success": true,
		},
	)
}

func UpdateSkillParentHandler(
	w http.ResponseWriter,
	r *http.Request,
) {
	idString := r.PathValue("id")

	id, err := strconv.ParseUint(
		idString,
		10,
		64,
	)

	if err != nil {
		http.Error(
			w,
			"Invalid skill ID",
			http.StatusBadRequest,
		)
		return
	}

	var request UpdateParentRequest

	err = json.NewDecoder(
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

	if !engine.UpdateSkillParent(
		id,
		request.ParentID,
	) {
		http.Error(
			w,
			"Failed to update parent",
			http.StatusInternalServerError,
		)
		return
	}

	w.WriteHeader(http.StatusNoContent)
}

func UpdateSkillPositionHandler(
	w http.ResponseWriter,
	r *http.Request,
) {
	idString := r.PathValue("id")

	id, err := strconv.ParseUint(
		idString,
		10,
		64,
	)

	if err != nil {
		http.Error(
			w,
			"Invalid skill ID",
			http.StatusBadRequest,
		)
		return
	}

	var request UpdatePositionRequest

	err = json.NewDecoder(
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

	if !engine.UpdateSkillPosition(
		id,
		request.Position,
	) {
		http.Error(
			w,
			"Failed to update position",
			http.StatusInternalServerError,
		)
		return
	}

	w.WriteHeader(http.StatusNoContent)
}
