<script lang="ts">
  import { enhance } from '$app/forms';
  import { getDifficultyStars } from '$lib/utils/xp';
  import { parseMarkdown } from '$lib/utils/markdown';
  import { ChevronLeft, Check, Lock, Download, Apple, Cpu, Lightbulb, Flag, PartyPopper, Loader2, ArrowRight, BookOpen, Target, ChevronRight } from 'lucide-svelte';

  let { data, form } = $props();

  let exercise = $derived(data.exercise);
  let initialHints = $derived(data.hintsRevealed);
  let additionalHintsRevealed = $state(0);
  let hintsRevealed = $derived(initialHints + additionalHintsRevealed);
  let loading = $state(false);
  let showSuccess = $state(false);

  // Tabs: theory vs practice
  let hasTheory = $derived(!!exercise.theory);
  let activeTab = $state<'theory' | 'practice'>('theory');

  function revealHint() {
    if (hintsRevealed < exercise.hints.length) {
      additionalHintsRevealed++;
    }
  }

  // Parse markdown for theory
  let theoryHtml = $derived(exercise.theory ? parseMarkdown(exercise.theory) : '');
</script>

<div class="max-w-4xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
  <!-- Back button -->
  <a href="/modules/{exercise.module.slug}" class="text-gray-400 hover:text-neon-cyan transition-colors mb-6 inline-flex items-center">
    <ChevronLeft class="w-5 h-5 mr-1" />
    Retour a {exercise.module.title}
  </a>

  <!-- Exercise header -->
  <div class="card mb-8">
    <div class="flex items-start justify-between mb-4">
      <div>
        <div class="flex items-center space-x-3 mb-2">
          {#if data.isCompleted}
            <span class="px-3 py-1 rounded-full bg-neon-green/20 text-neon-green text-sm font-semibold flex items-center">
              <Check class="w-4 h-4 mr-1" /> Complete
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
      <Lock class="w-16 h-16 mx-auto mb-4 text-gray-500" />
      <h2 class="text-xl font-bold text-white mb-2">Exercice verrouille</h2>
      <p class="text-gray-400">
        Debloque d'abord le module "{exercise.module.title}".
      </p>
    </div>
  {:else}
    <!-- Tabs -->
    {#if hasTheory}
      <div class="flex space-x-2 mb-8">
        <button
          onclick={() => activeTab = 'theory'}
          class="px-6 py-3 rounded-lg font-semibold transition-all flex items-center {activeTab === 'theory'
            ? 'bg-neon-cyan text-dark-900'
            : 'bg-dark-700 text-gray-300 hover:bg-dark-600'}"
        >
          <BookOpen class="w-5 h-5 mr-2" /> Theorie
        </button>
        <button
          onclick={() => activeTab = 'practice'}
          class="px-6 py-3 rounded-lg font-semibold transition-all flex items-center {activeTab === 'practice'
            ? 'bg-neon-green text-dark-900'
            : 'bg-dark-700 text-gray-300 hover:bg-dark-600'}"
        >
          <Target class="w-5 h-5 mr-2" /> Pratique
        </button>
      </div>
    {/if}

    <!-- Theory Tab -->
    {#if activeTab === 'theory' && hasTheory}
      <div class="card prose prose-invert prose-cyan max-w-none mb-8">
        {@html theoryHtml}
      </div>
      <div class="text-center mb-8">
        <button
          onclick={() => activeTab = 'practice'}
          class="btn btn-primary text-lg px-8 py-4 inline-flex items-center"
        >
          Passer a la pratique <ArrowRight class="w-5 h-5 ml-2" />
        </button>
      </div>
    {/if}

    <!-- Practice Tab -->
    {#if activeTab === 'practice'}
    <div class="grid lg:grid-cols-2 gap-8">
      <!-- Left: Download and hints -->
      <div class="space-y-6">
        <!-- Download section -->
        <section class="card">
          <h2 class="text-lg font-bold text-white mb-4 flex items-center">
            <Download class="w-5 h-5 mr-2 text-neon-cyan" /> Telecharger le binaire
          </h2>

          {#if exercise.binaryPath}
            <div class="space-y-3">
              <a
                href="{exercise.binaryPath}_arm64"
                download
                class="btn btn-primary w-full text-center flex items-center justify-center"
              >
                <Apple class="w-5 h-5 mr-2" /> macOS ARM64 (M1/M2/M3)
              </a>
              <a
                href="{exercise.binaryPath}_x86_64"
                download
                class="btn btn-secondary w-full text-center flex items-center justify-center"
              >
                <Cpu class="w-5 h-5 mr-2" /> macOS Intel (x86_64)
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
            <Lightbulb class="w-5 h-5 mr-2 text-yellow-400" /> Indices ({hintsRevealed}/{exercise.hints.length})
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
                    <p class="text-gray-500 italic flex items-center">
                      <Lock class="w-3 h-3 mr-1" /> Verrouille
                    </p>
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
            <Flag class="w-5 h-5 mr-2 text-neon-green" /> Soumettre le flag
          </h2>

          {#if data.isCompleted}
            <div class="text-center py-8">
              <PartyPopper class="w-16 h-16 mx-auto mb-4 text-neon-green" />
              <h3 class="text-xl font-bold text-neon-green mb-2">Exercice complete!</h3>
              <p class="text-gray-400">Tu as deja resolu cet exercice.</p>
            </div>
          {:else if form?.success}
            <div class="text-center py-8">
              <PartyPopper class="w-16 h-16 mx-auto mb-4 text-neon-green" />
              <h3 class="text-xl font-bold text-neon-green mb-2">Bravo!</h3>
              <p class="text-gray-400 mb-2">Flag correct!</p>
              <p class="text-neon-green text-2xl font-bold">+{form.xpEarned} XP</p>
              {#if form.isFirstBlood}
                <p class="text-yellow-400 text-sm mt-2">First Blood!</p>
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
                    <Loader2 class="w-5 h-5 mr-2 animate-spin" />
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
              <ChevronRight class="w-4 h-4 mr-1 text-neon-cyan shrink-0 mt-0.5" />
              Ouvre le binaire dans Binary Ninja pour l'analyse statique
            </li>
            <li class="flex items-start">
              <ChevronRight class="w-4 h-4 mr-1 text-neon-cyan shrink-0 mt-0.5" />
              Utilise LLDB pour l'analyse dynamique et les breakpoints
            </li>
            <li class="flex items-start">
              <ChevronRight class="w-4 h-4 mr-1 text-neon-cyan shrink-0 mt-0.5" />
              Le flag a le format FLAG...
            </li>
          </ul>
        </div>
      </div>
    </div>
    {/if}
  {/if}
</div>
