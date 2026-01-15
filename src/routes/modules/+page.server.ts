import type { PageServerLoad } from './$types';
import { prisma } from '$lib/server/db';

export const load: PageServerLoad = async ({ locals }) => {
  const modules = await prisma.module.findMany({
    include: {
      exercises: {
        select: {
          id: true,
        },
      },
    },
    orderBy: {
      orderIndex: 'asc',
    },
  });

  // Get user's completed exercises if logged in
  let completedExerciseIds = new Set<string>();
  let userXp = 0;

  if (locals.user) {
    const completions = await prisma.completedExercise.findMany({
      where: { userId: locals.user.id },
      select: { exerciseId: true },
    });
    completedExerciseIds = new Set(completions.map((c) => c.exerciseId));
    userXp = locals.user.totalXp;
  }

  // Calculate progress per module
  const modulesWithProgress = modules.map((mod) => {
    const totalInModule = mod.exercises.length;
    const completedInModule = mod.exercises.filter((e) => completedExerciseIds.has(e.id)).length;

    return {
      id: mod.id,
      slug: mod.slug,
      title: mod.title,
      description: mod.description,
      category: mod.category,
      difficulty: mod.difficulty,
      requiredXp: mod.requiredXp,
      xpReward: mod.xpReward,
      total: totalInModule,
      completed: completedInModule,
      progress: totalInModule > 0 ? Math.round((completedInModule / totalInModule) * 100) : 0,
      unlocked: userXp >= mod.requiredXp,
    };
  });

  return {
    modules: modulesWithProgress,
    userXp,
  };
};
