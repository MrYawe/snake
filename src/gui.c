/**
 * \file gui.c
 * \author groupe Larakis
 * \version 1
 * \date 19/04/2016
 * \brief Everything about the gui class
 * \details Contains all functions to manage the gui and interract with the user
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "gui.h"
#include "constants.h"
#include "game.h"
#include "ia.h"

/**
 * \fn guiPlay(BoardSize size)
 * \brief This function is the main game loop
 * \details Function that create a new game, perform the game loop and display the game with SDL
 * \param size Variable representing the size of the Board (Small, Medium or Large)
 */
int guiPlay(BoardSize size)
{
      /*******************/
     /**   VARIABLES   **/
    /*******************/

    /***** SDL Variables *****/
    SDL_Event event;    //Variable to capture mouse/keyboard events
    SDL_Surface* screen;//Buffer that will contain the pixels to render on the screen

    /***** General Variables *****/
    Assets assets;  //Variable that will contain all the assets used in the game
    Timer timer;    //Variable used to control the actions based on the time

    /***** Structure Variables *****/
    bool continueGameMove1, continueGameMove2;	//Variable used to check if the snake x is dead
    bool snake1InDebuff, snake2InDebuff;

    Game game;		//Variable to access the game
    Board board;	//Variable to access the board
    Snake snake1;	//Variable to access the first snake
    Snake snake2;	//Variable to access the first snake


	  /************************/
	 /**   INITIALIZATION   **/
	/************************/

	/***** SDL Initialization *****/
    SDL_Init(SDL_INIT_VIDEO);				//Initialization of the SDL Library
    TTF_Init();
    SDL_WM_SetCaption("Larasnake", NULL);	//Set the title and icon name of the displayed window
    screen = guiCreateScreen(size);

    /***** General Variables *****/
    srand(time(NULL));  //Initialization of the random function
    timer = guiCreateTimer();
    assets = guiLoadAssets(size);

    /***** Structure Variables *****/
    continueGameMove1 = true;
    continueGameMove2 = true;
    snake1InDebuff = false;
    snake2InDebuff = false;

    game = gameCreate(size);
    board = gameGetBoard(game);

    guiSetFieldType(game, assets, size);


    snake1 = gameGetSnake(game, 1);
    snake2 = gameGetSnake(game, 2);

    //music of the game
    Mix_Music *musiqueGame = Mix_LoadMUS("./sound/musiqueGame.mp3"); //music of the game
    Mix_PlayMusic(musiqueGame, -1); //loop  --PLAY HERE
    Mix_VolumeMusic(MIX_MAX_VOLUME / 6); //half of the maximum sound


      /************************/
     /**      GAME LOOP     **/
    /************************/
    gameFeed(game, true); //first ham appeared
    while (gameGetIsPlaying(game)) {
        timer->start = SDL_GetTicks(); // Start of the current frame

        guiEvent(&event, snake1, game); // catch player event and set the direction of snake1
        if(!gameGetIsPaused(game)) {
            if(gameGetPauseTimer(game)==0) //know if we don't leave the pause
            {
                if(boardGetType(board)) //if we have to change the background
                {
                    boardSetType(board, false); //disable change flag
                    guiChangeBackground(screen, assets,size);
                    guiSetFieldType(game, assets, size);
                    printf("MAP CHANGED\n"); //the map is now changed
                }
              ////// Move of snake 1 (player) //////
              timer->snake1MoveTimer += SDL_GetTicks() - timer->snake1LastMove;
              if (timer->snake1MoveTimer >= snakeGetSpeed(snake1)) {  // test if we wait enough time to move the snake 1
                  continueGameMove1 = gameMoveSnake(game, snake1);   // move th snake1. if snake1 is dead continueGameMove1=false
                  timer->snake1MoveTimer = 0;                         // set the move timer to 0 when the snake move
              }
              timer->snake1LastMove = SDL_GetTicks();
              /////////////////////////////////////

              ////// Move of snake 2 (AI) //////
              timer->snake2MoveTimer += SDL_GetTicks() - timer->snake2LastMove;
              if (timer->snake2MoveTimer >= snakeGetSpeed(snake2)) {  // test if we wait enough time to move the snake 2
                  snakeSetDirection(snake2, iaSurviveDepth(board, snake2, snake1));  // let ia choose the best direction of snake2
                  continueGameMove2 = gameMoveSnake(game, snake2);   // move the snake2. if snake2 is dead continueGameMove2=false
                  timer->snake2MoveTimer = 0;                        // set the move timer to 0 when the snake move
              }
              timer->snake2LastMove = SDL_GetTicks();
              /////////////////////////////////

              ///////////////// Debuff snake1 /////////////////
              if (!itemListIsEmpty(snakeGetItemList(snake1))) {
                  if (!snake1InDebuff) {
                      snake1InDebuff = true;
                      timer->snake1LastDebuff = SDL_GetTicks();
                  } else {
                      timer->snake1DebuffTimer += SDL_GetTicks() - timer->snake1LastDebuff;
                      if(timer->snake1DebuffTimer >= ITEM_DEBUFF_INTERVAL) {
                          gameItemDebuff(snakeGetItemList(snake1)->next, snake1);
                          snake1InDebuff = false;
                          timer->snake1DebuffTimer = 0;
                      }
                      timer->snake1LastDebuff = SDL_GetTicks();
                  }
              }
              ////////////////////////////////////////////////

              ///////////////// Debuff snake2 /////////////////
              if (!itemListIsEmpty(snakeGetItemList(snake2))) {
                  if (!snake2InDebuff) {
                      snake2InDebuff = true;
                      timer->snake2LastDebuff = SDL_GetTicks();
                  } else {
                      timer->snake2DebuffTimer += SDL_GetTicks() - timer->snake2LastDebuff;
                      if(timer->snake2DebuffTimer >= ITEM_DEBUFF_INTERVAL) {
                          gameItemDebuff(snakeGetItemList(snake2)->next, snake2);
                          snake2InDebuff = false;
                          timer->snake2DebuffTimer = 0;
                      }
                      timer->snake2LastDebuff = SDL_GetTicks();
                  }
              }
              ////////////////////////////////////////////////


              ///////// Item pop /////////
              timer->itemPopTimer += SDL_GetTicks() - timer->itemLastPop;
              if(timer->itemPopTimer >= ITEM_POP_INTERVAL) {
                  gameFeed(game, false); //Function called to put some food on the board
                  timer->itemPopTimer = 0;
              }
              timer->itemLastPop = SDL_GetTicks();
              ///////// Item pop /////////

//>>>>>>> 43afef4547198632093d7b891941aa8c99643d24
            }


        }

        /////// Draw ///////
        guiDrawGame(screen, game, assets, size);  // draw the board on screen with surfaces stored in the Assets struct
        guiReloadScreen(screen);            // reload all the screen
        //boardDisplay(board);
        ///////////////////

        if(!continueGameMove1 || !continueGameMove2) // if one snake die the game is over
            gameEnd(game);

        ////// Framerate management //////
        timer->end = SDL_GetTicks();                           // Get the time after the calculations
        timer->delay = FRAME_MS - (timer->end - timer->start); // Calculate how long to delay should be

        if(timer->delay > 0) {
            SDL_Delay(timer->delay);                           // Delay processing
        }
    }

    int idWinner;
    if(continueGameMove1) {
        if(snakeGetId(snake1) == 1)
            idWinner = 1;
        else
            idWinner = 2;
    } else {
        if(snakeGetId(snake1) == 1)
            idWinner = 2;
        else
            idWinner = 1;
    }

    ////// Free //////
    gameFree(game);
    guiFreeAssets(assets);
    Mix_FreeMusic(musiqueGame); //free the music

    //TTF_Quit();
    //SDL_Quit();
    return idWinner;
}

void guiChangeBackground(SDL_Surface* screen, Assets assets, BoardSize size)
{
    int tailleTerrain=2; //check the field size
    if(size==LARGE)
    tailleTerrain=3;
    else if(size==SMALL)
    tailleTerrain=1;
    int randType = rand()%3+1; /// PASSER LE MODULO A TROIS POUR LES RONCES
    while(randType==assets->currentBg)
    {
      randType=rand()%3+1;
    }


    assets->currentBg = randType;
    char path[31];
    sprintf(path,"./images/background/bg-%d-%d.jpg",randType, tailleTerrain);
    assets->background = guiLoadImage(path); //load random field
}

/**
 * \fn void guiSetFieldType(Game g, Assets a)
 * \brief The function load in the board the field associate to the txt file
 * \details The function load in the board array the field values, to check if the snake get a bonus or not according to the field
 * \param g Game: the game instance
 * \param a Assets: the gui assets (get the current backgroundType)
 * \param size BoardSize: the size of the board
 * \return The SDL_Surface* of the screen
 */
void guiSetFieldType(Game g, Assets a, BoardSize size)
{
    int fieldType=a->currentBg; //curent background type
    int tailleTerrain=2; //check the field size
    if(size==LARGE)
    tailleTerrain=3;
    else if(size==SMALL)
    tailleTerrain=1;
    gameSetFieldValue(g,fieldType, tailleTerrain);
}


/**
 * \fn SDL_Surface* guiCreateScreen(BoardSize size)
 * \brief The function creates the screen
 * \details The function creates the screen depending on the size in parameter
 * \param size BoardSize: the type of the board (Small, Medium, Large)
 * \return The SDL_Surface* of the screen
 */
SDL_Surface* guiCreateScreen(BoardSize size) {
    SDL_Surface* screen;
    switch (size) {
        case SMALL:
            screen = SDL_SetVideoMode(S_SIZE_BOARD_X*S_CELL_SIZE, 2*S_CELL_SIZE + S_SIZE_BOARD_Y*S_CELL_SIZE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
            break;

        case MEDIUM:
            screen = SDL_SetVideoMode(M_SIZE_BOARD_X*M_CELL_SIZE, 2*M_CELL_SIZE + M_SIZE_BOARD_Y*M_CELL_SIZE, 32, SDL_FULLSCREEN | SDL_DOUBLEBUF);
            break;

        case LARGE:
            screen = SDL_SetVideoMode(L_SIZE_BOARD_X*L_CELL_SIZE, 2*L_CELL_SIZE + L_SIZE_BOARD_Y*L_CELL_SIZE, 32, SDL_FULLSCREEN | SDL_DOUBLEBUF);
            break;
    }

    return screen;
}

/**
 * \fn SDL_Surface* guiLoadImage(char* path)
 * \brief The function load the image given in parameter
 * \details The function load the image given in parameter
 * \param path the path of the image relative to the root folder
 * \return The SDL_Surface* of the image
 */
SDL_Surface* guiLoadImage(char* path) {
    SDL_Surface *s;
    if(!(s = IMG_Load(path)))
        printf("%s\n", IMG_GetError());

    return s;
}

/**
 * \fn Assets guiLoadAssets()
 * \brief The function load all assets of the game
 * \details The function load all assets of the game
 * \return The Assets struct with all assets loaded
 */
Assets guiLoadAssets(BoardSize size) {
    Assets assets = malloc(sizeof(struct Assets));


    int tailleTerrain=2;
    if(size==LARGE)
      tailleTerrain=3;
    else if(size==SMALL)
      tailleTerrain=1;
    int randType = rand()%3+1; // PASSER LE MODULO A TROIS POUR LES RONCES

    char path[31];
    sprintf(path,"./images/background/bg-%d-%d.jpg", randType, tailleTerrain);
    assets->background = guiLoadImage(path); //load random field
    assets->currentBg = randType;
    assets->itemsAssets = guiLoadItems();
    assets->guiAssets = guiLoadGui(size);

    char *colors[3] = {"blue", "red", "green"};
    assets->snakesAssets = malloc(3*sizeof(SnakeAssets));
    int i;
    for (i = 0; i < 3; i++) { // mettre à i < 3 si 3 couleurs chargé
        assets->snakesAssets[i] = guiLoadSnake(i, colors[i]); // WATER=0, FIRE=1, GRASS=2
    }



    return assets;
}

/**
 * \fn void guiFreeAssets(Assets assets)
 * \brief The function free the struct Assets
 * \details The function free the struct Assets
 * \param assets The Assets struct to free
 */
void guiFreeAssets(Assets assets) {
    free(assets->background);
    int i;
    for (i = 0; i < 3; i++) { // mettre à i < 3 si 3 couleurs chargé
        free(assets->snakesAssets[i]); // WATER=0, FIRE=1, GRASS=2
    }

    free(assets);
}

/**
 * \fn SnakeAssets guiLoadSnake(SnakeType type, char* color)
 * \brief The function load a SnakeAssets
 * \details The function load all assets needed to draw a snake (head, body, corner, tail)
 * \param type The snake type (WATER, FIRE, GRASS)
 * \param color The color of the snake
 * \return The SnakeAssets struct of the loaded snake
 */
SnakeAssets guiLoadSnake(SnakeType type, char* color) {
    char *path = malloc(100*sizeof(char));
    SnakeAssets snakeAssets = malloc(sizeof(struct SnakeAssets));
    snakeAssets->head = malloc(4*sizeof(SDL_Surface*));
    snakeAssets->body = malloc(4*sizeof(SDL_Surface*));
    snakeAssets->corner = malloc(4*sizeof(SDL_Surface*));
    snakeAssets->tail = malloc(4*sizeof(SDL_Surface*));

    char sDirection[4];
    sDirection[0] = 'U';
    sDirection[1] = 'R';
    sDirection[2] = 'D';
    sDirection[3] = 'L';

    int direction; // UP=0, RIGHT=1, DOWN=2, LEFT=3
    for (direction = 0; direction < 4; direction++) {
        sprintf(path, "./images/snake/%s/head/%c.png", color, sDirection[direction]);
        snakeAssets->head[direction] = guiLoadImage(path);
        sprintf(path, "./images/snake/%s/corner/%c.png", color, sDirection[direction]);
        snakeAssets->corner[direction] = guiLoadImage(path);
        sprintf(path, "./images/snake/%s/tail/%c.png", color, sDirection[direction]);
        snakeAssets->tail[direction] = guiLoadImage(path);
        sprintf(path, "./images/snake/%s/body/%c.png", color, sDirection[direction]);
        snakeAssets->body[direction] = guiLoadImage(path);
    }

    return snakeAssets;
}

/**
 * \fn SDL_Surface** guiLoadItems()
 * \brief The function load Items assets
 * \details The function load all assets needed to draw items
 * \return Array of SDL_Surface*
 */
SDL_Surface** guiLoadItems() {

    SDL_Surface **itemsAssets = malloc(NB_ITEM*sizeof(SDL_Surface*));
    itemsAssets[FOOD] = guiLoadImage("./images/item/food.png");
    itemsAssets[SPEED_UP] = guiLoadImage("./images/item/speed-up.png");
    itemsAssets[GROW_UP] = guiLoadImage("./images/item/grow-up.png");
    itemsAssets[GROW_DOWN] = guiLoadImage("./images/item/grow-down.png");
    itemsAssets[REVERSE_CONTROL] = guiLoadImage("./images/item/reverse-control.png");
    itemsAssets[REVERSE_SNAKE] = guiLoadImage("./images/item/reverse-snake.png");
    itemsAssets[NO_BORDER] = guiLoadImage("./images/item/no-border.png");
    itemsAssets[GHOST] = guiLoadImage("./images/item/ghost.png");
    itemsAssets[SWAP_SNAKE] = guiLoadImage("./images/item/swap-snake.png");
    itemsAssets[NEW_COLOR] = guiLoadImage("./images/item/new-color.png");
    itemsAssets[NEW_MAP] = guiLoadImage("./images/item/new-map.png");
    itemsAssets[WALL] = guiLoadImage("./images/item/wall.png");
    itemsAssets[WALL_P] = guiLoadImage("./images/item/wall_p.png");


    return itemsAssets;
}

/**
 * \fn GuiAssets guiLoadGui()
 * \brief The function load Gui assets
 * \param board size choosen for the game
 * \details The function load all assets needed to draw the gui
 * \return Array of SDL_Surface*
 */
GuiAssets guiLoadGui(BoardSize size) {

    GuiAssets guiAssets = malloc(sizeof(struct GuiAssets));
    if(size==LARGE)
    {
      guiAssets->pauseScreen = guiLoadImage("./images/gui/pauseBig.png");
      guiAssets->timer3 = guiLoadImage("./images/gui/timer3big.png");
      guiAssets->timer2 = guiLoadImage("./images/gui/timer2big.png");
      guiAssets->timer1 = guiLoadImage("./images/gui/timer1big.png");
      guiAssets->sideBar = guiLoadImage("./images/gui/sideBar_big.png");
      guiAssets->endScreenSnake1 = guiLoadImage("./images/gui/snake1big.png");
      guiAssets->endScreenSnake2 = guiLoadImage("./images/gui/snake2big.png");
    }
    else if (size==SMALL)
    {
      guiAssets->pauseScreen = guiLoadImage("./images/gui/pauseSmall.png");
      guiAssets->timer3 = guiLoadImage("./images/gui/timer3small.png");
      guiAssets->timer2 = guiLoadImage("./images/gui/timer2small.png");
      guiAssets->timer1 = guiLoadImage("./images/gui/timer1small.png");
      guiAssets->sideBar = guiLoadImage("./images/gui/sideBar_small.png");
      guiAssets->endScreenSnake1 = guiLoadImage("./images/gui/snake1small.png");
      guiAssets->endScreenSnake2 = guiLoadImage("./images/gui/snake2small.png");
    }
    else
    {
      guiAssets->pauseScreen = guiLoadImage("./images/gui/pauseMedium.png");
      guiAssets->timer3 = guiLoadImage("./images/gui/timer3medium.png");
      guiAssets->timer2 = guiLoadImage("./images/gui/timer2medium.png");
      guiAssets->timer1 = guiLoadImage("./images/gui/timer1medium.png");
      guiAssets->sideBar = guiLoadImage("./images/gui/sideBar_medium.png");
      guiAssets->endScreenSnake1 = guiLoadImage("./images/gui/snake1medium.png");
      guiAssets->endScreenSnake2 = guiLoadImage("./images/gui/snake2medium.png");
    }




    return guiAssets;
}

/**
 * \fn void guiDrawGame(SDL_Surface *screen, Game game, Assets assets)
 * \brief Draw the entire game on the screen
 * \details Draw each element of the game on the screen with SDL
 * \param screen The screen to draw on
 * \param game The game struct containing all elements of the game
 * \param assets The Assets struct containing all assets of the game
 * \return The SnakeAssets struct of the loaded snake
 */
void guiDrawGame(SDL_Surface *screen, Game game, Assets assets, BoardSize size) {
    Snake snake1 = gameGetSnake(game, 1);
    Snake snake2 = gameGetSnake(game, 2);
    Item itemList = boardGetItemList(gameGetBoard(game));

    guiApplySurface(0, 0, assets->background, screen, NULL); // dessine le background
    guiDrawSnake(screen, snake1, assets->snakesAssets[snakeGetType(snake1)]);
    guiDrawSnake(screen, snake2, assets->snakesAssets[snakeGetType(snake2)]);
    guiDrawItems(screen, itemList, assets->itemsAssets);
    guiDrawGui(screen, game, assets->guiAssets, assets->itemsAssets, size);
}

/**
 * \fn void guiDrawItems(SDL_Surface *screen, Item itemList, SDL_Surface** itemsAssets)
 * \brief Draw all items on the screen
 * \details Draw each item of the game on the screen with SDL
 * \param screen The screen to draw on
 * \param itemList The linked list of item
 * \param itemsAssets The Assets struct containing all assets of items
 */
void guiDrawItems(SDL_Surface *screen, Item itemList, SDL_Surface** itemsAssets) {

    Item item = itemList;
    int x, y;

    while(item != NULL) {
        x = item->posX * M_CELL_SIZE;
        y = item->posY * M_CELL_SIZE;
        if(item->value != SENTRY) {
            guiApplySurface(x, y, itemsAssets[item->value], screen, NULL);
        }
        item = item->next;
    }
}


/**
 * \fn void guiDrawSnake(SDL_Surface *screen, Snake snake, SnakeAssets snakeAssets)
 * \brief Draw one snake on the screen
 * \details Draw one snake on the screen
 * \param screen The screen to draw on
 * \param snake The snake to draw
 * \param snakeAssets The skin to associate to the snake
 */
void guiDrawSnake(SDL_Surface *screen, Snake snake, SnakeAssets snakeAssets) {
    int i;
    int x, y;
    Direction currentDirection;
    Direction nextDirection;

    for (i=0; i<snakeGetSize(snake); i++) {
        x = snakeGetPos(snake, i)->x * M_CELL_SIZE;
        y = snakeGetPos(snake, i)->y * M_CELL_SIZE;

        currentDirection = snakeElementGetOrientation(snake, i);


        if(i == 0) // tail
        {
            nextDirection = snakeElementGetOrientation(snake, i+1);
            guiApplySurface(x, y, snakeAssets->tail[nextDirection], screen, NULL);
        }
        else if (i == snakeGetSize(snake)-1) // head
            guiApplySurface(x, y, snakeAssets->head[currentDirection], screen, NULL);
        else // body
        {
            nextDirection = snakeElementGetOrientation(snake, i+1);
            if(currentDirection != nextDirection)
            {
                if(currentDirection == RIGHT)
                {
                    if(nextDirection == UP)
                        guiApplySurface(x, y, snakeAssets->corner[LEFT], screen, NULL);
                    else // DOWN
                        guiApplySurface(x, y, snakeAssets->corner[DOWN], screen, NULL);
                }
                else if(currentDirection == UP)
                {
                    if(nextDirection == RIGHT)
                        guiApplySurface(x, y, snakeAssets->corner[RIGHT], screen, NULL);
                    else // LEFT
                        guiApplySurface(x, y, snakeAssets->corner[DOWN], screen, NULL);
                }
                else if(currentDirection == DOWN)
                {
                    if(nextDirection == RIGHT)
                        guiApplySurface(x, y, snakeAssets->corner[UP], screen, NULL);
                    else // LEFT
                guiApplySurface(x, y, snakeAssets->corner[LEFT], screen, NULL);
                }
                else if(currentDirection == LEFT)
                {
                    if(nextDirection == UP)
                        guiApplySurface(x, y, snakeAssets->corner[UP], screen, NULL);
                    else // DOWN
                        guiApplySurface(x, y, snakeAssets->corner[RIGHT], screen, NULL);
                }
            }
            else
                guiApplySurface(x, y, snakeAssets->body[currentDirection], screen, NULL);
        }
    }
}

/**
 * \fn void guiDrawGui(SDL_Surface *screen, Game game, GuiAssets guiAssets, SDL_Surface** itemsAssets, BoardSize size)
 * \brief Draw the GUI on the sreen
 * \details Draw the Graphical User Interface on the sreen
 * \param screen The screen to draw on
 * \param game The game struct
 * \param guiAssets The struct with gui surfaces
 * \param itemsAssets The struct with item surfaces
 * \param size The size of the board
 */
void guiDrawGui(SDL_Surface *screen, Game game, GuiAssets guiAssets, SDL_Surface** itemsAssets, BoardSize size) {

    if(gameGetIsPaused(game) && gameGetPauseTimer(game)==0) {
        guiApplySurface(0, 0, guiAssets->pauseScreen, screen, NULL);
    }
    else if(gameGetPauseTimer(game)==4)
    {
        guiApplySurface(0, 0, guiAssets->timer3, screen, NULL);
        gameSetPauseTimer(game, gameGetPauseTimer(game)-1);

    }
    else if(gameGetPauseTimer(game)==3)
    {
        guiApplySurface(0, 0, guiAssets->timer2, screen, NULL);
        gameSetPauseTimer(game, gameGetPauseTimer(game)-1);
        SDL_Delay(1000);
    }
    else if(gameGetPauseTimer(game)==2)
    {
        guiApplySurface(0, 0, guiAssets->timer1, screen, NULL);
        gameSetPauseTimer(game, gameGetPauseTimer(game)-1);
        SDL_Delay(1000);
    }
    else if(gameGetPauseTimer(game)==1)
    {

        SDL_Delay(1000);
        gameSetPauseTimer(game, 0);
        gameSetIsPaused(game, false);
    }

    int xOffset, x2Offset, xMax, y, maxItem;
    if (size == SMALL) {
        y = S_SIZE_BOARD_Y*M_CELL_SIZE;
        xMax = S_SIZE_BOARD_X*M_CELL_SIZE;
        xOffset = 110;
        x2Offset = 135;
        maxItem = 9;
    } else if(size == MEDIUM) {
        y = M_SIZE_BOARD_Y*M_CELL_SIZE;
        xMax = M_SIZE_BOARD_X*M_CELL_SIZE;
        xOffset = 130;
        x2Offset = 153;
        maxItem = 15;
    } else {
        y = L_SIZE_BOARD_Y*M_CELL_SIZE;
        xMax = L_SIZE_BOARD_X*M_CELL_SIZE;
        xOffset = 130;
        x2Offset = 150;
        maxItem = 21;
    }

    guiApplySurface(0, y, guiAssets->sideBar, screen, NULL);
    Snake snake1 = gameGetSnake(game, 1);
    Item item = snakeGetItemList(snake1)->next;
    int i = 0;
    while (item != NULL) {
        if(i < maxItem) {
            guiApplySurface(xOffset+i*1.2*M_CELL_SIZE, y+10, itemsAssets[item->value], screen, NULL);
        }
        item = item->next;
        i++;
    }

    Snake snake2 = gameGetSnake(game, 2);
    item = snakeGetItemList(snake2)->next;
    i = 0;
    while (item != NULL) {
        if(i < maxItem) {
            guiApplySurface(xMax-x2Offset-i*1.2*M_CELL_SIZE, y+10, itemsAssets[item->value], screen, NULL);
        }
        item = item->next;
        i++;
    }
}

/**
 * \fn void guiApplySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
 * \brief Draw one surface on the screen
 * \details Draw one surface on the screen at the position x, y with a SDL_BlitSurface call
 * \param x The x position
 * \param y The y position
 * \param source The surface to draw
 * \param destination The surface to draw on
 * \param clip The rectangle representing the area of source to draw (NULL = all the source surface)
 */
void guiApplySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    if(SDL_BlitSurface(source, clip, destination, &offset) < 0)
        printf("%s\n", SDL_GetError());
}

/**
 * \fn guiCreateTimer()
 * \brief Create the Timer struct
 * \details Create the Timer struct
 * \return The Timer struct created
 */
Timer guiCreateTimer() {
    Timer timer = malloc(sizeof(struct Timer));
    timer->snake1MoveTimer = SNAKE_DEFAULT_SPEED; // pour être certain que les deux snake bouge des le début
    timer->snake2MoveTimer = SNAKE_DEFAULT_SPEED;
    timer->itemPopTimer = ITEM_POP_INTERVAL;
    timer->snake1DebuffTimer = 0;
    timer->snake2DebuffTimer = 0;

    timer->snake1LastDebuff = 0;
    timer->snake2LastDebuff = 0;
    timer->snake1LastMove = 0;
    timer->snake2LastMove = 0;
    timer->itemLastPop = 0;

    return timer;
}

/**
 * \fn Title guiCreateTitle(int x, int y, MenuValue value, char* text, SDL_Color color, TTF_Font* font)
 * \brief Create one Title struct
 * \details Create one Title struct. A title is an option on the menu
 * \param x The x position
 * \param y The y position
 * \param value The MenuVAlue
 * \param text The text of the title
 * \param color The color of the title
 * \param color The font of the title
 */
Title guiCreateTitle(int x, int y, MenuValue value, char* text, SDL_Color color, TTF_Font* font) {
    Title title = malloc(sizeof(struct Title));
    title->x = x;
    title->y = y;
    title->value = value;
    title->text = text;
    title->color = color;
    title->font = font;
    title->surface = NULL;
    title->isSelected = false;

    return title;
}

/**
 * \fn void guiSetSelectedTitle(Title *titles, int nbTitles, int state)
 * \brief Set the selected titles
 * \details Set the selected titles within the titles array
 * \param titles The array of titles
 * \param nbTitles The number of title
 * \param state The state of the menu
 */
void guiSetSelectedTitle(Title *titles, int nbTitles, int state) {
    int i;
    for (i = 0; i < nbTitles; i++) {
        if( (state%nbTitles+nbTitles)%nbTitles == i ) { // modulo positif
            titles[i]->isSelected = true;
        } else {
            titles[i]->isSelected = false;
        }
    }
}

/**
 * \fn Title guiGetSelectedItem(Title *titles, int nbTitles)
 * \brief Get the selected titles
 * \details Get the selected titles within the titles array
 * \param titles The array of titles
 * \param nbTitles The number of title
 */
Title guiGetSelectedItem(Title *titles, int nbTitles) {
    int i;
    for (i = 0; i < nbTitles; i++) {
        if(titles[i]->isSelected) {
            return titles[i];
        }
    }
    return NULL;
}

/**
 * \fn void guiDrawTitles(SDL_Surface *screen, Title *titles, int nbTitles, SDL_Color ifSelectedColor)
 * \brief Draw the titles ont the screen
 * \param screen The screen to draw
 * \param titles The array of titles
 * \param nbTitles The number of title
 * \param ifSelectedColor The color of the title if selected
 */
void guiDrawTitles(SDL_Surface *screen, Title *titles, int nbTitles, SDL_Color ifSelectedColor) {
    SDL_Color color;
    int i;
    for (i = 0; i < nbTitles; i++) {

        color = titles[i]->color;
        if(titles[i]->isSelected) {
            color = ifSelectedColor;
        }
        titles[i]->surface = TTF_RenderText_Blended(titles[i]->font, titles[i]->text, color);
        guiApplySurface(titles[i]->x, titles[i]->y, titles[i]->surface, screen, NULL);
    }
}
/**
 * \fn void guiReloadScreen(SDL_Surface *screen)
 * \brief Reload the entire screen
 * \details Reload the entire screen with a SDL_Flip call
 * \param screen The screen surface to reload
 */
void guiReloadScreen(SDL_Surface *screen) {
    SDL_Flip(screen);
}

void guiPause(Game g) {
        gameSetIsPaused(g, true);
}



/**
 * \fn void guiSnakeEvent(SDL_Event *event, Snake s)
 * \brief Catch all player input
 * \details Catch all player input and set the snake direction
 * \param event The event struct to listen
 * \param s The snake to be oriented
 */
void guiEvent(SDL_Event *event, Snake s, Game g) {
    bool moved = false;
    bool controlReversed = snakeGetIsControlReversed(s);
    while (SDL_PollEvent(event)) {
        switch(event->type) {

            case SDL_KEYDOWN:
                if (!moved) {
                    switch(event->key.keysym.sym) {
                        case SDLK_UP: // Flèche haut 1
                            if(!gameGetIsPaused(g) && gameGetPauseTimer(g)==0) {
                                if(controlReversed)
                                    snakeSetDirection(s, DOWN);
                                else
                                    snakeSetDirection(s, UP);
                                moved = true;
                            }
                            break;
                        case SDLK_DOWN: // Flèche bas 2
                            if(!gameGetIsPaused(g) && gameGetPauseTimer(g)==0) {
                                if(controlReversed)
                                    snakeSetDirection(s, UP);
                                else
                                    snakeSetDirection(s, DOWN);
                                moved = true;
                            }
                            break;
                        case SDLK_RIGHT: // Flèche droite 3
                            if(!gameGetIsPaused(g) && gameGetPauseTimer(g)==0) {
                                if(controlReversed)
                                    snakeSetDirection(s, LEFT);
                                else
                                    snakeSetDirection(s, RIGHT);
                                moved = true;
                            }
                            break;
                        case SDLK_LEFT: // Flèche gauche 4
                            if(!gameGetIsPaused(g) && gameGetPauseTimer(g)==0) {
                                if(controlReversed)
                                    snakeSetDirection(s, RIGHT);
                                else
                                    snakeSetDirection(s, LEFT);
                                moved = true;
                            }
                            break;
                        case SDLK_SPACE:

                            if(gameGetIsPaused(g) && gameGetPauseTimer(g)==0)
                              gameSetPauseTimer(g,4);
                            else
                              guiPause(g);
                            break;
                        default:
                            break;
                    }
                }

                break;
            default:
                break;
        }
    }
}

/**
 * \fn void guiEndScreen(SDL_Surface* screen, GuiAssets guiAssets, BoardSize size, SDL_Event *event, bool snka1Win)
 * \brief Draw the end screen
 * \param screen The screen to draw
 * \param guiAssets Struct with the asset to draw
 * \param size The size of the board
 * \param event The event struc to get input from the player
 * \param idWinner Id of the winner
 */
void guiEndScreen(SDL_Surface* screen, BoardSize size, SDL_Event *event, int idWinner) {

    bool continuer = true;
    SDL_Surface* endScreenSnake1;
    SDL_Surface* endScreenSnake2;

    if(size==LARGE) {
        endScreenSnake1 = guiLoadImage("./images/gui/snake1big.png");
        endScreenSnake2 = guiLoadImage("./images/gui/snake2big.png");
    }
    else if (size==SMALL) {
        endScreenSnake1 = guiLoadImage("./images/gui/snake1small.png");
        endScreenSnake2 = guiLoadImage("./images/gui/snake2small.png");
    } else {
        endScreenSnake1 = guiLoadImage("./images/gui/snake1medium.png");
        endScreenSnake2 = guiLoadImage("./images/gui/snake2medium.png");
    }

    while (continuer)
    {

        if(idWinner == 1)
            guiApplySurface(0, 0, endScreenSnake1, screen, NULL);
        else
            guiApplySurface(0, 0, endScreenSnake2, screen, NULL);
        SDL_Flip(screen);

        SDL_WaitEvent(event);
        switch(event->type)
        {
            case SDL_QUIT:
                continuer = false;
                break;
            case SDL_KEYDOWN:
                switch(event->key.keysym.sym) {
                    case SDLK_RETURN:
                        idWinner = guiPlay(size);
                        break;
                    case SDLK_ESCAPE:
                        continuer = false;
                        break;
                    default:
                        break;
                }
                break;
        }
    }
}
