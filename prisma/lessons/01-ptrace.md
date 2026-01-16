# ptrace et l'Anti-Debug

<div class="lesson-objectives">

## 🎯 Objectifs de cette lecon

A la fin de cette lecon, tu sauras :
- Expliquer ce qu'est `ptrace` et pourquoi c'est crucial pour le debugging
- Comprendre comment `PT_DENY_ATTACH` bloque les debuggers sur macOS
- Utiliser 3 methodes differentes pour bypasser cette protection
- Identifier cette technique dans un binaire avec Binary Ninja

</div>

<div class="lesson-prerequisites">

## 📚 Prerequis

Avant de commencer, assure-toi de :
- Savoir ouvrir un terminal et taper des commandes basiques
- Avoir installe LLDB (inclus avec Xcode Command Line Tools)
- Avoir ouvert Binary Ninja au moins une fois

</div>

## Imagine un gardien de securite...

Pour comprendre `ptrace`, imagine un **gardien de securite** devant un batiment. Ce gardien a le pouvoir de :

- **Observer** tout ce qui se passe a l'interieur (comme un debugger observe un programme)
- **Controler** les entrees et sorties (comme un debugger peut arreter l'execution)
- **Inspecter** les affaires de chacun (comme un debugger peut lire la memoire)

`ptrace` est exactement ce gardien, mais pour les programmes informatiques !

## Qu'est-ce que ptrace ?

`ptrace` signifie "**P**rocess **TRACE**". C'est un appel systeme Unix (une fonction speciale du systeme d'exploitation) qui permet a un programme d'observer et de controler un autre programme.

:::info
Un **appel systeme** (syscall) est une demande qu'un programme fait au systeme d'exploitation. C'est comme appeler le service technique de ton immeuble : tu demandes, et le systeme fait le travail pour toi.
:::

### Pourquoi c'est important ?

Tous les debuggers utilisent `ptrace` pour fonctionner ! Quand tu lances :

```bash
lldb ./mon_programme
```

Voici ce qui se passe en coulisses :

```
Toi  -->  LLDB  -->  ptrace()  -->  Systeme macOS  -->  ton_programme
             ^                            |
             |____ "Je veux observer"_____|
```

LLDB demande a macOS : "Hey, je veux observer ce programme !" Et `ptrace` est l'outil qui rend cela possible.

:::tip
Tu peux voir `ptrace` comme des **lunettes a rayons X** pour les programmes. Sans ces lunettes, tu ne peux pas voir ce qui se passe a l'interieur !
:::

## Comment les debuggers utilisent ptrace

Quand tu debuggues un programme, `ptrace` permet a LLDB de faire plein de choses :

| Action dans LLDB | Ce que ptrace fait en coulisse |
|------------------|-------------------------------|
| `b main` (breakpoint) | Modifie le code pour inserer un point d'arret |
| `r` (run) | Lance le programme sous surveillance |
| `n` (next) | Execute une instruction et reprend le controle |
| `p variable` | Lit la memoire du programme |
| `register write` | Modifie les valeurs en memoire |

:::example
**Exemple concret** : Quand tu mets un breakpoint avec `b main`, LLDB utilise `ptrace` pour :

1. Trouver l'adresse de la fonction `main`
2. Sauvegarder l'instruction originale a cette adresse
3. La remplacer par une instruction speciale (`INT 3` sur Intel, `BRK` sur ARM)
4. Quand le programme atteint ce point, il s'arrete et LLDB reprend le controle
:::

## L'astuce anti-debug : PT_DENY_ATTACH

Maintenant que tu comprends `ptrace`, voici comment les developpeurs l'utilisent pour **bloquer les debuggers**.

Sur macOS, il existe une option speciale : `PT_DENY_ATTACH`. C'est comme si le programme disait au gardien : **"Personne n'a le droit de m'observer !"**

```c
#include <sys/ptrace.h>

int main() {
    // Cette ligne empeche tout debugger de s'attacher
    ptrace(PT_DENY_ATTACH, 0, 0, 0);

    // A partir d'ici, si un debugger essaie de s'attacher
    // le programme sera immediatement tue !

    printf("Programme protege!\n");
    secret_flag();  // Affiche le flag secret
    return 0;
}
```

:::warning
Une fois que `PT_DENY_ATTACH` est appele, c'est **irreversible** pour ce processus. Le programme ne peut plus etre debugge par les moyens normaux. C'est pourquoi on doit **intercepter l'appel avant qu'il ne soit execute** !
:::

### Que se passe-t-il exactement ?

Imaginons deux scenarios :

**Scenario 1 : Tu lances le programme normalement**
```
./challenge
--> ptrace(PT_DENY_ATTACH, ...) s'execute
--> "OK, plus personne ne peut m'observer"
--> secret_flag() s'execute
--> Tu vois le flag ! 🎉
```

**Scenario 2 : Tu lances avec LLDB**
```
lldb ./challenge
(lldb) r
--> LLDB s'attache au programme
--> ptrace(PT_DENY_ATTACH, ...) detecte le debugger
--> SIGKILL ! Le programme est tue 💀
--> Tu ne vois rien...
```

## Les 3 methodes pour bypasser

### Methode 1 : Intercepter avec LLDB (Dynamique)

C'est la methode la plus elegante. On va mettre un breakpoint sur `ptrace` et l'empecher de fonctionner !

```bash
lldb ./challenge
(lldb) b ptrace           # 1. Mettre un breakpoint sur ptrace
(lldb) r                  # 2. Lancer le programme
# Le programme s'arrete quand il appelle ptrace
(lldb) thread return 0    # 3. Forcer ptrace a retourner 0 (= succes)
(lldb) c                  # 4. Continuer l'execution
# Magie ! Le programme continue sans savoir qu'il est debugge
```

:::tip
`thread return 0` est super puissant ! Ca dit a LLDB : "Fais comme si la fonction venait de se terminer et qu'elle a retourne 0". Le code de `ptrace` n'est meme pas execute !
:::

### Methode 2 : Patcher le binaire (Statique)

Si tu veux une solution permanente, tu peux modifier le fichier binaire directement. C'est comme effacer la ligne de code !

**Etapes dans Binary Ninja :**

1. Ouvre le binaire dans Binary Ninja
2. Cherche les references a `ptrace` (View > Strings ou cherche dans les imports)
3. Trouve l'instruction `CALL ptrace` ou `BL _ptrace`
4. Tu as deux options :

**Option A : Remplacer par des NOP**
```
Avant : BL _ptrace     (appelle la fonction)
Apres : NOP NOP NOP... (ne fait rien)
```

**Option B : Modifier le saut conditionnel**
```
Avant : CBNZ W0, fail  (si ptrace != 0, aller a l'echec)
Apres : B continue     (toujours continuer)
```

:::info
**NOP** signifie "No Operation". C'est une instruction qui ne fait rien du tout. Les processeurs l'executent et passent a la suite. C'est tres utile pour "effacer" du code sans changer la taille du programme !
:::

### Methode 3 : Hook avec DYLD_INSERT_LIBRARIES

Cette methode remplace la fonction `ptrace` par la notre avant meme que le programme ne demarre !

**Etape 1 : Cree un fichier `fake_ptrace.c`**
```c
// Ce fichier definit notre propre version de ptrace
long ptrace(int request, ...) {
    // Retourne toujours 0 (= succes, pas de debugger)
    return 0;
}
```

**Etape 2 : Compile en librairie dynamique**
```bash
clang -shared -o fake.dylib fake_ptrace.c
```

**Etape 3 : Lance le programme avec notre librairie**
```bash
DYLD_INSERT_LIBRARIES=./fake.dylib ./challenge
```

:::warning
Cette methode ne fonctionne pas si le binaire a le flag `__RESTRICT` ou est signe par Apple (SIP). C'est une protection supplementaire sur macOS.
:::

## Comment identifier cette protection dans Binary Ninja ?

Quand tu ouvres un binaire, cherche ces indices :

1. **Import de `ptrace`** dans la table des imports (panneau de gauche)
2. **La constante `31`** quelque part (c'est la valeur de `PT_DENY_ATTACH`)
3. **Ce pattern typique** dans le code assembleur :

```asm
; Version ARM64 (Mac M1/M2/M3)
MOV  W0, #31        ; PT_DENY_ATTACH = 31
MOV  X1, #0
MOV  X2, #0
MOV  X3, #0
BL   _ptrace        ; Appel a ptrace
```

:::example
**Astuce de pro** : Dans Binary Ninja, tu peux chercher la valeur `0x1f` (31 en hexadecimal) pour trouver rapidement les appels a `PT_DENY_ATTACH`.
:::

<div class="lesson-summary">

## 📌 Resume / A retenir

:::remember
- **`ptrace`** = appel systeme qui permet aux debuggers d'observer les programmes
- **`PT_DENY_ATTACH`** = option macOS (valeur 31) qui bloque les debuggers
- **Bypass dynamique** : `thread return 0` dans LLDB apres un breakpoint sur `ptrace`
- **Bypass statique** : NOP l'appel ou modifier le saut conditionnel dans Binary Ninja
- **Hook** : Remplacer `ptrace` avec `DYLD_INSERT_LIBRARIES`
- La constante **31 (0x1f)** dans le code = probablement `PT_DENY_ATTACH`
:::

</div>

## Exercice pratique

Tu as maintenant toutes les connaissances pour resoudre ce challenge !

Le binaire utilise `ptrace(PT_DENY_ATTACH)` pour te bloquer. Ta mission :

1. **Analyse** : Ouvre le binaire dans Binary Ninja et trouve l'appel a `ptrace`
2. **Bypass** : Utilise une des 3 methodes apprises
3. **Flag** : Recupere le flag et valide !

:::tip
Commence par la **methode LLDB**, c'est la plus rapide pour tester. Si tu veux t'entrainer au patching, essaie ensuite la **methode Binary Ninja** !
:::

Bonne chance ! 🚀
