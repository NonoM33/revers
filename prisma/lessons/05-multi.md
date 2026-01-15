# Checks multiples combines

## Le concept

Les protections simples sont faciles a bypasser une par une. La solution ? **Combiner plusieurs techniques** !

## Architecture typique

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

    // Si on arrive ici, tous les checks sont passes
    show_flag();
}
```

## Les techniques combinees

| Technique | Ce qu'elle detecte |
|-----------|-------------------|
| ptrace | Attachement du debugger |
| sysctl P_TRACED | Etat de debug actif |
| Timing | Execution trop lente (breakpoints) |
| getppid | Parent = debugger |
| isatty | Flux rediriges |

## Le check de timing

Les debuggers ralentissent l'execution (breakpoints, single-step). On peut mesurer :

```c
#include <time.h>

int check_timing() {
    clock_t start = clock();

    // Code qui devrait etre rapide
    volatile int sum = 0;
    for (int i = 0; i < 100000; i++) {
        sum += i;
    }

    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;

    // Si plus de 0.1 seconde, c'est suspect
    return duration > 0.1;
}
```

## Comment bypasser plusieurs checks ?

### Strategie 1 : Un par un

```bash
(lldb) b check_ptrace
(lldb) b check_sysctl
(lldb) b check_timing
(lldb) r

# Pour chaque breakpoint :
(lldb) thread return 0
(lldb) c
```

### Strategie 2 : Bypass global avec script LLDB

```python
# bypass_all.py
import lldb

def bypass_checks(debugger, command, result, internal_dict):
    target = debugger.GetSelectedTarget()
    # Mettre des breakpoints sur toutes les fonctions de check
    checks = ['check_ptrace', 'check_sysctl', 'check_timing']
    for check in checks:
        bp = target.BreakpointCreateByName(check)
        bp.SetScriptCallbackFunction('auto_return_zero')

def auto_return_zero(frame, bp_loc, dict):
    frame.thread.ReturnFromFrame(frame, lldb.SBValue())
    return False
```

### Strategie 3 : Patcher le binaire

Dans Binary Ninja :
1. Trouve la fonction `main` ou l'orchestrateur des checks
2. NOP tous les appels aux fonctions de check
3. Ou change tous les jumps conditionnels

### Strategie 4 : Modifier une variable globale

Souvent, il y a un compteur ou flag :

```c
static int checks_passed = 0;

// Chaque check incremente
if (!check_ptrace()) checks_passed++;
if (!check_sysctl()) checks_passed++;

// A la fin
if (checks_passed == 3) show_flag();
```

```bash
(lldb) p &checks_passed   # Trouver l'adresse
(lldb) memory write 0x... 3  # Forcer la valeur
```

## A retenir

- Les protections combinees sont plus robustes
- Mais chaque check reste bypassable
- Scripts LLDB pour automatiser
- Chercher le point de decision final

## Exercice pratique

Ce binaire combine 3 techniques anti-debug. Bypass-les toutes pour obtenir le flag !
