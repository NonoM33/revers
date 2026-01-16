# Detection par terminal (isatty)

<div class="lesson-objectives">

## 🎯 Objectifs de cette lecon

A la fin de cette lecon, tu sauras :
- Comprendre ce qu'est un TTY (terminal) et pourquoi c'est important
- Expliquer comment `isatty()` detecte les environnements de debug
- Connaitre les file descriptors standards (stdin, stdout, stderr)
- Utiliser plusieurs methodes pour tromper cette detection

</div>

<div class="lesson-prerequisites">

## 📚 Prerequis

Avant de commencer, assure-toi de :
- Connaitre les bases des entrees/sorties en programmation
- Avoir compris les lecons precedentes
- Savoir ce qu'est stdin et stdout

</div>

## Imagine deux facons de parler a quelqu'un...

**Methode 1 : Face a face**
Tu parles directement a quelqu'un, il te voit, tu le vois. C'est une vraie conversation.

**Methode 2 : Par intermediaire**
Tu parles dans un talkie-walkie, et quelqu'un d'autre transmet ton message. La personne ne te voit pas directement.

:::info
Un programme peut faire la meme distinction ! Il peut detecter s'il parle **directement a un terminal** (face a face) ou si ses entrees/sorties passent par un **intermediaire** (comme un debugger).
:::

## Qu'est-ce qu'un TTY ?

**TTY** = **T**ele**TY**pewriter (machine a ecrire a distance)

C'est un terme historique qui designe aujourd'hui ton **terminal** :
- Terminal.app sur macOS
- iTerm2
- Le terminal integre de VS Code
- Tout ce qui te permet de taper des commandes

:::example
Quand tu ouvres Terminal.app et que tu tapes des commandes, tu utilises un TTY. Le systeme sait que c'est un "vrai" utilisateur qui interagit.
:::

## Les 3 flux standards

Tout programme a 3 connexions de base avec le monde exterieur :

| Nom | Numero | Description | Exemple |
|-----|--------|-------------|---------|
| **stdin** | 0 | Entree standard | Ton clavier |
| **stdout** | 1 | Sortie standard | Ton ecran (texte normal) |
| **stderr** | 2 | Sortie d'erreur | Ton ecran (messages d'erreur) |

```c
#define STDIN_FILENO  0   // Entree
#define STDOUT_FILENO 1   // Sortie
#define STDERR_FILENO 2   // Erreurs
```

:::tip
Tu peux voir ca comme 3 tuyaux : un pour les donnees qui **entrent** (clavier), deux pour les donnees qui **sortent** (ecran normal et erreurs).
:::

## Comment isatty() fonctionne

La fonction `isatty()` pose une question simple : "Est-ce que ce flux est connecte a un vrai terminal ?"

```c
#include <unistd.h>

int check_terminal() {
    // Verifie si stdin est un vrai terminal
    if (!isatty(STDIN_FILENO)) {
        printf("stdin n'est pas un terminal !\n");
        return 1;  // Suspect !
    }

    // Verifie si stdout est un vrai terminal
    if (!isatty(STDOUT_FILENO)) {
        printf("stdout n'est pas un terminal !\n");
        return 1;  // Suspect !
    }

    return 0;  // Tout est normal
}
```

**Valeurs de retour** :
- `isatty()` retourne **1** = oui, c'est un vrai terminal
- `isatty()` retourne **0** = non, c'est autre chose (pipe, fichier, debugger...)

## Pourquoi ca detecte les debuggers ?

Quand un debugger controle un programme, il peut :

1. **Rediriger stdin** vers un pipe pour injecter des commandes
2. **Capturer stdout** pour lire les sorties
3. **Intercepter stderr** pour les erreurs

Ces redirections **cassent** la connexion directe au terminal !

```
Normal :
Terminal <---> Programme     (isatty = 1)

Avec debugger :
Terminal <---> LLDB <---> Programme   (isatty peut etre 0)
                  ^
                  |
              Pipe/redirect
```

:::warning
Ce n'est pas toujours le cas ! Certains debuggers (LLDB inclus) preservent bien la connexion TTY. Mais des techniques de debug plus avancees peuvent la casser.
:::

## Les methodes pour bypasser

### Methode 1 : Hook isatty avec LLDB

```bash
lldb ./challenge
(lldb) b isatty
(lldb) r
# Quand isatty est appele :
(lldb) thread return 1    # Retourner "oui c'est un terminal"
(lldb) c
```

:::warning
**Attention** : `isatty` peut etre appele plusieurs fois (pour stdin ET stdout). Tu devras peut-etre faire `thread return 1` plusieurs fois ou utiliser un script.
:::

### Methode 2 : Script LLDB automatique

Pour ne pas avoir a repeter manuellement :

```python
# isatty_bypass.py
import lldb

def isatty_hook(frame, bp_loc, dict):
    # Force le retour a 1 (vrai terminal)
    thread = frame.GetThread()
    thread.ReturnFromFrame(frame, lldb.SBValue.CreateValueFromExpression("", "1"))
    return False  # Ne pas s'arreter

def setup(debugger, command, result, internal_dict):
    target = debugger.GetSelectedTarget()
    bp = target.BreakpointCreateByName("isatty")
    bp.SetScriptCallbackFunction("isatty_bypass.isatty_hook")
```

```bash
(lldb) command script import isatty_bypass.py
(lldb) command script add -f isatty_bypass.setup bypass_isatty
(lldb) bypass_isatty
(lldb) r
```

### Methode 3 : Utiliser un vrai terminal dans LLDB

Tu peux dire a LLDB d'utiliser un autre terminal pour stdin/stdout :

**Etape 1 : Ouvre un second terminal et tape :**
```bash
tty
# Affiche quelque chose comme : /dev/ttys001
```

**Etape 2 : Dans LLDB :**
```bash
lldb ./challenge
(lldb) process launch -i /dev/ttys001 -o /dev/ttys001 -e /dev/ttys001
```

Maintenant stdin/stdout/stderr sont de vrais terminaux !

### Methode 4 : Patcher le binaire

Dans Binary Ninja :

1. Trouve tous les appels a `isatty`
2. **Option A** : Remplace par `MOV W0, #1` (toujours retourner vrai)
3. **Option B** : NOP les checks et force le bon chemin

:::example
**Pattern a chercher** :

```asm
; Appel isatty(0) pour stdin
MOV   W0, #0              ; STDIN_FILENO
BL    _isatty
CBZ   W0, not_a_terminal  ; Si 0, pas un terminal

; Change le CBZ en NOP ou inverse en CBNZ
```
:::

### Methode 5 : Variable d'environnement TERM

Certains programmes verifient aussi la variable `TERM` :

```bash
TERM=xterm-256color lldb ./challenge
(lldb) r
```

## Detection avancee : au-dela de isatty

Certains programmes plus sophistiques verifient :

| Technique | Fonction | Ce qu'elle detecte |
|-----------|----------|-------------------|
| `tcgetattr()` | Attributs du terminal | Vitesse, mode, etc. |
| `ioctl(TIOCGWINSZ)` | Taille de la fenetre | Largeur/hauteur du terminal |
| Codes ANSI | Sequences d'echappement | Support des couleurs |

:::info
Ces techniques avancees sont rares car elles peuvent avoir des faux positifs (terminaux non-standard, SSH, etc.)
:::

<div class="lesson-summary">

## 📌 Resume / A retenir

:::remember
- **TTY** = Terminal, la connexion directe entre toi et le programme
- **`isatty(fd)`** = Verifie si un file descriptor est un vrai terminal
- Retourne **1** = terminal, **0** = autre chose (pipe, fichier...)
- **File descriptors** : stdin (0), stdout (1), stderr (2)
- **Bypass** : `thread return 1` ou rediriger vers un vrai TTY
- Les debuggers peuvent casser la connexion TTY (mais pas toujours)
:::

</div>

## Exercice pratique

Le binaire verifie que stdin et stdout sont connectes a de vrais terminaux.

Ta mission :
1. Lance le binaire avec LLDB
2. Observe si `isatty` est appele (mets un breakpoint dessus)
3. Force le retour a 1 ou utilise la technique du second terminal
4. Recupere le flag !

:::tip
La **methode du second terminal** (avec `process launch -i -o`) est tres elegante car elle ne necessite pas de modifier le comportement du programme. Essaie-la !
:::

Bonne chance ! 📺
