/**
 * \file serpent.c
 * \author groupe Larakis
 * \version 1
 * \date 21/02/2016
 * \brief Classe qui contient toute la gestion du snake
 * \details Toutes les fonctions necessaires à gérer le snake : création, récupération des valeurs, changer de direction, affichage et libération de la mémoire
 */
#include <stdio.h>
#include <stdlib.h>
#include "serpent.h"

// Headers for static functions of snake file
static void addFirstElement(Snake *s, int posX, int posY);
static void addLastElement(Snake *s, int posX, int posY);
static void deleteFirstElement(Snake *s);
static void deleteLastElement(Snake *s);


/**
 * \struct Snake
 * \brief La structure représente le snake
 * \details La structure contient un pointeur vers le début et la fin de la liste, un attribut taille de la liste, et un attribut direction qui représente la direction actuelle du snake
 **/
struct Snake
{
	Element *first;
	Element *last;
	int size;
	Direction direction;
	Way way;
};

/**
 * \struct Element
 * \brief La structure représente un élément de la liste chainée qui contient le snake
 * \details La structure contient 4 éléments : la position X, la position Y, l'élément suivant et précedent de la liste
 **/
struct Element 
{
	int posX;
	int posY;
	Element *next;
	Element *previous;
};

/**
 * \fn createSnake
 * \brief La fonction crée et renvoie un snake
 * \details La fonction alloue la mémoire necessaire à un snake et remplit les éléments de la structure
 * \param size Entier qui correspond au nombre d'anneaux pour le snake
 * \return Variable de type Snake qui contiendra un snake
 */
Snake* createSnake(int size)
{
	Snake *s = malloc(sizeof(Snake));
	s->first = NULL;
	s->last = NULL;
	s->direction = NIL;
	s->size = 0;
	s->way = Normal;
	int i;
	for (i=0; i<size; i++)
	{
		addFirstElement(s, 0, 0);
	}	
	return s;
}

/**
 * \fn goUp
 * \brief La fonction ajuste le snake pour que sa position goUp d'une case
 * \details La fonction rajoute un élément en tête de liste et supprime le last de manière à permettre au snake de goUpr
 * \param s Variable de type Snake* qui pointe vers le snake à faire goUpr
 */
void goUp(Snake *s)
{
	if (s->way == Normal)
	{
		addLastElement(s, s->last->posX, s->last->posY + 1);
		deleteFirstElement(s);
	}
	else if (s->way == Reversed)
	{
		addFirstElement(s, s->first->posX, s->first->posY + 1);
		deleteLastElement(s);
	}	
	s->direction = UP;
}

/**
 * \fn goDown
 * \brief La fonction ajuste le snake pour que sa position goDowne d'une case
 * \details La fonction rajoute un élément en tête de liste et supprime le last de manière à permettre au snake de goDownre
 * \param s Variable de type Snake* qui pointe vers le snake à faire goDownre
 */
void goDown(Snake *s)
{
	if (s->way == Normal)
	{
		addLastElement(s, s->last->posX, s->last->posY - 1);
		deleteFirstElement(s);
	}
	else if (s->way == Reversed)
	{
		addFirstElement(s, s->first->posX, s->first->posY - 1);
		deleteLastElement(s);
	}	
	s->direction = DOWN;
}

/**
 * \fn turnLeft
 * \brief La fonction ajuste le snake pour qu'il soit déplacé vers la gauche
 * \details La fonction rajoute un élément en tête de liste et supprime le last de manière à permettre au snake de tourner à gauche
 * \param s Variable de type Snake* qui pointe vers le snake à faire tourner à gauche
 */
void turnLeft(Snake *s)
{
	if (s->way == Normal)
	{
		addLastElement(s, s->last->posX - 1, s->last->posY);
		deleteFirstElement(s);
	}
	else if (s->way == Reversed)
	{
		addFirstElement(s, s->first->posX - 1, s->first->posY);
		deleteLastElement(s);
	}	
	s->direction = LEFT;
}

/**
 * \fn turnRight
 * \brief La fonction ajuste le snake pour qu'il soit déplacé vers la droite
 * \details La fonction rajoute un élément en tête de liste et supprime le last de manière à permettre au snake de tourner à droite
 * \param s Variable de type Snake* qui pointe vers le snake à faire tourner à droite
 */
void turnRight(Snake *s)
{
	if (s->way == Normal)
	{
		addLastElement(s, s->last->posX + 1, s->last->posY);
		deleteFirstElement(s);
	}
	else if (s->way == Reversed)
	{
		addFirstElement(s, s->first->posX + 1, s->first->posY);
		deleteLastElement(s);
	}
	s->direction = RIGHT;
}

/**
 * \fn getPosX
 * \brief La fonction renvoie la valeur posX d'un maillon
 * \details La fonction se déplace dans la liste jusqu'à l'endroit voulu puis renvoie la valeur. Une erreur est levée si la valeur passée en paramètre n'est pas bonne.
 * \param s Variable de type Snake* qui pointe vers le snake à analyser
 * \param pos Variable de type int qui correspond à l'emplacement voulu
 */
int getPosX(Snake *s, int pos)
{
	int res = 0;
	if (pos < 0 || pos >= s->size)
	{
		printf("getPosition : Error pos out of range\n");
	}
	else
	{
		int i;
		Element *e = s->first;
		for (i = 0; i < pos; i++)
		{
			e = e->next;
		}
		res = e->posX;
	}		
	return res;
}

/**
 * \fn getPosY
 * \brief La fonction renvoie la valeur posY d'un maillon
 * \details La fonction se déplace dans la liste jusqu'à l'endroit voulu puis renvoie la valeur. Une erreur est levée si la valeur passée en paramètre n'est pas bonne.
 * \param s Variable de type Snake* qui pointe vers le snake à analyser
 * \param pos Variable de type int qui correspond à l'emplacement voulu
 */
int getPosY(Snake *s, int pos)
{
	int res = 0;
	if (pos < 0 || pos >= s->size)
	{
		printf("getPosition : Error pos out of range\n");
	}
	else
	{
		int i;
		Element *e = s->first;
		for (i = 0; i < pos; i++)
		{
			e = e->next;
		}
		res = e->posY;
	}		
	return res;
}

/**
 * \fn getWay
 * \brief La fonction renvoie le sens du serpent
 * \details La fonction récupère le sens du serpent dans la structure
 * \param s Variable de type Snake* qui pointe vers le snake à parcourir
 */
Way getWay(Snake *s)
{
	return s->way;
}

/**
 * \fn setWay
 * \brief La fonction modifie le sens du serpent
 * \details La fonction modifie le sens du serpent dans la structure par la valeur passée en paramètres
 * \param s Variable de type Snake* qui pointe vers le snake à parcourir
 * \param w Variable de type Way qui est la valeur à attribuer au snake
 */
void setWay(Snake *s, Way w)
{
	s->way = w;
}

/**
 * \fn getSize
 * \brief La fonction renvoie la taille du serpent
 * \details La fonction récupère la taille dans la structure
 * \param s Variable de type Snake* qui pointe vers le snake à parcourir
 */
int getSize(Snake *s)
{
	return s->size;
}

/**
 * \fn displaySnake
 * \brief La fonction affiche le contenu du snake
 * \details La fonction affiche pour chaque element du plateau son contenu ainsi que la direction du snake
 * \param s Variable de type Snake* qui pointe vers le snake à afficher
 */
void displaySnake(Snake *s)
{
	char const* direc[] = {"le haut", "la gauche", "la droite", "le bas", "indéfini"};
	printf("Le snake va vers %s\n", direc[s->direction]);

	Element *curseur = s->first;
	int i=0;
	while(curseur != NULL)
	{
		printf("Maillon %d : [posX: %d, posY: %d]\n", i, curseur->posX, curseur->posY);
		curseur = curseur->next;
		i++;
	}
}

/**
 * \fn updateElement
 * \brief La fonction modifie les valeurs d'un élément de la liste
 * \details La fonction attribue les valeurs passées en paramètres à l'élément situé à la posElem position
 * \param s Variable de type Snake* qui pointe vers le snake à modifier
 * \param posElem Variable de type int qui correspond à l'emplacement dans la liste de l'element à modifier
 * \param posX Variable de type int qui correspond à la valeur à rajouter pour la position X
 * \param posY Variable de type int qui correspond à la valeur à rajouter pour la position Y
 */
void updateElement(Snake *s, int posElem, int posX, int posY)
{
	if (posElem < 0 || posElem >= s->size)
	{
		printf("updateElement : Error posElem out of range\n");
	}
	else
	{
		int i;
		Element *e = s->first;
		for (i = 0; i < posElem; i++)
		{
			e = e->next;
		}
		e->posX = posX;
		e->posY = posY;
	}	
}


/**
 * \fn deleteSnake
 * \brief La fonction libère le serpent de la mémoire
 * \details La fonction libère entièrement chaque élément de la liste chainée
 * \param s Variable de type Snake* qui pointe vers le snake à libérer
 */
void deleteSnake(Snake *s)
{
	Element *curs = s->first;
	Element *curssuiv;
	while(curs != NULL)
	{
		curssuiv = curs->next;
		free(curs);
		curs = curssuiv;
	}
}


/**
 * \fn addFirstElement
 * \brief La fonction ajoute un élément au début de la liste chaînée
 * \details La fonction crée un élément en mémoire, lui attribue les valeurs passées en paramètres et gère le cas de la liste vide et l'ajoute en début de liste
 * \param s Variable de type Snake* qui pointe vers le snake à modifier
 * \param posX Variable de type int qui correspond à la valeur à rajouter pour la position X
 * \param posY Variable de type int qui correspond à la valeur à rajouter pour la position Y
 */
static void addFirstElement(Snake *s, int posX, int posY)
{
	Element *e = (Element*) malloc(sizeof(Element));
	e->posX = posX;
	e->posY = posY;
	e->previous = NULL;	
	if (s->size == 0)
	{
		e->next = NULL;
		s->last = e;
	}		
	else
	{
		s->first->previous = e;
		e->next = s->first;		
	}
	s->first = e;
	s->size ++;
}

/**
 * \fn addFirstElement
 * \brief La fonction ajoute un élément à la fin de la liste chaînée
 * \details La fonction crée un élément en mémoire, lui attribue les valeurs passées en paramètres et gère le cas de la liste vide et l'ajoute en fin de liste
 * \param s Variable de type Snake* qui pointe vers le snake à modifier
 * \param posX Variable de type int qui correspond à la valeur à rajouter pour la position X
 * \param posY Variable de type int qui correspond à la valeur à rajouter pour la position Y
 */
static void addLastElement(Snake *s, int posX, int posY)
{
	if (s->size == 0)
	{
		addFirstElement(s, posX, posY);
	}
	else
	{
		Element *e = (Element*) malloc (sizeof(Element));
		e->posX = posX;
		e->posY = posY;
		e->previous = s->last;
		e->next = NULL;
		s->last->next = e;
		s->last = e;
		s->size++;
	}	
}

/**
 * \fn deleteFirstElement
 * \brief La fonction supprime un élément au début de la liste chaînée
 * \details La fonction supprime le first élément de la liste et envoie un message d'erreur si la liste est vide
 * \param s Variable de type Snake* qui pointe vers le snake à modifier
 */
static void deleteFirstElement(Snake *s)
{
	if (s->size == 0)
	{
		printf("deleteFirstElement : Empty list\n");
	}
	else
	{
		Element *e = s->first;
		s->first = s->first->next;
		s->first->previous = NULL;
		free(e);
		s->size --;
	}	
}

/**
 * \fn deleteLastElement
 * \brief La fonction supprime un élément à la fin de la liste chaînée
 * \details La fonction supprime le last élément de la liste et envoie un message d'erreur si la liste est vide
 * \param s Variable de type Snake* qui pointe vers le snake à modifier
 */
static void deleteLastElement(Snake *s)
{
	if (s->size == 0)	
	{
		printf("deletaLastElement : Empty List !\n");
	}
	else
	{
		Element *e = s->last;
		s->last = s->last->previous;
		s->last->next = NULL;
		free(e);
		s->size --;
	}
}
