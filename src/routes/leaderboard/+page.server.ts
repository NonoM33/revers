import type { PageServerLoad } from './$types';
import { prisma } from '$lib/server/db';

export const load: PageServerLoad = async ({ locals }) => {
  // Get top users by XP
  const users = await prisma.user.findMany({
    orderBy: {
      totalXp: 'desc',
    },
    take: 50,
    select: {
      id: true,
      username: true,
      totalXp: true,
      level: true,
      currentStreak: true,
      _count: {
        select: {
          completedExercises: true,
          unlockedBadges: true,
        },
      },
    },
  });

  // Calculate ranks
  const leaderboard = users.map((user, index) => ({
    rank: index + 1,
    id: user.id,
    username: user.username,
    totalXp: user.totalXp,
    level: user.level,
    currentStreak: user.currentStreak,
    exercisesCompleted: user._count.completedExercises,
    badgeCount: user._count.unlockedBadges,
    isCurrentUser: locals.user?.id === user.id,
  }));

  // Find current user's rank if not in top 50
  let currentUserRank = null;
  if (locals.user && !leaderboard.find((u) => u.isCurrentUser)) {
    const usersAbove = await prisma.user.count({
      where: {
        totalXp: {
          gt: locals.user.totalXp,
        },
      },
    });
    currentUserRank = usersAbove + 1;
  }

  return {
    leaderboard,
    currentUserRank,
    totalUsers: await prisma.user.count(),
  };
};
