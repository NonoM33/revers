import { prisma } from './db';
import bcrypt from 'bcryptjs';
import crypto from 'crypto';

const SESSION_DURATION = 7 * 24 * 60 * 60 * 1000; // 7 days

// Simple session storage (in production, use Redis or similar)
const sessions = new Map<string, { userId: string; expiresAt: Date }>();

export function generateSessionToken(): string {
  return crypto.randomBytes(32).toString('hex');
}

export async function createSession(userId: string): Promise<string> {
  const token = generateSessionToken();
  const expiresAt = new Date(Date.now() + SESSION_DURATION);

  sessions.set(token, { userId, expiresAt });

  return token;
}

export async function validateSession(token: string) {
  const session = sessions.get(token);

  if (!session) {
    return null;
  }

  if (session.expiresAt < new Date()) {
    sessions.delete(token);
    return null;
  }

  const user = await prisma.user.findUnique({
    where: { id: session.userId },
    select: {
      id: true,
      email: true,
      username: true,
      totalXp: true,
      level: true,
      currentStreak: true,
      longestStreak: true,
      lastActiveAt: true,
    },
  });

  return user;
}

export function deleteSession(token: string): void {
  sessions.delete(token);
}

export async function authenticateUser(email: string, password: string) {
  const user = await prisma.user.findUnique({
    where: { email },
  });

  if (!user) {
    return null;
  }

  const isValid = await bcrypt.compare(password, user.passwordHash);

  if (!isValid) {
    return null;
  }

  return user;
}

export async function getUserStats(userId: string) {
  const user = await prisma.user.findUnique({
    where: { id: userId },
    include: {
      completedExercises: {
        include: {
          exercise: {
            include: {
              module: true,
            },
          },
        },
      },
      unlockedBadges: {
        include: {
          badge: true,
        },
      },
    },
  });

  if (!user) return null;

  const completedCount = user.completedExercises.length;
  const badgeCount = user.unlockedBadges.length;

  // Calculate completed modules
  const moduleProgress = new Map<string, number>();
  for (const completion of user.completedExercises) {
    const moduleId = completion.exercise.moduleId;
    moduleProgress.set(moduleId, (moduleProgress.get(moduleId) || 0) + 1);
  }

  return {
    ...user,
    completedCount,
    badgeCount,
    moduleProgress: Object.fromEntries(moduleProgress),
  };
}
