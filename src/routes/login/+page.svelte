<script lang="ts">
  import { Unlock, ArrowLeft } from 'lucide-svelte';

  let { form } = $props();
</script>

<div class="min-h-screen flex items-center justify-center px-4 py-12">
  <div class="w-full max-w-md">
    <!-- Logo -->
    <div class="text-center mb-8">
      <a href="/" class="inline-flex items-center space-x-3">
        <Unlock class="w-10 h-10 text-neon-cyan" />
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

      {#if form?.error}
        <div class="bg-red-500/20 border border-red-500 text-red-400 px-4 py-3 rounded-lg mb-6">
          {form.error}
        </div>
      {/if}

      <form method="POST">
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
            class="btn btn-primary w-full text-lg py-3"
          >
            Se connecter
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
      <a href="/" class="text-gray-400 hover:text-neon-cyan transition-colors inline-flex items-center">
        <ArrowLeft class="w-4 h-4 mr-1" /> Retour a l'accueil
      </a>
    </div>
  </div>
</div>
