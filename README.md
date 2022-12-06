<h1 align="center">
  <br>
  LINFO1252 – Systèmes informatiques
  <br>
</h1>

<h4 align="center">Projet 1 : Programmation multithreadée et évaluation des performances.</h4>

<p align="center">
  <a href="#sujets-étudiés">Sujets étudiés</a> •
  <a href="#usage">Usage</a> •
  <a href="#auteurs">Auteurs</a> •
  <a href="#ressources">Ressources</a>
</p>

## Sujets étudiés

* Problème des [philosophes](https://en.wikipedia.org/wiki/Dining_philosophers_problem), 
des [lecteurs & écrivains](https://en.wikipedia.org/wiki/Readers–writers_problem) et 
des [producteurs & consommateurs](https://en.wikipedia.org/wiki/Producer–consumer_problem)
  - Implémentations via la librairie POSIX et les verrous d'attente active
* Algorithme test-and-set ([TAS](https://en.wikipedia.org/wiki/Test-and-set))  
* Algorithme test-and-test-and-set ([TTAS](https://en.wikipedia.org/wiki/Test_and_test-and-set))
* Algorithme backoff-test-and-test-and-set ([backoff](https://en.wikipedia.org/wiki/Exponential_backoff) TTAS)


## Usage

Le répertoire propose les fonctionnalités ci-dessous. De plus, la commande `make` permet de lancer les toutes les expériences et de produire les graphiques correspondants.

```bash
# Philosopher's problem implementation using POSIX library
$ make philo
# ./philo <number of philosopher threads>
$ ./philo 4

# Producers-consumers problem implementation using POSIX library
$ make prodcons
# ./prodcons <number of consumer threads> <number of producer threads>
$ ./prodcons 2 2

# Readers-writers problem implementation using POSIX library
$ make rw
# ./rw <number of reader threads> <number of writer threads>
$ ./rw 2 2

# Dummy multithreading program using spinlocks build on TAS algorithm
$ make tas
# ./tas <number of threads used>
$ ./tas 4

# Dummy multithreading program using spinlocks build on TTAS algorithm
$ make ttas
# ./ttas <number of threads used>
$ ./ttas 4

# Dummy multithreading program using spinlocks build on backoff-TTAS algorithm
$ make backoff
# ./backoff <number of threads used> <minimum waiting time> <maximum waiting time>
$ ./backoff 4 2 100

# Philosopher's problem implementation using TAS algorithm
$ make philotas
# ./philotas <number of philosopher threads>
$ ./philotas 4

# Philosopher's problem implementation using TTAS algorithm
$ make philottas
# ./philottas <number of philosopher threads>
$ ./philottas 4

# Producers-consumers problem implementation using TAS algorithm
$ make prodconstas
# ./prodconstas <number of consumer threads> <number of producer threads>
$ ./prodconstas 2 2

# Producers-consumers problem implementation using TTAS algorithm
$ make prodconsttas
# ./prodconsttas <number of consumer threads> <number of producer threads>
$ ./prodconsttas 2 2

# Readers-writers problem implementation using TAS algorithm
$ make rwtas
# ./rwtas <number of reader threads> <number of writer threads>
$ ./rwtas 2 2

# Readers-writers problem implementation using TTAS algorithm
$ make rwttas
# ./rwttas <number of reader threads> <number of writer threads>
$ ./rwttas 2 2
```
De plus, la commandes `make zip` permet de générer une archive du projet. La commande `make scripts` performe une série d'expériences sur les problèmes de synchronisations recensés ci-dessus et recueille les résultats sous la forme de fichiers *.csv* répertoriés dans le sous-dossier *measures*.
Enfin, un fichier écrit en language *python* est également fourni et permet de représenter graphiquement les données des fichiers *.csv*. Son utilisation est décrite ci-dessous :
```bash
$ python3 plot.py --help
usage: plot.py [-h] [-d DUAL_PLOT] [-m MERGED_PLOT] file_csv

positional arguments:
  file_csv              a csv file to plot

options:
  -h, --help            show this help message and exit
  -d DUAL_PLOT, --dual_plot DUAL_PLOT
                        a second csv file to plot as dual plot
  -m MERGED_PLOT, --merged_plot MERGED_PLOT
                        a second csv file to plot as merged plot
```
La commande `make plot` automatise la compilation de `plot.py` en affichant les résultats principaux des expériences performées.

> **Note**
> L'exécution de `make scripts` peut prendre un certain temps.

## Auteurs

Ce répertoire a été entièrement implémenté par Ygor Lausberg et Victor Lepère dans le cadre du cours de *Systèmes Informatiques* dispensé par Pr. Etienne Rivière.

## Ressources

* [Systèmes informatique - LINFO1252](https://sites.uclouvain.be/archives-portail/cdc2020/cours-2020-LINFO1252.html) - Site du cours
* [Syllabus](https://sites.uclouvain.be/SystInfo/) - Syllabus du cours
* [Moodle](https://moodle.uclouvain.be/course/view.php?id=4851) - Moodle du cours

---

> GitHub [Projet](https://github.com/Lopiuy/Projet.23-LINFO1252) &nbsp;&middot;&nbsp;
> [Ygor Lausberg](mailto:ygor.lausberg@student.uclouvain.be) &nbsp;&middot;&nbsp;
> GitHub [@Lopiuy](https://github.com/Lopiuy) &nbsp;&middot;&nbsp;
> [Victor Lepère](mailto:victor.lepere@student.uclouvain.be) &nbsp;&middot;&nbsp;
> GitHub [@victxrrr](https://github.com/victxrrr)