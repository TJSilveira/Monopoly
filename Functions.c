#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <time.h>

// TODO
// 1. Implement a dictionary using a hashmap - https://troydhanson.github.io/uthash/

typedef struct player player;
typedef struct property property;
typedef struct dice dice;
typedef struct chance_CC chance_CC;
void move_player(player *a, property** b,chance_CC **all,player **player_list,int total_players,int i,int move_to_location);

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

typedef struct chance_CC{
    char* text;
    int adv_location;
    int adv_near_RR;
    int adv_near_UT;
    int change_loc;
    int CF_bank;
    int CF_players;
    int GOJ;
    int GTJ;
    int CF_house;
    int CF_hotel;
}chance_CC;

chance_CC* create_chanceCC(char* text,int adv_location,int adv_near_RR,int adv_near_UT,int change_loc,int CF_bank,int CF_players,int GOJ,int GTJ,int CF_house,int CF_hotel){
    chance_CC* i = malloc(sizeof(chance_CC));
    i->text=strdup(text);
    i->adv_location = adv_location;
    i->adv_near_RR = adv_near_RR;
    i->adv_near_UT = adv_near_UT;
    i->change_loc = change_loc;
    i->CF_bank = CF_bank;
    i->CF_players = CF_players;
    i->GOJ = GOJ;
    i->GTJ = GTJ;
    i->CF_house = CF_house;
    i->CF_hotel = CF_hotel;
    return i;
}

chance_CC** create_all_chanceCC(FILE *infile){
    chance_CC** all_chanceCC = NULL;
    char buffer[1000];
    int i =0;
    char *data;

    //struct variables
    char text[250];
    int adv_location;
    int adv_near_RR;
    int adv_near_UT;
    int change_loc;
    int CF_bank;
    int CF_players;
    int GOJ;
    int GTJ;
    int CF_house;
    int CF_hotel;

    while(fgets(buffer,sizeof(buffer),infile)){
        if (i==0)
        {
            i++;
            continue;
        }
        all_chanceCC = realloc(all_chanceCC,i*sizeof(chance_CC*));
        
        //Get Text of the Card
        data=strtok(buffer,",");
        strcpy(text,data);

        adv_location=atoi(strtok(NULL,","));
        adv_near_RR=atoi(strtok(NULL,","));
        adv_near_UT=atoi(strtok(NULL,","));
        change_loc=atoi(strtok(NULL,","));
        CF_bank=atoi(strtok(NULL,","));
        CF_players=atoi(strtok(NULL,","));
        GOJ=atoi(strtok(NULL,","));
        GTJ=atoi(strtok(NULL,","));
        CF_house=atoi(strtok(NULL,","));
        CF_hotel=atoi(strtok(NULL,"\n"));

        all_chanceCC[i-1]=create_chanceCC(text,adv_location,adv_near_RR,adv_near_UT,change_loc,CF_bank,CF_players,GOJ,GTJ,CF_house,CF_hotel);        
        i++;
    }
    return all_chanceCC;
}

void display_chanceCC(chance_CC *card){
    printf("Text: %s\nAdvance to location: %i; Advance to near RR: %i; Advance to near UT: %i; Change location to: %i; CF from Bank: %i; CF from players: %i; GOJ: %i; GTJ: %i; CF per house: %i; CF per hotel: %i\n",card->text,card->adv_location,card->adv_near_RR,card->adv_near_UT,card->change_loc,card->CF_bank,card->CF_players,card->GOJ,card->GTJ,card->CF_house,card->CF_hotel);
    //printf("Text: %s\n",card->text);
}

player* create_player(short pl_number, char *pl_name,short location, int money, short order){
    player *i=malloc(sizeof(player));
    i->pl_number=pl_number;
    i->pl_name=strdup(pl_name);
    i->location=location;
    i->money=money;
    i->order=order;
    i->laps=0;
    i->wallet=calloc(28,sizeof(property*));
    i->rounds_jail=0;
    i->num_GOJ=1;
    i->isBankrupt=0;
    return i;
}

player** create_list_players_test(FILE *infile,int *total_players){
    player** list_players =NULL;
    char buffer[500];
    int i=0;
    short pl_number;
    char *pl_name;
    short location;
    int money;
    short order;

    while(fgets(buffer,sizeof(buffer),infile)){
        if (i==0){
            i++;
            continue;
        }
        list_players = realloc(list_players,i*sizeof(player*));

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
        i++;
    }
    *total_players = i-1;
    return list_players;
}

void check_bankruptcy_status(player *a){
    if(a->money<0){
        a->isBankrupt=1;
        printf("You entered Bankruptcy. The game is over for you. :(");
    }
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
    char colours_temp[3];

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

void pay_rent(player *a,property*b,int i){
    if(strcmp(b->colours,"RR")==0){
        a->money-=b->rent_railroad[b->owner->num_RR-1];
        b->owner->money+=b->rent_railroad[b->owner->num_RR-1];
        printf("%s is owned by %s and has %i Railroads in total. You need to pay: %i\n",b->pt_name,b->owner->pl_name,b->owner->num_RR,b->rent_railroad[b->owner->num_RR-1]);
        printf("\nNew Cash Balance of %s: %i\n\n",b->owner->pl_name,b->owner->money);
    }
    else if(strcmp(b->colours,"UT")==0){
        if(b->owner->num_Util==1){
            a->money-=(4*i);
            b->owner->money+=(4*i);
            printf("%s is owned by %s and has %i Utilities in total. You need to pay: %i\n",b->pt_name,b->owner->pl_name,b->owner->num_Util,(4*i));    
        }
        else{
            a->money-=(10*i);
            b->owner->money+=(10*i);
            printf("%s is owned by %s and has %i Utilities in total. You need to pay: %i\n",b->pt_name,b->owner->pl_name,b->owner->num_Util,(10*i));
        }
        printf("\nNew Cash Balance of %s: %i\n\n",b->owner->pl_name,b->owner->money);
    }
    else{
        a->money-=b->rent[b->n_houses];
        b->owner->money+=b->rent[b->n_houses];
        printf("%s is owned by %s and has %i Houses. You need to pay: %i\n",b->pt_name,b->owner->pl_name,b->n_houses,b->rent[b->n_houses]);
        printf("\nNew Cash Balance of %s: %i\n\n",b->owner->pl_name,b->owner->money);
    }
}

void assign_property(player *a, property *b,short i){
    switch(i){
        case 1:
            a->wallet[0] = b;
            break;
        case 3:
            a->wallet[1] = b;
            break;
        case 5:
            a->wallet[2] = b;
            break;
        case 6:
            a->wallet[3] = b;
            break;
        case 8:
            a->wallet[4] = b;
            break;
        case 9:
            a->wallet[5] = b;
            break;
        case 11:
            a->wallet[6] = b;
            break;
        case 12:
            a->wallet[7] = b;
            break;
        case 13:
            a->wallet[8] = b;
            break;
        case 14:
            a->wallet[9] = b;
            break;
        case 15:
            a->wallet[10] = b;
            break;
        case 16:
            a->wallet[11] = b;
            break;
        case 18:
            a->wallet[12] = b;
            break;
        case 19:
            a->wallet[13] = b;
            break;
        case 21:
            a->wallet[14] = b;
            break;
        case 23:
            a->wallet[15] = b;
            break;
        case 24:
            a->wallet[16] = b;
            break;
        case 25:
            a->wallet[17] = b;
            break;
        case 26:
            a->wallet[18] = b;
            break;
        case 27:
            a->wallet[19] = b;
            break;
        case 28:
            a->wallet[20] = b;
            break;
        case 29:
            a->wallet[21] = b;
            break;
        case 31:
            a->wallet[22] = b;
            break;
        case 32:
            a->wallet[23] = b;
            break;
        case 34:
            a->wallet[24] = b;
            break;
        case 35:
            a->wallet[25] = b;
            break;
        case 37:
            a->wallet[26] = b;
            break;
        case 39:
            a->wallet[27] = b;
            break;
        default:
            break;
    }   
}

void update_build_counter(player *a){
    int i;
    short counter_BR = 0;
    short counter_LB = 0;
    short counter_PK = 0;
    short counter_OR = 0;
    short counter_RD = 0;
    short counter_YW = 0;
    short counter_GR = 0;
    short counter_DB = 0;
    short counter_RR = 0;
    short counter_UT = 0;


    for(i=0;i<(28);i++){
        if (a->wallet[i] == NULL) continue;
        
        if(strcmp(a->wallet[i]->colours,"BR")==0){
            counter_BR++;
            continue;
        }
        if(strcmp(a->wallet[i]->colours,"LB")==0){
            counter_LB++;
            continue;
        }
        if(strcmp(a->wallet[i]->colours,"PK")==0){
            counter_PK++;
            continue;
        }
        if(strcmp(a->wallet[i]->colours,"OR")==0){
            counter_OR++;
            continue;
        }
        if(strcmp(a->wallet[i]->colours,"RD")==0){
            counter_RD++;
            continue;
        }
        if(strcmp(a->wallet[i]->colours,"YW")==0){
            counter_YW++;
            continue;
        }
        if(strcmp(a->wallet[i]->colours,"GR")==0){
            counter_GR++;
            continue;
        }
        if(strcmp(a->wallet[i]->colours,"DB")==0){
            counter_DB++;
            continue;
        }
        if(strcmp(a->wallet[i]->colours,"RR")==0){
            counter_RR++;
            continue;
        }
        if(strcmp(a->wallet[i]->colours,"UT")==0){
            counter_UT++;
            continue;
        }
    }

    if(counter_BR==2){
        a->build_BR=1;
        printf("%s can now build houses in BR locations!\n",a->pl_name);
    } else {
        a->build_BR=0;
    }
    
    if(counter_LB==3){
        a->build_LB=1;
        printf("%s can now build houses in LB locations!\n",a->pl_name);
    } else {
        a->build_LB=0;
    }

    if(counter_PK==3){
        a->build_PK=1;
        printf("%s can now build houses in PK locations!\n",a->pl_name);
    } else {
        a->build_PK=0;
    }

    if(counter_OR==3){
        a->build_OR=1;
        printf("%s can now build houses in OR locations!\n",a->pl_name);
    } else {
        a->build_OR=0;
    }

    if(counter_RD==3){
        printf("%s can now build houses in RD locations!\n",a->pl_name);
        a->build_RD=1;
    } else{
        a->build_RD=0;
    }

    if(counter_YW==3){
        printf("%s can now build houses in YW locations!\n",a->pl_name);
        a->build_YW=1;
    } else {
        a->build_YW=0;
    }

    if(counter_GR==3){
        printf("%s can now build houses in GR locations!\n",a->pl_name);
        a->build_GR=1;
    } else {
        a->build_GR=0;
    }
    if(counter_DB==2){
        printf("%s can now build houses in DB locations!\n",a->pl_name);
        a->build_DB=1;
    } else {
        a->build_DB=0;
    }
    a->num_RR = counter_RR;
    a->num_Util = counter_UT;
}

void buy_property(player *a,property *b,short i){
    int choice;
    int val_input=0;
    printf("Do you want to buy %s (%s) for %i? [1. yes / 0. no]: \n",b->pt_name,b->colours,b->price);

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
            assign_property(a,b,i);
            a->money -=b->price;
            update_build_counter(a);
            printf("%s is now the owner of %s for the price of %i\n",a->pl_name,b->pt_name,b->price);
            val_input=1;
            break;
        case 0:
            printf("Property was not bought\n");
            val_input=1;
            break;
        default:
            printf("Invalid input. Please enter 1 for yes or 0 for no\n");
            break;
        }
    }
}

void print_wallet(player *a){
    int i;

    for(i=0;i<28;i++){
        if(a->wallet[i]==NULL) continue;
        printf("%i: %s (%s) - %i houses\n",i,a->wallet[i]->pt_name,a->wallet[i]->colours,a->wallet[i]->n_houses);
    }
}

void choose(int *choice, int arr[], int n){
    int criteria=0;
    while(criteria==0){
        if (scanf("%d", choice) != 1) { // Validate input type
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        for(int i=0; i<n;i++){
            if((*choice)==arr[i]) {
                criteria=1;
                break;
            }
        }
        if(criteria==0){
            printf("Invalid input. Not one of the available options\n");
        }
    }
}

void buy_house(player *a){
    if((a->build_BR+a->build_DB+a->build_GR+a->build_LB+a->build_OR+a->build_PK+a->build_RD+a->build_YW)>0){
        int choice_pt;
        int arr1[22] = {0};
        
        for(int i = 0;i<22;i++){
            arr1[i]=-999;
        }

        if(a->build_BR==1){
            arr1[0]=0;
            arr1[1]=1;
        }
        if (a->build_LB==1){
            arr1[2]=3;
            arr1[3]=4;
            arr1[4]=5;
        }
        if (a->build_PK==1){
            arr1[5]=6;
            arr1[6]=8;
            arr1[7]=9;
        }
        if (a->build_OR==1){
            arr1[8]=11;
            arr1[9]=12;
            arr1[10]=13;
        }
        if (a->build_RD==1){
            arr1[11]=14;
            arr1[12]=15;
            arr1[13]=16;
        }
        if (a->build_YW==1){
            arr1[14]=18;
            arr1[15]=19;
            arr1[16]=21;
        }
        if (a->build_GR==1){
            arr1[17]=22;
            arr1[18]=23;
            arr1[19]=24;
        }
        if (a->build_DB==1){
            arr1[20]=26;
            arr1[21]=27;
        }

        //Choose in which property to purchase houses
        //TODO - You cannot exit the menu if a player has all the houses in the game as there will not be any element -999 in the arr1
        printf("Choose the property in which you want to build according to the numbers of your wallet. If you want to exit, type -999: ");
        choose(&choice_pt,arr1,sizeof(arr1)/4);

        //Check if the number provided was not -999, the only negative number that the array of choices can have
        if (choice_pt>-1){
            int choice_HO;
            int arr2[5]={0};
            for(int i = 0;i<(5-(a->wallet[choice_pt]->n_houses));i++){
                arr2[i]=i+1;
            }
            //Choose how many houses you want to purchase
            printf("Choose how many houses to add: ");
            choose(&choice_HO,arr2,sizeof(arr2)/4);
            
            //Check if the player has enough money to buy a house
            if((a->money)-((a->wallet[choice_pt]->p_house)*choice_HO)>0){
                //Add the house in the property and take the money
                a->wallet[choice_pt]->n_houses+=choice_HO;
                a->money-=(a->wallet[choice_pt]->p_house)*choice_HO;
            }
        }
    }

    if((a->build_BR+a->build_DB+a->build_GR+a->build_LB+a->build_OR+a->build_PK+a->build_RD+a->build_YW)==0){
        printf("You cannot buy any houses yet");
    }
}

void GTJ(player *a){
    a->location = 10;
    a->rounds_jail = 3;
}

void do_chanceCC(player *a,property *p,chance_CC **all,player **player_list,int total_players,property** b,int i){
    //draw card
    int draw=(rand()%16);
    if (strcmp(p->colours,"CH")==0)
    {
        draw+=16;
    }
    

    printf("%s\n",all[draw]->text);

    if(all[draw]->adv_location>0){
        //The value is subtracted by 1 due to data input constraints. Given that NULL are extracted as 0, it creates confusion with cards that send the player to location 0. Thus, 1 was added to all locations.
        move_player(a,b,all,player_list,total_players,0,all[draw]->adv_location-1);
    }
    else if(all[draw]->adv_near_RR==1){
        if(a->location<5){
            move_player(a,b,all,player_list,total_players,0,5);
        }
        else if(a->location>5 && a->location<15){
            move_player(a,b,all,player_list,total_players,0,15);
        }
        else if(a->location>15 && a->location<25){
            move_player(a,b,all,player_list,total_players,0,25);
        }
        else if(a->location>25 && a->location<35){
            move_player(a,b,all,player_list,total_players,0,35);
        }
        else if(a->location>35){
            move_player(a,b,all,player_list,total_players,0,45);
        }
    }
    else if(all[draw]->adv_near_UT==1){
        if(a->location<12){
            move_player(a,b,all,player_list,total_players,0,12);
        }
        else if(a->location>12 && a->location<28){
            move_player(a,b,all,player_list,total_players,0,28);
        }
        else if(a->location>28){
            move_player(a,b,all,player_list,total_players,0,52);
        }
    }
    else if(all[draw]->change_loc!=0){
        move_player(a,b,all,player_list,total_players,0,((a->location)-(all[draw]->change_loc)));
    }
    else if(all[draw]->CF_bank!=0){
        a->money+=all[draw]->CF_bank;
    }
    else if(all[draw]->CF_players!=0){
        for(int i=0;i<total_players;i++){
            //Note: No if statement to ensure the payments are only done to players that are not the one that drew the card because the net effect of that cycle is 0: the money decrease will be compensated by the same increase
            a->money+=all[draw]->CF_players;
            player_list[i]->money-=all[draw]->CF_players;
        }
    }
    else if(all[draw]->GOJ!=0){
        a->num_GOJ++;
    }
    else if(all[draw]->GTJ!=0){
        GTJ(a);
    }
    else if(all[draw]->CF_house!=0){
        int counter_houses=0;
        int counter_hotels=0;
        for(int i=0;i<28;i++){
            if(a->wallet[i] == NULL){
                continue;
            }
            else if(a->wallet[i]->n_houses==5){
                counter_hotels++;
            }
            else{
            counter_houses+=a->wallet[i]->n_houses;
            }
        }
        a->money+=(counter_houses*all[draw]->CF_house)+(counter_hotels*all[draw]->CF_hotel);
    }
}

void move_player(player *a, property** b,chance_CC **all,player **player_list,int total_players,int i,int move_to_location){
    //Move the player to new location. These if statements give the function the flexibility to use the dice with int i or an actual location on the board with the int move_to_location
    if(move_to_location==0){
        a->location=a->location+i;
    }
    if(i==0){
        if (a->location<move_to_location)
        {
            a->location=move_to_location;
        }
        else{
            a->location=move_to_location+40;
        }
    }

    //if location is higher than 39, it means that the playes did a full round. Thus, it needs to go back to a lower location and receive the CF from the Start
    if (a->location>39)
    {
        a->location=a->location%40;
        a->laps++;

        //if the player lands on the start, he will not receive the $200 bonus yet. The player will receive it later in the CF from the property
        if (a->location!=0)
        {
            a->money+=b[0]->CF;
        }
    }

    // Check onwership of the property. Either the property is already yours, it is from someone else and you need to pay rent or it is free. If it is free, it is either because the property cannot be bought or was not bought yet
    if (b[a->location]->owner==a)
    {
        printf("\nThe %s property is already yours\n",b[a->location]->pt_name);
    }
    else if(strcmp(b[a->location]->colours,"JA")==0){
        printf("You are visiting Jail\n");
    }
    else if(strcmp(b[a->location]->colours,"GJ")==0){
        printf("Oh no! You are going to Jail!\n");
        GTJ(a);
    }
    else if(strcmp(b[a->location]->colours,"CH")==0){
        printf("You landed on Chance!\n");
        do_chanceCC(a,b[a->location],all,player_list,total_players,b,i);
    }
    else if(strcmp(b[a->location]->colours,"CC")==0){
        printf("You landed on Community Chest!\n");
        do_chanceCC(a,b[a->location],all,player_list,total_players,b,i);
    }
    else if(strcmp(b[a->location]->colours,"NU")==0){
        // Get the player the CF of the property
        printf("You landed on %s! CF: %i\n",b[a->location]->pt_name,b[a->location]->CF);
        a->money += b[a->location]->CF;
        check_bankruptcy_status(a);
    }
    else if ((b[a->location]->owner==NULL))
    {
        printf("\n%s has no owner.\n",b[a->location]->pt_name);
        
        // Only allow to buy properties that have a price, which are the only ones that can be bought
        if ((b[a->location]->price!=0) && (a->money<=b[a->location]->price)){
            printf("Unfortunately, you don't hve enough money.\n");
        }
        if ((b[a->location]->price!=0) && (a->money>b[a->location]->price))
        {
            buy_property(a,b[a->location],a->location);
        }
    }
    else
    {
        pay_rent(a,b[a->location],i);
        check_bankruptcy_status(a);
    };
}

void throw_dice(dice *a){
    
    a->d1=one_dice();
    a->d2=one_dice();
    a->dtotal = a->d1+a->d2;
    if(a->d1==a->d2){
        a->db=1;
    }
    else{
        a->db=0;
    }
}

void print_dice(dice *a){
    printf("Dados:%i, %i\n",a->d1,a->d2);
}

void print_text1(player *a){
    printf("\n\n====================================================\n\n");
    printf("%s (Player %i), it is your turn!\n",a->pl_name,a->order);
    printf("1. Lancar dado\n");
    printf("9. Acabar o jogo\n");
    printf("Enter your choice: ");
}

void print_text2(){
    printf("\n\n====================================================\n\n");
    printf("1. Acabar Turno\n");
    printf("2. Comprar Casas\n");
    printf("9. Acabar o jogo\n");
    printf("Enter your choice: ");
}

void print_textJail_no_GOJ(player *a){
    printf("\n\n====================================================\n\n");
    printf("%s (Player %i), it is your turn! You are in Jail.\n",a->pl_name,a->order);
    printf("1. Lancar dados\n");
    printf("2. Pagar $50 e Lancar dados\n");
    printf("9. Acabar o jogo\n");
    printf("Enter your choice: ");
}

void print_textJail_with_GOJ(player *a){
    printf("\n\n====================================================\n\n");
    printf("%s (Player %i), it is your turn! You are in Jail.\n",a->pl_name,a->order);
    printf("1. Lancar dados\n");
    printf("2. Pagar $50 e Lancar dados\n");
    printf("3. Usar Get Out of Jail Free Card e Lancar dados\n");
    printf("9. Acabar o jogo\n");
    printf("Enter your choice: ");
}

void remove_properties_bankruptcy(player *a){
    for(int property = 0;property<28;property++){
        if (a->wallet[property]!=NULL){
            a->wallet[property]->n_houses=0;
            a->wallet[property]->owner=NULL;
            a->wallet[property]=NULL;
        }
    }
}