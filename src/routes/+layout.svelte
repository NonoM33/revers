<script lang="ts">
  import '../app.css';
  import { page } from '$app/stores';

  let { children, data } = $props();

  const navItems = [
    { href: '/', label: 'Accueil', icon: '🏠' },
    { href: '/dashboard', label: 'Dashboard', icon: '📊' },
    { href: '/modules', label: 'Modules', icon: '📚' },
    { href: '/leaderboard', label: 'Classement', icon: '🏆' },
  ];

  let isMenuOpen = $state(false);
</script>

<svelte:head>
  <title>REverse Academy - Apprends le Reverse Engineering</title>
  <meta name="description" content="Plateforme d'apprentissage gamifiee pour devenir expert en reverse engineering" />
</svelte:head>

<div class="min-h-screen bg-dark-900">
  <!-- Navigation -->
  <nav class="fixed top-0 left-0 right-0 z-50 bg-dark-800/95 backdrop-blur border-b border-dark-600">
    <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
      <div class="flex items-center justify-between h-16">
        <!-- Logo -->
        <a href="/" class="flex items-center space-x-3">
          <span class="text-2xl">🔓</span>
          <span class="text-xl font-bold bg-gradient-to-r from-neon-cyan to-neon-green bg-clip-text text-transparent">
            REverse Academy
          </span>
        </a>

        <!-- Desktop Navigation -->
        <div class="hidden md:flex items-center space-x-1">
          {#each navItems as item}
            <a
              href={item.href}
              class="px-4 py-2 rounded-lg transition-all duration-200 {$page.url.pathname === item.href
                ? 'bg-dark-600 text-neon-cyan'
                : 'text-gray-300 hover:bg-dark-700 hover:text-white'}"
            >
              <span class="mr-2">{item.icon}</span>
              {item.label}
            </a>
          {/each}
        </div>

        <!-- User Section -->
        <div class="hidden md:flex items-center space-x-4">
          {#if data?.user}
            <div class="flex items-center space-x-3">
              <div class="text-right">
                <div class="text-sm font-medium text-white">{data.user.username}</div>
                <div class="text-xs text-neon-green">Niveau {data.user.level}</div>
              </div>
              <a
                href="/profile"
                class="w-10 h-10 rounded-full bg-gradient-to-br from-neon-cyan to-neon-green flex items-center justify-center text-dark-900 font-bold"
              >
                {data.user.username.charAt(0).toUpperCase()}
              </a>
            </div>
          {:else}
            <a href="/login" class="btn btn-primary">
              Connexion
            </a>
          {/if}
        </div>

        <!-- Mobile menu button -->
        <button
          class="md:hidden p-2 rounded-lg text-gray-400 hover:text-white hover:bg-dark-700"
          onclick={() => (isMenuOpen = !isMenuOpen)}
        >
          <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            {#if isMenuOpen}
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12" />
            {:else}
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16M4 18h16" />
            {/if}
          </svg>
        </button>
      </div>
    </div>

    <!-- Mobile Navigation -->
    {#if isMenuOpen}
      <div class="md:hidden bg-dark-800 border-t border-dark-600">
        <div class="px-4 py-3 space-y-2">
          {#each navItems as item}
            <a
              href={item.href}
              class="block px-4 py-2 rounded-lg transition-all {$page.url.pathname === item.href
                ? 'bg-dark-600 text-neon-cyan'
                : 'text-gray-300 hover:bg-dark-700'}"
              onclick={() => (isMenuOpen = false)}
            >
              <span class="mr-2">{item.icon}</span>
              {item.label}
            </a>
          {/each}
          {#if !data?.user}
            <a
              href="/login"
              class="block px-4 py-2 rounded-lg bg-neon-green text-dark-900 font-semibold text-center"
              onclick={() => (isMenuOpen = false)}
            >
              Connexion
            </a>
          {/if}
        </div>
      </div>
    {/if}
  </nav>

  <!-- Main Content -->
  <main class="pt-16">
    {@render children()}
  </main>

  <!-- Footer -->
  <footer class="bg-dark-800 border-t border-dark-600 mt-20">
    <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
      <div class="flex flex-col md:flex-row justify-between items-center">
        <div class="flex items-center space-x-2 mb-4 md:mb-0">
          <span class="text-xl">🔓</span>
          <span class="text-lg font-bold text-gray-300">REverse Academy</span>
        </div>
        <p class="text-gray-500 text-sm">
          Apprends le reverse engineering de maniere ludique et progressive.
        </p>
      </div>
    </div>
  </footer>
</div>
