# ptrace et l'Anti-Debug

## Qu'est-ce que ptrace ?

`ptrace` (process trace) est un appel systeme Unix qui permet a un processus d'observer et controler l'execution d'un autre processus. C'est **la base de tous les debuggers** comme LLDB, GDB, etc.

## Comment ca marche ?

Quand tu lances `lldb ./programme`, LLDB utilise `ptrace` pour :
- Attacher au processus
- Mettre des breakpoints
- Lire/ecrire la memoire
- Controler l'execution pas a pas

## L'astuce anti-debug : PT_DENY_ATTACH

Sur macOS, il existe une option speciale : `PT_DENY_ATTACH`

```c
#include <sys/ptrace.h>

int main() {
    // Empeche tout debugger de s'attacher
    ptrace(PT_DENY_ATTACH, 0, 0, 0);

    // Le reste du programme...
    printf("Programme protege!\n");
    return 0;
}
```

**Si un debugger est deja attache**, l'appel `ptrace(PT_DENY_ATTACH, ...)` retourne `-1` et le programme peut detecter qu'il est debugge.

## Comment bypasser ?

### Methode 1 : Avec LLDB (dynamique)

```bash
lldb ./challenge
(lldb) b ptrace           # Breakpoint sur ptrace
(lldb) r                  # Lancer le programme
# Le breakpoint est touche
(lldb) thread return 0    # Forcer ptrace a retourner 0 (succes)
(lldb) c                  # Continuer
```

### Methode 2 : Patcher le binaire (statique)

Dans Binary Ninja, trouve l'appel a `ptrace` ou `is_debugger_present` et :
- Change le `JNZ` (jump if not zero) en `JMP` (jump always)
- Ou remplace l'appel par des `NOP` (no operation)

### Methode 3 : Hook avec DYLD_INSERT_LIBRARIES

Cree une librairie qui remplace `ptrace` :

```c
// fake_ptrace.c
long ptrace(int request, ...) {
    return 0;  // Toujours retourner succes
}
```

```bash
clang -shared -o fake.dylib fake_ptrace.c
DYLD_INSERT_LIBRARIES=./fake.dylib ./challenge
```

## A retenir

| Concept | Description |
|---------|-------------|
| `ptrace` | Appel systeme pour debugger |
| `PT_DENY_ATTACH` | Option macOS anti-debug |
| Retour `-1` | Indique qu'un debugger est present |
| Bypass | Forcer le retour a `0` |

## Exercice pratique

Le binaire utilise `ptrace(PT_DENY_ATTACH)` pour te bloquer. Utilise une des methodes ci-dessus pour obtenir le flag !
