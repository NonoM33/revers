# Detection par processus parent (getppid)

## Le concept

Quand tu lances un programme depuis le terminal :
```
Terminal (bash) -> ton programme
```

Quand tu le lances depuis un debugger :
```
Terminal -> LLDB -> ton programme
```

Le **processus parent** est different ! Un programme peut verifier qui est son parent.

## Comment ca marche ?

```c
#include <unistd.h>
#include <libproc.h>

int is_parent_debugger() {
    pid_t ppid = getppid();  // Obtenir le PID du parent
    char path[PROC_PIDPATHINFO_MAXSIZE];

    // Obtenir le chemin de l'executable du parent
    proc_pidpath(ppid, path, sizeof(path));

    // Verifier si c'est un debugger connu
    if (strstr(path, "lldb") != NULL) return 1;
    if (strstr(path, "gdb") != NULL) return 1;
    if (strstr(path, "debugserver") != NULL) return 1;

    return 0;
}
```

## Les debuggers courants

| Nom | Chemin typique |
|-----|----------------|
| LLDB | `/usr/bin/lldb` |
| GDB | `/usr/local/bin/gdb` |
| debugserver | `/Library/.../debugserver` |

## Comment bypasser ?

### Methode 1 : Lancer via un script wrapper

```bash
#!/bin/bash
# wrapper.sh
./challenge
```

```bash
lldb ./wrapper.sh
```

Maintenant le parent est `bash`, pas `lldb` !

### Methode 2 : Hook getppid ou proc_pidpath

```bash
(lldb) b proc_pidpath
(lldb) r
# Modifier le buffer de sortie pour ne pas contenir "lldb"
```

### Methode 3 : Modifier la comparaison de strings

Dans Binary Ninja :
1. Trouve les appels a `strstr` avec "lldb", "gdb"
2. Remplace les strings par des choses impossibles
3. Ou change le resultat du test

### Methode 4 : Forcer le retour de la fonction

```bash
(lldb) b is_parent_debugger
(lldb) r
# A la fin de la fonction :
(lldb) thread return 0
(lldb) c
```

## Detection plus avancee

Certains programmes verifient aussi :
- Le nom du processus parent (`ps`)
- Les variables d'environnement
- La presence de fichiers specifiques LLDB

## A retenir

- `getppid()` retourne le PID du processus parent
- `proc_pidpath()` obtient le chemin de l'executable
- Les debuggers ont des noms reconnaissables
- Utiliser un wrapper ou hooker les fonctions

## Exercice pratique

Le binaire verifie si son parent est un debugger. Trouve un moyen de le tromper !
