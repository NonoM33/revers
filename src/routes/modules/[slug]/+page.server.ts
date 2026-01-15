import type { PageServerLoad } from './$types';
import { error } from '@sveltejs/kit';
import { prisma } from '$lib/server/db';

export const load: PageServerLoad = async ({ params, locals }) => {
  const module = await prisma.module.findUnique({
    where: { slug: params.slug },
    include: {
      exercises: {
        orderBy: {
          orderIndex: 'asc',
        },
      },
    },
  });

  if (!module) {
    throw error(404, 'Module not found');
  }

  // Check if module is unlocked
  const userXp = locals.user?.totalXp || 0;
  const isUnlocked = userXp >= module.requiredXp;

  // Get user's completed exercises
  let completedExerciseIds = new Set<string>();
  if (locals.user) {
    const completions = await prisma.completedExercise.findMany({
      where: {
        userId: locals.user.id,
        exercise: {
          moduleId: module.id,
        },
      },
      select: { exerciseId: true },
    });
    completedExerciseIds = new Set(completions.map((c) => c.exerciseId));
  }

  // Add completion status to exercises
  const exercisesWithStatus = module.exercises.map((ex) => ({
    ...ex,
    completed: completedExerciseIds.has(ex.id),
    hints: JSON.parse(ex.hints) as string[],
  }));

  const completedCount = exercisesWithStatus.filter((e) => e.completed).length;
  const progress = Math.round((completedCount / exercisesWithStatus.length) * 100);

  return {
    module: {
      ...module,
      exercises: exercisesWithStatus,
    },
    isUnlocked,
    userXp,
    completedCount,
    progress,
  };
};
