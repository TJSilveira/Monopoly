#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "C:\Users\tiago\Documents\C_programming\Monopoly\Functions.h"

typedef struct player{
    short pl_number;
    char *pl_name;
    short location;
    int money;
    short order;
    int laps;
    short rounds_jail;
    short num_GOJ;
    short isBankrupt;
    property **wallet;
    short build_BR;
    short build_LB;
    short build_PK;
    short build_OR;
    short build_RD;
    short build_YW;
    short build_GR;
    short build_DB;
    short num_RR;
    short num_Util;
}player;

typedef struct dice
{
    int d1;
    int d2;
    int db;
    int dtotal;
}dice;


int main(){
    srand(time(NULL));
    // File import
    FILE *board_file = fopen("C:\\Users\\tiago\\Documents\\C_programming\\Monopoly\\csv\\board_setup - 2.csv", "r");
    FILE  *player_file = fopen("C:\\Users\\tiago\\Documents\\C_programming\\Monopoly\\csv\\players.csv", "r");
    FILE *chanceCC_file = fopen("C:\\Users\\tiago\\Documents\\C_programming\\Monopoly\\csv\\Chance and CC.csv", "r");

    // Create Player list and board
    property **board;
    player **list_players;
    chance_CC **all_chanceCC;
    int total_players=0;
    board=(create_board(board_file));
    printf("Board Loaded Successfully\n");
    list_players=create_list_players_test(player_file,&total_players);
    printf("Players Loaded Successfully - %i players\n",total_players);
    all_chanceCC=create_all_chanceCC(chanceCC_file);
    printf("Chance and CC cards Loaded Successfully\n");

    // Create Variables to play
    dice dice1;
    int current_player = 0;

    //variables that end the while loops
    int end_game=0;
    int end_menu2=0;

    //Tracks the choice each player makes 
    int choice1;

    //There are two types of menu. The first is to throw the dice; the second is to end the turn of buy a house. For more options, add them in each of the menus
    int menu_1[] = {1,9};
    int menu_2[] = {1,2,9};
    int menu_3[] = {1,2,3,9};

    while (end_game!=1) {
        //Check if the player is still playing or whether he already lost due to bankruptcy
        if(list_players[current_player]->isBankrupt==1) {
            current_player++;
            current_player=current_player%total_players;
            continue;
        }

        //First menu when the player is not in Jail. The player can either throw dice or exit the game
        if(list_players[current_player]->rounds_jail==0){
            //Do cycle ensures that the player plays again if there is a double
            do{
                // Request input from user
                print_text1(list_players[current_player]);
                choose(&choice1, menu_1,sizeof(menu_1)/4);

                //Possible paths in menu 1
                switch (choice1)
                {
                case 1:
                    throw_dice(&dice1);
                    print_dice(&dice1);
                    move_player(list_players[current_player],board,all_chanceCC,list_players,total_players,dice1.dtotal,0);
                    print_wallet(list_players[current_player]);
                    display_player(list_players[current_player]);
                    break;
                case 9:
                    end_game=1;
                    printf("Game is over!\n");
                    break;
                default:
                    printf("Invalid input. Please enter 1 or 9.\n");
                    break;
                }
            } while(dice1.db==1);
        }
        
        //First menu when the player is in Jail. The player has several options to exit the jail, throw dice or exit the game
        if(list_players[current_player]->rounds_jail>0){
            if (list_players[current_player]==0)
            {
                print_textJail_no_GOJ(list_players[current_player]);
                choose(&choice1, menu_2,sizeof(menu_2)/4);
            }
            if (list_players[current_player]>0)
            {
                print_textJail_with_GOJ(list_players[current_player]);
                choose(&choice1, menu_3,sizeof(menu_3)/4);
            }
            
            switch(choice1){
                case 1:
                    throw_dice(&dice1);
                    print_dice(&dice1);
                    if (dice1.db==1)
                    {
                        printf("You are free from jail!\n");
                        list_players[current_player]->rounds_jail=0;
                        move_player(list_players[current_player],board,all_chanceCC,list_players,total_players,dice1.dtotal,0);
                        print_wallet(list_players[current_player]);
                        display_player(list_players[current_player]);
                        break;
                    }
                    else if(list_players[current_player]->rounds_jail==1){
                        printf("You are free from jail, but you need to pay 50M!\n");
                        list_players[current_player]->money-=50;
                        list_players[current_player]->rounds_jail=0;
                        move_player(list_players[current_player],board,all_chanceCC,list_players,total_players,dice1.dtotal,0);
                        print_wallet(list_players[current_player]);
                        display_player(list_players[current_player]);
                        break;
                    }
                    else{
                        list_players[current_player]->rounds_jail--;
                        printf("You are still in jail. You still have %i turns left\n",list_players[current_player]->rounds_jail);
                        break;
                    }
                case 2:
                    list_players[current_player]->money-=50;
                    list_players[current_player]->rounds_jail=0;
                    throw_dice(&dice1);
                    print_dice(&dice1);
                    move_player(list_players[current_player],board,all_chanceCC,list_players,total_players,dice1.dtotal,0);
                    print_wallet(list_players[current_player]);
                    display_player(list_players[current_player]);
                    break;
                case 3:
                    list_players[current_player]->num_GOJ--;
                    list_players[current_player]->rounds_jail=0;
                    throw_dice(&dice1);
                    print_dice(&dice1);
                    move_player(list_players[current_player],board,all_chanceCC,list_players,total_players,dice1.dtotal,0);
                    print_wallet(list_players[current_player]);
                    display_player(list_players[current_player]);
                    break;
                case 9:
                    end_game=1;
                    printf("Game is over!\n");
                    break;
                default:
                    printf("Invalid input. Please enter 1, 2, or 9.\n");
                    break;
            }
        }

        //Antes de acabar o turno, dar a oportunidade ao jogador de comprar casas. Eventualmente, será incluido a possibilidade de trocar casas e dinheiro
        while(end_menu2==0 && end_game ==0 && list_players[current_player]->isBankrupt==0){
            // Request input from user
            print_text2();
            choice1=0;
            choose(&choice1, menu_2,sizeof(menu_2)/4);
            //Possible paths in menu 2
            switch (choice1){
                case 1:
                    end_menu2=1;
                    break;
                case 2:
                    buy_house(list_players[current_player]);
                    break;
                case 9:
                    end_menu2=1;
                    end_game=1;
                    printf("Game is over!\n");
                    break;
                default:
                    printf("Invalid input. Please enter 1, 2 or 9.\n");
                    break;
            }
        }
        end_menu2=0;

        //Take out the properties held by the player
        if(list_players[current_player]->isBankrupt==1){
            remove_properties_bankruptcy(list_players[current_player]);
        }

        // Next Player
        current_player++;
        current_player=current_player%total_players;
    }

    for(int i=0; i<total_players;i++){
        release_player(list_players[i]);
    }
}