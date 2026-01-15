import type { PageServerLoad, Actions } from './$types';
import { error, fail, redirect } from '@sveltejs/kit';
import { prisma } from '$lib/server/db';
import crypto from 'crypto';

export const load: PageServerLoad = async ({ params, locals }) => {
  const exercise = await prisma.exercise.findUnique({
    where: { slug: params.slug },
    include: {
      module: true,
    },
  });

  if (!exercise) {
    throw error(404, 'Exercise not found');
  }

  // Check if module is unlocked
  const userXp = locals.user?.totalXp || 0;
  const isUnlocked = userXp >= exercise.module.requiredXp;

  // Check if already completed
  let isCompleted = false;
  let hintsRevealed = 0;

  if (locals.user) {
    const completion = await prisma.completedExercise.findUnique({
      where: {
        userId_exerciseId: {
          userId: locals.user.id,
          exerciseId: exercise.id,
        },
      },
    });
    isCompleted = !!completion;
    hintsRevealed = completion?.hintsUsed || 0;
  }

  return {
    exercise: {
      ...exercise,
      hints: JSON.parse(exercise.hints) as string[],
    },
    isUnlocked,
    isCompleted,
    hintsRevealed,
  };
};

export const actions: Actions = {
  submitFlag: async ({ request, params, locals }) => {
    if (!locals.user) {
      throw redirect(303, '/login');
    }

    const formData = await request.formData();
    const flag = (formData.get('flag') as string)?.trim();

    if (!flag) {
      return fail(400, { error: 'Le flag est requis' });
    }

    const exercise = await prisma.exercise.findUnique({
      where: { slug: params.slug },
      include: { module: true },
    });

    if (!exercise) {
      return fail(404, { error: 'Exercice non trouve' });
    }

    // Check if already completed
    const existingCompletion = await prisma.completedExercise.findUnique({
      where: {
        userId_exerciseId: {
          userId: locals.user.id,
          exerciseId: exercise.id,
        },
      },
    });

    if (existingCompletion) {
      return fail(400, { error: 'Tu as deja complete cet exercice!' });
    }

    // Hash the submitted flag and compare
    const flagHash = crypto.createHash('sha256').update(flag).digest('hex');
    const isCorrect = flagHash === exercise.expectedFlag;

    // Record submission
    await prisma.submission.create({
      data: {
        userId: locals.user.id,
        exerciseId: exercise.id,
        flag,
        isCorrect,
      },
    });

    // Update exercise stats
    await prisma.exercise.update({
      where: { id: exercise.id },
      data: {
        totalAttempts: { increment: 1 },
        totalSolves: isCorrect ? { increment: 1 } : undefined,
      },
    });

    if (!isCorrect) {
      return fail(400, { error: 'Flag incorrect. Reessaie!' });
    }

    // Calculate XP (with potential bonuses)
    let xpEarned = exercise.xpReward;

    // Check if first blood
    const previousSolves = await prisma.completedExercise.count({
      where: { exerciseId: exercise.id },
    });
    const isFirstBlood = previousSolves === 0;
    if (isFirstBlood) {
      xpEarned += 100; // First blood bonus
    }

    // Create completion
    await prisma.completedExercise.create({
      data: {
        userId: locals.user.id,
        exerciseId: exercise.id,
        xpEarned,
        isFirstBlood,
      },
    });

    // Update user XP
    await prisma.user.update({
      where: { id: locals.user.id },
      data: {
        totalXp: { increment: xpEarned },
        lastActiveAt: new Date(),
      },
    });

    return {
      success: true,
      xpEarned,
      isFirstBlood,
    };
  },
};
