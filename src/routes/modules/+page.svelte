<script lang="ts">
  import { getDifficultyStars } from '$lib/utils/xp';

  let { data } = $props();

  const categoryColors: Record<string, string> = {
    protection: 'from-red-400 to-orange-500',
    analysis: 'from-cyan-400 to-blue-500',
    crypto: 'from-purple-400 to-pink-500',
    obfuscation: 'from-yellow-400 to-orange-500',
    patching: 'from-green-400 to-emerald-500',
    advanced: 'from-indigo-400 to-purple-500',
    packing: 'from-pink-400 to-red-500',
    virtualization: 'from-blue-400 to-indigo-500',
    kernel: 'from-gray-400 to-gray-600',
    challenge: 'from-yellow-500 to-red-500',
  };

  const categoryIcons: Record<string, string> = {
    protection: '🛡️',
    analysis: '🔍',
    crypto: '🔐',
    obfuscation: '🌫️',
    patching: '🔧',
    advanced: '⚡',
    packing: '📦',
    virtualization: '🖥️',
    kernel: '⚙️',
    challenge: '🏆',
  };
</script>

<div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
  <!-- Header -->
  <div class="mb-8">
    <h1 class="text-3xl font-bold text-white mb-2">Modules d'apprentissage</h1>
    <p class="text-gray-400">
      Progresse du niveau debutant a expert avec nos 16 modules structures.
    </p>
  </div>

  <!-- Stats -->
  <div class="grid grid-cols-2 md:grid-cols-4 gap-4 mb-8">
    <div class="card text-center py-4">
      <div class="text-2xl font-bold text-neon-cyan">{data.modules.length}</div>
      <div class="text-sm text-gray-400">Modules</div>
    </div>
    <div class="card text-center py-4">
      <div class="text-2xl font-bold text-neon-green">
        {data.modules.reduce((acc, m) => acc + m.total, 0)}
      </div>
      <div class="text-sm text-gray-400">Exercices</div>
    </div>
    <div class="card text-center py-4">
      <div class="text-2xl font-bold text-neon-purple">
        {data.modules.filter((m) => m.progress === 100).length}
      </div>
      <div class="text-sm text-gray-400">Completes</div>
    </div>
    <div class="card text-center py-4">
      <div class="text-2xl font-bold text-white">
        {data.userXp}
      </div>
      <div class="text-sm text-gray-400">Ton XP</div>
    </div>
  </div>

  <!-- Modules Grid -->
  <div class="grid md:grid-cols-2 lg:grid-cols-3 gap-6">
    {#each data.modules as mod, i}
      <a
        href={mod.unlocked ? `/modules/${mod.slug}` : '#'}
        class="card module-card group {!mod.unlocked ? 'opacity-60 cursor-not-allowed' : 'hover:border-neon-cyan'}"
      >
        <!-- Module number and status -->
        <div class="flex items-center justify-between mb-4">
          <div class="flex items-center space-x-2">
            <div class="w-8 h-8 rounded-full bg-gradient-to-br {categoryColors[mod.category] || 'from-gray-400 to-gray-600'} flex items-center justify-center text-white text-sm font-bold">
              {i + 1}
            </div>
            <span class="text-lg">{categoryIcons[mod.category] || '📚'}</span>
          </div>

          {#if mod.progress === 100}
            <span class="px-2 py-1 rounded-full bg-neon-green/20 text-neon-green text-xs font-semibold">
              Complete
            </span>
          {:else if !mod.unlocked}
            <span class="px-2 py-1 rounded-full bg-gray-500/20 text-gray-400 text-xs font-semibold flex items-center">
              <span class="mr-1">🔒</span> {mod.requiredXp} XP
            </span>
          {:else if mod.completed > 0}
            <span class="px-2 py-1 rounded-full bg-neon-cyan/20 text-neon-cyan text-xs font-semibold">
              En cours
            </span>
          {/if}
        </div>

        <!-- Title and description -->
        <h3 class="text-lg font-bold text-white mb-2 group-hover:text-neon-cyan transition-colors">
          {mod.title}
        </h3>
        <p class="text-gray-400 text-sm mb-4 line-clamp-2">
          {mod.description}
        </p>

        <!-- Difficulty and progress -->
        <div class="flex items-center justify-between mb-3">
          <div class="text-yellow-400 text-sm">
            {getDifficultyStars(mod.difficulty)}
          </div>
          <div class="text-sm text-gray-400">
            {mod.completed}/{mod.total} exercices
          </div>
        </div>

        <!-- Progress bar -->
        <div class="progress-bar h-2">
          <div class="progress-bar-fill" style="width: {mod.progress}%"></div>
        </div>

        <!-- XP reward -->
        <div class="mt-3 text-right">
          <span class="text-xs text-gray-500">
            Recompense: <span class="text-neon-green">{mod.xpReward} XP</span>
          </span>
        </div>
      </a>
    {/each}
  </div>
</div>
