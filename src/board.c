/**
 * \file board.c
 * \author groupe Larakis
 * \version 1
 * \date 20/02/2016
 * \brief Contrôle la gestion du plateau
 * \details Toutes les fonctions necessaires à gérer le plateau : création, récupérer des valeurs, les modifier, afficher le plateau et libérer la mémoire
 */

#include <stdio.h>
#include <stdlib.h>
#include "board.h"


/**
 * \struct Board
 * \brief La structure représente un plateau
 * \details La structure contient un tableau à deux dimensions contenant les valeurs du plateau, un entier pour la taille X et un entier pour la taille Y
 **/
struct Board
{
  int** tab;
  int sizeX;
  int sizeY;
};

/**
 * \fn boardInit
 * \brief La fonction crée le plateau
 * \details La fonction alloue la mémoire necessaire au plateau et remplit les éléments de la structure
 * \param sizeX Entier qui done la taille en X du plateau créé
 * \param sizeY Entier qui done la taille en Y du plateau créé
 * \return Variable de type Board qui contiendra le plateau
 */
Board* boardInit(int sizeX, int sizeY)
{
  Board *b = (Board*) malloc(sizeof(Board));
  // Check for negative sizes
  if (sizeX >= 0 && sizeY >=0)
  {
    // Allocation of tab
    b->tab = (int**) malloc(sizeY*sizeof(int*));
    int i;
    for ( i = 0; i < sizeY; i++)
    {
      b->tab[i] = (int*) calloc(sizeX, sizeof(int));
    }
    // Set sizes
    b->sizeX = sizeX;
    b->sizeY = sizeY;
  }
  else
  {
    printf("boardInit: error negative size!\n");
  }
  return b;
}

/**
 * \fn boardGetValue
 * \brief La fonction renvoie la valeur d'un emplacement du plateau
 * \details La fonction teste si les paramètres sont corrects et renvoie, le cas échéant, la valeur correspondante
 * \param b Variable de type Board qui correspond au tableau à parcourir pour trouver la valeur
 * \param posx Variable de type Int qui correpond à l'emplacement X de la valeur à récupérer
 * \param posy Variable de type Int qui correpond à l'emplacement Y de la valeur à récupérer
 * \return Variable de type int qui correspond à l'entier situé l'emplacement voulu sur le plateau
 */
int boardGetValue(Board *b, int posx, int posy)
{
  // Check for out of range exceptions
  if (posx < b->sizeX && posy < b->sizeY && posx >= 0 && posy >=0)
  {
    return b->tab[posy][posx];
  }
  else
  {
    printf("boardGetValue: error out of range (%d, %d) max(%d, %d)\n", posx, posy, b->sizeX, b->sizeY);
    return -1;
  }
}

/**
 * \fn boardSetValue
 * \brief La fonction attribue une valeur à un emplacement du plateau
 * \details La fonction teste si les paramètres sont corrects et attribue, le cas échéant, la valeur correspondante au bon emplacement du plateau
 * \param b Variable de type Board qui correspond au tableau à parcourir pour attribuer la valeur
 * \param posx Variable de type Int qui correpond à l'emplacement X de la valeur à attriber
 * \param posy Variable de type Int qui correpond à l'emplacement Y de la valeur à attribuer
 * \param val Variable de type Int qui correpond à la valeur à attribuer
 */
void boardSetValue(Board *b, int posx, int posy, int val)
{
  // Check for out of range exceptions
  if (posx < b->sizeX && posy < b->sizeY && posx >= 0 && posy >=0)
  {
    b->tab[posy][posx] = val;
  }
  else
  {
    printf("boardSetValue: error out of range\n");
  }
}


/**
 * \fn boardGetSize
 * \brief La fonction renvoie la taille du plateau
 * \details La fonction renvoie la valeur correspondante à la taille en X du tableau
 * \param b Variable de type Board qui correspond au tableau en question
 * \param c Variable de type enum Control qui correspond à ligne ou colonne suivant ce que l'on souhaite récupérer
 * \return Variable de type int qui correspond à la taille en X du tableau
 */
int boardGetSize(Board *b, Control c)
{
  int res = 0;
  if (b != NULL) {
    if (c == Line) {
      res = b->sizeX;
    } else if (c == Column) {
      res = b->sizeY;
    } else {
      printf("boardGetSize : error enum not known\n");
    }
  }
  return res;
}

/**
 * \fn boardDisplay
 * \brief La fonction affiche le plateau dans la console
 * \details La fonction affiche pour chaque case du plateau sa valeur dans la console
 * \param b Variable de type Board qui correspond au tableau à afficher
 */
void boardDisplay(Board *b)
{
  system("clear");
  int i, j;
  for(j=0; j<b->sizeY; j++)
  {
    for (i=0; i<b->sizeX; i++)
    {
      printf(" %d ", boardGetValue(b, i, j));
    }
    printf("\n");
  }
}

/**
 * \fn boardFree
 * \brief La fonction libère la mémoire du plateau
 * \details La fonction libère entièrement le tableau à deux dimensions de la mémoire
 * \param b Variable de type Board qui correspond au tableau à effacer
 */
void boardFree(Board *b)
{
  int i;
  for ( i = 0; i < b->sizeY; i++ )
  {
    free(b->tab[i]);
  }
  free(b->tab);
  free(b);
}