import { useState } from "react";

type Skill = {
  id: number;
  name: string;
  children: Skill[] | null;
};

type Props = {
  skills: Skill[];
  onCreated: () => void;
};

function CreateSkillForm({
  skills,
  onCreated,
}: Props) {
  const [name, setName] = useState("");
  const [description, setDescription] = useState("");
  const [weight, setWeight] = useState(100);
  const [progress, setProgress] = useState(0);
  const [parentId, setParentId] = useState(0);

  function createSkill() {
    if (!name.trim()) {
      return;
    }

    fetch("http://localhost:8087/skills/create", {
      method: "POST",

      headers: {
        "Content-Type": "application/json",
      },

      body: JSON.stringify({
        name,
        description,
        weight,
        progress,
        parentId,
      }),
    })
      .then((response) => {
        if (!response.ok) {
          throw new Error(
            "Failed to create skill"
          );
        }

        return response.json();
      })
      .then(() => {
        setName("");
        setDescription("");
        setWeight(100);
        setProgress(0);
        setParentId(0);

        onCreated();
      })
      .catch(console.error);
  }

  return (
    <section
      className="
        mx-auto
        mb-10
        max-w-7xl
      "
    >
      <div
        className="
          rounded-2xl
          border
          border-slate-700
          bg-slate-900
          p-6
          shadow-lg
        "
      >

        <h2 className="mb-5 text-xl font-bold text-white">
          Add Skill
        </h2>

        <div
          className="
            grid
            gap-4
            md:grid-cols-2
            lg:grid-cols-5
          "
        >

          {/* NAME */}

          <input
            type="text"
            placeholder="Skill name"
            value={name}
            onChange={(event) =>
              setName(event.target.value)
            }
            className="
              rounded-lg
              border
              border-slate-700
              bg-slate-800
              px-4
              py-2
              text-white
              outline-none
              placeholder:text-slate-500
              focus:border-blue-500
            "
          />

          {/* DESCRIPTION */}

          <input
            type="text"
            placeholder="Description"
            value={description}
            onChange={(event) =>
              setDescription(event.target.value)
            }
            className="
              rounded-lg
              border
              border-slate-700
              bg-slate-800
              px-4
              py-2
              text-white
              outline-none
              placeholder:text-slate-500
              focus:border-blue-500
            "
          />

          {/* WEIGHT */}

          <input
            type="number"
            min="1"
            placeholder="Weight"
            value={weight}
            onChange={(event) =>
              setWeight(
                Number(event.target.value)
              )
            }
            className="
              rounded-lg
              border
              border-slate-700
              bg-slate-800
              px-4
              py-2
              text-white
              outline-none
              focus:border-blue-500
            "
          />

          {/* PROGRESS */}

          <input
            type="number"
            min="0"
            max="100"
            placeholder="Progress"
            value={progress}
            onChange={(event) =>
              setProgress(
                Number(event.target.value)
              )
            }
            className="
              rounded-lg
              border
              border-slate-700
              bg-slate-800
              px-4
              py-2
              text-white
              outline-none
              focus:border-blue-500
            "
          />

          {/* PARENT */}

          <select
            value={parentId}
            onChange={(event) =>
              setParentId(
                Number(event.target.value)
              )
            }
            className="
              rounded-lg
              border
              border-slate-700
              bg-slate-800
              px-4
              py-2
              text-white
              outline-none
              focus:border-blue-500
            "
          >
            <option value={0}>
              No parent
            </option>

            {skills.map((skill) => (
              <option
                key={skill.id}
                value={skill.id}
              >
                {skill.name}
              </option>
            ))}
          </select>

        </div>

        {/* BUTTON */}

        <button
          onClick={createSkill}
          className="
            mt-5
            rounded-lg
            bg-blue-600
            px-5
            py-2
            font-semibold
            text-white
            transition
            hover:bg-blue-500
          "
        >
          Add Skill
        </button>

      </div>
    </section>
  );
}

export default CreateSkillForm;