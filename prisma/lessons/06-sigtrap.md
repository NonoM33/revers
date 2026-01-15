# Detection par SIGTRAP et exceptions

## Qu'est-ce que SIGTRAP ?

`SIGTRAP` est un signal Unix envoye quand :
- Un breakpoint est touche (instruction `INT 3` / `BRK`)
- Un single-step est execute
- Une exception de debug se produit

## Le piege anti-debug

L'idee : le programme **genere volontairement** un SIGTRAP et verifie qui le capture.

```c
#include <signal.h>
#include <setjmp.h>

static jmp_buf jump_buffer;
static int trap_caught = 0;

void sigtrap_handler(int sig) {
    trap_caught = 1;
    longjmp(jump_buffer, 1);  // Retourner au checkpoint
}

int main() {
    // Installer notre handler
    signal(SIGTRAP, sigtrap_handler);

    if (setjmp(jump_buffer) == 0) {
        // Premier passage : generer le trap
        raise(SIGTRAP);

        // Si on arrive ici, le debugger a intercepte le signal !
        printf("Debugger detecte!\n");
        return 1;
    }

    // On arrive ici via longjmp = notre handler a fonctionne
    if (trap_caught) {
        printf("Pas de debugger, voici le flag!\n");
        show_flag();
    }
}
```

## Pourquoi ca marche ?

1. **Sans debugger** : Le signal va a notre handler → `trap_caught = 1` → `longjmp` → succes
2. **Avec debugger** : Le debugger intercepte SIGTRAP → notre handler n'est jamais appele → on continue apres `raise()` → detecte !

## Les signaux importants

| Signal | Usage |
|--------|-------|
| SIGTRAP | Breakpoints, debug |
| SIGSTOP | Pause du processus |
| SIGINT | Ctrl+C |
| SIGSEGV | Violation memoire |

## setjmp / longjmp expliques

```c
if (setjmp(buffer) == 0) {
    // Code normal - premiere execution
    // setjmp retourne 0 la premiere fois
}
else {
    // On arrive ici apres longjmp(buffer, X)
    // setjmp "retourne" X cette fois
}
```

C'est comme un **checkpoint** dans un jeu video !

## Comment bypasser ?

### Methode 1 : Passer le signal au programme

```bash
(lldb) target create ./challenge
(lldb) process launch --stop-at-entry
(lldb) process handle SIGTRAP -p true -s false
# -p true  = passer au programme
# -s false = ne pas stopper
(lldb) c
```

### Methode 2 : Ne pas intercepter SIGTRAP

```bash
(lldb) settings set target.process.unwind-on-error-in-expression false
(lldb) process handle SIGTRAP --stop false --pass true
```

### Methode 3 : Modifier le handler

```bash
(lldb) b sigtrap_handler
(lldb) r
# Quand le handler est appele, tout va bien !
(lldb) c
```

### Methode 4 : Patcher le binaire

Dans Binary Ninja :
1. Trouve l'appel a `raise(SIGTRAP)`
2. NOP l'appel ou remplace par `raise(0)` (signal invalide = ignore)
3. Ou force `trap_caught = 1`

### Methode 5 : Skip le raise

```bash
(lldb) b raise
(lldb) r
# Quand raise est appele :
(lldb) thread return 0  # Ne pas envoyer le signal
(lldb) c
```

## Variations avancees

- `__builtin_trap()` : Genere un trap directement
- `asm("int3")` / `asm("brk #0")` : Instruction de breakpoint
- Exception handlers sur Windows

## A retenir

- SIGTRAP est intercepte par les debuggers
- Le programme peut installer son propre handler
- Si le handler n'est pas appele → debugger detecte
- Configurer LLDB pour passer les signaux

## Exercice pratique

Le binaire utilise SIGTRAP pour te pieger. Configure ton debugger correctement ou patch le binaire !
