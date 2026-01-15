<script lang="ts">
  import { getDifficultyStars } from '$lib/utils/xp';

  let { data } = $props();

  const mod = data.module;
</script>

<div class="max-w-4xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
  <!-- Back button -->
  <a href="/modules" class="text-gray-400 hover:text-neon-cyan transition-colors mb-6 inline-flex items-center">
    <svg class="w-5 h-5 mr-2" fill="none" stroke="currentColor" viewBox="0 0 24 24">
      <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 19l-7-7 7-7" />
    </svg>
    Retour aux modules
  </a>

  <!-- Module header -->
  <div class="card mb-8">
    <div class="flex flex-col md:flex-row md:items-center md:justify-between gap-4 mb-4">
      <div>
        <h1 class="text-2xl font-bold text-white mb-2">{mod.title}</h1>
        <p class="text-gray-400">{mod.description}</p>
      </div>
      <div class="text-right">
        <div class="text-yellow-400 text-lg mb-1">
          {getDifficultyStars(mod.difficulty)}
        </div>
        <div class="text-sm text-gray-400">
          Difficulte {mod.difficulty}/5
        </div>
      </div>
    </div>

    <!-- Progress -->
    <div class="mb-4">
      <div class="flex justify-between text-sm mb-2">
        <span class="text-gray-400">Progression</span>
        <span class="text-neon-green">{data.completedCount}/{mod.exercises.length} exercices</span>
      </div>
      <div class="progress-bar">
        <div class="progress-bar-fill" style="width: {data.progress}%"></div>
      </div>
    </div>

    <!-- Stats -->
    <div class="flex flex-wrap gap-4 text-sm">
      <div class="px-3 py-1 rounded-full bg-dark-600">
        <span class="text-gray-400">XP requis:</span>
        <span class="text-white font-semibold ml-1">{mod.requiredXp}</span>
      </div>
      <div class="px-3 py-1 rounded-full bg-dark-600">
        <span class="text-gray-400">Recompense:</span>
        <span class="text-neon-green font-semibold ml-1">{mod.xpReward} XP</span>
      </div>
      {#if mod.badgeSlug}
        <div class="px-3 py-1 rounded-full bg-dark-600">
          <span class="text-gray-400">Badge:</span>
          <span class="text-neon-purple font-semibold ml-1">🏅</span>
        </div>
      {/if}
    </div>
  </div>

  {#if !data.isUnlocked}
    <!-- Locked message -->
    <div class="card text-center py-12 mb-8">
      <span class="text-6xl mb-4 block">🔒</span>
      <h2 class="text-xl font-bold text-white mb-2">Module verrouille</h2>
      <p class="text-gray-400 mb-4">
        Tu as besoin de <span class="text-neon-green font-bold">{mod.requiredXp} XP</span> pour debloquer ce module.
      </p>
      <p class="text-gray-500 text-sm">
        Tu as actuellement <span class="text-white">{data.userXp} XP</span>. Continue les modules precedents!
      </p>
    </div>
  {:else}
    <!-- Exercises list -->
    <section>
      <h2 class="text-xl font-bold text-white mb-4">Exercices</h2>

      <div class="space-y-4">
        {#each mod.exercises as exercise, i}
          <a
            href="/exercise/{exercise.slug}"
            class="card block group hover:border-neon-cyan transition-all"
          >
            <div class="flex items-start justify-between">
              <div class="flex items-start space-x-4">
                <!-- Status indicator -->
                <div class="w-10 h-10 rounded-lg flex items-center justify-center shrink-0 {exercise.completed ? 'bg-neon-green/20 text-neon-green' : 'bg-dark-600 text-gray-400'}">
                  {#if exercise.completed}
                    <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                      <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M5 13l4 4L19 7" />
                    </svg>
                  {:else}
                    <span class="font-bold">{i + 1}</span>
                  {/if}
                </div>

                <!-- Exercise info -->
                <div>
                  <h3 class="font-semibold text-white group-hover:text-neon-cyan transition-colors">
                    {exercise.title}
                  </h3>
                  <p class="text-sm text-gray-400 mt-1 line-clamp-2">
                    {exercise.description}
                  </p>

                  <!-- Tags -->
                  <div class="flex flex-wrap gap-2 mt-3">
                    <span class="px-2 py-0.5 rounded text-xs bg-dark-600 text-gray-300">
                      {exercise.validationType}
                    </span>
                    <span class="px-2 py-0.5 rounded text-xs bg-dark-600 text-yellow-400">
                      {'★'.repeat(exercise.difficulty)}{'☆'.repeat(5 - exercise.difficulty)}
                    </span>
                    {#if exercise.hints.length > 0}
                      <span class="px-2 py-0.5 rounded text-xs bg-dark-600 text-gray-400">
                        💡 {exercise.hints.length} indices
                      </span>
                    {/if}
                  </div>
                </div>
              </div>

              <!-- XP reward -->
              <div class="text-right shrink-0 ml-4">
                <div class="text-neon-green font-bold">+{exercise.xpReward} XP</div>
                {#if exercise.bonusXp > 0}
                  <div class="text-xs text-gray-500">+{exercise.bonusXp} bonus</div>
                {/if}
              </div>
            </div>
          </a>
        {/each}
      </div>
    </section>
  {/if}
</div>
