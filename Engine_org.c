#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <time.h>

// TODO
// 1. Implement a dictionary using a hashmap - https://troydhanson.github.io/uthash/

typedef struct player player;
typedef struct property property;

typedef struct player{
    short pl_number;
    char *pl_name;
    short location;
    int money;
    short order;
    int laps;
    property **wallet;
}player;

typedef struct dice
{
    int d1;
    int d2;
    int db;
    int dtotal;
}dice;

typedef struct property{
    char *pt_name;
    short n_houses;
    short p_house;
    short price;
    int *rent;
    short mortgage;
    int *rent_railroad;
    short CF;
    char *colours;
    player *owner;
}property;

player* create_player(short pl_number, char *pl_name,short location, int money, short order){
    player *i=malloc(sizeof(player));
    i->pl_number=pl_number;
    i->pl_name=strdup(pl_name);
    i->location=location;
    i->money=money;
    i->order=order;
    i->laps=0;
    return i;
}

player** create_list_players(FILE *in_file){
    player** list_players = malloc(4*sizeof(player*));
    char buffer[500];
    // char *data;
    int i;
    short pl_number;
    char *pl_name;
    short location;
    int money;
    short order;


    for(i=0;i<5;i++){
        if (i==0){
            fgets(buffer,sizeof(buffer),in_file);
            continue;
        }
        
        fgets(buffer, sizeof(buffer),in_file);

        // Player number
        pl_number = atoi(strtok(buffer,","));
        
        // Player name
        pl_name = strtok(NULL,",");

        // Location
        location = atoi(strtok(NULL,","));

        // Money
        money = atoi(strtok(NULL,","));

        // Order
        order = atoi(strtok(NULL,","));

        list_players[i-1] = create_player(pl_number,pl_name,location,money,order);

    }
    return list_players;
}

void display_property(property *a){
    property *i = a;
    printf("Name: %s, Colour: %s, Owner: %s, Number of Houses: %i, Price per House: %i, Price of Property: %i, Rent (0 houses): %i,Rent (1 house): %i,Rent (2 houses): %i,Rent (3 houses): %i,Rent (4 houses): %i,Rent (Hotel): %i, Mortgage: %i, Rent (1 Railroad): %i, Rent (2 Railroad): %i, Rent (3 Railroad): %i, Rent (4 Railroad): %i, CF: %i.\n\n",i->pt_name,i->colours,i->owner->pl_name,i->n_houses,i->p_house,i->price,i->rent[0],i->rent[1],i->rent[2],i->rent[3],i->rent[4],i->rent[5],i->mortgage,i->rent_railroad[0],i->rent_railroad[1],i->rent_railroad[2],i->rent_railroad[3],i->CF);
}

property* create_property(char *pt_name,short n_houses, short p_house,short price, int *rent,short mortgage,int *rent_railroad,short CF,char *colours){
    property *i = malloc(sizeof(property));
    i->pt_name=strdup(pt_name);
    i->n_houses=n_houses;
    i->p_house=p_house;
    i->price=price;
    i->rent = malloc(6*sizeof(int));    
    memcpy(i->rent,rent,6*sizeof(int));
    i->mortgage = mortgage;
    i->rent_railroad = malloc(4*sizeof(int));    
    memcpy(i->rent_railroad,rent_railroad,4*sizeof(int));
    i->CF=CF;
    i->colours=strdup(colours);
    i->owner=NULL;
    return i;
}

property** create_board(FILE *in_file){
    property** board = malloc(40*sizeof(property*));
    char buffer[500];
    char *data;
    char pt_name_temp[50];
    short price_temp;
    short p_house_temp;
    short mortgage_temp;
    short CF_temp;
    char colours_temp[50];

    char name[20];
    // property *board_temp[40];
    int i;

    for(i=0;i<40;i++){
        if (i==0)
        {
            fgets(buffer,sizeof(buffer),in_file);
        }
        
        int rent2[6];
        int rent_railroad[4];
        fgets(buffer,sizeof(buffer),in_file);

        // Property name
        data = strtok(buffer,",");
        strcpy(pt_name_temp,data);

        // Price
        price_temp = atoi(strtok(NULL,","));        

        // Price per house
        p_house_temp = atoi(strtok(NULL,","));

        // Rent
        rent2[0] = atoi(strtok(NULL,","));

        // Rent (1 house)
        rent2[1] = atoi(strtok(NULL,","));

        // Rent (2 house)
        rent2[2] = atoi(strtok(NULL,","));

        // Rent (3 house)
        rent2[3] = atoi(strtok(NULL,","));

        // Rent (4 house)
        rent2[4] = atoi(strtok(NULL,","));

        // Rent (Hotel)
        rent2[5] = atoi(strtok(NULL,","));

        // Mortgage
        data = strtok(NULL,",");
        mortgage_temp = atoi(data);

        // Rent (1 Railroad)
        rent_railroad[0] = atoi(strtok(NULL,","));

        // Rent (2 Railroad)
        rent_railroad[1] = atoi(strtok(NULL,","));

        // Rent (3 Railroad)
        rent_railroad[2] = atoi(strtok(NULL,","));

        // Rent (4 Railroad)
        rent_railroad[3] = atoi(strtok(NULL,","));

        // CF
        CF_temp = atoi(strtok(NULL,","));

        // Colours
        data = strtok(NULL,"\n");
        strcpy(colours_temp,data);
        
        board[i]=create_property(pt_name_temp,0,p_house_temp,price_temp,rent2,mortgage_temp,rent_railroad,CF_temp,colours_temp);
    }
    
    return board;
}

void display_player(player *a){
    player *i = a;
    printf("Player number:%i; Name: %s; Location: %i; Money: %i; Laps: %i; Order: %i\n",i->pl_number,i->pl_name,i->location,i->money,i->laps,i->order);
}

void release_player(player *a){
    player *i=a;
    free(i);
}

int one_dice(){
    int d1 = (rand()%(6))+1;
    return d1;
}

void pay_rent(player *a,property*b){
    a->money-=b->rent[b->n_houses];
    b->owner->money+=b->rent[b->n_houses];
}

void buy_property(player *a,property *b){
    int choice;
    int val_input=0;
    printf("Do you want to buy %s for %i? [1. yes / 0. no]: \n",b->pt_name,b->price);

    while (val_input==0)
    {
        if(scanf("%d",&choice)!=1){
            while(getchar()!='\n');
            printf("Invalid input. Please enter 1 for yes or 0 for no\n");
            continue;
        }
        switch (choice)
        {
        case 1:
            b->owner = a;
            a->money -=b->price;
            printf("%s is now the owner of %s for the price of %i\n",a->pl_name,b->pt_name,b->price);
            val_input=1;
            break;
        default:
            printf("Property was not bought");
            val_input=1;
            break;
        }
    }
    
}

void buy_house(player *a,property *b){
    
}

void move_player(player *a, property** b,int i){
    int payment;
    a->location=a->location+i;
    if (a->location>39)
    {
        a->location=a->location%40;
        a->laps++;
        if (a->location!=0)
        {
            a->money+=b[0]->CF;
        }
    }
    // Get the player the CF of the property
    a->money += b[a->location]->CF;

    // Check onwership of the property. Either the property is already yours, it is from someone else and you need to pay rent or it is free. If it is free, it is either because the property cannot be bought or was not bought yet
    if (b[a->location]->owner==a)
    {
        printf("The %s property is already yours\n",b[a->location]->pt_name);
    }
    else if (b[a->location]->owner==NULL)
    {
        printf("The property has no owner\n");
        
        // Only allow to buy properties that have a price, which are the only ones that can be bought
        if (b[a->location]->price!=0)
        {
            buy_property(a,b[a->location]);
        }
    }
    else
    {
        pay_rent(a,b[a->location]);
        printf("The property is owned by %s. You need to pay: %i\n",b[a->location]->owner->pl_name,b[a->location]->rent[b[a->location]->n_houses]);
        printf("New Cash Balance of %s: %i\n",b[a->location]->owner->pl_name,b[a->location]->owner->money);

    };




}

void throw_dice(dice *a){
    a->d1=one_dice();
    a->d2=one_dice();
    a->dtotal = a->d1+a->d2;
}

void print_dice(dice *a){
    printf("dado 1, %i\n",a->d1);
    printf("dado 2, %i\n",a->d2);
}

void print_text1(player *a){
    printf("====================================================\n\n");
    printf("%s (Player 1), it is your turn!\n",a->pl_name);
    printf("1. Lancar dado\n");
    printf("9. Acabar o jogo\n");
    printf("Enter your choice: ");
}

int main(){
    srand(time(NULL));

    // File import
    FILE *board_file = fopen("board_setup - 2.csv", "r");
    FILE  *player_file = fopen("players.csv", "r");

    // Create Player list and board
    char name[20];
    property **board;
    player **list_players;
    printf("before");
    board=(create_board(board_file));
    printf("after");
    list_players=(create_list_players(player_file));

    // Create Variables to play
    dice dice1;

    int end=0;
    int i;

    display_player(list_players[0]);
    board[1]->owner = list_players[1];
    board[3]->owner = list_players[2];
    display_property(board[1]);
    display_property(board[3]);

    // throw_dice(&dice1);
    int choice1;

    while (end!=1) {
        // Player 1
        print_text1(list_players[0]);

        if (scanf("%d", &choice1) != 1) { // Validate input type
            while (getchar() != '\n');  // Clear invalid input
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        switch (choice1)
        {
        case 1:
            throw_dice(&dice1);
            print_dice(&dice1);
            move_player(list_players[0],board,dice1.dtotal);
            display_player(list_players[0]);
            break;
        case 9:
            end=1;
            printf("Game is over!\n");
            break;
        default:
            printf("Invalid input. Please enter 1 or 9.\n");
            break;
        }
        
        // Player 2
        print_text1(list_players[1]);

        if (scanf("%d", &choice1) != 1) { // Validate input type
            while (getchar() != '\n');  // Clear invalid input
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        switch (choice1)
        {
        case 1:
            throw_dice(&dice1);
            print_dice(&dice1);
            move_player(list_players[1],board,dice1.dtotal);
            display_player(list_players[1]);
            break;
        case 9:
            end=1;
            printf("Game is over!\n");
            break;
        default:
            printf("Invalid input. Please enter 1 or 9.\n");
            break;
        }
    }



// Playing the game

    // while (end!=1)
    // {
    //     /* code */
    // }
    

// Lançar dados    
    // board[0]=create_property(name,0,150,150,rent,300);
    // display_property(board[0]);


    release_player(list_players[0]);
    release_player(list_players[1]);
    release_player(list_players[2]);
    release_player(list_players[3]);

}