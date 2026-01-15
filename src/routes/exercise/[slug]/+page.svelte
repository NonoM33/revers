<script lang="ts">
  import { enhance } from '$app/forms';
  import { getDifficultyStars } from '$lib/utils/xp';

  let { data, form } = $props();

  const exercise = data.exercise;
  let hintsRevealed = $state(data.hintsRevealed);
  let loading = $state(false);
  let showSuccess = $state(false);

  function revealHint() {
    if (hintsRevealed < exercise.hints.length) {
      hintsRevealed++;
    }
  }
</script>

<div class="max-w-4xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
  <!-- Back button -->
  <a href="/modules/{exercise.module.slug}" class="text-gray-400 hover:text-neon-cyan transition-colors mb-6 inline-flex items-center">
    <svg class="w-5 h-5 mr-2" fill="none" stroke="currentColor" viewBox="0 0 24 24">
      <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 19l-7-7 7-7" />
    </svg>
    Retour a {exercise.module.title}
  </a>

  <!-- Exercise header -->
  <div class="card mb-8">
    <div class="flex items-start justify-between mb-4">
      <div>
        <div class="flex items-center space-x-3 mb-2">
          {#if data.isCompleted}
            <span class="px-3 py-1 rounded-full bg-neon-green/20 text-neon-green text-sm font-semibold">
              ✓ Complete
            </span>
          {/if}
          <span class="text-yellow-400 text-sm">
            {getDifficultyStars(exercise.difficulty)}
          </span>
        </div>
        <h1 class="text-2xl font-bold text-white">{exercise.title}</h1>
      </div>
      <div class="text-right">
        <div class="text-neon-green font-bold text-xl">+{exercise.xpReward} XP</div>
        {#if exercise.bonusXp > 0}
          <div class="text-xs text-gray-500">+{exercise.bonusXp} bonus sans indice</div>
        {/if}
      </div>
    </div>

    <div class="terminal-text mb-6">
      {exercise.description}
    </div>

    <!-- Type badge -->
    <div class="flex items-center space-x-4 text-sm">
      <span class="px-3 py-1 rounded-full bg-dark-600 text-gray-300">
        Type: {exercise.validationType}
      </span>
      <span class="px-3 py-1 rounded-full bg-dark-600 text-gray-300">
        Module: {exercise.module.title}
      </span>
    </div>
  </div>

  {#if !data.isUnlocked}
    <!-- Locked message -->
    <div class="card text-center py-12">
      <span class="text-6xl mb-4 block">🔒</span>
      <h2 class="text-xl font-bold text-white mb-2">Exercice verrouille</h2>
      <p class="text-gray-400">
        Debloque d'abord le module "{exercise.module.title}".
      </p>
    </div>
  {:else}
    <div class="grid lg:grid-cols-2 gap-8">
      <!-- Left: Download and hints -->
      <div class="space-y-6">
        <!-- Download section -->
        <section class="card">
          <h2 class="text-lg font-bold text-white mb-4 flex items-center">
            <span class="mr-2">📥</span> Telecharger le binaire
          </h2>

          {#if exercise.binaryPath}
            <div class="space-y-3">
              <a
                href="{exercise.binaryPath}_arm64"
                download
                class="btn btn-primary w-full text-center flex items-center justify-center"
              >
                <span class="mr-2">🍎</span> macOS ARM64 (M1/M2/M3)
              </a>
              <a
                href="{exercise.binaryPath}_x86_64"
                download
                class="btn btn-secondary w-full text-center flex items-center justify-center"
              >
                <span class="mr-2">💻</span> macOS Intel (x86_64)
              </a>
            </div>
          {:else}
            <p class="text-gray-400 text-center py-4">
              Binaire non disponible pour cet exercice.
            </p>
          {/if}
        </section>

        <!-- Hints section -->
        <section class="card">
          <h2 class="text-lg font-bold text-white mb-4 flex items-center">
            <span class="mr-2">💡</span> Indices ({hintsRevealed}/{exercise.hints.length})
          </h2>

          {#if exercise.hints.length > 0}
            <div class="space-y-3">
              {#each exercise.hints as hint, i}
                {#if i < hintsRevealed}
                  <div class="p-3 rounded-lg bg-dark-700 border border-dark-500">
                    <div class="text-xs text-gray-500 mb-1">Indice {i + 1}</div>
                    <p class="text-gray-300">{hint}</p>
                  </div>
                {:else}
                  <div class="p-3 rounded-lg bg-dark-700 border border-dark-500 opacity-50">
                    <div class="text-xs text-gray-500 mb-1">Indice {i + 1}</div>
                    <p class="text-gray-500 italic">Verrouille</p>
                  </div>
                {/if}
              {/each}

              {#if hintsRevealed < exercise.hints.length && !data.isCompleted}
                <button
                  onclick={revealHint}
                  class="btn btn-secondary w-full text-sm"
                >
                  Reveler l'indice suivant (-10 XP)
                </button>
              {/if}
            </div>
          {:else}
            <p class="text-gray-400 text-center py-4">
              Pas d'indices pour cet exercice. Tu peux le faire!
            </p>
          {/if}
        </section>
      </div>

      <!-- Right: Submit flag -->
      <div>
        <section class="card">
          <h2 class="text-lg font-bold text-white mb-4 flex items-center">
            <span class="mr-2">🚩</span> Soumettre le flag
          </h2>

          {#if data.isCompleted}
            <div class="text-center py-8">
              <span class="text-6xl mb-4 block">🎉</span>
              <h3 class="text-xl font-bold text-neon-green mb-2">Exercice complete!</h3>
              <p class="text-gray-400">Tu as deja resolu cet exercice.</p>
            </div>
          {:else if form?.success}
            <div class="text-center py-8">
              <span class="text-6xl mb-4 block">🎉</span>
              <h3 class="text-xl font-bold text-neon-green mb-2">Bravo!</h3>
              <p class="text-gray-400 mb-2">Flag correct!</p>
              <p class="text-neon-green text-2xl font-bold">+{form.xpEarned} XP</p>
              {#if form.isFirstBlood}
                <p class="text-yellow-400 text-sm mt-2">🩸 First Blood!</p>
              {/if}
            </div>
          {:else}
            <form
              method="POST"
              action="?/submitFlag"
              use:enhance={() => {
                loading = true;
                return async ({ update }) => {
                  loading = false;
                  update();
                };
              }}
            >
              {#if form?.error}
                <div class="bg-red-500/20 border border-red-500 text-red-400 px-4 py-3 rounded-lg mb-4">
                  {form.error}
                </div>
              {/if}

              <div class="mb-4">
                <label for="flag" class="block text-sm font-medium text-gray-300 mb-2">
                  Flag
                </label>
                <input
                  type="text"
                  id="flag"
                  name="flag"
                  class="flag-input"
                  placeholder="FLAG&#123;...&#125;"
                  required
                />
              </div>

              <button
                type="submit"
                disabled={loading}
                class="btn btn-primary w-full disabled:opacity-50"
              >
                {#if loading}
                  <span class="inline-flex items-center">
                    <svg class="animate-spin -ml-1 mr-2 h-5 w-5" fill="none" viewBox="0 0 24 24">
                      <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle>
                      <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
                    </svg>
                    Verification...
                  </span>
                {:else}
                  Soumettre
                {/if}
              </button>
            </form>

            <div class="mt-6 pt-6 border-t border-dark-600">
              <h3 class="text-sm font-semibold text-gray-400 mb-3">Stats</h3>
              <div class="grid grid-cols-2 gap-4 text-sm">
                <div>
                  <span class="text-gray-500">Tentatives:</span>
                  <span class="text-white ml-2">{exercise.totalAttempts}</span>
                </div>
                <div>
                  <span class="text-gray-500">Resolus:</span>
                  <span class="text-neon-green ml-2">{exercise.totalSolves}</span>
                </div>
              </div>
            </div>
          {/if}
        </section>

        <!-- Tips -->
        <div class="card mt-6">
          <h3 class="text-sm font-semibold text-gray-400 mb-3">Conseils</h3>
          <ul class="text-sm text-gray-400 space-y-2">
            <li class="flex items-start">
              <span class="text-neon-cyan mr-2">→</span>
              Ouvre le binaire dans Binary Ninja pour l'analyse statique
            </li>
            <li class="flex items-start">
              <span class="text-neon-cyan mr-2">→</span>
              Utilise LLDB pour l'analyse dynamique et les breakpoints
            </li>
            <li class="flex items-start">
              <span class="text-neon-cyan mr-2">→</span>
              Le flag a le format FLAG...
            </li>
          </ul>
        </div>
      </div>
    </div>
  {/if}
</div>
