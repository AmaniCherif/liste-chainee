# Réponses au TP (liste chaînée, compilation séparée)

Environnement : gcc 16 (WinLibs), mingw32-make, Valgrind 3.22 sous WSL Ubuntu
(`-fsanitize=address` indisponible avec ce gcc Windows : `cannot find -lasan`).

## Ex. 1 – .gitignore
Les `.o` et l'exécutable sont des produits dérivés des sources : on peut les
régénérer, ils dépendent de la machine, et ils pollueraient chaque diff.

## Ex. 2 – Découpage
- A : `main.c` et `liste.c` doivent connaître la structure (`m->valeur`,
  `sizeof(Maillon)`) ; chaque fichier .c est compilé seul, donc le type doit
  être dans le .h inclus par les deux.
- B : pour que le compilateur vérifie que les définitions correspondent aux
  déclarations publiques (signature différente = erreur) et pour avoir `Maillon`.

## Ex. 3 – Compilation séparée
- A : 2 fichiers .o ; pas de `liste.h.o` : un en-tête n'est pas compilé seul,
  il est copié par `#include` dans les .c.
- B : elle recompile tout à chaque fois (rien d'incrémental) et mélange
  compilation et édition des liens.

## Ex. 4 – Trois erreurs
| Cas | Premier message | Phase |
|---|---|---|
| 1 | `main.o:main.c:(.text+0x34): undefined reference to 'liste_inserer'` | lien (`ld.exe`) |
| 2 | `main.c:5:5: error: unknown type name 'Maillon'` | compilation |
| 3 (avec `-std=c11`) | `liste.h:4:16: error: redefinition of 'struct Maillon'` | compilation |

- Q1 : le cas 1 : le préfixe `ld.exe` et « undefined reference », sans ligne de source.
- Q2 : `unknown type name 'Maillon'` ; les suivants en découlent (déclarations implicites).
- Q3 : dès qu'un .h en inclut un autre (main.c inclut a.h et b.h, qui incluent tous deux liste.h).
- Note : gcc 16 est par défaut en C23, qui tolère la redéfinition identique de struct :
  le cas 3 n'échoue qu'avec `-std=c11`.

## Ex. 5 – Makefile
- Seconde exécution : `'demo.exe' is up to date.` ; aucune source n'est plus récente que sa cible.
- B (4 espaces) : `*** missing separator.  Stop.`

## Ex. 6 – Dépendance sur liste.h
- Avec liste.h : `touch liste.h` recompile main.o et liste.o. Sans : seul liste.o (via sa propre règle).
- B : main.o garderait l'ancienne disposition de Maillon : l'exécutable mélange deux
  définitions de la structure (offsets faux, lectures/écritures corrompues, sans erreur de compilation).

## Ex. 7 – Compteur
Sans fuite : 5 après construction, 0 après libération. Avec fuite : 3.
- A : ce sont des détails d'implémentation : `static` les rend invisibles aux autres
  fichiers (pas de collision de noms, pas de contournement du compteur).
- B : sans le test de malloc, un échec (NULL) serait compté comme un bloc ; sans celui
  de free, `free(NULL)` (légal) décrémenterait à tort : compteur négatif/faux.
- C : non, il ne dit pas où. Avec le seul compteur : l'afficher à plusieurs points du
  programme et dichotomiser, ou (ex. 12) tracer taille + étiquette à chaque allocation.

## Ex. 8 – Valgrind
Sans fuite : `All heap blocks were freed -- no leaks are possible`, 0 errors.
Avec fuite : `definitely lost: 16 bytes in 1 blocks`, `indirectly lost: 32 bytes in 2 blocks`,
48 octets / 3 blocs ; pile : `malloc` ← `suivi_malloc (liste.c:11)` ← `liste_inserer (liste.c:33)` ← `main (main.c:21)`.
- A : la ligne de main.c est celle de l'allocation (l'appel à `liste_inserer`), pas
  celle du `free` oublié (qui n'existe pas) : on cherche la ligne où la liste aurait dû être libérée.
- B : 1 definitely (la tête, plus aucun pointeur dessus), 2 indirectly (atteignables
  seulement via la tête perdue).
- C : 9 allocs, 6 frees avec fuite (9/9 sans). 9 > 8 maillons : un bloc est alloué par la
  bibliothèque C (tampon de stdout).

## Ex. 9 – deborde.c
Sortie `42`, code de sortie 0, aucun signalement.
Valgrind : `Invalid write of size 4 ... Address ... is 0 bytes after a block of size 20 alloc'd`.
- A : non, 1 malloc / 1 free : le compteur ne voit rien.
- B : 20 = 5 × sizeof(int) ; 0 = l'adresse est juste après le dernier octet du bloc ;
  pour `t[6]` : `4 bytes after a block of size 20`.
- C : un test qui passe prouve l'absence de symptôme, pas la correction (comportement indéfini).
- D : compteur : test automatique, léger, dans les tests unitaires, vérifie l'équilibre
  malloc/free ; Valgrind : localiser une fuite, détecter accès hors bornes, lecture de mémoire
  non initialisée, use-after-free.

## Ex. 10 – liste_maximum
- A : 3 fichiers modifiés (liste.h, liste.c, main.c) ; make a recompilé main.o et liste.o.
- B : -1 est une valeur légitime de la liste ; le booléen sépare « pas de résultat » de la valeur.

## Bonus
Voir les commits : test.h/test.c, `static dernier` (appel depuis main.c :
`implicit declaration of function 'dernier'` ; si on le déclarait, ce serait un
`undefined reference` à l'édition des liens), cible `make test`
(`valgrind --error-exitcode=1`), traçage taille + étiquette.
