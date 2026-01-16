# sysctl et la detection de debugger

<div class="lesson-objectives">

## 🎯 Objectifs de cette lecon

A la fin de cette lecon, tu sauras :
- Comprendre ce qu'est `sysctl` et comment il interroge le systeme
- Expliquer le flag `P_TRACED` et pourquoi il revele les debuggers
- Utiliser 4 methodes differentes pour bypasser cette detection
- Faire la difference entre detection **active** (ptrace) et **passive** (sysctl)

</div>

<div class="lesson-prerequisites">

## 📚 Prerequis

Avant de commencer, assure-toi de :
- Avoir compris la lecon sur ptrace (lecon 1)
- Savoir utiliser les commandes de base de LLDB
- Comprendre ce qu'est un PID (identifiant de processus)

</div>

## Imagine un fichier medical...

Tu vas chez le medecin, et il consulte ton **dossier medical**. Ce dossier contient plein d'informations sur toi : allergies, maladies, traitements en cours...

`sysctl` fonctionne exactement pareil, mais pour les programmes ! C'est comme consulter le "dossier medical" d'un processus pour savoir ce qui lui arrive.

:::info
Contrairement a `ptrace` qui **agit** sur le programme (comme un traitement medical), `sysctl` ne fait que **lire** des informations (comme consulter un dossier). C'est une detection **passive**.
:::

## Qu'est-ce que sysctl ?

`sysctl` est un appel systeme qui permet de **lire des informations** sur le systeme et les processus. C'est comme poser une question au systeme d'exploitation :

```
Programme : "Hey macOS, dis-moi tout sur le processus 12345"
macOS : "Voici son dossier complet : memoire, CPU, etat..."
```

## La structure kinfo_proc : le "dossier" du processus

Sur macOS, chaque processus a un "dossier" appele `kinfo_proc`. Ce dossier contient plein de champs, mais un seul nous interesse : **`p_flag`**.

```c
struct kinfo_proc {
    // ... plein d'autres informations ...
    struct extern_proc kp_proc;  // Contient p_flag
    // ...
};
```

:::tip
Tu peux voir `kinfo_proc` comme une **fiche d'identite** du processus. Elle contient son nom, son etat, sa memoire utilisee, et surtout... si quelqu'un l'observe !
:::

## Le flag P_TRACED : le signal d'alarme

Dans le champ `p_flag`, il y a un bit special : `P_TRACED` (valeur `0x800` en hexadecimal, soit 2048 en decimal).

Quand ce bit est **active**, ca veut dire : "Attention ! Ce processus est en train d'etre debugge !"

```
p_flag normal :    0000 0000 0000 0000
p_flag debugge :   0000 1000 0000 0000   <- bit 11 active (0x800)
```

:::warning
Ce flag est **automatiquement active par le systeme** quand un debugger s'attache. Le programme n'a aucun controle dessus - il ne peut que le lire.
:::

## Comment un programme detecte le debugger

Voici le code complet que les programmes utilisent :

```c
#include <sys/sysctl.h>
#include <sys/types.h>
#include <unistd.h>

int is_debugged() {
    // 1. Preparer la "question" a poser au systeme
    int mib[4];
    mib[0] = CTL_KERN;        // Je veux des infos kernel
    mib[1] = KERN_PROC;       // Sur un processus
    mib[2] = KERN_PROC_PID;   // Identifie par son PID
    mib[3] = getpid();        // Mon propre PID

    // 2. Preparer l'espace pour la "reponse"
    struct kinfo_proc info;
    size_t size = sizeof(info);
    info.kp_proc.p_flag = 0;  // Initialiser a zero

    // 3. Poser la question
    sysctl(mib, 4, &info, &size, NULL, 0);

    // 4. Verifier le flag P_TRACED
    return (info.kp_proc.p_flag & 0x800) != 0;
}
```

:::example
**Decomposition pas a pas** :

1. **Ligne 6-9** : On prepare notre question. C'est comme dire "Je veux le dossier du processus numero X"
2. **Ligne 12-14** : On prepare une boite vide pour recevoir la reponse
3. **Ligne 17** : On envoie la question au systeme
4. **Ligne 20** : On regarde si le bit `P_TRACED` est active avec l'operateur `&` (ET binaire)
:::

## Comparaison : ptrace vs sysctl

Ces deux techniques ont des approches tres differentes :

| Aspect | ptrace (lecon 1) | sysctl (cette lecon) |
|--------|-----------------|---------------------|
| Type | **Prevention active** | **Detection passive** |
| Action | Empeche l'attachement | Lit l'etat actuel |
| Moment | Au demarrage | N'importe quand |
| Effet | Tue le processus | Retourne vrai/faux |
| Bypass | Empecher l'appel | Modifier le resultat |

:::info
`ptrace` c'est comme **verrouiller la porte** pour empecher les intrus. `sysctl` c'est comme **regarder par la fenetre** pour voir s'il y a deja quelqu'un.
:::

## Les 4 methodes pour bypasser

### Methode 1 : Hook sysctl avec LLDB

```bash
lldb ./challenge
(lldb) b sysctl           # Breakpoint sur l'appel systeme
(lldb) r
# Quand sysctl est touche :
(lldb) thread return 0    # Forcer le retour (pas de donnees)
(lldb) c
```

:::warning
Attention ! Forcer `sysctl` a retourner 0 peut casser d'autres fonctionnalites du programme qui utilisent aussi cet appel systeme.
:::

### Methode 2 : Modifier le resultat en memoire

Plus precis : on laisse `sysctl` s'executer, mais on modifie le resultat.

```bash
(lldb) b is_debugged              # Breakpoint sur la fonction de check
(lldb) r
# Laisser sysctl s'executer, puis juste avant le return :
(lldb) finish                      # Aller a la fin de la fonction
(lldb) register write x0 0        # Forcer le retour a 0 (pas debugge)
(lldb) c
```

### Methode 3 : Modifier p_flag directement en memoire

Technique avancee : on modifie la structure `kinfo_proc` apres l'appel.

```bash
(lldb) b sysctl
(lldb) r
(lldb) finish                      # Laisser sysctl finir
# Maintenant info.kp_proc.p_flag contient 0x800
# Trouver l'adresse et effacer le bit
(lldb) memory write 0x... 0x00    # Effacer le flag
(lldb) c
```

### Methode 4 : Patcher le binaire (permanent)

Dans Binary Ninja :

1. Trouve la fonction `is_debugged` ou equivalent
2. Cherche l'instruction `AND` avec `0x800`
3. **Option A** : Remplace par `MOV X0, #0` (toujours retourner faux)
4. **Option B** : Change le saut conditionnel qui suit

:::example
**Pattern a chercher dans Binary Ninja** :

```asm
; Avant le check
LDR   W8, [X0, #offset]    ; Charger p_flag
AND   W8, W8, #0x800       ; Masquer pour P_TRACED
CBNZ  W8, debugger_found   ; Si non-zero, debugger detecte !

; Tu peux changer en :
LDR   W8, [X0, #offset]
MOV   W8, #0               ; <-- Force a zero
CBNZ  W8, debugger_found   ; Ne sera jamais pris
```
:::

## Comment identifier cette protection dans Binary Ninja ?

Cherche ces indices :

1. **Import de `sysctl`** dans les imports
2. **Les constantes** : `CTL_KERN` (1), `KERN_PROC` (14), `KERN_PROC_PID` (1)
3. **La valeur `0x800`** (P_TRACED) dans une operation `AND`
4. **Une structure** avec `kinfo_proc` ou `extern_proc`

<div class="lesson-summary">

## 📌 Resume / A retenir

:::remember
- **`sysctl`** = appel systeme pour lire les infos du systeme
- **`P_TRACED`** = flag (0x800) indiquant qu'un debugger est attache
- **Detection passive** : ne bloque pas, detecte seulement
- **Bypass** : Hook sysctl, modifier le resultat, ou patcher le binaire
- Chercher les constantes **CTL_KERN**, **KERN_PROC**, et **0x800**
:::

</div>

## Exercice pratique

Le binaire utilise `sysctl` pour lire le flag `P_TRACED` et detecter ton debugger.

Ta mission :
1. Lance le binaire dans LLDB
2. Identifie ou se fait la detection (cherche `sysctl` ou `P_TRACED`)
3. Utilise une des 4 methodes pour bypasser
4. Recupere le flag !

:::tip
La **methode 2** (modifier le retour de la fonction de check) est souvent la plus simple car elle n'affecte pas les autres appels a `sysctl`.
:::

Bonne chance ! 🔍
