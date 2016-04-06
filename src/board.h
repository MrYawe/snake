#ifndef BOARD_H
#define BOARD_H
/**
 * \file board.h
 * \author groupe Larakis
 * \version 1
 * \date 20/02/2016
 * \brief Entêtes des fonctions et structure pour la gestion du plateau
 * \details Toutes les entêtes de fonctions et structure necessaires à gérer le plateau : création, récupérer des valeurs, les modifier, afficher le plateau et libérer la mémoire
 */

// ifndef pour être sûr de définir l'enum Control qu'une seule fois
#ifndef ControlDef

#include <stdbool.h>
#include "coord.h"
 
 /**
 * \enum Control
 * \brief Valeurs de gestion du snake
 * \details Contient toutes les valeurs pour la gestion du snake
 **/
typedef enum Control {Line, Column} Control;
#define ControlDef
#endif
typedef struct Board Board;

/* ***************
 *   Init Board  *
 *************** */
Board* boardInit(int sizeX, int sizeY, int sizeCell);

/* ****************
 *    Accessors   *
 **************** */
int boardGetValue(Board *b, int posx, int posy);
void boardSetValue(Board *b, int posx, int posy, int val);

int boardGetHeight(Board *b);
int boardGetWidth(Board *b);

/* ***************
 *   Utilitary   *
 *************** */
void boardDisplay(Board *b);
void boardFree(Board *b);
bool boardInside(Board *b, Coord coord);
bool boardIsSnake(Board *b, Coord coord);

// TODO: bordure qui se grise
// TODO: bordure qui se dégrise

#endif
