# Detection par processus parent (getppid)

<div class="lesson-objectives">

## 🎯 Objectifs de cette lecon

A la fin de cette lecon, tu sauras :
- Comprendre la relation parent-enfant entre processus
- Expliquer comment `getppid()` revele qui a lance le programme
- Identifier les noms des debuggers courants (lldb, gdb, debugserver)
- Utiliser plusieurs techniques pour tromper cette detection

</div>

<div class="lesson-prerequisites">

## 📚 Prerequis

Avant de commencer, assure-toi de :
- Connaitre les bases des processus Unix (PID)
- Avoir compris les lecons 1 et 2
- Savoir naviguer dans le terminal

</div>

## Imagine une reunion de famille...

Quand tu nais, tu as des **parents**. Et tes parents savent qui tu es. Dans le monde Unix, c'est pareil : quand un programme est lance, il a un **processus parent** - celui qui l'a demarre.

```
Grand-parent : Terminal (bash/zsh)
    |
    └── Parent : LLDB (si tu debuggues)
            |
            └── Enfant : ton_programme
```

:::info
Un programme peut demander : "Qui est mon parent ?" Et si la reponse est "LLDB" ou "GDB"... c'est suspect !
:::

## La chaine des processus

**Cas normal** (sans debugger) :
```
Terminal (PID 100)
    └── ton_programme (PID 200)
        Parent = Terminal ✓
```

**Cas debug** (avec LLDB) :
```
Terminal (PID 100)
    └── LLDB (PID 150)
        └── ton_programme (PID 200)
            Parent = LLDB ⚠️
```

Le programme peut detecter que son parent est un debugger !

## Comment ca marche techniquement ?

Le code utilise deux fonctions :

```c
#include <unistd.h>
#include <libproc.h>
#include <string.h>

int is_parent_debugger() {
    // 1. Obtenir le PID du parent
    pid_t parent_pid = getppid();

    // 2. Obtenir le chemin de l'executable du parent
    char parent_path[PROC_PIDPATHINFO_MAXSIZE];
    proc_pidpath(parent_pid, parent_path, sizeof(parent_path));

    // 3. Verifier si c'est un debugger connu
    if (strstr(parent_path, "lldb") != NULL) return 1;
    if (strstr(parent_path, "gdb") != NULL) return 1;
    if (strstr(parent_path, "debugserver") != NULL) return 1;

    return 0;  // Parent normal
}
```

:::example
**Decomposition pas a pas** :

1. **`getppid()`** retourne le PID (numero) du processus parent
2. **`proc_pidpath()`** transforme ce PID en chemin d'executable (ex: `/usr/bin/lldb`)
3. **`strstr()`** cherche si le chemin contient un mot suspect

Si le chemin contient "lldb", "gdb" ou "debugserver" → debugger detecte !
:::

## Les debuggers courants sur macOS

| Debugger | Chemin typique |
|----------|---------------|
| LLDB | `/usr/bin/lldb` |
| GDB | `/usr/local/bin/gdb` ou `/opt/homebrew/bin/gdb` |
| debugserver | `/Library/Apple/usr/libexec/debugserver` |

:::tip
`debugserver` est le composant backend d'LLDB. Meme si tu n'utilises pas LLDB directement, il peut etre present quand tu debuggues depuis Xcode ou VS Code.
:::

## Pourquoi cette technique est "faible" ?

Cette detection a des failles :

1. **Facile a contourner** : Il suffit de changer le parent
2. **Liste non exhaustive** : Il existe d'autres debuggers
3. **Faux positifs** : Des outils legitimes peuvent avoir des noms similaires

:::warning
Cette technique seule n'est jamais suffisante. Elle est toujours combinee avec d'autres (ptrace, sysctl, timing).
:::

## Les 4 methodes pour bypasser

### Methode 1 : Le script wrapper (la plus simple !)

L'idee : on cree un intermediaire entre LLDB et notre programme.

**Etape 1 : Cree un fichier `wrapper.sh`**
```bash
#!/bin/bash
./challenge  # Lance le vrai programme
```

**Etape 2 : Rends-le executable**
```bash
chmod +x wrapper.sh
```

**Etape 3 : Debug le wrapper**
```bash
lldb ./wrapper.sh
(lldb) r
```

Maintenant le parent du challenge est `bash`, pas `lldb` !

```
Terminal
    └── LLDB
        └── bash (wrapper.sh)  ← Parent
            └── challenge      ← Voit "bash", pas "lldb" !
```

### Methode 2 : Hook proc_pidpath

```bash
lldb ./challenge
(lldb) b proc_pidpath
(lldb) r
# Quand proc_pidpath est appele :
(lldb) finish
# Modifier le buffer pour enlever "lldb"
(lldb) memory write $x1 "bash"
(lldb) c
```

### Methode 3 : Forcer le retour de la fonction

```bash
(lldb) b is_parent_debugger    # Nom de la fonction de check
(lldb) r
# A l'entree de la fonction :
(lldb) thread return 0         # Retourner "pas de debugger"
(lldb) c
```

:::tip
C'est souvent la methode la plus rapide ! Tu n'as pas besoin de comprendre tout le code, juste de trouver la fonction de check.
:::

### Methode 4 : Patcher le binaire

Dans Binary Ninja :

1. Trouve les appels a `strstr` avec les strings "lldb", "gdb"
2. **Option A** : Modifie les strings pour des valeurs impossibles (ex: "XXXXXX")
3. **Option B** : Change le `CBZ`/`CBNZ` apres le `strstr` pour toujours continuer

:::example
**Pattern a chercher** :

```asm
; Cherche "lldb" dans le chemin
ADRP  X1, "lldb"@PAGE
ADD   X1, X1, "lldb"@PAGEOFF
BL    _strstr
CBNZ  X0, debugger_detected    ; Si trouve -> detecte

; Change en :
CBZ   X0, debugger_detected    ; Inverse la logique !
; Ou NOP tout le bloc
```
:::

## Detection avancee : autres indices

Certains programmes plus sophistiques verifient aussi :

1. **Le nom du processus parent** (pas juste le chemin)
2. **Les variables d'environnement** (LLDB en ajoute)
3. **Les fichiers de config LLDB** (`~/.lldbinit`)
4. **Le terminal** (pseudo-TTY de LLDB vs vrai terminal)

<div class="lesson-summary">

## 📌 Resume / A retenir

:::remember
- **`getppid()`** = obtient le PID du processus parent
- **`proc_pidpath()`** = transforme un PID en chemin d'executable
- Les debuggers ont des noms reconnaissables : `lldb`, `gdb`, `debugserver`
- **Bypass facile** : Script wrapper pour changer le parent
- **Bypass rapide** : `thread return 0` sur la fonction de check
- Cette technique seule est **faible** et toujours combinee avec d'autres
:::

</div>

## Exercice pratique

Le binaire verifie si son processus parent est un debugger en regardant son nom.

Ta mission :
1. Essaie de lancer directement avec LLDB (tu verras l'echec)
2. Utilise la technique du wrapper ou une autre methode
3. Recupere le flag !

:::tip
Commence par la **methode du wrapper** - c'est la plus simple et elle fonctionne a tous les coups. Puis essaie les autres methodes pour t'entrainer !
:::

Bonne chance ! 👨‍👩‍👧
