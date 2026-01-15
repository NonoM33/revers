# Detection par terminal (isatty)

## Le concept

Quand tu lances un programme normalement, stdin/stdout sont connectes a un **vrai terminal** (TTY). Quand un debugger controle un programme, ces flux peuvent etre rediriges.

## Qu'est-ce qu'un TTY ?

TTY = TeleTYpewriter (historique). Aujourd'hui, c'est ton terminal :
- Terminal.app sur macOS
- iTerm2
- Le terminal integre de VS Code

## Comment ca marche ?

```c
#include <unistd.h>

int check_terminal() {
    // isatty() retourne 1 si c'est un vrai terminal
    if (!isatty(STDIN_FILENO)) {
        return 1;  // stdin n'est pas un terminal - suspect !
    }
    if (!isatty(STDOUT_FILENO)) {
        return 1;  // stdout n'est pas un terminal - suspect !
    }
    return 0;  // Tout est normal
}
```

## Pourquoi ca detecte les debuggers ?

En mode debug, les flux peuvent etre :
- Rediriges vers des pipes
- Connectes a des pseudo-terminaux
- Manipules par le debugger

Certains debuggers ne preservent pas correctement l'etat TTY.

## Les constantes importantes

```c
STDIN_FILENO  = 0   // Entree standard
STDOUT_FILENO = 1   // Sortie standard
STDERR_FILENO = 2   // Sortie d'erreur
```

## Comment bypasser ?

### Methode 1 : Hook isatty avec LLDB

```bash
(lldb) b isatty
(lldb) r
# Quand isatty est appele :
(lldb) thread return 1  # Retourner "oui c'est un terminal"
(lldb) c
```

Note : Tu devras peut-etre le faire plusieurs fois (stdin ET stdout).

### Methode 2 : Patcher le binaire

Dans Binary Ninja :
1. Trouve les appels a `isatty`
2. Remplace par `MOV result, 1` (toujours vrai)
3. Ou NOP les checks et force le bon chemin

### Methode 3 : Utiliser un vrai terminal dans LLDB

```bash
# Dans un terminal, cree un pseudo-terminal
tty
# -> /dev/ttys001

# Dans LLDB
(lldb) target create ./challenge
(lldb) process launch -i /dev/ttys001 -o /dev/ttys001
```

### Methode 4 : Variables d'environnement

Certains programmes regardent aussi `$TERM` :
```bash
TERM=xterm-256color lldb ./challenge
```

## Detection avancee

Programmes plus sophistiques verifient :
- `tcgetattr()` pour les attributs du terminal
- `ioctl(TIOCGWINSZ)` pour la taille de la fenetre
- Les codes d'echappement ANSI

## A retenir

- `isatty()` verifie si un fd est un terminal
- Retourne 1 = terminal, 0 = autre chose
- Les debuggers peuvent perturber les flux
- Hook ou patch pour bypasser

## Exercice pratique

Le binaire verifie que stdin/stdout sont des vrais terminaux. Fais-lui croire que tout est normal !
