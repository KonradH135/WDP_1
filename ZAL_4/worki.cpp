#include<bits/stdc++.h>
#include "worki.h"

using namespace std;

struct worek *desk = NULL;
int last_bag_number = 0;

void initialize(){
    desk = (worek *)malloc(sizeof(worek));
    worek *desk_inside = (worek *)malloc(sizeof(worek));
    if( !desk || !desk_inside ){
        //jakis blad
        exit(1);
    }
    desk->number = -1;
    desk->inside = desk_inside;
    desk->placement = NULL;

    desk_inside->number = 0;
    desk_inside->inside = NULL;
    desk_inside->placement = desk;
}

przedmiot *nowy_przedmiot(){
    if( !desk ){
        initialize();
    }
    struct przedmiot *new_item = (przedmiot *)malloc(sizeof(przedmiot));
    if( !new_item ){
        //jakis blad
        exit(1);
    }
    new_item->placement = desk->inside;
    desk->inside->number++;
    return new_item;
}

worek *nowy_worek(){
    if( !desk ){
        initialize();
    }
    struct worek *new_bag = (worek *)malloc(sizeof(worek));
    struct worek *new_bag_inside = (worek *)malloc(sizeof(worek));
    if( !new_bag || !new_bag_inside ){
        //jakis blad
        exit(1);
    }
    new_bag->number = last_bag_number;
    new_bag->inside = new_bag_inside;
    new_bag->placement = desk->inside;

    new_bag_inside->number = 0;
    new_bag_inside->inside = NULL;
    new_bag_inside->placement = new_bag;
    ++last_bag_number;
    return new_bag;
}

void wloz(przedmiot *what, worek *where){
    what->placement = where->inside;
    where->inside->number++;
}

void wloz(worek *what, worek *where){
    what->placement = where->inside;
    where->inside->number += what->inside->number;
}

void wyjmij(przedmiot *item){
    item->placement->number--;
    item->placement = desk->inside;
}

void wyjmij(worek *bag){
    bag->placement->number -= bag->inside->number;
    bag->placement = desk->inside;
}

int w_ktorym_worku(przedmiot *item){
    return item->placement->placement->number;
}

int w_ktorym_worku(worek *bag){
    return bag->placement->placement->number;
}

int ile_przedmiotow(worek *bag){
    return bag->inside->number;
}

void na_odwrot(worek *bag){
    bag->placement = bag->inside;
    bag->inside = desk->inside;
    desk->inside = bag->placement;
    desk->inside->placement = desk;
    bag->inside->placement = bag;

    int number_of_items = bag->inside->number;
    bag->inside->number = number_of_items - desk->inside->number;
    desk->inside->number = number_of_items;
}

void gotowe(){
    //usuń pamiec

}