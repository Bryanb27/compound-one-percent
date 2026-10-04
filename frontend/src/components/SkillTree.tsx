import { useState, useEffect } from "react";
import {
  useDraggable,
  useDroppable,
} from "@dnd-kit/core";

type Skill = {
  id: number;
  name: string;
  description: string;
  progress: number;
  children: Skill[] | null;
};

type Props = {
  skill: Skill;
  onUpdated: () => void;
};

function SkillTree({ skill, onUpdated }: Props) {
    const {
      attributes,
      listeners,
      setNodeRef,
      transform,
    } = useDraggable({
      id: String(skill.id),
    });

    const {
      isOver,
      setNodeRef: setDropRef,
    } = useDroppable({
      id: `drop-${skill.id}`,
    });

  const style = transform
  ? {
      transform: `translate3d(
        ${transform.x}px,
        ${transform.y}px,
        0
      )`,
      zIndex: 1000,
    }
  : undefined;

  const [expanded, setExpanded] = useState(false);
  const [progress, setProgress] = useState(skill.progress);

  useEffect(() => {
    setProgress(skill.progress);
  }, [skill.progress]);

  const hasChildren =
    skill.children !== null &&
    skill.children.length > 0;

  function updateProgress(value: number) {
    setProgress(value);
  }

  function saveProgress() {
    fetch(
      `http://localhost:8087/skills/${skill.id}/progress`,
      {
        method: "PUT",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          progress,
        }),
      }
    )
      .then((response) => {
        if (!response.ok) {
          throw new Error("Failed to update progress");
        }

        onUpdated();
      })
      .catch(console.error);
  }

  function addDailyProgress() {
    fetch(
      `http://localhost:8087/skills/${skill.id}/daily`,
      {
        method: "POST",
      }
    )
      .then((response) => {
        if (response.status === 409) {
          alert(
            "You've already improved this skill today."
          );

          return null;
        }

        if (!response.ok) {
          throw new Error(
            "Failed to add daily progress"
          );
        }

        return response.json();
      })
      .then((data) => {
        if (!data) {
          return;
        }

        setProgress((current) =>
          Math.min(current + 1, 100)
        );

        onUpdated();
      })
      .catch(console.error);
  }

  function deleteSkill() {
    fetch(
      `http://localhost:8087/skills/${skill.id}`,
      {
        method: "DELETE",
      }
    )
      .then((response) => {
        if (!response.ok) {
          throw new Error("Failed to delete skill");
        }

        onUpdated();
      })
      .catch(console.error);
  }

  return (
      <div
        ref={setDropRef}
        className={`
          w-80
          flex-shrink-0
          rounded-2xl
          transition
          ${
            isOver
              ? "ring-4 ring-blue-400"
              : ""
          }
        `}
      >
        <div
          ref={setNodeRef}
          style={style}
          {...attributes}
          {...listeners}
          className="
            touch-none
            cursor-grab
            active:cursor-grabbing
          "
        >

          {/* CARD */}

          <div
            className="
              w-80
              rounded-2xl
              border
              p-5
              shadow-lg
              transition
              duration-200
              bg-slate-800
            "
          >

        {/* HEADER */}

        <div className="flex items-center justify-between">

          <h2 className="text-xl font-bold text-white">
            {skill.name}
          </h2>

          <button
            onPointerDown={(event) => {
              event.stopPropagation();
            }}
            onClick={deleteSkill}
            className="
              flex
              h-7
              w-7
              items-center
              justify-center
              rounded-full
              bg-slate-700
              text-slate-300
              transition
              hover:bg-red-500
              hover:text-white
            "
          >
            ×
          </button>

        </div>

        {/* DESCRIPTION */}

        {skill.description && (
          <p className="mt-2 text-sm text-slate-400">
            {skill.description}
          </p>
        )}

        {/* PROGRESS */}

        <div className="mt-5">

          <div className="mb-2 flex justify-between text-sm">

            <span className="text-slate-400">
              Progress
            </span>

            <span className="font-semibold text-white">
              {Math.round(progress)}%
            </span>

          </div>

          {/* BAR */}

          <div className="h-2 w-full overflow-hidden rounded-full bg-slate-700">

            <div
              className="
                h-full
                rounded-full
                bg-blue-500
                transition-all
                duration-200
              "
              style={{
                width: `${progress}%`,
              }}
            />

          </div>

          {/* LEAF CONTROLS */}

          {!hasChildren && (
            <>

              <input
                type="range"
                min="0"
                max="100"
                value={progress}
                onPointerDown={(event) => {
                  event.stopPropagation();
                }}
                onChange={(event) =>
                  updateProgress(
                    Number(event.target.value)
                  )
                }
                onMouseUp={saveProgress}
                onTouchEnd={saveProgress}
                className="
                  mt-4
                  w-full
                  cursor-pointer
                  accent-blue-500
                "
              />

              <button
                onPointerDown={(event) => {
                  event.stopPropagation();
                }}
                onClick={addDailyProgress}
                className="
                  mt-4
                  w-full
                  rounded-lg
                  bg-blue-600
                  px-4
                  py-2
                  font-semibold
                  text-white
                  transition
                  hover:bg-blue-500
                "
              >
                +1% Today
              </button>

            </>
          )}

        </div>

        {/* EXPAND */}

        {hasChildren && (
          <button
            onPointerDown={(event) => {
              event.stopPropagation();
            }}
            onClick={() => {
              setExpanded(!expanded);
            }}
            className="
              mx-auto
              mt-5
              flex
              h-9
              w-9
              items-center
              justify-center
              rounded-full
              bg-slate-700
              text-xl
              text-white
              transition
              hover:bg-slate-600
            "
          >
            {expanded ? "−" : "+"}
          </button>
        )}

      </div>

      {/* CHILDREN */}

      {hasChildren && expanded && (
        <div
          className="
            ml-6
            mt-5
            flex
            flex-wrap
            gap-5
            border-l-2
            border-slate-700
            pl-6
          "
        >

          {skill.children!.map((child) => (
            <SkillTree
              key={child.id}
              skill={child}
              onUpdated={onUpdated}
            />
          ))}

        </div>
      )}
      </div>
    </div>
  );
}

export default SkillTree;