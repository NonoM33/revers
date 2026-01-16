# Detection par SIGTRAP et exceptions

<div class="lesson-objectives">

## 🎯 Objectifs de cette lecon

A la fin de cette lecon, tu sauras :
- Comprendre ce qu'est un signal Unix et comment il fonctionne
- Expliquer le mecanisme de SIGTRAP et pourquoi les debuggers l'interceptent
- Connaitre setjmp/longjmp et leur role dans la detection
- Utiliser 5 methodes differentes pour bypasser cette protection

</div>

<div class="lesson-prerequisites">

## 📚 Prerequis

Avant de commencer, assure-toi de :
- Avoir compris les lecons precedentes (ptrace, sysctl, getppid, isatty)
- Connaitre les bases de la gestion des erreurs en C
- Savoir utiliser les commandes de base de LLDB

</div>

## Imagine un systeme d'alarme intelligent...

Tu as un systeme d'alarme chez toi. Quand quelqu'un ouvre une fenetre, l'alarme se declenche et **tu recois une notification** sur ton telephone. Tu peux alors decider quoi faire : appeler la police, ignorer (c'est le chat), etc.

Les **signaux Unix** fonctionnent exactement pareil ! Ce sont des notifications que le systeme envoie aux programmes quand quelque chose se passe.

:::info
Un **signal** est comme un SMS envoye a un programme. Le programme peut :
- **Ignorer** le signal (ne pas lire le SMS)
- **Agir** selon un comportement par defaut
- **Installer un handler** = une fonction qui sera appelee quand le signal arrive (comme programmer une reponse automatique)
:::

## Qu'est-ce que SIGTRAP ?

`SIGTRAP` est un signal special. Son nom vient de "**TRAP**" (piege). Il est envoye quand :

| Situation | Ce qui se passe |
|-----------|-----------------|
| Breakpoint touche | L'instruction `BRK` (ARM) ou `INT 3` (Intel) est executee |
| Single-step | On avance instruction par instruction |
| Exception de debug | Le CPU detecte une condition de debug |

:::tip
Tu peux voir SIGTRAP comme le **signal d'alarme des debuggers**. C'est comme ca que LLDB sait quand s'arreter a un breakpoint !
:::

## Le piege astucieux

Voici l'idee geniale des developpeurs : **generer volontairement un SIGTRAP et verifier qui le recoit**.

### Sans debugger :

```
Programme : "Je m'envoie un SIGTRAP a moi-meme"
         ↓
Mon handler : "Signal recu ! Je fais mon traitement"
         ↓
Programme : "Mon handler a bien recu le signal, tout va bien"
```

### Avec debugger :

```
Programme : "Je m'envoie un SIGTRAP a moi-meme"
         ↓
Debugger : "Oh un SIGTRAP ! C'est pour moi, je le capture"
         ↓
Mon handler : "..." (jamais appele !)
         ↓
Programme : "Mon handler n'a rien recu... INTRUS !"
```

:::warning
Le debugger **intercepte** SIGTRAP avant que ton handler ne le recoive. C'est son job ! Mais ca permet au programme de detecter sa presence.
:::

## Le code explique en detail

Voici le code complet avec des explications :

```c
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>

// Variables globales
static jmp_buf jump_buffer;  // Notre "checkpoint"
static int trap_caught = 0;  // Le signal a-t-il ete capture ?

// Notre handler personnalise
void sigtrap_handler(int sig) {
    trap_caught = 1;                    // Marquer que le signal est arrive
    longjmp(jump_buffer, 1);            // Retourner au checkpoint
}

int main() {
    // Etape 1 : Installer notre handler
    signal(SIGTRAP, sigtrap_handler);

    // Etape 2 : Creer un checkpoint
    if (setjmp(jump_buffer) == 0) {
        // Premier passage : on arrive ici normalement

        // Etape 3 : Generer le signal
        raise(SIGTRAP);

        // Etape 4 : Si on arrive ICI, c'est mauvais signe !
        // Ca veut dire que raise() s'est execute mais
        // notre handler n'a PAS ete appele (le debugger l'a vole)
        printf("Debugger detecte!\n");
        return 1;
    }

    // On arrive ici UNIQUEMENT via longjmp
    // = notre handler a fonctionne = pas de debugger
    if (trap_caught) {
        printf("Pas de debugger, voici le flag!\n");
        show_flag();
    }

    return 0;
}
```

:::example
**Decomposition pas a pas :**

1. **`signal(SIGTRAP, sigtrap_handler)`** : On dit au systeme "Quand tu recois SIGTRAP, appelle ma fonction"
2. **`setjmp(jump_buffer)`** : On cree un "checkpoint" (comme dans un jeu video)
3. **`raise(SIGTRAP)`** : On s'envoie le signal a soi-meme
4. **Dans le handler** : `longjmp` nous teleporte au checkpoint
5. **Retour au checkpoint** : Cette fois `setjmp` retourne 1, pas 0

Si le debugger intercepte le signal, on ne passe jamais dans le handler, donc on continue apres `raise()` et on detecte le debugger !
:::

## setjmp / longjmp : le systeme de checkpoint

Ces deux fonctions forment un duo puissant, comme un systeme de **sauvegarde/chargement** dans un jeu :

```c
jmp_buf buffer;  // L'emplacement de sauvegarde

if (setjmp(buffer) == 0) {
    // PREMIERE FOIS : setjmp() retourne 0
    // C'est comme "sauvegarder la partie"
    printf("Partie sauvegardee !\n");

    // ... du code qui peut appeler longjmp ...
}
else {
    // APRES longjmp : on "charge la sauvegarde"
    // setjmp() retourne la valeur passee a longjmp
    printf("Partie chargee !\n");
}
```

:::info
**Analogie du jeu video :**
- `setjmp()` = Appuyer sur "Sauvegarder"
- `longjmp()` = Appuyer sur "Charger la sauvegarde"
- Le `jmp_buf` = Le fichier de sauvegarde

Quand tu charges, tu reviens exactement ou tu etais, mais tu "sais" que tu as charge (le retour de setjmp change).
:::

## Les signaux importants a connaitre

| Signal | Numero | Description | Utilisation anti-debug |
|--------|--------|-------------|----------------------|
| **SIGTRAP** | 5 | Trap de debug | Detection de debugger |
| **SIGSTOP** | 17 | Pause forcee | Ne peut pas etre intercepte |
| **SIGINT** | 2 | Ctrl+C | Interruption utilisateur |
| **SIGSEGV** | 11 | Violation memoire | Parfois utilise comme piege |
| **SIGKILL** | 9 | Mort immediate | Ne peut pas etre intercepte |

:::tip
SIGKILL et SIGSTOP sont **impossibles a capturer**. Ils vont toujours directement au processus. C'est une securite du systeme.
:::

## Les 5 methodes pour bypasser

### Methode 1 : Configurer LLDB pour passer le signal

La methode la plus elegante : dire a LLDB de **ne pas intercepter** SIGTRAP.

```bash
lldb ./challenge
(lldb) target create ./challenge
(lldb) process launch --stop-at-entry   # Lancer mais s'arreter immediatement
(lldb) process handle SIGTRAP -p true -s false
# -p true  = passer le signal au programme
# -s false = ne pas stopper quand il arrive
(lldb) c                                 # Continuer
```

:::info
**Explication des options :**
- **`-p true`** (pass) : Le signal va au programme, pas au debugger
- **`-s false`** (stop) : LLDB ne s'arrete pas quand il voit ce signal

Avec ces options, SIGTRAP se comporte comme si LLDB n'existait pas !
:::

### Methode 2 : Configuration alternative

```bash
lldb ./challenge
(lldb) settings set target.process.unwind-on-error-in-expression false
(lldb) process handle SIGTRAP --stop false --pass true
(lldb) r
```

### Methode 3 : Breakpoint sur le handler

Si le handler est appele, tout va bien ! Donc on peut juste verifier qu'il fonctionne :

```bash
lldb ./challenge
(lldb) b sigtrap_handler        # Breakpoint sur le handler
(lldb) r

# Si le breakpoint est touche = le handler est appele = succes !
(lldb) c
```

:::warning
Cette methode ne "bypass" pas vraiment, elle te permet juste de voir si ta config fonctionne. Combine-la avec la methode 1.
:::

### Methode 4 : Skipper l'appel a raise

On empeche completement l'envoi du signal :

```bash
lldb ./challenge
(lldb) b raise              # Breakpoint sur raise()
(lldb) r

# Quand raise est appele :
(lldb) thread return 0      # Retourner sans executer
(lldb) c
```

:::tip
`thread return 0` fait croire que `raise()` s'est execute et a retourne 0 (succes), mais en realite le signal n'a jamais ete envoye !
:::

### Methode 5 : Patcher le binaire (permanent)

Dans Binary Ninja, plusieurs options :

**Option A : NOP l'appel a raise**
```asm
; Avant
BL    _raise        ; Appeler raise()

; Apres
NOP                 ; Ne rien faire
NOP
NOP
NOP
```

**Option B : Changer le signal**
```asm
; Avant
MOV   W0, #5        ; SIGTRAP = 5
BL    _raise

; Apres
MOV   W0, #0        ; Signal 0 = invalide, ignore
BL    _raise
```

**Option C : Forcer trap_caught**
```bash
(lldb) p &trap_caught         # Trouver l'adresse de la variable
(lldb) memory write 0x... 1   # La forcer a 1
(lldb) c
```

## Variations avancees

Les developpeurs utilisent parfois des variantes plus difficiles a detecter :

| Technique | Description | Bypass |
|-----------|-------------|--------|
| `__builtin_trap()` | Genere un trap sans appeler raise | NOP ou hook |
| `asm("brk #0")` | Instruction breakpoint directe (ARM) | NOP |
| `asm("int3")` | Instruction breakpoint directe (Intel) | NOP |
| Exception C++ | try/catch avec throw | Modifier le flow |

:::example
**Instruction `BRK` directe :**

```c
// Au lieu de raise(SIGTRAP), le code fait :
asm volatile("brk #0");  // Genere SIGTRAP directement
```

C'est plus difficile a trouver car il n'y a pas d'appel a une fonction. Cherche les instructions `BRK` dans Binary Ninja !
:::

<div class="lesson-summary">

## 📌 Resume / A retenir

:::remember
- **SIGTRAP** = signal envoye lors d'un breakpoint ou trap
- Les **debuggers interceptent** SIGTRAP avant le programme
- **setjmp/longjmp** = systeme de checkpoint (sauvegarde/chargement)
- Le programme peut **detecter** si son handler a ete appele
- **Bypass principal** : `process handle SIGTRAP -p true -s false`
- **Bypass alternatif** : `thread return 0` sur raise ou NOP dans le binaire
- Chercher les appels a **raise**, **signal**, et les instructions **BRK**
:::

</div>

## Exercice pratique

Le binaire utilise la technique SIGTRAP pour te pieger. Ta mission :

1. Identifie le mecanisme (cherche `raise`, `signal`, `SIGTRAP`)
2. Configure LLDB pour passer les signaux correctement
3. OU patch le binaire pour eliminer le check
4. Recupere le flag !

:::tip
La **methode 1** (configurer `process handle`) est la plus propre. Elle ne modifie pas le comportement du programme, elle dit juste a LLDB de ne pas etre "trop curieux" !
:::

:::warning
Si tu utilises des breakpoints apres avoir configure `process handle`, fais attention : les breakpoints generent aussi SIGTRAP ! Utilise le moins de breakpoints possible ou place-les strategiquement.
:::

Bonne chance ! 🪤
