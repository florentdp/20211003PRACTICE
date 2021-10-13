// 20211003PRACTICE.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <stdio.h>
#include <stdlib.h>
#include "CustomType.h"
#include <time.h>
#include <math.h>
#include <windows.h>

bool make_maze(new_coordinate* maze, int maze_size, int maze_parameter);
bool make_move(new_coordinate* maze, int step, int size, int maze_parameter);
bool choose_cube(new_coordinate* maze, new_coordinate* position, int step);
bool decide_direction(new_coordinate* maze, new_coordinate *position, int maze_parameter, new_coordinate* next_move, new_coordinate* sub_next_move);
bool make_direction_list(int* direction_list);
bool check_direction_list(int* direction_list, int direction);
bool test_direction(new_coordinate *position, int direction, int maze_parameter, new_coordinate* target, new_coordinate* sub_target);
bool cube_look_up(new_coordinate* maze, new_coordinate target, int size);
bool maze_init(new_coordinate* maze);
bool close_cube(new_coordinate* maze, coordinate* position);
void show_maze(coordinate* maze, int maze_size, int parameter);
void seperator(int parameter, char type);

void color(int t, int f)
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, f * 16 + t);
}

int main()
{
    int maze_parameter = 8, wall_parameter = maze_parameter - 1, maze_size = (maze_parameter +wall_parameter)* (maze_parameter + wall_parameter), maze_max_step = maze_parameter*maze_parameter , i=0;
    new_coordinate maze[300];

    printf("\n");
    for (i = 0; i < 300; i++)
    {
        maze_init(&maze[i]);
    }
    i = 0;
    maze[0] = { 1, 1 };
    make_maze(maze, (maze_max_step-1)*2, maze_parameter+wall_parameter);
    show_maze(maze, (maze_max_step - 1) * 2+1, (maze_parameter + wall_parameter));
    return 1;
}

//step 0
bool maze_init(new_coordinate* maze)
{
    maze->x = 0;
    maze->y = 0;
    maze->closed = false;
    return true;
}

//step 1
bool make_maze(new_coordinate* maze, int maze_size, int maze_parameter)
{
    int i = 0;
    for (i = 0; i < maze_size; i+=2)// for each cube of the maze
    {
        //decide on a move
        make_move(maze, i, maze_size, maze_parameter);
    }
    return true;
}

//step 2
bool make_move(new_coordinate* maze, int step, int size, int maze_parameter)
{
    int i = 0;
    bool found_cube = false;
    new_coordinate next_move = { 0, 0, false }, sub_next_move = { 0, 0, false }, position;//recipient for the next move
    while (!found_cube )// if there still no cube found
    {
        i++;
        color(3, 0);
        //printf("[ACTION] _MAKE_MOVE_ : process to grow the tree \n");
        color(15, 0);

        choose_cube(maze, &position, step); // choose a cube in a a tree to make it grow
        found_cube = decide_direction(maze, &position, maze_parameter, &next_move, &sub_next_move);//choose in which way to grow
    }
    if (found_cube) 
    {
        color(11, 0);
        //printf("[RESULT] _MAKE_MOVE_ : cube has been found ? true\n");
        color(15, 0);
        (&maze[step + 1])->x = sub_next_move.x;
        (&maze[step + 1])->y = sub_next_move.y;

        (&maze[step + 2])->x = next_move.x;//new branch is set
        (&maze[step + 2])->y = next_move.y;
    }
    else
    {
        color(13, 0);
        //printf("[RESULT] _MAKE_MOVE_ : cube has been found ? false\n");
        color(15, 0);
    }

    return found_cube;
}

//step 2.1
bool choose_cube(new_coordinate* maze, new_coordinate* position, int step)
{
    int choice = 0;
    bool cube_is_not_locked = false;
    srand(time(NULL));
    while (!cube_is_not_locked)
    {
        color(3, 0);
        //printf("[ACTION] _CHOOSE_CUBE_ : process to choose a cube \n");
        color(15, 0);

        choice = rand() % (step + 1);
        //printf("[TEST] _CHOOSE_CUBE_ : testing cube n°%d x:%d, y:%d \n", choice, maze[choice].x, maze[choice].y);
        cube_is_not_locked = (maze[choice].closed == false && (maze[choice].x % 2 != 0) && (maze[choice].y %2 != 0)) ? true : false;
    }

    if (cube_is_not_locked)
    {
        color(1, 0);
        position->x = maze[choice].x;
        position->y = maze[choice].y;
        //printf("[SETTING] _CHOOSE_CUBE_ : choosen_cube > choice : %d, x : %d, y : %d\n", choice, maze[choice].x, maze[choice].y);
        color(15, 0);
    }
    return true;
}

//step 2.2
bool decide_direction(new_coordinate* maze, new_coordinate *position, int maze_parameter, new_coordinate *next_move, new_coordinate* sub_next_move)
{
    //set variable
    int random_direction = 0, total_size = maze_parameter * maze_parameter, i = 0, direction_list[4] = {0, 0, 0, 0};
    bool found_direction = false;
    new_coordinate target = { 0, 0 }, sub_target = {0,0};

    //prepare a random list to test
    make_direction_list(direction_list);

    for (i = 0; i < 4; i++)
    {
        color(3, 0);
        //printf("[ACTION] _DECIDE_DIRECTION_ : test the direction : %d\n", direction_list[i]);

        if (test_direction(position, direction_list[i], maze_parameter, &target, &sub_target) && !found_direction && !cube_look_up(maze, target, total_size))
        {
            found_direction = true;
            sub_next_move->x = sub_target.x;
            sub_next_move->y = sub_target.y;

            next_move->x = target.x;
            next_move->y = target.y;

            color(1, 0);
            //printf( "[SETTING] _DECIDE_DIRECTION_ : next_move : %d > x : %d, y : %d\n", i, next_move->x, next_move->y);

            break;
        }
        color(15, 0);
    }
    if (!found_direction)
    {
        color(12, 0);
        //printf("[RESULT] _DECIDE_DIRECTION_ : direction found ? false\n");

        close_cube(maze, position);
    }
    else
    {
        color(11, 0);
        //printf("[RESULT] _DECIDE_DIRECTION_ : direction found ? true\n");
    }
    color(15, 0);
    return found_direction;
}

bool close_cube(new_coordinate* maze, coordinate* position)
{
    //printf("[INFO] _CLOSE_CUBE_ :  looking for x : %d, y : %d\n", position->x, position->y);

    int i = 0;
    bool right_cube = false;
    while (!right_cube && i < 100)
    {
        //printf("[INFO] _CLOSE_CUBE_ : comparing to x : %d, y : %d\n", (&maze[i])->x, (&maze[i])->y);

        if ((&maze[i])->x == position->x && (&maze[i])->y == position->y)
        {
            right_cube = true;
            (&maze[i])->closed = true;

            color(3, 0);
            //printf("[ACTION] _CLOSE_CUBE_ : cube %d,%d closed...\n", position->x, position->y);
            color(15, 0);

            break;
        }
        i++;
    }
    return true;
}

bool make_direction_list(int* direction_list)
{
    bool completed = false;
    int random_direction = 0, i=0;
    srand(time(NULL));
    while (!completed && i<4)
    {
        random_direction = (rand() % 4)+1;
        if (!check_direction_list(direction_list, random_direction))
        {
            direction_list[i] = random_direction;
            i++;
        }
    }

    color(3, 0);
    //printf("[ACTION] _MAKE_DIRECTION_LIST_ : make the direction list...\n");
    color(15, 0);

    return true;
}

bool check_direction_list(int* direction_list, int direction) 
{
    int i = 0;
    bool found = false;
    for (i = 0; i < 4; i++)
    {
        if (direction_list[i] == direction)
        {
            found = true;
        }
    }
    return found;
}

bool test_direction(new_coordinate *position, int direction, int maze_parameter, new_coordinate* target, new_coordinate* sub_target)
{
    new_coordinate hypothesis = { position->x, position->y }, sub_hypothesis = { position->x, position->y };
    bool respect_the_rule = true;
    switch (direction)
    {
        case 1:
            hypothesis.y-=2;
            sub_hypothesis.y--;
            break;
        case 2:
            hypothesis.x+=2;
            sub_hypothesis.x++;
            break;
        case 3:
            hypothesis.y+=2;
            sub_hypothesis.y++;
            break;
        default:
            hypothesis.x-=2;
            sub_hypothesis.x--;
            break;
    }
    if (hypothesis.x * hypothesis.y < 1 || hypothesis.x > maze_parameter || hypothesis.y > maze_parameter)
    {
        respect_the_rule = false;

        color(12, 0);
        //printf("[RESULT] _TEST_DIRECTION_ : can go in this direction ? false\n");
        color(15, 0);
    }
    else
    {
        color(11, 0);
        //printf("[RESULT] _TEST_DIRECTION_ : can go in this direction ? true\n");
        color(15, 0);
        sub_target->x = sub_hypothesis.x;
        sub_target->y = sub_hypothesis.y;
        target->x = hypothesis.x;
        target->y = hypothesis.y;
    }
    return respect_the_rule;//true > good, a direction can be used
}

bool cube_look_up(new_coordinate* maze, new_coordinate target, int size)
{
    int i = 0;
    bool found = false;
    for (i = 0; i < size; i++)
    {
        if (maze[i].x == target.x && maze[i].y == target.y)
        {
            found = true;

            color(12, 0);
            color(15, 0);

            break;
        }
    }
    if (!found) {
        color(11, 0);
        //printf("[RESULT] _CUBE_LOOKUP_ : is this cube already part of the tree ? false\n");
        color(15, 0);
    }    
    return found;//false > good, it's not part of the tree, it can use
}

void show_maze(coordinate* maze, int maze_size, int parameter)
{
    int i = 0, j = 0;
    bool open = false;
    char representation = 'a';
    new_coordinate cube = { 0, 0, false };
    for (i = 0; i < maze_size; i++)
    {
        //printf("[OBJECT] _SHOW_MAZE_ : x :  %d, y : %d\n", (&maze[i])->x, (&maze[i])->y);
    }

    seperator(parameter, 't');
    for (i = 0; i < parameter; i++)
    {
        if (i % 2 != 0)
        {
            printf("┣━");
        }
        else
        {
            printf("┃ ");
        }
        for (j = 0; j < parameter; j++)
        {
            if (j % 2 != 0)
            {
                if (i % 2 != 0)
                {
                    printf("━╋━");
                }
                else
                {
                    cube.x = j+1;
                    cube.y = i+1;
                    open = cube_look_up(maze, cube, maze_size);

                    if (open)
                    {
                        printf("   ");
                    }
                    else
                    {
                        printf(" ┃ ");
                    }
                }
            }
            else
            {
                if (i % 2 == 0)
                {
                    printf("   ");
                }
                else
                {
                    cube.x = j+1;
                    cube.y = i+1;
                    open = cube_look_up(maze, cube, maze_size);

                    if (open)
                    {
                        printf("   ");
                    }
                    else
                    {
                        printf("━━━");
                    }
                }
                
            }
        }
        if (i % 2 != 0)
        {
            printf("━┫\n");
        }
        else
        {
            printf(" ┃\n");
        }
    }
    seperator(parameter, 'b');
}

void seperator(int parameter, char type) 
{
    int i;
    if (type == 't')
    {
        printf("┏━");
    }
    else
    {
        printf("┗━");
    }
    for (i = 0; i < parameter; i++)
    {
        if (i % 2 != 0)
        {
            if (type == 't')
            {
                printf("━┳━");
            }
            else
            {
                printf("━┻━");
            }
        }
        else
        {
            printf("━━━");
        }
    }
    if (type == 't')
    {
        printf("━┓\n");
    }
    else
    {
        printf("━┛\n");
    }
}





// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
