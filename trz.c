/*
Zadanie:  Trzy różne
autor:    Konrad Hałaszko
indeks:   479234
*/
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

struct pair{

    int* first;
    int* second;

}pair;

struct trio{

    int first, second, third;

}trio;

int min( int a, int b ){
    if( a < b ){
        return a;
    }
    else{
        return b;
    }
}

int max( int a, int b){
    if( a < b ){
        return b;
    }
    else{
        return a;
    }
}
    

void declare_memory( int n, struct pair* X ){

    (*X).first = (int*)malloc((unsigned)n*sizeof(int*));
    (*X).second = (int*)malloc((unsigned)n*sizeof(int*));

}

void answer( int a, int b ){

    printf("%d %d\n", a, b);

}

void read_data( int* n, int** network, int** distance, int** network_back, int** distance_back ){

    if( scanf( "%d", n ) != 1 ){
        fprintf(stderr,"ERROR");exit(1);
    }

    (*network)=(int*)malloc((unsigned)(*n)*sizeof(int*));
    (*distance)=(int*)malloc((unsigned)(*n)*sizeof(int*));
    (*network_back)=(int*)malloc((unsigned)(*n)*sizeof(int*));
    (*distance_back)=(int*)malloc((unsigned)(*n)*sizeof(int*));

    for( int i = 0 ; i < *n ; ++i ){

        if( scanf( "%d %d", &(*network)[i], &(*distance)[i] ) != 2 ){
            fprintf(stderr,"ERROR");exit(1);
        }

    }

    for( int i = 0 ; i < *n ; ++i ){
        
        (*network_back)[i] = (*network)[*n-1-i];
        (*distance_back)[i] = (*distance)[*n-1-i];

    }
}

bool solution_exists( int n, int* network ){

    int x, y, z;
    x = y = z = -1;

    for( int i = 0 ; i < n ; ++i ){

        if( network[i] != z && network[i] != y && network[i] != x ){

            z = y;

            y = x;

            x = network[i];

        }
    }

    return (z != -1);
}

void reverse_table( int n, int** table ){

    int tem, index = 0;

    while( index <= n-1-index ){

        tem = (*table)[index];

        (*table)[index] = n - 1 - (*table)[n-1-index];
        (*table)[n-1-index] = n - 1 - tem;

        ++index;
    }
}

void add_possibility_min( struct trio* net, struct trio* index, int motel_network, int motel_index ){//funkcja zachowuje monotoniczność

    if( (*net).first != motel_network && (*net).second != motel_network && (*net).third != motel_network ){

        (*net).third = (*net).second;
        (*net).second = (*net).first;
        (*net).first = motel_network;

        (*index).third = (*index).second;
        (*index).second = (*index).first;
        (*index).first = motel_index;

    }
    else{
        if( (*net).third == motel_network ){

            (*net).third = (*net).second;
            (*net).second = motel_network;

            (*index).third = (*index).second;
            (*index).second = motel_index;

        }
        if( (*net).second == motel_network ){

            (*net).second = (*net).first;
            (*net).first = motel_network;

            (*index).second = (*index).first;
            (*index).first = motel_index;

        }
        if( (*net).first == motel_network ){

            (*index).first = motel_index;

        }
    }
}

void update_table( struct trio net, struct trio index, struct pair* table, int motel_network, int motel_index ){

    if( net.first == motel_network ){

        (*table).first[motel_index] = index.second;
        (*table).second[motel_index] = index.third; 

    }
    else{

        (*table).first[motel_index] = index.first;

        if( net.second == motel_network ){

            (*table).second[motel_index] = index.third;

        }
        else{

            (*table).second[motel_index] = index.second;

        }
    }
}

void caterpillar_min( int n, int* network, int* distance, struct pair* closest ){//mozna odwrocic tablice distance i network i znalesc closest_right

    int i=0, j=1;//head and tail of a caterpillar_min

    struct trio net;//our candidates
    struct trio index;//distances of our candidates

    //we want distance[index.first] R distance[index.second] R distance[index.third], R = (>=)

    net.second = net.third = -1;
    index.second = index.third = -1;
    net.first = network[0];
    index.first = 0;

    while( j < n ){//gasieniczka

        if( distance[j] == distance[j-1] || i == j ){//add some statement for tail
            
            add_possibility_min( &net, &index, network[j], j );

            ++j;

        }
        else{//distance[j] > distance[j-1], tail needs to 
            
            update_table( net, index, closest, network[i], i );

            ++i;

        }
    }

    for( ; i < n; ++i ){
        update_table( net, index, closest, network[i], i);
    }
}

void calculate_minimum( int n, int* network, int *distance, struct pair closest_left, struct pair closest_right, int* RES_MIN ){

    for( int i = 0 ; i < n ; ++i ){
        
        if( closest_left.first[i] != -1 && closest_right.first[n-1-i] != -1 ){

            if( network[closest_left.first[i]] != network[n-1-closest_right.first[n-1-i]] ){

                (*RES_MIN) = min( *RES_MIN, max( distance[i] - distance[closest_left.first[i]], distance[n-1-closest_right.first[n-1-i]] - distance[i]) );

            }
            if( closest_left.second[i] != -1 ){

                (*RES_MIN) = min( *RES_MIN, max( distance[i] - distance[closest_left.second[i]], distance[n-1-closest_right.first[n-1-i]] - distance[i]) );

            }
            if( closest_right.second[n-1-i] != -1 ){

                (*RES_MIN) = min( *RES_MIN, max( distance[i] - distance[closest_left.first[i]], distance[n-1-closest_right.second[n-1-i]] - distance[i]) );

            }
        }
    }
}

int solve_min( int n, int* network, int *distance, int* network_back, int *distance_back ){
    //2 closest left and 2 closest righr
    struct pair closest_left, closest_right; 

    declare_memory( n, &closest_left );
    declare_memory( n, &closest_right );

    caterpillar_min( n, network, distance, &closest_left );
    caterpillar_min( n, network_back, distance_back, &closest_right );

    int RES_MIN = 1000000000;//dac tu int_max

    calculate_minimum( n, network, distance, closest_left, closest_right, &RES_MIN );

    free(closest_left.first);
    free(closest_left.second);
    free(closest_right.first);
    free(closest_right.second);

    return RES_MIN;
}

void add_possibility_max( struct trio* net, struct trio* index, int motel_network, int motel_index ){//funkcja zachowuje monotoniczność
    
    if( (*net).third == -1 && (*net).first != motel_network && (*net).second != motel_network && (*net).third != motel_network ){

        if( (*net).second == -1 ){

            (*net).second = motel_network;

            (*index).second = motel_index;

        }
        else{

            (*net).third = motel_network;

            (*index).third = motel_index;

        }
    }
}

void caterpillar_max( int n, int* network, struct pair* furthest ){

    struct trio net;//our candidates
    struct trio index;//distances of our candidates

    //we want distance[index.first] R distance[index.second] R distance[index.third], R = (>=)

    net.second = net.third = -1;
    index.second = index.third = -1;
    net.first = network[0];
    index.first = 0;

    update_table( net, index, furthest, network[0], 0 );

    for( int i = 1 ; i < n ; ++i ){

        add_possibility_max( &net, &index, network[i], i );

        update_table( net, index, furthest, network[i], i );

    }
}

void calculate_maximum( int n, int* network, int* distance, struct pair furthest_left, struct pair furthest_right, int* RES_MAX ){
    
    for( int i = 0 ; i < n ; ++i ){
        
        if( furthest_left.first[i] != -1 && furthest_right.first[n-1-i] != -1 ){

            if( network[furthest_left.first[i]] != network[n-1-furthest_right.first[n-1-i]] ){

                (*RES_MAX) = max( *RES_MAX, min( distance[i] - distance[furthest_left.first[i]], distance[n-1-furthest_right.first[n-1-i]] - distance[i]) );

            }
            if( furthest_left.second[i] != -1 ){

                (*RES_MAX) = max( *RES_MAX, min( distance[i] - distance[furthest_left.second[i]], distance[n-1-furthest_right.first[n-1-i]] - distance[i]) );

            }
            if( furthest_right.second[n-1-i] != -1 ){

                (*RES_MAX) = max( *RES_MAX, min( distance[i] - distance[furthest_left.first[i]], distance[n-1-furthest_right.second[n-1-i]] - distance[i]) );

            }
        }

    }
}

int solve_max( int n, int* network, int* distance, int* network_back ){
    struct pair furthest_left, furthest_right;

    declare_memory( n, &furthest_left );
    declare_memory( n, &furthest_right );

    caterpillar_max( n, network, &furthest_left );
    caterpillar_max( n, network_back, &furthest_right );

    int RES_MAX = 0;
    calculate_maximum( n, network, distance, furthest_left, furthest_right, &RES_MAX );

    free(furthest_left.first);
    free(furthest_left.second);
    free(furthest_right.first);
    free(furthest_right.second);

    return RES_MAX;
}

int main(){

    int n;
    int* network, *network_back, * distance, * distance_back;

    read_data( &n, &network, &distance, &network_back, &distance_back );

    if(!solution_exists( n, network )){
        answer( 0, 0 );
    }
    else{
        int res_min, res_max;

        res_min = solve_min( n, network, distance, network_back, distance_back );
        res_max = solve_max( n, network, distance, network_back );

        answer( res_min, res_max );
    }

    free(network);
    free(distance);
    free(network_back);
    free(distance_back);

    return 0;
}