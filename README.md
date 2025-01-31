# Projet-PARM-SI3
A repo to keep track of components built for the Architecture Project

## Description
Le projet **PARM** consiste à implémenter un microprocesseur simulant le **Cortex-M0** à l’aide de **Logisim Evolution**. L’objectif est de comprendre et modéliser le fonctionnement d’un processeur ARM simplifié en divisant son architecture en plusieurs blocs fondamentaux.

## Structure du projet
Le projet est divisé en deux parties principales :
1. **Partie matérielle** (Hardware)
   - ALU (Arithmetic Logic Unit) 32 bits
   - Banc de registres (8 registres)
   - Contrôleur et unité de décodage d’instruction
   - Chemin de données (liaison entre l’ALU, le banc de registres et la mémoire)
   - Interface mémoire (RAM, ROM)
   - Périphériques d’entrée/sortie

2. **Partie logicielle** (Software)
   - Assembleur pour transformer le code assembleur en instructions compréhensibles par le processeur
   - Compilation de code C pour générer du code compatible avec l’architecture du processeur simulé
  
## Fonctionnalités et Spécifications
- **Architecture**
  - Instructions sur **16 bits**
  - Données stockées sur **32 bits**
  - Gestion de la RAM sur **9 bits d’adressage**
  - Uniquement **un cycle d’exécution par instruction**(LDR/STR)
  - Implémentation du **jeu d’instructions ARMv7-M**
  - Pas de gestion d’interruptions, d’appels de fonction, de pipeline ou de FPU/MMU

- **Modules principaux**
  - **ALU** : Effectue les opérations arithmétiques et logiques (addition, soustraction, décalages, comparaisons…)
  - **Banc de registres** : Contient **8 registres généraux** pour stocker les données intermédiaires
  - **Contrôleur** : Décode les instructions et pilote le chemin de données
  - **Chemin de données** : Assure l’échange des données entre l’ALU, les registres et la mémoire
  - **Mémoire** :
    - **RAM** : Stockage temporaire des données (256 mots de 32 bits)
    - **ROM** : Contient le programme à exécuter
  - **Interface Périphériques** : Simulation d'entrées/sorties comme un terminal et des DIP Switches
 
    ## Outils et Dépendances
- **Logisim Evolution** 
- **C** (pour exécuter l'assembleur et générer des instructions compatibles avec la simulation)

