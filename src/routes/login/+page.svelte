<script lang="ts">
  import { enhance } from '$app/forms';

  let { data } = $props();
  let loading = $state(false);
</script>

<div class="min-h-screen flex items-center justify-center px-4 py-12">
  <div class="w-full max-w-md">
    <!-- Logo -->
    <div class="text-center mb-8">
      <a href="/" class="inline-flex items-center space-x-3">
        <span class="text-4xl">🔓</span>
        <span class="text-2xl font-bold bg-gradient-to-r from-neon-cyan to-neon-green bg-clip-text text-transparent">
          REverse Academy
        </span>
      </a>
    </div>

    <!-- Login Card -->
    <div class="card">
      <h1 class="text-2xl font-bold text-white text-center mb-6">
        Connexion
      </h1>

      {#if data.error}
        <div class="bg-red-500/20 border border-red-500 text-red-400 px-4 py-3 rounded-lg mb-6">
          {data.error}
        </div>
      {/if}

      <form
        method="POST"
        action="/api/auth/login"
        use:enhance={() => {
          loading = true;
          return async ({ update }) => {
            loading = false;
            update();
          };
        }}
      >
        <div class="space-y-4">
          <div>
            <label for="email" class="block text-sm font-medium text-gray-300 mb-2">
              Email
            </label>
            <input
              type="email"
              id="email"
              name="email"
              required
              class="flag-input"
              placeholder="ton@email.com"
            />
          </div>

          <div>
            <label for="password" class="block text-sm font-medium text-gray-300 mb-2">
              Mot de passe
            </label>
            <input
              type="password"
              id="password"
              name="password"
              required
              class="flag-input"
              placeholder="••••••••"
            />
          </div>

          <button
            type="submit"
            disabled={loading}
            class="btn btn-primary w-full text-lg py-3 disabled:opacity-50"
          >
            {#if loading}
              <span class="inline-flex items-center">
                <svg class="animate-spin -ml-1 mr-2 h-5 w-5" fill="none" viewBox="0 0 24 24">
                  <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle>
                  <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
                </svg>
                Connexion...
              </span>
            {:else}
              Se connecter
            {/if}
          </button>
        </div>
      </form>

      <div class="mt-6 text-center">
        <p class="text-gray-400 text-sm">
          Pas encore de compte? Contacte l'administrateur.
        </p>
      </div>
    </div>

    <!-- Back to home -->
    <div class="text-center mt-6">
      <a href="/" class="text-gray-400 hover:text-neon-cyan transition-colors">
        ← Retour a l'accueil
      </a>
    </div>
  </div>
</div>
