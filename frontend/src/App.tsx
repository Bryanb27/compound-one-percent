import { useEffect, useState } from "react";
import SkillTree from "./components/SkillTree";
import CreateSkillForm from "./components/CreateSkillForm";
import {
  DndContext,
  type DragEndEvent,
} from "@dnd-kit/core";

type Skill = {
  id: number;
  name: string;
  description: string;
  progress: number;
  children: Skill[] | null;
};

function App() {
  const [skills, setSkills] = useState<Skill[]>([]);

  function loadSkills() {
    fetch("http://localhost:8087/skills")
      .then((response) => {
        if (!response.ok) {
          throw new Error("Failed to load skills");
        }

        return response.json();
      })
      .then((data) => {
        setSkills(data);
      })
      .catch(console.error);
  }

  useEffect(() => {
    loadSkills();
  }, []);

  function handleDragEnd(event: DragEndEvent) {
  const {
    active,
    over,
  } = event;

  if (!over) {
    return;
  }

  const draggedId = Number(active.id);

  const parentId = Number(
    String(over.id).replace("drop-", "")
  );

  if (draggedId === parentId) {
    return;
  }

  fetch(
    `http://localhost:8087/skills/${draggedId}/parent`,
    {
      method: "PUT",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        parentId,
      }),
    }
  )
    .then((response) => {
      if (!response.ok) {
        throw new Error(
          "Failed to update parent"
        );
      }

      loadSkills();
    })
    .catch(console.error);
}

  return (
    <main className="min-h-screen bg-slate-950 px-8 py-10">

      {/* HEADER */}

      <header className="mx-auto mb-10 max-w-7xl">

        <h1 className="text-4xl font-bold text-white">
          Compound One Percent
        </h1>

        <p className="mt-2 text-slate-400">
          Improve your skills by 1% every day.
        </p>

      </header>

      {/*  Create Skill */}
      <CreateSkillForm
        skills={skills}
        onCreated={loadSkills}
      />

      <section
        className="
        mx-auto
        flex
        max-w-7xl
        flex-wrap
        items-start
        gap-6
      "
      ></section>

      {/* SKILLS */}

      <DndContext
        onDragEnd={handleDragEnd}
      >
        <section
          className="
            mx-auto
            flex
            max-w-7xl
            flex-wrap
            items-start
            gap-6
          "
        >
          {skills.map((skill) => (
            <SkillTree
              key={skill.id}
              skill={skill}
              onUpdated={loadSkills}
            />
          ))}
        </section>
      </DndContext>

    </main>
  );
}

export default App;