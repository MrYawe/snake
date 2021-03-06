#ifndef SNAKE_H
#define SNAKE_H
/**
 * \file snake.h
 * \author groupe Larakis
 * \version 1
 * \date 20/02/2016
 * \brief Headers of snake file
 * \details All header of the snake file functions
 */

#include <stdbool.h>
#include "coord.h"
#include "item.h"


/**
 * \enum Direction
 * \brief Permet de gérer la direction du snake
 * \details Contient toutes les directions possibles pour le snake
 **/
typedef enum Direction {NIL=-1, UP, RIGHT, DOWN, LEFT} Direction;

/**
 * \enum Type
 * \brief Permet de gérer le type du snake
 * \details Contient tous les types possibles pour le snake
 **/
typedef enum SnakeType {WATER, FIRE, GRASS} SnakeType;

typedef struct Element Element;
/**
 * \struct Element
 * \brief The whole body of the snake
 * \details The Element structure which is a chained list which represents the body of the snake
 **/
struct Element
{
	Coord pos;
	Element *next;
	Element *previous;
	Direction orientation;
};

/**
 * \struct Snake
 * \brief The snake structure
 * \details The snake structure with all the snake attributes
 **/
struct Snake
{
	int id;
	Element *head;
	Element *tail;
	int size;
	int speed;
	Direction direction;
	SnakeType type;
	Item itemList;

	bool isControlReversed;
	bool canCrossBorder;
	bool canCrossSnake;

	bool isAffectedByField;
	//bool isGhost;
};
typedef struct Snake* Snake;


/* ***************
 *   Init Snake  *
 *************** */
Snake snakeCreate(int size, int id, Direction d, SnakeType type);

/* ***************
 *   Move Snake  *
 *************** */
void snakeGoUp(Snake s);
void snakeGoDown(Snake s);
void snakeTurnLeft(Snake s);
void snakeTurnRight(Snake s);
void snakeTeleportation(Snake s, int posX, int posY);

void snakeGrowHead(Snake s);
void snakeGrowTail(Snake s);

/* ****************
 *    Accessors   *
 **************** */
Coord snakeGetPos(Snake s, int posBloc);

Direction snakeGetDirection(Snake s);
void snakeSetDirection(Snake s, Direction d);

int snakeGetSize(Snake s);

Item snakeGetItemList(Snake s);
void snakeSetItemList(Snake s, Item i);
int snakeGetSpeed(Snake s);
void snakeSetSpeed(Snake s, int speed);

Direction snakeElementGetOrientation(Snake s, int posElem);
void snakeElementSetOrientation(Snake s, int posElem, Direction d);

void snakeSetIsControlReversed(Snake s, bool b);
bool snakeGetIsControlReversed(Snake s);
void snakeSetCanCrossBorder(Snake s, bool b);
bool snakeGetCanCrossBorder(Snake s);
void snakeSetCanCrossSnake(Snake s, bool b);
bool snakeGetCanCrossSnake(Snake s);
bool snakeIsGhost(Snake s);

int snakeGetId(Snake s);

SnakeType snakeGetType(Snake s);
void snakeSetType(Snake s, SnakeType t);

/* ***************
 *   Utilitary   *
 *************** */
// Headers for static functions of snake file
void snakeAddFirstElement(Snake s, int posX, int posY, Direction orientation);
void snakeAddLastElement(Snake s, int posX, int posY, Direction orientation);
void snakeDeleteFirstElement(Snake s);
//static void snakeDeleteLastElement(Snake s);
//static int mod(int a, int b);
void snakeInverseWay(Snake s);
void snakeDisplay(Snake s);
void snakeUpdateElement(Snake s, int posElem, int posX, int posY);
void snakeFree(Snake s);

// TODO: Augmenter la taille de X elements
// TODO: Diminuer la taille de X elements

#endif
