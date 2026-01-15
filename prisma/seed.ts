import { PrismaClient } from '@prisma/client';
import bcrypt from 'bcryptjs';
import crypto from 'crypto';

const prisma = new PrismaClient();

// Helper to hash flags
function hashFlag(flag: string): string {
  return crypto.createHash('sha256').update(flag).digest('hex');
}

async function main() {
  console.log('Seeding database...');

  // ══════════════════════════════════════════════════════════════
  // UTILISATEUR INITIAL
  // ══════════════════════════════════════════════════════════════
  const passwordHash = await bcrypt.hash('24536Tetr@', 10);

  const user = await prisma.user.upsert({
    where: { email: 'renaudlemagicien@gmail.com' },
    update: {},
    create: {
      email: 'renaudlemagicien@gmail.com',
      passwordHash,
      username: 'RenaudLeMagicien',
      totalXp: 0,
      level: 1,
    },
  });
  console.log('Created user:', user.username);

  // ══════════════════════════════════════════════════════════════
  // BADGES
  // ══════════════════════════════════════════════════════════════
  const badges = [
    // Module badges
    { slug: 'ptrace-hunter', name: 'Chasseur de ptrace', description: 'Complete le module Anti-Debug Basics', category: 'MODULE', rarity: 'COMMON', xpBonus: 50 },
    { slug: 'distributed-mind', name: 'Esprit Distribue', description: 'Complete le module Checks Distribues', category: 'MODULE', rarity: 'COMMON', xpBonus: 50 },
    { slug: 'hash-breaker', name: 'Briseur de Hash', description: 'Complete le module Hash & Comparaisons', category: 'MODULE', rarity: 'UNCOMMON', xpBonus: 75 },
    { slug: 'decoder', name: 'Decodeur', description: 'Complete le module Obfuscation Legere', category: 'MODULE', rarity: 'UNCOMMON', xpBonus: 75 },
    { slug: 'patch-artist', name: 'Artiste du Patch', description: 'Complete le module Patch Propre vs Rapide', category: 'MODULE', rarity: 'UNCOMMON', xpBonus: 75 },
    { slug: 'crypto-initiate', name: 'Initie Crypto', description: 'Complete le module Crypto Basics', category: 'MODULE', rarity: 'RARE', xpBonus: 100 },
    { slug: 'anti-anti-debug', name: 'Anti-Anti-Debug', description: 'Complete le module Anti-Debug Avance', category: 'MODULE', rarity: 'RARE', xpBonus: 100 },
    { slug: 'runtime-guardian', name: 'Gardien Runtime', description: 'Complete le module Protections Runtime', category: 'MODULE', rarity: 'RARE', xpBonus: 100 },
    { slug: 'metamorph', name: 'Metamorphe', description: 'Complete le module Code Self-Modifying', category: 'MODULE', rarity: 'EPIC', xpBonus: 150 },
    { slug: 'obfuscation-master', name: 'Maitre de l\'Obfuscation', description: 'Complete le module Obfuscation Avancee', category: 'MODULE', rarity: 'EPIC', xpBonus: 150 },
    { slug: 'packer-slayer', name: 'Tueur de Packers', description: 'Complete le module Packers & Unpackers', category: 'MODULE', rarity: 'EPIC', xpBonus: 150 },
    { slug: 'crypto-breaker', name: 'Casseur de Crypto', description: 'Complete le module Crypto Avancee', category: 'MODULE', rarity: 'EPIC', xpBonus: 200 },
    { slug: 'tamper-proof', name: 'Anti-Tampering', description: 'Complete le module Anti-Tampering', category: 'MODULE', rarity: 'LEGENDARY', xpBonus: 250 },
    { slug: 'vm-slayer', name: 'Tueur de VMs', description: 'Complete le module Virtualisation & VMs', category: 'MODULE', rarity: 'LEGENDARY', xpBonus: 300 },
    { slug: 'kernel-master', name: 'Maitre du Kernel', description: 'Complete le module Kernel & Syscalls', category: 'MODULE', rarity: 'LEGENDARY', xpBonus: 300 },
    { slug: 're-legend', name: 'Legende du RE', description: 'Complete le module Real-World Apps', category: 'MODULE', rarity: 'LEGENDARY', xpBonus: 500 },

    // Achievement badges
    { slug: 'first-blood', name: 'First Blood', description: 'Premier a resoudre un exercice', category: 'ACHIEVEMENT', rarity: 'RARE', xpBonus: 100 },
    { slug: 'speed-demon', name: 'Speed Demon', description: 'Resous un exercice en moins de 5 minutes', category: 'ACHIEVEMENT', rarity: 'UNCOMMON', xpBonus: 50 },
    { slug: 'no-hints-warrior', name: 'Guerrier Sans Indices', description: 'Complete 10 exercices sans utiliser d\'indice', category: 'ACHIEVEMENT', rarity: 'RARE', xpBonus: 100 },
    { slug: 'perfectionist', name: 'Perfectionniste', description: 'Complete un module entier sans erreur de soumission', category: 'ACHIEVEMENT', rarity: 'EPIC', xpBonus: 200 },
    { slug: 'night-owl', name: 'Hibou Nocturne', description: 'Resous un exercice entre 2h et 5h du matin', category: 'ACHIEVEMENT', rarity: 'UNCOMMON', xpBonus: 25 },
    { slug: 'completionist', name: 'Completionniste', description: 'Complete TOUS les exercices de la plateforme', category: 'ACHIEVEMENT', rarity: 'LEGENDARY', xpBonus: 1000 },

    // Streak badges
    { slug: 'streak-7', name: 'Une Semaine de Feu', description: 'Maintiens un streak de 7 jours', category: 'STREAK', rarity: 'COMMON', xpBonus: 50 },
    { slug: 'streak-14', name: 'Deux Semaines', description: 'Maintiens un streak de 14 jours', category: 'STREAK', rarity: 'UNCOMMON', xpBonus: 100 },
    { slug: 'streak-30', name: 'Mois Ininterrompu', description: 'Maintiens un streak de 30 jours', category: 'STREAK', rarity: 'RARE', xpBonus: 200 },
    { slug: 'streak-60', name: 'Deux Mois de Feu', description: 'Maintiens un streak de 60 jours', category: 'STREAK', rarity: 'EPIC', xpBonus: 300 },
    { slug: 'streak-100', name: 'Centurion', description: 'Maintiens un streak de 100 jours', category: 'STREAK', rarity: 'LEGENDARY', xpBonus: 500 },

    // Level badges
    { slug: 'level-10', name: 'Apprenti', description: 'Atteins le niveau 10', category: 'SKILL', rarity: 'COMMON', xpBonus: 50 },
    { slug: 'level-25', name: 'Confirme', description: 'Atteins le niveau 25', category: 'SKILL', rarity: 'UNCOMMON', xpBonus: 100 },
    { slug: 'level-50', name: 'Expert', description: 'Atteins le niveau 50', category: 'SKILL', rarity: 'EPIC', xpBonus: 300 },
    { slug: 'level-100', name: 'Transcendance', description: 'Atteins le niveau 100', category: 'SKILL', rarity: 'LEGENDARY', xpBonus: 1000 },
  ];

  for (const badge of badges) {
    await prisma.badge.upsert({
      where: { slug: badge.slug },
      update: badge,
      create: badge,
    });
  }
  console.log(`Created ${badges.length} badges`);

  // ══════════════════════════════════════════════════════════════
  // MODULES
  // ══════════════════════════════════════════════════════════════
  const modules = [
    // Phase 1: Fondations
    { slug: 'anti-debug-basics', title: 'Anti-Debug Basics', description: 'Decouvre les techniques de detection de debugger classiques et apprends a les contourner.', category: 'protection', difficulty: 1, orderIndex: 1, requiredXp: 0, xpReward: 150, badgeSlug: 'ptrace-hunter' },
    { slug: 'distributed-checks', title: 'Checks Distribues', description: 'Analyse du code ou la logique de validation est repartie sur plusieurs fonctions.', category: 'analysis', difficulty: 2, orderIndex: 2, requiredXp: 100, xpReward: 200, badgeSlug: 'distributed-mind' },
    { slug: 'hash-comparisons', title: 'Hash & Comparaisons', description: 'Identifie quand une comparaison directe est remplacee par un hash et apprends a le reverser.', category: 'crypto', difficulty: 2, orderIndex: 3, requiredXp: 200, xpReward: 250, badgeSlug: 'hash-breaker' },
    { slug: 'light-obfuscation', title: 'Obfuscation Legere', description: 'Reconnais les patterns d\'obfuscation simples: XOR, substitution, encodage basique.', category: 'obfuscation', difficulty: 2, orderIndex: 4, requiredXp: 300, xpReward: 250, badgeSlug: 'decoder' },

    // Phase 2: Intermediaire
    { slug: 'clean-patching', title: 'Patch Propre vs Rapide', description: 'Apprends la difference entre un patch quick & dirty et un patch propre qui preserve l\'integrite.', category: 'patching', difficulty: 3, orderIndex: 5, requiredXp: 500, xpReward: 300, badgeSlug: 'patch-artist' },
    { slug: 'crypto-basics', title: 'Crypto Basics', description: 'Identifie les algorithmes crypto courants (XOR, RC4, TEA) et extrait les cles.', category: 'crypto', difficulty: 3, orderIndex: 6, requiredXp: 700, xpReward: 350, badgeSlug: 'crypto-initiate' },
    { slug: 'anti-debug-advanced', title: 'Anti-Debug Avance', description: 'Timing attacks, detection single-step, hardware breakpoints, techniques basees sur exceptions.', category: 'protection', difficulty: 3, orderIndex: 7, requiredXp: 1000, xpReward: 400, badgeSlug: 'anti-anti-debug' },
    { slug: 'runtime-protections', title: 'Protections Runtime', description: 'Checksums de code, CRC verification, detection de modification memoire, stack canaries.', category: 'protection', difficulty: 3, orderIndex: 8, requiredXp: 1300, xpReward: 400, badgeSlug: 'runtime-guardian' },

    // Phase 3: Avance
    { slug: 'self-modifying', title: 'Code Self-Modifying', description: 'Code auto-modifiant, decryptage a l\'execution, reconstruction du code original.', category: 'advanced', difficulty: 4, orderIndex: 9, requiredXp: 1700, xpReward: 500, badgeSlug: 'metamorph' },
    { slug: 'advanced-obfuscation', title: 'Obfuscation Avancee', description: 'Control-flow flattening, Mixed Boolean-Arithmetic, opaque predicates, dead code.', category: 'obfuscation', difficulty: 4, orderIndex: 10, requiredXp: 2200, xpReward: 550, badgeSlug: 'obfuscation-master' },
    { slug: 'packers-unpackers', title: 'Packers & Unpackers', description: 'Fonctionnement des packers, identification de binaires packes, unpacking manuel.', category: 'packing', difficulty: 4, orderIndex: 11, requiredXp: 2800, xpReward: 550, badgeSlug: 'packer-slayer' },
    { slug: 'advanced-crypto', title: 'Crypto Avancee', description: 'AES, DES, RSA, implementations custom, side-channel attacks basiques.', category: 'crypto', difficulty: 4, orderIndex: 12, requiredXp: 3500, xpReward: 600, badgeSlug: 'crypto-breaker' },

    // Phase 4: Expert
    { slug: 'anti-tampering', title: 'Anti-Tampering', description: 'Systemes anti-tampering complets, code signing, self-healing, detection de modification.', category: 'protection', difficulty: 5, orderIndex: 13, requiredXp: 4500, xpReward: 650, badgeSlug: 'tamper-proof' },
    { slug: 'virtualization-vms', title: 'Virtualisation & VMs', description: 'Protections basees sur VM, analyse de bytecode custom, devirtualisation.', category: 'virtualization', difficulty: 5, orderIndex: 14, requiredXp: 5500, xpReward: 750, badgeSlug: 'vm-slayer' },
    { slug: 'kernel-syscalls', title: 'Kernel & Syscalls', description: 'Appels systeme directs, bypass des wrappers, anti-debug kernel level.', category: 'kernel', difficulty: 5, orderIndex: 15, requiredXp: 6500, xpReward: 700, badgeSlug: 'kernel-master' },
    { slug: 'real-world', title: 'Real-World Apps', description: 'Applique toutes tes competences sur des cas realistes de protections commerciales.', category: 'challenge', difficulty: 5, orderIndex: 16, requiredXp: 8000, xpReward: 1000, badgeSlug: 're-legend' },
  ];

  for (const mod of modules) {
    await prisma.module.upsert({
      where: { slug: mod.slug },
      update: mod,
      create: mod,
    });
  }
  console.log(`Created ${modules.length} modules`);

  // ══════════════════════════════════════════════════════════════
  // EXERCICES - Module 1: Anti-Debug Basics
  // ══════════════════════════════════════════════════════════════
  const module1 = await prisma.module.findUnique({ where: { slug: 'anti-debug-basics' } });
  if (module1) {
    const exercises1 = [
      {
        moduleId: module1.id,
        slug: 'antidebug-01-ptrace',
        title: 'Detect ptrace',
        description: 'Ce binaire utilise ptrace(PT_DENY_ATTACH) pour detecter si un debugger est attache. Contourne cette protection et trouve le flag cache.',
        hints: JSON.stringify([
          'Cherche l\'appel a ptrace avec nm ou dans Binary Ninja',
          'ptrace retourne -1 si le processus est deja trace',
          'Tu peux patcher l\'appel ou modifier le retour avec LLDB'
        ]),
        difficulty: 1,
        orderIndex: 1,
        binaryPath: '/binaries/01-anti-debug-basics/01-ptrace/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{PTRACE_BYPASSED_a7f3b2c1}'),
        xpReward: 30,
        bonusXp: 15,
      },
      {
        moduleId: module1.id,
        slug: 'antidebug-02-sysctl',
        title: 'Sysctl Check',
        description: 'Detection via sysctl et le flag P_TRACED. Le binaire verifie si le processus est en cours de debug via une requete kernel.',
        hints: JSON.stringify([
          'Cherche les appels a sysctl',
          'Le flag P_TRACED est dans kinfo_proc.kp_proc.p_flag',
          'Tu peux faker la reponse ou patcher le check'
        ]),
        difficulty: 1,
        orderIndex: 2,
        binaryPath: '/binaries/01-anti-debug-basics/02-sysctl/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{SYSCTL_DEFEATED_b8e4c3d2}'),
        xpReward: 35,
        bonusXp: 15,
      },
      {
        moduleId: module1.id,
        slug: 'antidebug-03-getppid',
        title: 'Parent Process Check',
        description: 'Le binaire verifie si son processus parent est un debugger connu (lldb, gdb, etc.) via getppid.',
        hints: JSON.stringify([
          'Regarde ce qui est compare apres getppid',
          'Les noms de debuggers sont probablement hardcodes',
          'Tu peux renommer ton terminal ou patcher le check'
        ]),
        difficulty: 1,
        orderIndex: 3,
        binaryPath: '/binaries/01-anti-debug-basics/03-getppid/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{PARENT_SPOOFED_c9f5d4e3}'),
        xpReward: 35,
        bonusXp: 15,
      },
      {
        moduleId: module1.id,
        slug: 'antidebug-04-isatty',
        title: 'Terminal Detection',
        description: 'Ce binaire utilise isatty() pour detecter si stdin/stdout sont rediriges, ce qui peut indiquer un environnement de debug.',
        hints: JSON.stringify([
          'isatty() retourne 1 si c\'est un vrai terminal',
          'La redirection de I/O peut declencher la detection',
          'Patch le retour de isatty ou la condition qui suit'
        ]),
        difficulty: 2,
        orderIndex: 4,
        binaryPath: '/binaries/01-anti-debug-basics/04-isatty/challenge',
        validationType: 'AUTO_PATCH',
        expectedFlag: null,
        xpReward: 45,
        bonusXp: 20,
      },
      {
        moduleId: module1.id,
        slug: 'antidebug-05-multi',
        title: 'Multi-Check',
        description: 'Combine ptrace ET sysctl. Tu dois contourner les deux protections pour obtenir le flag.',
        hints: JSON.stringify([
          'Il y a deux checks independants',
          'Les deux doivent echouer pour que tu passes',
          'Utilise les techniques des exercices precedents'
        ]),
        difficulty: 2,
        orderIndex: 5,
        binaryPath: '/binaries/01-anti-debug-basics/05-multi/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{MULTI_BYPASS_d0g6e5f4}'),
        xpReward: 50,
        bonusXp: 25,
      },
      {
        moduleId: module1.id,
        slug: 'antidebug-06-sigtrap',
        title: 'Exception Trap',
        description: 'Utilise SIGTRAP pour detecter le single-stepping. Si un debugger intercepte le signal, le comportement change.',
        hints: JSON.stringify([
          'SIGTRAP est envoye lors d\'un breakpoint',
          'Le programme installe un handler pour SIGTRAP',
          'Le handler modifie une variable globale'
        ]),
        difficulty: 2,
        orderIndex: 6,
        binaryPath: '/binaries/01-anti-debug-basics/06-sigtrap/challenge',
        validationType: 'KEYGEN',
        expectedFlag: null,
        xpReward: 55,
        bonusXp: 25,
      },
    ];

    for (const ex of exercises1) {
      await prisma.exercise.upsert({
        where: { slug: ex.slug },
        update: ex,
        create: ex,
      });
    }
    console.log(`Created ${exercises1.length} exercises for Module 1`);
  }

  // ══════════════════════════════════════════════════════════════
  // EXERCICES - Module 2: Checks Distribues
  // ══════════════════════════════════════════════════════════════
  const module2 = await prisma.module.findUnique({ where: { slug: 'distributed-checks' } });
  if (module2) {
    const exercises2 = [
      {
        moduleId: module2.id,
        slug: 'distributed-01-three-steps',
        title: 'Three Steps',
        description: 'La validation est repartie sur 3 fonctions sequentielles. Chaque fonction verifie une partie de la cle.',
        hints: JSON.stringify([
          'Identifie les 3 fonctions de check',
          'Chaque fonction valide une contrainte differente',
          'Le retour de chaque fonction affecte la variable valid'
        ]),
        difficulty: 1,
        orderIndex: 1,
        binaryPath: '/binaries/02-distributed-checks/01-three-steps/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{THREE_STEPS_e1h7f6g5}'),
        xpReward: 35,
        bonusXp: 15,
      },
      {
        moduleId: module2.id,
        slug: 'distributed-02-inverted',
        title: 'Inverted Logic',
        description: 'Attention! Une des conditions est inversee. Si la fonction retourne true, c\'est un echec.',
        hints: JSON.stringify([
          'Lis attentivement comment chaque retour est utilise',
          'Un des checks fonctionne a l\'inverse',
          'Cherche les if(!check) vs if(check)'
        ]),
        difficulty: 2,
        orderIndex: 2,
        binaryPath: '/binaries/02-distributed-checks/02-inverted/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{INVERTED_MIND_f2i8g7h6}'),
        xpReward: 50,
        bonusXp: 20,
      },
      {
        moduleId: module2.id,
        slug: 'distributed-03-global',
        title: 'Global State',
        description: 'Les checks modifient une variable globale qui est verifiee a la fin. Trouve la bonne combinaison.',
        hints: JSON.stringify([
          'Cherche les variables globales modifiees',
          'La valeur finale doit atteindre un certain seuil',
          'Chaque check ajoute ou soustrait a la variable'
        ]),
        difficulty: 2,
        orderIndex: 3,
        binaryPath: '/binaries/02-distributed-checks/03-global/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{GLOBAL_STATE_g3j9h8i7}'),
        xpReward: 55,
        bonusXp: 25,
      },
      {
        moduleId: module2.id,
        slug: 'distributed-04-callbacks',
        title: 'Callback Maze',
        description: 'La validation utilise des pointeurs de fonction. Le flow n\'est pas lineaire!',
        hints: JSON.stringify([
          'Identifie le tableau de pointeurs de fonction',
          'Trace quelles fonctions sont appelees',
          'L\'ordre d\'appel peut dependre de l\'input'
        ]),
        difficulty: 2,
        orderIndex: 4,
        binaryPath: '/binaries/02-distributed-checks/04-callbacks/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{CALLBACK_MAZE_h4k0i9j8}'),
        xpReward: 60,
        bonusXp: 25,
      },
      {
        moduleId: module2.id,
        slug: 'distributed-05-herring',
        title: 'Red Herring',
        description: 'Plusieurs fonctions ressemblent a des checks mais sont des leurres. Trouve les vraies validations.',
        hints: JSON.stringify([
          'Certaines fonctions ne font rien d\'utile',
          'Suis le flow: qui appelle quoi?',
          'Les vraies fonctions modifient l\'etat'
        ]),
        difficulty: 3,
        orderIndex: 5,
        binaryPath: '/binaries/02-distributed-checks/05-herring/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{RED_HERRING_i5l1j0k9}'),
        xpReward: 70,
        bonusXp: 30,
      },
      {
        moduleId: module2.id,
        slug: 'distributed-06-order',
        title: 'Order Matters',
        description: 'L\'ordre d\'appel des fonctions change le resultat. Trouve la bonne sequence.',
        hints: JSON.stringify([
          'Les fonctions modifient un etat partage',
          'L\'ordre A-B-C donne un resultat different de B-A-C',
          'Il y a une seule sequence valide'
        ]),
        difficulty: 3,
        orderIndex: 6,
        binaryPath: '/binaries/02-distributed-checks/06-order/challenge',
        validationType: 'KEYGEN',
        expectedFlag: null,
        xpReward: 75,
        bonusXp: 35,
      },
      {
        moduleId: module2.id,
        slug: 'distributed-07-recursive',
        title: 'Recursive Check',
        description: 'Validation recursive avec conditions imbriquees. Chaque niveau verifie une partie.',
        hints: JSON.stringify([
          'La fonction s\'appelle elle-meme',
          'Chaque niveau de recursion check un caractere',
          'La profondeur maximale te donne la longueur'
        ]),
        difficulty: 3,
        orderIndex: 7,
        binaryPath: '/binaries/02-distributed-checks/07-recursive/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{RECURSIVE_j6m2k1l0}'),
        xpReward: 80,
        bonusXp: 35,
      },
    ];

    for (const ex of exercises2) {
      await prisma.exercise.upsert({
        where: { slug: ex.slug },
        update: ex,
        create: ex,
      });
    }
    console.log(`Created ${exercises2.length} exercises for Module 2`);
  }

  // ══════════════════════════════════════════════════════════════
  // EXERCICES - Module 3: Hash & Comparaisons
  // ══════════════════════════════════════════════════════════════
  const module3 = await prisma.module.findUnique({ where: { slug: 'hash-comparisons' } });
  if (module3) {
    const exercises3 = [
      {
        moduleId: module3.id,
        slug: 'hash-01-simple',
        title: 'Simple Hash',
        description: 'Un hash maison simple (somme des caracteres). Reverse le pour trouver un input valide.',
        hints: JSON.stringify([
          'Le hash est juste la somme des valeurs ASCII',
          'Tu connais la valeur cible du hash',
          'Plusieurs inputs peuvent donner le meme hash'
        ]),
        difficulty: 1,
        orderIndex: 1,
        binaryPath: '/binaries/03-hash-comparisons/01-simple/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{SIMPLE_HASH_k7n3l2m1}'),
        xpReward: 35,
        bonusXp: 15,
      },
      {
        moduleId: module3.id,
        slug: 'hash-02-xor',
        title: 'XOR Compare',
        description: 'La comparaison se fait via XOR avec une constante. Si le resultat est 0, c\'est valide.',
        hints: JSON.stringify([
          'XOR de deux valeurs identiques = 0',
          'Trouve la constante XOR',
          'Applique XOR pour retrouver l\'input'
        ]),
        difficulty: 1,
        orderIndex: 2,
        binaryPath: '/binaries/03-hash-comparisons/02-xor/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{XOR_COMPARE_l8o4m3n2}'),
        xpReward: 40,
        bonusXp: 20,
      },
      {
        moduleId: module3.id,
        slug: 'hash-03-lookup',
        title: 'Lookup Table',
        description: 'Une table de substitution transforme l\'input avant comparaison.',
        hints: JSON.stringify([
          'Trouve la table dans le binaire',
          'Chaque caractere est substitue',
          'Inverse la table pour decoder'
        ]),
        difficulty: 2,
        orderIndex: 3,
        binaryPath: '/binaries/03-hash-comparisons/03-lookup/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{LOOKUP_TABLE_m9p5n4o3}'),
        xpReward: 50,
        bonusXp: 25,
      },
      {
        moduleId: module3.id,
        slug: 'hash-04-crc32',
        title: 'CRC32 Check',
        description: 'Le binaire verifie le CRC32 de l\'input. Trouve une valeur avec le bon CRC.',
        hints: JSON.stringify([
          'CRC32 est un algorithme connu',
          'Tu peux bruteforcer des inputs courts',
          'Ou modifier quelques caracteres pour ajuster le CRC'
        ]),
        difficulty: 2,
        orderIndex: 4,
        binaryPath: '/binaries/03-hash-comparisons/04-crc32/challenge',
        validationType: 'KEYGEN',
        expectedFlag: null,
        xpReward: 55,
        bonusXp: 25,
      },
      {
        moduleId: module3.id,
        slug: 'hash-05-md5prefix',
        title: 'MD5 Prefix',
        description: 'Trouve un input dont le MD5 commence par "deadbeef".',
        hints: JSON.stringify([
          'C\'est du bruteforce intelligent',
          'Genere des inputs aleatoires',
          'Verifie le prefixe du hash'
        ]),
        difficulty: 3,
        orderIndex: 5,
        binaryPath: '/binaries/03-hash-comparisons/05-md5prefix/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{MD5_PREFIX_n0q6o5p4}'),
        xpReward: 70,
        bonusXp: 30,
      },
      {
        moduleId: module3.id,
        slug: 'hash-06-multi',
        title: 'Multi Hash',
        description: 'Plusieurs algorithmes de hash combines. L\'input doit satisfaire tous les checks.',
        hints: JSON.stringify([
          'Identifie chaque algorithme utilise',
          'Les contraintes se cumulent',
          'Commence par la contrainte la plus restrictive'
        ]),
        difficulty: 3,
        orderIndex: 6,
        binaryPath: '/binaries/03-hash-comparisons/06-multi/challenge',
        validationType: 'KEYGEN',
        expectedFlag: null,
        xpReward: 75,
        bonusXp: 35,
      },
      {
        moduleId: module3.id,
        slug: 'hash-07-timing',
        title: 'Timing Hash',
        description: 'Le hash a un timing variable selon les caracteres. Exploite ce side-channel!',
        hints: JSON.stringify([
          'Le temps de calcul revele des infos',
          'Mesure le temps pour chaque caractere',
          'Un caractere correct prend plus/moins de temps'
        ]),
        difficulty: 3,
        orderIndex: 7,
        binaryPath: '/binaries/03-hash-comparisons/07-timing/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{TIMING_ATTACK_o1r7p6q5}'),
        xpReward: 80,
        bonusXp: 35,
      },
    ];

    for (const ex of exercises3) {
      await prisma.exercise.upsert({
        where: { slug: ex.slug },
        update: ex,
        create: ex,
      });
    }
    console.log(`Created ${exercises3.length} exercises for Module 3`);
  }

  // ══════════════════════════════════════════════════════════════
  // EXERCICES - Module 4: Obfuscation Legere
  // ══════════════════════════════════════════════════════════════
  const module4 = await prisma.module.findUnique({ where: { slug: 'light-obfuscation' } });
  if (module4) {
    const exercises4 = [
      {
        moduleId: module4.id,
        slug: 'obf-01-xorstring',
        title: 'XOR String',
        description: 'Les strings sont XOR avec une cle fixe. Decode le flag cache.',
        hints: JSON.stringify([
          'Trouve la cle XOR dans le binaire',
          'La cle peut etre un seul octet ou plusieurs',
          'XOR est reversible: encode = decode'
        ]),
        difficulty: 1,
        orderIndex: 1,
        binaryPath: '/binaries/04-light-obfuscation/01-xorstring/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{XOR_STRINGS_p2s8q7r6}'),
        xpReward: 30,
        bonusXp: 15,
      },
      {
        moduleId: module4.id,
        slug: 'obf-02-caesar',
        title: 'Caesar Cipher',
        description: 'Rotation des caracteres (cipher de Cesar). Trouve le decalage.',
        hints: JSON.stringify([
          'C\'est une simple rotation de l\'alphabet',
          'Teste les 26 decalages possibles',
          'Ou trouve le decalage dans le code'
        ]),
        difficulty: 1,
        orderIndex: 2,
        binaryPath: '/binaries/04-light-obfuscation/02-caesar/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{CAESAR_ROT_q3t9r8s7}'),
        xpReward: 35,
        bonusXp: 15,
      },
      {
        moduleId: module4.id,
        slug: 'obf-03-base64',
        title: 'Base64 Layers',
        description: 'Plusieurs couches de base64. Decode jusqu\'au flag.',
        hints: JSON.stringify([
          'Reconnais le format base64',
          'Decode jusqu\'a obtenir du texte lisible',
          'Le nombre de couches est fixe'
        ]),
        difficulty: 1,
        orderIndex: 3,
        binaryPath: '/binaries/04-light-obfuscation/03-base64/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{BASE64_LAYERS_r4u0s9t8}'),
        xpReward: 35,
        bonusXp: 15,
      },
      {
        moduleId: module4.id,
        slug: 'obf-04-substitution',
        title: 'Substitution',
        description: 'Table de substitution pour les caracteres. Inverse la table.',
        hints: JSON.stringify([
          'Chaque caractere est remplace par un autre',
          'La table est quelque part dans le binaire',
          'Construis la table inverse'
        ]),
        difficulty: 2,
        orderIndex: 4,
        binaryPath: '/binaries/04-light-obfuscation/04-substitution/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{SUBSTITUTION_s5v1t0u9}'),
        xpReward: 50,
        bonusXp: 25,
      },
      {
        moduleId: module4.id,
        slug: 'obf-05-stackstrings',
        title: 'Stack Strings',
        description: 'Les strings sont construites caractere par caractere sur la stack.',
        hints: JSON.stringify([
          'Pas de strings dans la section .rodata',
          'Regarde les MOV vers la stack',
          'Reconstruis la string depuis les instructions'
        ]),
        difficulty: 2,
        orderIndex: 5,
        binaryPath: '/binaries/04-light-obfuscation/05-stackstrings/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{STACK_STRINGS_t6w2u1v0}'),
        xpReward: 55,
        bonusXp: 25,
      },
      {
        moduleId: module4.id,
        slug: 'obf-06-arithmetic',
        title: 'Arithmetic Obf',
        description: 'Constantes obfusquees par operations arithmetiques. Simplifie les expressions.',
        hints: JSON.stringify([
          'Les nombres sont calcules dynamiquement',
          'a = ((5 * 3) + 2) ^ 1 au lieu de a = 16',
          'Evalue les expressions ou debug'
        ]),
        difficulty: 2,
        orderIndex: 6,
        binaryPath: '/binaries/04-light-obfuscation/06-arithmetic/challenge',
        validationType: 'KEYGEN',
        expectedFlag: null,
        xpReward: 60,
        bonusXp: 30,
      },
      {
        moduleId: module4.id,
        slug: 'obf-07-mixed',
        title: 'Mixed Encoding',
        description: 'XOR + base64 + substitution combines. Decode dans le bon ordre!',
        hints: JSON.stringify([
          'L\'ordre des operations est important',
          'Identifie chaque couche',
          'Decode dans l\'ordre inverse'
        ]),
        difficulty: 3,
        orderIndex: 7,
        binaryPath: '/binaries/04-light-obfuscation/07-mixed/challenge',
        validationType: 'FLAG',
        expectedFlag: hashFlag('FLAG{MIXED_ENCODING_u7x3v2w1}'),
        xpReward: 75,
        bonusXp: 35,
      },
    ];

    for (const ex of exercises4) {
      await prisma.exercise.upsert({
        where: { slug: ex.slug },
        update: ex,
        create: ex,
      });
    }
    console.log(`Created ${exercises4.length} exercises for Module 4`);
  }

  console.log('Seeding complete!');
}

main()
  .catch((e) => {
    console.error(e);
    process.exit(1);
  })
  .finally(async () => {
    await prisma.$disconnect();
  });
