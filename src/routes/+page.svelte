<script lang="ts">
  import { Target, Trophy, Code2, Unlock, Microscope, Bug, Monitor, ArrowRight, ChevronRight } from 'lucide-svelte';

  const exampleFlag = "FLAG{BYPASS_SUCCESS_a1b2c3}";

  const features = [
    {
      icon: Target,
      title: '16 Modules Progressifs',
      description: 'Du debutant a l\'expert, maitrise chaque technique de reverse engineering.'
    },
    {
      icon: Trophy,
      title: 'Gamification Complete',
      description: 'XP, niveaux, badges, streaks et leaderboard pour rester motive.'
    },
    {
      icon: Code2,
      title: 'Exercices Pratiques',
      description: 'Des binaires reels a analyser avec Binary Ninja et LLDB.'
    },
    {
      icon: Unlock,
      title: 'Validation Immediate',
      description: 'Soumets tes flags et obtiens des feedbacks instantanes.'
    }
  ];

  const modules = [
    { name: 'Anti-Debug Basics', level: 1, color: 'from-green-400 to-emerald-500' },
    { name: 'Checks Distribues', level: 2, color: 'from-cyan-400 to-blue-500' },
    { name: 'Crypto Basics', level: 3, color: 'from-purple-400 to-pink-500' },
    { name: 'Packers & VMs', level: 4, color: 'from-orange-400 to-red-500' },
  ];
</script>

<!-- Hero Section -->
<section class="relative overflow-hidden">
  <!-- Background effects -->
  <div class="absolute inset-0 bg-gradient-to-b from-dark-800 to-dark-900"></div>
  <div class="absolute top-0 left-1/2 -translate-x-1/2 w-[800px] h-[800px] bg-neon-cyan/5 rounded-full blur-3xl"></div>
  <div class="absolute bottom-0 right-0 w-[600px] h-[600px] bg-neon-green/5 rounded-full blur-3xl"></div>

  <div class="relative max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-24 md:py-32">
    <div class="text-center">
      <!-- Badge -->
      <div class="inline-flex items-center px-4 py-2 rounded-full bg-dark-700 border border-dark-500 mb-8">
        <span class="w-2 h-2 rounded-full bg-neon-green animate-pulse mr-2"></span>
        <span class="text-sm text-gray-300">110 exercices disponibles</span>
      </div>

      <!-- Title -->
      <h1 class="text-4xl md:text-6xl lg:text-7xl font-bold mb-6">
        <span class="text-white">Deviens un</span>
        <br />
        <span class="bg-gradient-to-r from-neon-cyan via-neon-green to-neon-cyan bg-clip-text text-transparent">
          Expert en Reverse Engineering
        </span>
      </h1>

      <!-- Subtitle -->
      <p class="text-xl md:text-2xl text-gray-400 max-w-3xl mx-auto mb-10">
        Plateforme d'apprentissage gamifiee. Progresse du niveau Script Kiddie
        a Legende du RE avec des exercices pratiques et des challenges reels.
      </p>

      <!-- CTA Buttons -->
      <div class="flex flex-col sm:flex-row gap-4 justify-center">
        <a
          href="/modules"
          class="btn btn-primary text-lg px-8 py-4 inline-flex items-center justify-center"
        >
          Commencer l'aventure
          <ArrowRight class="w-5 h-5 ml-2" />
        </a>
        <a
          href="/leaderboard"
          class="btn btn-secondary text-lg px-8 py-4 inline-flex items-center justify-center"
        >
          Voir le classement
        </a>
      </div>

      <!-- Stats -->
      <div class="grid grid-cols-2 md:grid-cols-4 gap-6 mt-16 max-w-4xl mx-auto">
        <div class="text-center">
          <div class="text-3xl md:text-4xl font-bold text-neon-cyan">16</div>
          <div class="text-gray-400">Modules</div>
        </div>
        <div class="text-center">
          <div class="text-3xl md:text-4xl font-bold text-neon-green">110</div>
          <div class="text-gray-400">Exercices</div>
        </div>
        <div class="text-center">
          <div class="text-3xl md:text-4xl font-bold text-neon-purple">31</div>
          <div class="text-gray-400">Badges</div>
        </div>
        <div class="text-center">
          <div class="text-3xl md:text-4xl font-bold text-neon-pink">50+</div>
          <div class="text-gray-400">Niveaux</div>
        </div>
      </div>
    </div>
  </div>
</section>

<!-- Features Section -->
<section class="py-20 bg-dark-800/50">
  <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
    <div class="text-center mb-16">
      <h2 class="text-3xl md:text-4xl font-bold text-white mb-4">
        Tout ce qu'il te faut pour progresser
      </h2>
      <p class="text-gray-400 text-lg max-w-2xl mx-auto">
        Une methodologie eprouvee pour apprendre le reverse engineering de maniere structuree.
      </p>
    </div>

    <div class="grid md:grid-cols-2 lg:grid-cols-4 gap-6">
      {#each features as feature}
        <div class="card group">
          <div class="w-12 h-12 rounded-lg bg-gradient-to-br from-neon-cyan/20 to-neon-green/20 flex items-center justify-center mb-4 group-hover:scale-110 transition-transform">
            <feature.icon class="w-6 h-6 text-neon-cyan" />
          </div>
          <h3 class="text-xl font-semibold text-white mb-2">{feature.title}</h3>
          <p class="text-gray-400">{feature.description}</p>
        </div>
      {/each}
    </div>
  </div>
</section>

<!-- Modules Preview -->
<section class="py-20">
  <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
    <div class="text-center mb-16">
      <h2 class="text-3xl md:text-4xl font-bold text-white mb-4">
        Parcours d'apprentissage
      </h2>
      <p class="text-gray-400 text-lg max-w-2xl mx-auto">
        Des fondations jusqu'aux techniques les plus avancees.
      </p>
    </div>

    <div class="relative">
      <!-- Connection line -->
      <div class="absolute top-1/2 left-0 right-0 h-1 bg-gradient-to-r from-green-400 via-cyan-400 via-purple-400 to-red-400 hidden md:block"></div>

      <div class="grid md:grid-cols-4 gap-8">
        {#each modules as module, i}
          <div class="relative">
            <div class="card text-center relative z-10">
              <!-- Level indicator -->
              <div class="w-16 h-16 mx-auto mb-4 rounded-full bg-gradient-to-br {module.color} flex items-center justify-center">
                <span class="text-2xl font-bold text-white">{module.level}</span>
              </div>
              <h3 class="text-lg font-semibold text-white mb-2">{module.name}</h3>
              <div class="text-sm text-gray-400">Phase {module.level}</div>
            </div>
          </div>
        {/each}
      </div>
    </div>

    <div class="text-center mt-12">
      <a href="/modules" class="btn btn-secondary inline-flex items-center">
        Voir tous les modules
        <ChevronRight class="w-5 h-5 ml-2" />
      </a>
    </div>
  </div>
</section>

<!-- Tools Section -->
<section class="py-20 bg-dark-800/50">
  <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
    <div class="grid md:grid-cols-2 gap-12 items-center">
      <div>
        <h2 class="text-3xl md:text-4xl font-bold text-white mb-6">
          Outils recommandes
        </h2>
        <p class="text-gray-400 text-lg mb-8">
          Tu utiliseras les memes outils que les pros pour analyser les binaires et trouver les vulnerabilites.
        </p>

        <div class="space-y-4">
          <div class="flex items-center space-x-4">
            <div class="w-12 h-12 rounded-lg bg-dark-600 flex items-center justify-center">
              <Microscope class="w-6 h-6 text-neon-cyan" />
            </div>
            <div>
              <div class="font-semibold text-white">Binary Ninja</div>
              <div class="text-sm text-gray-400">Analyse statique et desassemblage</div>
            </div>
          </div>

          <div class="flex items-center space-x-4">
            <div class="w-12 h-12 rounded-lg bg-dark-600 flex items-center justify-center">
              <Bug class="w-6 h-6 text-neon-green" />
            </div>
            <div>
              <div class="font-semibold text-white">LLDB</div>
              <div class="text-sm text-gray-400">Debug dynamique et analyse runtime</div>
            </div>
          </div>

          <div class="flex items-center space-x-4">
            <div class="w-12 h-12 rounded-lg bg-dark-600 flex items-center justify-center">
              <Monitor class="w-6 h-6 text-neon-purple" />
            </div>
            <div>
              <div class="font-semibold text-white">macOS / ARM64</div>
              <div class="text-sm text-gray-400">Binaires optimises pour ton Mac</div>
            </div>
          </div>
        </div>
      </div>

      <div class="relative">
        <div class="terminal-text">
          <div class="text-gray-500 mb-2"># Exemple de session LLDB</div>
          <div><span class="text-neon-cyan">$</span> lldb ./challenge</div>
          <div class="text-gray-400">(lldb) target create "./challenge"</div>
          <div><span class="text-neon-cyan">(lldb)</span> b *0x100003f40</div>
          <div class="text-gray-400">Breakpoint 1: address = 0x100003f40</div>
          <div><span class="text-neon-cyan">(lldb)</span> r</div>
          <div class="text-gray-400">Process launched...</div>
          <div><span class="text-neon-cyan">(lldb)</span> register write eax 1</div>
          <div><span class="text-neon-cyan">(lldb)</span> c</div>
          <div class="text-neon-green mt-2">{exampleFlag}</div>
        </div>
      </div>
    </div>
  </div>
</section>

<!-- CTA Section -->
<section class="py-20">
  <div class="max-w-4xl mx-auto px-4 sm:px-6 lg:px-8 text-center">
    <div class="card bg-gradient-to-r from-dark-700 to-dark-800 border-neon-cyan/30">
      <h2 class="text-3xl md:text-4xl font-bold text-white mb-4">
        Pret a commencer?
      </h2>
      <p class="text-gray-400 text-lg mb-8">
        Rejoins la communaute et commence ton parcours vers l'expertise en reverse engineering.
      </p>
      <a href="/login" class="btn btn-primary text-lg px-8 py-4 inline-flex items-center">
        Creer mon compte
        <ArrowRight class="w-5 h-5 ml-2" />
      </a>
    </div>
  </div>
</section>
