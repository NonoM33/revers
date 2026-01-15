# sysctl et la detection de debugger

## Qu'est-ce que sysctl ?

`sysctl` est un appel systeme qui permet de lire des informations sur le systeme et les processus. Les programmes l'utilisent pour detecter si un debugger est attache.

## La structure kinfo_proc

Sur macOS/BSD, chaque processus a des informations stockees dans une structure `kinfo_proc`. Le champ important est `p_flag` qui contient des drapeaux d'etat.

## Le flag P_TRACED

Quand un processus est debugge, le systeme active le flag `P_TRACED` (valeur 0x800). Un programme peut verifier ce flag :

```c
#include <sys/sysctl.h>
#include <sys/types.h>
#include <unistd.h>

int is_debugged() {
    int mib[4];
    struct kinfo_proc info;
    size_t size = sizeof(info);

    // Configuration de la requete sysctl
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();  // Notre propre PID

    // Initialiser a zero
    info.kp_proc.p_flag = 0;

    // Recuperer les infos du processus
    sysctl(mib, 4, &info, &size, NULL, 0);

    // Verifier le flag P_TRACED
    return (info.kp_proc.p_flag & P_TRACED) != 0;
}
```

## Comment bypasser ?

### Methode 1 : Hook sysctl avec LLDB

```bash
lldb ./challenge
(lldb) b sysctl
(lldb) r
# Quand sysctl est appele :
(lldb) thread return 0
(lldb) c
```

Mais attention ! Cela peut casser d'autres fonctionnalites.

### Methode 2 : Modifier le resultat en memoire

```bash
(lldb) b is_debugged      # Ou le nom de la fonction de check
(lldb) r
# Apres l'appel sysctl, avant le test du flag :
(lldb) register read      # Voir les registres
(lldb) register write x0 0  # Forcer le resultat a 0
(lldb) c
```

### Methode 3 : Patcher le binaire

Dans Binary Ninja :
1. Trouve la fonction qui fait le check
2. Trouve le `AND` avec `P_TRACED` (0x800)
3. Remplace par `MOV result, 0` ou change le jump conditionnel

### Methode 4 : Patcher p_flag directement

Le flag P_TRACED est dans `info.kp_proc.p_flag`. Tu peux :
- Mettre un breakpoint apres sysctl
- Modifier la memoire pour effacer le bit 0x800

## Comparaison ptrace vs sysctl

| Aspect | ptrace | sysctl |
|--------|--------|--------|
| Type | Prevention active | Detection passive |
| Effet | Bloque l'attachement | Lit l'etat actuel |
| Bypass | Forcer retour 0 | Modifier le resultat |

## A retenir

- `sysctl` lit les infos du processus
- `P_TRACED` (0x800) indique un debugger
- Le check est passif (ne bloque pas, detecte seulement)
- Plusieurs points de bypass possibles

## Exercice pratique

Le binaire utilise sysctl pour detecter P_TRACED. Empeche-le de voir que tu le debuggues !
