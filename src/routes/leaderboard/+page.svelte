<script lang="ts">
  import { getTitleForLevel } from '$lib/utils/xp';

  let { data } = $props();

  const podiumColors = ['from-yellow-400 to-yellow-600', 'from-gray-300 to-gray-500', 'from-orange-400 to-orange-600'];
  const podiumIcons = ['🥇', '🥈', '🥉'];
</script>

<div class="max-w-4xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
  <!-- Header -->
  <div class="text-center mb-8">
    <h1 class="text-3xl font-bold text-white mb-2">Classement</h1>
    <p class="text-gray-400">
      {data.totalUsers} reverseurs en competition
    </p>
  </div>

  <!-- Top 3 Podium -->
  {#if data.leaderboard.length >= 3}
    <div class="flex items-end justify-center gap-4 mb-12">
      <!-- 2nd place -->
      <div class="text-center">
        <div class="w-20 h-20 mx-auto mb-2 rounded-full bg-gradient-to-br {podiumColors[1]} flex items-center justify-center text-dark-900 text-3xl font-bold">
          {data.leaderboard[1].username.charAt(0).toUpperCase()}
        </div>
        <div class="text-4xl mb-1">{podiumIcons[1]}</div>
        <div class="text-white font-semibold">{data.leaderboard[1].username}</div>
        <div class="text-neon-green text-sm">{data.leaderboard[1].totalXp} XP</div>
        <div class="bg-dark-700 rounded-lg h-16 w-24 mx-auto mt-2"></div>
      </div>

      <!-- 1st place -->
      <div class="text-center">
        <div class="w-24 h-24 mx-auto mb-2 rounded-full bg-gradient-to-br {podiumColors[0]} flex items-center justify-center text-dark-900 text-4xl font-bold shadow-lg shadow-yellow-500/30">
          {data.leaderboard[0].username.charAt(0).toUpperCase()}
        </div>
        <div class="text-5xl mb-1">{podiumIcons[0]}</div>
        <div class="text-white font-bold text-lg">{data.leaderboard[0].username}</div>
        <div class="text-neon-green font-semibold">{data.leaderboard[0].totalXp} XP</div>
        <div class="bg-dark-700 rounded-lg h-24 w-28 mx-auto mt-2"></div>
      </div>

      <!-- 3rd place -->
      <div class="text-center">
        <div class="w-20 h-20 mx-auto mb-2 rounded-full bg-gradient-to-br {podiumColors[2]} flex items-center justify-center text-dark-900 text-3xl font-bold">
          {data.leaderboard[2].username.charAt(0).toUpperCase()}
        </div>
        <div class="text-4xl mb-1">{podiumIcons[2]}</div>
        <div class="text-white font-semibold">{data.leaderboard[2].username}</div>
        <div class="text-neon-green text-sm">{data.leaderboard[2].totalXp} XP</div>
        <div class="bg-dark-700 rounded-lg h-12 w-24 mx-auto mt-2"></div>
      </div>
    </div>
  {/if}

  <!-- Current user rank (if not in top 50) -->
  {#if data.currentUserRank}
    <div class="card mb-6 border-neon-cyan">
      <div class="flex items-center justify-between">
        <div class="flex items-center space-x-4">
          <div class="text-2xl font-bold text-gray-400">#{data.currentUserRank}</div>
          <div>
            <div class="text-white font-semibold">Ton classement</div>
            <div class="text-sm text-gray-400">Continue pour monter!</div>
          </div>
        </div>
      </div>
    </div>
  {/if}

  <!-- Full leaderboard -->
  <div class="card">
    <table class="w-full">
      <thead>
        <tr class="text-left text-gray-400 text-sm border-b border-dark-600">
          <th class="pb-3 font-medium">Rang</th>
          <th class="pb-3 font-medium">Joueur</th>
          <th class="pb-3 font-medium text-center">Niveau</th>
          <th class="pb-3 font-medium text-center">Exercices</th>
          <th class="pb-3 font-medium text-center">Badges</th>
          <th class="pb-3 font-medium text-right">XP</th>
        </tr>
      </thead>
      <tbody>
        {#each data.leaderboard as user, i}
          <tr class="border-b border-dark-700 {user.isCurrentUser ? 'bg-neon-cyan/10' : ''} hover:bg-dark-700 transition-colors">
            <td class="py-4">
              {#if user.rank <= 3}
                <span class="text-2xl">{podiumIcons[user.rank - 1]}</span>
              {:else}
                <span class="text-gray-400 font-semibold">#{user.rank}</span>
              {/if}
            </td>
            <td class="py-4">
              <div class="flex items-center space-x-3">
                <div class="w-10 h-10 rounded-full bg-gradient-to-br from-neon-cyan to-neon-green flex items-center justify-center text-dark-900 font-bold">
                  {user.username.charAt(0).toUpperCase()}
                </div>
                <div>
                  <div class="font-semibold {user.isCurrentUser ? 'text-neon-cyan' : 'text-white'}">
                    {user.username}
                    {#if user.isCurrentUser}
                      <span class="text-xs ml-1">(toi)</span>
                    {/if}
                  </div>
                  <div class="text-xs text-gray-500">{getTitleForLevel(user.level)}</div>
                </div>
              </div>
            </td>
            <td class="py-4 text-center">
              <span class="px-2 py-1 rounded bg-dark-600 text-white text-sm font-semibold">
                {user.level}
              </span>
            </td>
            <td class="py-4 text-center text-gray-400">
              {user.exercisesCompleted}
            </td>
            <td class="py-4 text-center text-gray-400">
              {user.badgeCount}
            </td>
            <td class="py-4 text-right">
              <span class="text-neon-green font-bold">{user.totalXp}</span>
            </td>
          </tr>
        {/each}
      </tbody>
    </table>

    {#if data.leaderboard.length === 0}
      <div class="text-center py-12">
        <span class="text-4xl mb-4 block">🏆</span>
        <p class="text-gray-400">Aucun joueur pour le moment.</p>
        <p class="text-gray-500 text-sm">Sois le premier!</p>
      </div>
    {/if}
  </div>
</div>
