// XP and Level calculations

// XP required for a specific level (not cumulative)
export function xpForLevel(level: number): number {
  return Math.floor(100 * Math.pow(level, 1.8));
}

// Total XP needed to reach a level (cumulative)
export function totalXpForLevel(level: number): number {
  let total = 0;
  for (let i = 1; i <= level; i++) {
    total += xpForLevel(i);
  }
  return total;
}

// Calculate level from total XP
export function levelFromXp(totalXp: number): number {
  let level = 1;
  let accumulatedXp = 0;

  while (accumulatedXp + xpForLevel(level + 1) <= totalXp) {
    accumulatedXp += xpForLevel(level + 1);
    level++;
  }

  return level;
}

// Get progress to next level (0-100)
export function progressToNextLevel(totalXp: number): number {
  const currentLevel = levelFromXp(totalXp);
  const xpForCurrentLevel = totalXpForLevel(currentLevel);
  const xpForNextLevel = totalXpForLevel(currentLevel + 1);
  const xpInCurrentLevel = totalXp - xpForCurrentLevel;
  const xpNeededForNext = xpForNextLevel - xpForCurrentLevel;

  return Math.floor((xpInCurrentLevel / xpNeededForNext) * 100);
}

// Level titles
export const LEVEL_TITLES: Record<number, string> = {
  1: 'Script Kiddie',
  5: 'Padawan du Binaire',
  10: 'Apprenti Reverseur',
  15: 'Reverseur Confirme',
  20: 'Analyste Binaire',
  25: 'Chasseur de Bugs',
  30: 'Expert Reverse',
  40: 'Maitre du Desassemblage',
  50: 'Legende du RE',
  60: 'Architecte de l\'Ombre',
  75: 'Neo du Binaire',
  100: 'Transcendance',
};

export function getTitleForLevel(level: number): string {
  const levels = Object.keys(LEVEL_TITLES)
    .map(Number)
    .sort((a, b) => b - a);

  for (const l of levels) {
    if (level >= l) {
      return LEVEL_TITLES[l];
    }
  }

  return LEVEL_TITLES[1];
}

// Difficulty stars helper
export function getDifficultyStars(difficulty: number, max: number = 5): string {
  const filled = Math.min(difficulty, max);
  const empty = max - filled;
  return '★'.repeat(filled) + '☆'.repeat(empty);
}

// Rarity colors
export const RARITY_COLORS: Record<string, string> = {
  COMMON: 'text-gray-400',
  UNCOMMON: 'text-green-400',
  RARE: 'text-blue-400',
  EPIC: 'text-purple-400',
  LEGENDARY: 'text-yellow-400',
};

export const RARITY_BG_COLORS: Record<string, string> = {
  COMMON: 'bg-gray-400/10 border-gray-400',
  UNCOMMON: 'bg-green-400/10 border-green-400',
  RARE: 'bg-blue-400/10 border-blue-400',
  EPIC: 'bg-purple-400/10 border-purple-400',
  LEGENDARY: 'bg-yellow-400/10 border-yellow-400',
};
