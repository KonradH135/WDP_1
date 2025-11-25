#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct card{
    char object_type;
    double first, second, third, fourth, fifth;
};

void read_data( int n, struct card* list_of_cards ){
    for( int i = 1 ; i <= n ; i++ ){
        if( scanf(" %c", &list_of_cards[i].object_type ) != 1){
            //jakis blad
        }
        switch( list_of_cards[i].object_type ){
            case 'P':
                if( scanf("%lf %lf %lf %lf", 
                    &list_of_cards[i].first, 
                    &list_of_cards[i].second, 
                    &list_of_cards[i].third, 
                    &list_of_cards[i].fourth ) != 4){
                    //jakis blad
                }
                break;
            case 'K':
                if( scanf("%lf %lf %lf", 
                    &list_of_cards[i].first, 
                    &list_of_cards[i].second, 
                    &list_of_cards[i].third ) != 3){
                    //jakis blad
                }
                break;
            case 'Z':
                if( scanf("%lf %lf %lf %lf %lf", 
                    &list_of_cards[i].first, 
                    &list_of_cards[i].second, 
                    &list_of_cards[i].third, 
                    &list_of_cards[i].fourth, 
                    &list_of_cards[i].fifth ) != 5){
                    //jakis blad
                }
                break;
        }
    }
}

int recursion( double sheet, double x_3, double y_3, struct card* list_of_cards ){
    if( list_of_cards[(int)sheet].object_type == 'Z' ){
        double x_1, x_2, y_1, y_2, new_sheet;
        new_sheet = list_of_cards[(int)sheet].first;
        x_1 = list_of_cards[(int)sheet].second;
        y_1 = list_of_cards[(int)sheet].third;
        x_2 = list_of_cards[(int)sheet].fourth;
        y_2 = list_of_cards[(int)sheet].fifth;
        double d = (x_2-x_1)*(y_3-y_1)-(y_2-y_1)*(x_3-x_1);
        double epsilon = 0.000001;
        //printf("jakies d: %lf \n",d);
        if( d <= epsilon && d >= -epsilon ){
            return recursion( new_sheet, x_3, y_3, list_of_cards );
        }
        if( d < 0 ){
            return 0;
        }
        if( d > 0 ){
            double sym_x_3, sym_y_3;
            sym_x_3 = x_3-(2*(y_1-y_2)*d)/((y_2-y_1)*(y_2-y_1) + (x_2-x_1)*(x_2-x_1));
            sym_y_3 = y_3-(2*(x_2-x_1)*d)/((y_2-y_1)*(y_2-y_1) + (x_2-x_1)*(x_2-x_1));
            //printf("sym: %lf %lf\n",sym_x_3,sym_y_3);
            return (recursion( new_sheet, x_3, y_3, list_of_cards ) + recursion( new_sheet, sym_x_3, sym_y_3, list_of_cards ));
        }
    }
    else{
        if( list_of_cards[(int)sheet].object_type == 'P' ){
            double x_1, x_2, y_1, y_2;
            double epsilon = 0.000001;
            x_1 = list_of_cards[(int)sheet].first;
            y_1 = list_of_cards[(int)sheet].second;
            x_2 = list_of_cards[(int)sheet].third;
            y_2 = list_of_cards[(int)sheet].fourth;
            if( x_1 <= x_3 + epsilon && x_3 <= x_2 + epsilon && y_1 <= y_3 + epsilon && y_3 <= y_2 + epsilon ){
                return 1;
            }
            else{
                return 0;
            }
        } 
        else{
            double x_1, y_1, r;
            double epsilon = 0.000001;
            x_1 = list_of_cards[(int)sheet].first;
            y_1 = list_of_cards[(int)sheet].second;
            r = list_of_cards[(int)sheet].third;
            if ( (x_3-x_1)*(x_3-x_1)+(y_3-y_1)*(y_3-y_1) <= r*r + epsilon){
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
    double sheet, x_3, y_3;
    for( int i = 0 ; i < q ; i++ ){
        if( scanf("%lf %lf %lf", &sheet, &x_3, &y_3) != 3 ){
            //jakis blad
        }
        int res = recursion( sheet, x_3, y_3, list_of_cards );
        printf("%d \n",res);
    }
}

int main(){
    int n,q;
    if( scanf("%d %d",&n,&q) != 2 ){
        //jakis blad
    }
    struct card* list_of_cards;
    list_of_cards = (struct card*)malloc((unsigned)(n+1)*sizeof(struct card));
    if( !list_of_cards ){
        //jakis blad
    }
    read_data( n, list_of_cards );
    solve(q, list_of_cards );
    free(list_of_cards);
}