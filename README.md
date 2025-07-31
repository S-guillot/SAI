# SAI

Solution pour une gestion fluide des tâches

## Fonctionnalités

L'application  axée sur la productivité est composée de deux parties distinctes. 

### 1 : Gestion des tâches

La première concerne une gestion des tâches. Celles-ci peuvent être regroupées dans des projets, et les tâches comme les projets sont forcément associés à une catégorie (par défaut la catégorie "générale").

Une tâche a plusieurs caractéristiques particulières qui lui sont attachées telles que :
  - Une **date butoire** ou "step date" qui est la date à laquelle elle doit être terminée
  - Une **durée estimée**
  - Un **statut** à faire, en cours...
  - Le **temps passé sur la tâche**
  - La date indiquant **la dernière fois où la tâche a changée de statut**
  - Un **indice de priorité**, plus l'indice est petit plus la tâche est urgente
  - Un **indice d'intérêt** de l'utilisateur pour cette tâche. Son fonctionnement est détaillé plus bas
  - L'information selon laquelle la tâche est **redondante** ou non


#### Indice d'intérêt

Cet indice par défaut à 1.0 permet de déterminer l'intérêt de l'utilisateur pour une tâche, un projet, ou une catégorie. Plus l'indice est élevé (donc > 1.0) plus l'utilisateur "aime" la tâche / projet / catégorie, à l'inverse, moins l'indice est élevé, moins l'utilisateur "aime" cette tâche. Le calcul  de l'indice reste tout de même différent pour chaque entité. 

Pour **une tâche**, on part du principe que si un utilisateur la fait dans le temps imparti, alors il "aime" la tâche et l'indice augmentera. L'augmentation elle, est déterminée par la rapidité à laquelle il a fait la tâche. Si il l'a terminée avant le premier tiers du temps imparti alors l'indice augmentera de manière conséquente. Un peu moins si la tâche est réalisée avant les deux tiers du temps imparti, et encore moins si elle est réalisée dans le dernier tiers. Cette augmentation est aussi variable en fonction de l'indice d'intérêt de départ. L'augmentation suivant la première complétion d'une tâche (indice de 1.0 donc) conptera moins qu'une augmentation sur un indice de 1.40 montrant un intérêt croissant si ce n'est constant de l'utilisateur pour cette tâche. 
Maintenant l'indice peut aussi diminuer si l'utilisateur ne respecte pas le temps imparti pour la tâche. La diminution varie elle en fonction du temps passé après la date butoire, si celui-ci ne dépasse pas 24h alors la diminution est moindre, puis si la tâche est réalisée 24h à 48h après la diminution est plus importante, pour atteindre la diminution maximale au delà de 48h.
Pour **un projet**, l'indice d'intérêt est calculé en prenant compte des indice d'intérêt des tâches composant ce projet.
Pour **une catégorie**, l'indice est calculé en fonction des indices d'intérêt des projets et des tâches appartenant à cette catégorie.

Bien entendu il sera possible pour l'utilisateur d'indique son intérêt pour chaque catégorie / projet / tâche à leurs création ce qui influencera la valeur par défaut de l'indice d'intérêt.

#### Indice de productivité

L'indice de productivité est associé à une période de la journée (matin ou après-midi) un certain jour de la semaine. il y a donc 14 indice de productivité au total. Il est censé quadriller les périodes de productivité de l'utilisateur en traquant via l'application ses périodes de travail, et est calculé en fonction du temps passé à effectuer des tâches.
L'indice peut même tomber à 0.0 pour certaines périodes d'inactivité (par exemple le dimanche matin)

#### À quoi servent ces indices ?

Ces deux indices serviront à l'algorithme central de l'application pour proposer à l'utilisateur à l'ouverture de l'application les tâches à effectuer aisni que l'ordre dans lequel les faire en fonction de la période dans laquelle il se trouve. Par exemple, si l'utilisateur ouvre l'application un lundi matin et que l'indice de productivité à cette période est de 0.70, c'est que l'utilisateur n'est généralement pas productif dans cette période et on lui proposera donc peut être que 3 tâches dont 2 qui lui plairont particulièrement et une un peu moins.

##### NOTE :

On proposera toujours au moins une tâche (si l'utilisateur en a au moins une) même si l'indice de productivité de la période est 0.0.

### 2 : Gestion des méthodes de travail / apprentissage

Cette deuxième partie de l'application permet à l'utilisateur de chosir une méthode de travail pour réaliser ses tâches. Par défaut on retrouvera la méthode **pomodoro**, d'autres seront ajoutées à l'application au fil du temps. L'utilisateur aura accès à un écran de paramétrage des méthodes permettant par exemple dans le cas de la méthode pomodoro, de changer la durée et la fréquences des péridoes de travail et de pause.

#### Mode travail

L'application s'accompagnera d'un mode travail. Une fois ce mode activé et une méthode de travail sélectionnée, l'application devient l'assistant de l'utilisateur l'accompagnant en fonction de la méthode sélectionnée. Pour la méthode pomodoro par exemple, c'est l'application qui aura la gestion du timmer et qui indiquera à l'utilisateur quand faire ses pauses ou reprendre le travail.
Ce système est intéractif, à chaque fois que l'utilisateur reçoit la notification du début d'une péridoe de pause, il devra faire un choix entre accepter la pause, l'ignorer, ou prolonger la période de travail, et devra donc cliquer sur la notification. Ce système permet deux choses, d'abord à l'application d'être toujours synchronisé avec l'utilisateur et son rythme, et d'accumuler des statistiques qui seront ensuite disponibles sur un tableau de bord.

## Prérequis

- Qt 6.x ou Qt 5.15+
- Compilateur C++ (GCC, Clang, MSVC)
- CMake 3.16+ ou qmake

## Installation

### Cloner le projet
```bash
git clone https://github.com/VOTRE_USERNAME/mon-app-qt.git
cd mon-app-qt
