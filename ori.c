/*
Zadanie:  Origami
autor:    Konrad Hałaszko
indeks:   479234
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

const double EPSILON = 0.000001;

struct card{
    char object_type; // \in { 'Z', 'P', 'K' }
    double first, second, third, fourth, fifth;
    int paper;
};

void read_data( int n, struct card* list_of_cards ){
    // gets input into list_of_cards

    for( int i = 1 ; i <= n ; i++ ){
        if( scanf(" %c", &list_of_cards[i].object_type ) != 1 ){
            fprintf(stderr,"ERROR");exit(1);
        }
        switch( list_of_cards[i].object_type ){
            case 'P':
                if( scanf("%lf %lf %lf %lf", 
                    &list_of_cards[i].first, 
                    &list_of_cards[i].second, 
                    &list_of_cards[i].third, 
                    &list_of_cards[i].fourth ) != 4 ){
                    fprintf(stderr,"ERROR");exit(1);
                }
                break;
            case 'K':
                if( scanf("%lf %lf %lf", 
                    &list_of_cards[i].first, 
                    &list_of_cards[i].second, 
                    &list_of_cards[i].third ) != 3 ){
                    fprintf(stderr,"ERROR");exit(1);
                }
                break;
            case 'Z':
                if( scanf("%d %lf %lf %lf %lf", 
                    &list_of_cards[i].paper, 
                    &list_of_cards[i].second, 
                    &list_of_cards[i].third, 
                    &list_of_cards[i].fourth, 
                    &list_of_cards[i].fifth ) != 5 ){
                    fprintf(stderr,"ERROR");exit(1);
                }
                break;
        }
    }
}

bool inside_the_rectangle( struct card specific_card, double x_3, double y_3 ){// cheaks if the point is inside the rectangle
    double x_1, x_2, y_1, y_2;
    x_1 = specific_card.first;
    y_1 = specific_card.second;
    x_2 = specific_card.third;
    y_2 = specific_card.fourth;
    if( x_1 <= x_3 + EPSILON && x_3 <= x_2 + EPSILON && y_1 <= y_3 + EPSILON && y_3 <= y_2 + EPSILON ){
        return true;
    }
    return false;
}

bool inside_the_circle( struct card specific_card, double x_3, double y_3 ){// cheaks if the point is inside the circle
    double x_1, y_1, r;
    x_1 = specific_card.first;
    y_1 = specific_card.second;
    r = specific_card.third;
    if ( (x_3-x_1)*(x_3-x_1)+(y_3-y_1)*(y_3-y_1) <= r*r + EPSILON ){
        return true;
    }
    return false;
}

int pin_the_sheet( int sheet, double x_3, double y_3, struct card* list_of_cards ){
    // function returns the number of layers pined
    if( list_of_cards[sheet].object_type == 'Z' ){
        int new_sheet;
        double x_1, x_2, y_1, y_2;
        new_sheet = list_of_cards[sheet].paper; // number of sheet we want to go back to
        x_1 = list_of_cards[sheet].second;
        y_1 = list_of_cards[sheet].third;
        x_2 = list_of_cards[sheet].fourth;
        y_2 = list_of_cards[sheet].fifth;
        double d = (x_2-x_1)*(y_3-y_1)-(y_2-y_1)*(x_3-x_1);
        if( d <= EPSILON && d >= -EPSILON ){ // point is on edge
            return pin_the_sheet(new_sheet, x_3, y_3, list_of_cards);
        }
        if( d < 0 ){ // point is not in the figure
            return 0;
        }
        if( d > 0 ){ // point is in figure 
            double symetric_x3, symetric_y3; // symetric point
            symetric_x3 = x_3-(2*(y_1-y_2)*d)/((y_2-y_1)*(y_2-y_1) + (x_2-x_1)*(x_2-x_1));
            symetric_y3 = y_3-(2*(x_2-x_1)*d)/((y_2-y_1)*(y_2-y_1) + (x_2-x_1)*(x_2-x_1));
            return (pin_the_sheet(new_sheet, x_3, y_3, list_of_cards) + pin_the_sheet(new_sheet, symetric_x3, symetric_y3, list_of_cards));
        }
    }
    else{
        if( list_of_cards[sheet].object_type == 'P' ){ // returns 1 if point is inside the rectangle, returns 0 if point in not inside the restangle
            if( inside_the_rectangle(list_of_cards[sheet], x_3, y_3) ){
                return 1;
            }
            else{
                return 0;
            }
        } 
        else{ // returns 1 if point is inside the circle, returns 0 if point in not inside the circle
            if ( inside_the_circle(list_of_cards[sheet], x_3, y_3) ){
                return 1;
            }
            else{
                return 0;
            }
        }
    }
    return 0;
}

void solve( int q, struct card* list_of_cards ){
    // the process is not imitaded, 
    // function pin_the_sheet does it backwards
    int sheet;
    double x_3, y_3;
    for( int i = 0 ; i < q ; i++ ){ // solves q problems
        if( scanf("%d %lf %lf", &sheet, &x_3, &y_3) != 3 ){
            fprintf(stderr,"ERROR");exit(1);
        }
        int res = pin_the_sheet(sheet, x_3, y_3, list_of_cards); // recursive function that finds the result
        printf("%d \n",res);
    }
}

int main(){
    int n,q;
    if( scanf("%d %d",&n,&q) != 2 ){
        fprintf(stderr,"ERROR");exit(1);
    }
    struct card* list_of_cards;
    list_of_cards = (struct card*)malloc((unsigned)(n+1)*sizeof(struct card));
    if( !list_of_cards ){
        printf("Allocation Failed");
        exit(1);
    }
    read_data(n, list_of_cards);
    solve(q, list_of_cards);
    free(list_of_cards);
}