# Page d'Accueil - SAI (Smart Assistant Interface)

## Description fonctionnelle

### Vue d'ensemble
La page d'accueil présente un tableau de bord centralisé permettant à l'utilisateur de visualiser ses tâches prioritaires et de démarrer une session de travail. L'interface suit une logique de productivité avec mise en avant des prochaines actions à effectuer.

## Architecture de l'interface

### Zone d'en-tête
- Logo de l'application 
- Informations de l'utilisateur connecté (nom + avatar)
- Barre persistante en haut de toutes les pages

### Panneau latéral gauche
**Section Catégories**
- Liste dynamique des catégories de tâches disponibles
- Chaque catégorie possède une couleur distinctive pour l'identification visuelle
- Navigation cliquable vers la vue détaillée de chaque catégorie
- Bouton d'ajout de nouvelle catégorie

**Section Projets**
- Liste des projets
- Sélectionner un projet filtrera les tâches visibles sur l'écran.

### Zone de contenu principale
**Contrôles de session**
- Bouton principal "Commencer" pour démarrer une session de travail
- Sélecteur de méthode de productivité (Pomodoro, Time Blocking, etc.)
- Interface de paramétrage de la session

**Timeline des tâches prioritaires**
- Affichage séquentiel des prochaines tâches à traiter
- Organisation en étapes numérotées avec progression visuelle
- Layout en escalier sur 2 colonnes pour optimiser l'espace
- Chaque tâche affiche :
  - Titre et description
  - Durée estimée
  - Échéance
  - Indicateur de priorité si applicable
  - Catégorie d'appartenance (code couleur)

## Fonctionnalités

### Navigation
- Clic sur une catégorie → redirection vers la vue détaillée
- Bouton "Commencer" → lancement d'une session de travail focalisée

### Interactions
- Animations de survol sur les éléments cliquables
- Transitions fluides entre les états
- Indicateurs visuels de priorité (animations pulsées)
- Retour d'information lors du démarrage de session

### Logique métier
- Algorithme de priorisation des tâches par échéance et importance
- Mise à jour dynamique de la timeline selon la durée qui sépare la date actuelle de l'échéance de chaque tâche

## Objectifs UX
- **Simplicité** : Interface épurée focalisée sur l'essentiel
- **Démarrage rapide** : Accès immédiat aux prochaines actions
- **Vue d'ensemble** : Compréhension globale de la charge de travail
- **Motivation** : Progression visuelle et gamification subtile
