# Checks multiples combines

<div class="lesson-objectives">

## 🎯 Objectifs de cette lecon

A la fin de cette lecon, tu sauras :
- Comprendre pourquoi les protections sont combinees
- Identifier les patterns de checks multiples dans un binaire
- Utiliser des strategies pour bypasser plusieurs protections a la fois
- Ecrire des scripts LLDB pour automatiser les bypass

</div>

<div class="lesson-prerequisites">

## 📚 Prerequis

**Important** : Cette lecon est une synthese ! Assure-toi d'avoir compris :
- La lecon 1 : ptrace
- La lecon 2 : sysctl
- La lecon 3 : getppid
- La lecon 4 : isatty

</div>

## Imagine un coffre-fort a plusieurs serrures...

Un coffre-fort avec **une seule serrure** est facile a crocheter. Mais un coffre avec **3 serrures differentes** ? Il faut les crocheter toutes les trois !

:::info
C'est exactement la strategie des protections combinees : meme si tu sais bypasser une technique, tu dois toutes les bypasser pour reussir.
:::

## Pourquoi combiner les protections ?

Chaque technique anti-debug a ses **faiblesses** :

| Technique | Faiblesse principale |
|-----------|---------------------|
| ptrace | Un seul breakpoint suffit |
| sysctl | Modifier le resultat est facile |
| getppid | Script wrapper contourne tout |
| isatty | LLDB preserve souvent le TTY |

**Solution** : Les combiner ! Si tu bypasses ptrace mais oublies sysctl → echec.

## Architecture typique d'un programme protege

Voici comment les developpeurs structurent leurs protections :

```c
int main() {
    // Check 1 : ptrace
    if (check_ptrace()) {
        printf("Debugger detecte (ptrace)!\n");
        exit(1);
    }

    // Check 2 : sysctl
    if (check_sysctl()) {
        printf("Debugger detecte (sysctl)!\n");
        exit(1);
    }

    // Check 3 : timing
    if (check_timing()) {
        printf("Debugger detecte (timing)!\n");
        exit(1);
    }

    // Si TOUS les checks sont passes
    printf("Bravo ! Voici le flag : FLAG{...}\n");
}
```

:::warning
L'ordre des checks est souvent **aleatoire** ou **cache**. Tu ne sais pas toujours lequel va echouer en premier !
:::

## Le check de timing : nouveau concept !

Le **timing check** est une technique qu'on n'a pas encore vue. Elle detecte si le programme s'execute trop lentement (a cause des breakpoints).

```c
#include <time.h>

int check_timing() {
    clock_t start = clock();

    // Code qui devrait etre TRES rapide
    volatile int sum = 0;
    for (int i = 0; i < 100000; i++) {
        sum += i;
    }

    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;

    // Si plus de 0.1 seconde, c'est suspect
    // (normalement ca prend quelques millisecondes)
    return duration > 0.1;
}
```

:::example
**Pourquoi ca marche ?**

- **Execution normale** : La boucle prend ~1ms
- **Avec breakpoint** : Chaque arret prend du temps → detecte !
- **En single-step** : Chaque instruction est lente → detecte !
:::

## Tableau recapitulatif des techniques

| Technique | Ce qu'elle detecte | Bypass rapide |
|-----------|-------------------|---------------|
| ptrace | Attachement debugger | `thread return 0` sur ptrace |
| sysctl P_TRACED | Flag de debug actif | Modifier le resultat |
| getppid | Parent = debugger | Script wrapper |
| isatty | Flux rediriges | `thread return 1` |
| timing | Execution lente | Pas de single-step |

## Les 4 strategies de bypass

### Strategie 1 : Un par un (manuel)

La methode basique : bypasser chaque check individuellement.

```bash
lldb ./challenge
(lldb) b check_ptrace
(lldb) b check_sysctl
(lldb) b check_timing
(lldb) r

# Premier check atteint (ptrace)
(lldb) thread return 0
(lldb) c

# Deuxieme check atteint (sysctl)
(lldb) thread return 0
(lldb) c

# Troisieme check atteint (timing)
(lldb) thread return 0
(lldb) c

# Flag ! 🎉
```

:::warning
Cette methode est **fastidieuse** et tu peux oublier un check. Prefere l'automatisation !
:::

### Strategie 2 : Script LLDB automatique

Cree un script qui bypass tous les checks automatiquement :

```python
# bypass_all.py
import lldb

# Liste des fonctions de check a bypasser
CHECK_FUNCTIONS = [
    'check_ptrace',
    'check_sysctl',
    'check_timing',
    'check_parent',
    'check_terminal',
    'is_debugged',
    'anti_debug'
]

def auto_return_zero(frame, bp_loc, dict):
    """Force toute fonction de check a retourner 0 (pas de debugger)"""
    thread = frame.GetThread()
    # Retourne 0 sans executer la fonction
    return_value = frame.EvaluateExpression("(int)0")
    thread.ReturnFromFrame(frame, return_value)
    print(f"[BYPASS] {frame.GetFunctionName()} -> 0")
    return False  # Ne pas s'arreter

def setup_bypass(debugger, command, result, internal_dict):
    """Configure les breakpoints sur toutes les fonctions de check"""
    target = debugger.GetSelectedTarget()

    for func_name in CHECK_FUNCTIONS:
        bp = target.BreakpointCreateByName(func_name)
        if bp.GetNumLocations() > 0:
            bp.SetScriptCallbackFunction("bypass_all.auto_return_zero")
            print(f"[+] Breakpoint sur {func_name}")
        else:
            print(f"[-] {func_name} non trouve")

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('command script add -f bypass_all.setup_bypass bypass')
    print("Commande 'bypass' ajoutee. Utilise-la apres 'target create'")
```

**Utilisation :**
```bash
lldb ./challenge
(lldb) command script import bypass_all.py
(lldb) bypass
(lldb) r
# Tous les checks sont automatiquement bypasses !
```

:::tip
Ce script est **reutilisable** ! Tu peux l'enrichir avec d'autres noms de fonctions au fur et a mesure que tu decouvres de nouvelles protections.
:::

### Strategie 3 : Patcher le binaire (solution permanente)

Dans Binary Ninja, tu as plusieurs options :

**Option A : NOP tous les appels aux fonctions de check**
```asm
; Avant
BL    _check_ptrace
CBNZ  W0, fail

; Apres
NOP
NOP
NOP
NOP
NOP   ; ou B continue
```

**Option B : Modifier les jumps conditionnels**
```asm
; Avant
CBNZ  W0, debugger_detected

; Apres
CBZ   W0, debugger_detected  ; Inverse la logique
; ou
B     continue               ; Saut inconditionnel
```

**Option C : Modifier une variable globale**

Souvent, il y a un compteur ou flag :

```c
static int checks_passed = 0;

if (!check_ptrace()) checks_passed++;
if (!check_sysctl()) checks_passed++;
if (!check_timing()) checks_passed++;

if (checks_passed == 3) show_flag();
```

Dans LLDB :
```bash
(lldb) p &checks_passed       # Trouver l'adresse
(lldb) memory write 0x... 3   # Forcer la valeur a 3
(lldb) c
```

### Strategie 4 : Trouver le point de decision final

Au lieu de bypasser chaque check, trouve **ou le programme decide** d'afficher le flag ou de quitter.

```bash
(lldb) b exit                 # Breakpoint sur exit()
(lldb) b show_flag            # Breakpoint sur l'affichage du flag
(lldb) r

# Si exit() est atteint, regarde la call stack
(lldb) bt                     # Backtrace
# Tu verras quel check a echoue

# Ou trouve directement show_flag dans Binary Ninja
# et force l'execution a aller la
```

:::example
**Astuce avancee** : Dans Binary Ninja, cherche les strings "FLAG" ou "Bravo". Remonte le graphe de flux pour voir toutes les conditions necessaires.
:::

## Pattern de detection dans Binary Ninja

Quand tu ouvres un binaire avec protections multiples, cherche :

1. **Plusieurs appels a des fonctions de check** au debut du `main`
2. **Des comparaisons avec 0** suivies de sauts vers `exit`
3. **Une section "clean"** a la fin qui affiche le flag
4. **Des imports suspects** : `ptrace`, `sysctl`, `clock`, `getppid`, `isatty`

<div class="lesson-summary">

## 📌 Resume / A retenir

:::remember
- Les protections sont **combinees** car chacune a des faiblesses
- **Timing check** : Detecte si l'execution est trop lente (breakpoints)
- **Bypass manuel** : `thread return 0` sur chaque fonction de check
- **Bypass automatique** : Script LLDB avec breakpoints callbacks
- **Bypass permanent** : NOP les appels ou modifier les jumps
- **Astuce** : Trouve le point de decision final plutot que chaque check
:::

</div>

## Exercice pratique

Ce binaire combine **3 techniques anti-debug** differentes. Ta mission :

1. Identifie les techniques utilisees (ptrace ? sysctl ? timing ? autre ?)
2. Choisis ta strategie : manuelle, script, ou patching
3. Bypass toutes les protections
4. Recupere le flag !

:::tip
Commence par le **script LLDB** - c'est la methode la plus efficace pour les protections multiples. Tu peux toujours affiner ensuite si certains checks ne sont pas detectes.
:::

:::warning
N'oublie pas le **timing check** ! Si tu utilises trop de breakpoints ou de single-step, tu risques de le declencher. Lance le programme d'un coup avec `c` apres avoir configure tes bypass.
:::

Bonne chance ! 🔐🔐🔐
