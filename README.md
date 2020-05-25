# SSM
Projet NE302 - Etape 3
RIBIERE Matthieu
MANCEAU Thibaut
LECOMTE Benjamin

# Compiler
```
> make clean && make 
```

# Lancer
```
> make test
```
Puis accès aux sites via les adresses 127.0.0.0/24:8080 par un navigateur ou autre


# Configuration
Dans le fichier sites.conf :

Pour rajouter un site sur le serveur :
  - Ajouter le contenu du champ Host désiré suivi du nom de la racine du site
Pour ajouter un message d'erreur particulier à un site :
  - Sous le site : rajouter une tabulation, le code de l'erreur puis la page souhaitée

Pour tester, il y a déjà 5 sites (127.0.0.1 à 127.0.0.5) disponibles sur le serveur
