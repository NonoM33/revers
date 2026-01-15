<script lang="ts">
  import { getTitleForLevel, progressToNextLevel, totalXpForLevel, RARITY_BG_COLORS } from '$lib/utils/xp';
  import { BookOpen, Clock, Award, BarChart3, Zap, Check, Lock, ArrowRight } from 'lucide-svelte';

  let { data } = $props();

  const user = data.user!;
  const progress = progressToNextLevel(user.totalXp);
  const xpForNext = totalXpForLevel(user.level + 1) - totalXpForLevel(user.level);
  const currentLevelXp = user.totalXp - totalXpForLevel(user.level);
</script>

<div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
  <!-- Header with user stats -->
  <div class="mb-8">
    <div class="card bg-gradient-to-r from-dark-800 to-dark-700">
      <div class="flex flex-col md:flex-row items-start md:items-center justify-between gap-6">
        <!-- User info -->
        <div class="flex items-center space-x-4">
          <div class="w-20 h-20 rounded-full bg-gradient-to-br from-neon-cyan to-neon-green flex items-center justify-center text-dark-900 text-3xl font-bold">
            {user.username.charAt(0).toUpperCase()}
          </div>
          <div>
            <h1 class="text-2xl font-bold text-white">{user.username}</h1>
            <p class="text-neon-cyan">{getTitleForLevel(user.level)}</p>
            <p class="text-gray-400 text-sm">Niveau {user.level}</p>
          </div>
        </div>

        <!-- XP Progress -->
        <div class="flex-1 max-w-md w-full">
          <div class="flex justify-between text-sm mb-2">
            <span class="text-gray-400">Progression</span>
            <span class="text-neon-green font-semibold">{user.totalXp} XP</span>
          </div>
          <div class="progress-bar">
            <div class="progress-bar-fill" style="width: {progress}%"></div>
          </div>
          <div class="flex justify-between text-xs mt-1">
            <span class="text-gray-500">Niveau {user.level}</span>
            <span class="text-gray-500">{currentLevelXp} / {xpForNext} XP</span>
          </div>
        </div>

        <!-- Quick stats -->
        <div class="flex space-x-6">
          <div class="text-center">
            <div class="text-2xl font-bold text-neon-cyan">{data.completedCount}</div>
            <div class="text-xs text-gray-400">Exercices</div>
          </div>
          <div class="text-center">
            <div class="text-2xl font-bold text-neon-green">{data.badges.length}</div>
            <div class="text-xs text-gray-400">Badges</div>
          </div>
          <div class="text-center">
            <div class="text-2xl font-bold {user.currentStreak > 0 ? 'streak-fire' : 'text-gray-500'}">{user.currentStreak}</div>
            <div class="text-xs text-gray-400">Streak</div>
          </div>
        </div>
      </div>
    </div>
  </div>

  <div class="grid lg:grid-cols-3 gap-8">
    <!-- Main content -->
    <div class="lg:col-span-2 space-y-8">
      <!-- Module Progress -->
      <section>
        <h2 class="text-xl font-bold text-white mb-4 flex items-center">
          <BookOpen class="w-5 h-5 mr-2 text-neon-cyan" /> Progression des modules
        </h2>
        <div class="space-y-4">
          {#each data.moduleProgress.slice(0, 6) as mod}
            <div class="card module-card {!mod.unlocked ? 'module-locked' : ''}">
              <div class="flex items-center justify-between">
                <div class="flex items-center space-x-4">
                  <div class="w-10 h-10 rounded-lg bg-dark-600 flex items-center justify-center">
                    {#if mod.progress === 100}
                      <Check class="w-5 h-5 text-neon-green" />
                    {:else if !mod.unlocked}
                      <Lock class="w-5 h-5 text-gray-500" />
                    {:else}
                      <span class="text-white font-bold">{mod.difficulty}</span>
                    {/if}
                  </div>
                  <div>
                    <h3 class="font-semibold text-white">{mod.title}</h3>
                    <p class="text-sm text-gray-400">{mod.completed} / {mod.total} exercices</p>
                  </div>
                </div>
                <div class="flex items-center space-x-4">
                  <div class="w-24">
                    <div class="progress-bar h-2">
                      <div class="progress-bar-fill" style="width: {mod.progress}%"></div>
                    </div>
                  </div>
                  <span class="text-sm font-semibold {mod.progress === 100 ? 'text-neon-green' : 'text-gray-400'}">
                    {mod.progress}%
                  </span>
                </div>
              </div>
            </div>
          {/each}

          <a href="/modules" class="block text-center text-neon-cyan hover:underline py-2 flex items-center justify-center">
            Voir tous les modules <ArrowRight class="w-4 h-4 ml-1" />
          </a>
        </div>
      </section>

      <!-- Recent Activity -->
      <section>
        <h2 class="text-xl font-bold text-white mb-4 flex items-center">
          <Clock class="w-5 h-5 mr-2 text-neon-cyan" /> Activite recente
        </h2>

        {#if data.recentCompletions.length > 0}
          <div class="space-y-3">
            {#each data.recentCompletions as completion}
              <div class="card py-3 flex items-center justify-between">
                <div class="flex items-center space-x-3">
                  <Check class="w-5 h-5 text-neon-green" />
                  <div>
                    <p class="text-white font-medium">{completion.exercise.title}</p>
                    <p class="text-sm text-gray-400">{completion.exercise.module.title}</p>
                  </div>
                </div>
                <div class="text-right">
                  <p class="text-neon-green font-semibold">+{completion.xpEarned} XP</p>
                  <p class="text-xs text-gray-500">
                    {new Date(completion.completedAt).toLocaleDateString('fr-FR')}
                  </p>
                </div>
              </div>
            {/each}
          </div>
        {:else}
          <div class="card text-center py-8">
            <p class="text-gray-400 mb-4">Aucune activite recente</p>
            <a href="/modules" class="btn btn-primary">
              Commencer un exercice
            </a>
          </div>
        {/if}
      </section>
    </div>

    <!-- Sidebar -->
    <div class="space-y-8">
      <!-- Badges -->
      <section>
        <h2 class="text-xl font-bold text-white mb-4 flex items-center">
          <Award class="w-5 h-5 mr-2 text-neon-cyan" /> Badges ({data.badges.length})
        </h2>

        {#if data.badges.length > 0}
          <div class="grid grid-cols-3 gap-3">
            {#each data.badges.slice(0, 9) as userBadge}
              <div
                class="aspect-square rounded-lg border flex flex-col items-center justify-center p-2 badge-{userBadge.badge.rarity.toLowerCase()}"
                title={userBadge.badge.description}
              >
                <Award class="w-6 h-6 mb-1" />
                <span class="text-xs text-center truncate w-full">{userBadge.badge.name}</span>
              </div>
            {/each}
          </div>
          {#if data.badges.length > 9}
            <a href="/profile" class="block text-center text-neon-cyan hover:underline py-2 text-sm flex items-center justify-center">
              Voir tous les badges <ArrowRight class="w-4 h-4 ml-1" />
            </a>
          {/if}
        {:else}
          <div class="card text-center py-6">
            <Award class="w-12 h-12 mx-auto mb-2 opacity-50 text-gray-500" />
            <p class="text-gray-400 text-sm">Aucun badge pour le moment</p>
            <p class="text-gray-500 text-xs">Complete des modules pour debloquer des badges!</p>
          </div>
        {/if}
      </section>

      <!-- Stats Card -->
      <section>
        <h2 class="text-xl font-bold text-white mb-4 flex items-center">
          <BarChart3 class="w-5 h-5 mr-2 text-neon-cyan" /> Statistiques
        </h2>

        <div class="card space-y-4">
          <div class="flex justify-between items-center">
            <span class="text-gray-400">XP Total</span>
            <span class="text-neon-green font-bold">{user.totalXp}</span>
          </div>
          <div class="flex justify-between items-center">
            <span class="text-gray-400">Exercices completes</span>
            <span class="text-white font-bold">{data.completedCount} / {data.totalExercises}</span>
          </div>
          <div class="flex justify-between items-center">
            <span class="text-gray-400">Streak actuel</span>
            <span class="font-bold {user.currentStreak > 0 ? 'streak-fire' : 'text-gray-500'}">{user.currentStreak} jours</span>
          </div>
          <div class="flex justify-between items-center">
            <span class="text-gray-400">Meilleur streak</span>
            <span class="text-white font-bold">{user.longestStreak} jours</span>
          </div>
        </div>
      </section>

      <!-- Quick Actions -->
      <section>
        <h2 class="text-xl font-bold text-white mb-4 flex items-center">
          <Zap class="w-5 h-5 mr-2 text-neon-cyan" /> Actions rapides
        </h2>

        <div class="space-y-3">
          <a href="/modules" class="btn btn-primary w-full text-center">
            Continuer l'apprentissage
          </a>
          <a href="/leaderboard" class="btn btn-secondary w-full text-center">
            Voir le classement
          </a>
        </div>
      </section>
    </div>
  </div>
</div>
