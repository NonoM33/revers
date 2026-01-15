import type { PageServerLoad } from './$types';
import { redirect } from '@sveltejs/kit';
import { prisma } from '$lib/server/db';

export const load: PageServerLoad = async ({ locals }) => {
  if (!locals.user) {
    throw redirect(303, '/login');
  }

  // Get user stats with completions
  const user = await prisma.user.findUnique({
    where: { id: locals.user.id },
    include: {
      completedExercises: {
        include: {
          exercise: {
            include: {
              module: true,
            },
          },
        },
        orderBy: {
          completedAt: 'desc',
        },
        take: 5,
      },
      unlockedBadges: {
        include: {
          badge: true,
        },
        orderBy: {
          unlockedAt: 'desc',
        },
      },
    },
  });

  // Get total exercise count
  const totalExercises = await prisma.exercise.count();

  // Get module progress
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

  // Calculate progress per module
  const completedExerciseIds = new Set(
    user?.completedExercises.map((c) => c.exerciseId) || []
  );

  const moduleProgress = modules.map((mod) => {
    const totalInModule = mod.exercises.length;
    const completedInModule = mod.exercises.filter((e) => completedExerciseIds.has(e.id)).length;

    return {
      id: mod.id,
      slug: mod.slug,
      title: mod.title,
      difficulty: mod.difficulty,
      total: totalInModule,
      completed: completedInModule,
      progress: totalInModule > 0 ? Math.round((completedInModule / totalInModule) * 100) : 0,
      unlocked: user ? user.totalXp >= mod.requiredXp : false,
    };
  });

  return {
    user,
    totalExercises,
    completedCount: user?.completedExercises.length || 0,
    moduleProgress,
    recentCompletions: user?.completedExercises || [],
    badges: user?.unlockedBadges || [],
  };
};
