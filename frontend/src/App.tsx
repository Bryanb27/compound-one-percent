import { useEffect, useState } from "react";

type Skill = {
  id: number;
  name: string;
  description: string;
  progress: number;
  children: Skill[] | null;
};

function SkillItem({
  skill,
  onUpdated,
}: {
  skill: Skill;
  onUpdated: () => void;
}) {
  const [progress, setProgress] = useState(skill.progress);

  function updateProgress(value: number) {
    setProgress(value);

    fetch(
      `http://localhost:8087/skills/${skill.id}/progress`,
      {
        method: "PUT",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({
          progress: value,
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
    <div className="skill-card">

      <div className="skill-header">
        <h2>{skill.name}</h2>

        <button onClick={deleteSkill}>
          Delete
        </button>
      </div>

      {skill.description && (
        <p className="description">
          {skill.description}
        </p>
      )}

      <div className="progress-info">
        <span>Progress</span>

        <span>{progress}%</span>
      </div>

      <div className="progress-bar">
        <div
          className="progress-fill"
          style={{
            width: `${progress}%`,
          }}
        />
      </div>

      <input
        type="range"
        min="0"
        max="100"
        value={progress}
        onChange={(event) =>
          updateProgress(
            Number(event.target.value)
          )
        }
      />

      {skill.children?.map((child) => (
        <SkillItem
          key={child.id}
          skill={child}
          onUpdated={onUpdated}
        />
      ))}

    </div>
  );
}

function CreateSkillForm({
  onCreated,
}: {
  onCreated: () => void;
}) {
  const [name, setName] = useState("");
  const [description, setDescription] = useState("");
  const [weight, setWeight] = useState(100);
  const [progress, setProgress] = useState(0);

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
      }),
    })
      .then((response) => {
        if (!response.ok) {
          throw new Error("Failed to create skill");
        }

        return response.json();
      })
      .then(() => {
        setName("");
        setDescription("");
        setWeight(100);
        setProgress(0);

        onCreated();
      })
      .catch(console.error);
  }

  return (
    <section className="create-form">

      <h2>Add Skill</h2>

      <input
        placeholder="Skill name"
        value={name}
        onChange={(event) =>
          setName(event.target.value)
        }
      />

      <input
        placeholder="Description"
        value={description}
        onChange={(event) =>
          setDescription(event.target.value)
        }
      />

      <input
        type="number"
        min="1"
        value={weight}
        onChange={(event) =>
          setWeight(
            Number(event.target.value)
          )
        }
      />

      <input
        type="number"
        min="0"
        max="100"
        value={progress}
        onChange={(event) =>
          setProgress(
            Number(event.target.value)
          )
        }
        placeholder="Progress"
      />

      <button onClick={createSkill}>
        Add Skill
      </button>

    </section>
  );
}

function App() {
  const [skills, setSkills] = useState<Skill[]>([]);

  function loadSkills() {
    fetch("http://localhost:8087/skills")
      .then((response) => response.json())
      .then((data) => {
        setSkills(data);
      })
      .catch(console.error);
  }

  useEffect(() => {
    loadSkills();
  }, []);

  return (
    <main className="container">

      <header>
        <h1>Compound One Percent</h1>

        <p>
          Improve your skills by 1% every day.
        </p>
      </header>

      <CreateSkillForm
        onCreated={loadSkills}
      />

      <section className="skills">

        {skills.map((skill) => (
          <SkillItem
            key={skill.id}
            skill={skill}
            onUpdated={loadSkills}
          />
        ))}

      </section>

    </main>
  );
}

export default App;