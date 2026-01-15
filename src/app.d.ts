// See https://svelte.dev/docs/kit/types#app.d.ts
// for information about these interfaces
declare global {
  namespace App {
    // interface Error {}
    interface Locals {
      user: {
        id: string;
        email: string;
        username: string;
        totalXp: number;
        level: number;
        currentStreak: number;
        longestStreak: number;
        lastActiveAt: Date | null;
      } | null;
    }
    // interface PageData {}
    // interface PageState {}
    // interface Platform {}
  }
}

export {};
