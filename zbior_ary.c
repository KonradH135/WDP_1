#include <stdbool.h>
#include <stdlib.h>
#include "zbior_ary.h"

int Q;

void initiate_Q(int q){
    Q = q;
}

int positive_modulo(int a,int b){
    if( (a%b) >= 0 ){
        return a%b;
    }
    else{
        return a%b + b;
    }
}

int minimum(int a,int b){
    if(a>b){
        return b;
    }
    else{
        return a;
    }
}

int maximum(int a,int b){
    if(a>b){
        return a;
    }
    else{
        return b;
    }
}

zbior_ary create_empty_set(int len_1,int len_2){//tworzy zbior_ary z tablicami dlugosci (len_1 + len_2)
    zbior_ary Tem;
    Tem.len = len_1 + len_2;
    Tem.modulo_q = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));
    Tem.first_ele = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));
    Tem.second_ele = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));
    for(int i = 0;i < Tem.len;++i){
        Tem.modulo_q[i] = -1;
    }
    return Tem;
}

zbior_ary free_unused_memory(zbior_ary *Result){
    zbior_ary Tem;
    Tem.len = 0;
    for(int i = 0;i < (*Result).len ; ++i){
        if( (*Result).modulo_q[i] != -1){
            ++Tem.len;
        }
    }
    Tem.modulo_q = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));
    Tem.first_ele = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));
    Tem.second_ele = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));
    for(int i = 0; i < Tem.len; ++i){
        Tem.modulo_q[i] = (*Result).modulo_q[i];
        Tem.first_ele[i] = (*Result).first_ele[i];
        Tem.second_ele[i] = (*Result).second_ele[i];
    }
    free((*Result).modulo_q);free((*Result).first_ele);free((*Result).second_ele);

    return Tem;
}

void interval(zbior_ary X,int *x_1,int *x_2,int finger){
    *x_1 = X.first_ele[finger];
    *x_2 = X.second_ele[finger];
}

void add_interval(zbior_ary *Result,int x_1,int x_2,int y_1,int y_2,int *finger_X,int *finger_Y,int indeks_res){//mamy zapewnione x_1 <= y_1

    if( (x_2 +Q < y_1) || (y_2 <= x_2) ){//przedzialy rozlacznie lub przedzial (y_1,y_2) zawiera sie w (x_1,x_2)
        (*Result).first_ele[indeks_res] = x_1;
        (*Result).second_ele[indeks_res] = x_2;
        if(y_2 <= x_2){//przedzial (y_1,y_2) zawiera sie w (x_1,x_2)
            (*finger_Y)++;
        }
    }   
    else{//( (x_2 >= y_1 - Q) && (y_2 >= x_2) )//przedzialy na siebie nachodza
        (*Result).first_ele[indeks_res] = x_1;
        (*Result).second_ele[indeks_res] = y_2;
        (*finger_Y)++;
    }
    (*finger_X)++;
}

int last_ele_with_modulo(zbior_ary X, int finger_X){ //modulo = X.modulo_q[finger_X]
    int res;
    if( X.modulo_q[X.len-1] == X.modulo_q[finger_X]){
        res = X.len - 1;
    }
    else{
        res = finger_X;
        while( X.modulo_q[res + 1] == X.modulo_q[finger_X]){
            ++res;
        }
    }
    return res;
}

void copy_ele(zbior_ary *Result,zbior_ary X,int p,int k,int *indeks_res){//kopiuje elementy na indeksach od p do k ze zbioru X do Result
    int primary_ele = p;
    if(*indeks_res > 0){
        if(((*Result).modulo_q[*indeks_res - 1] == X.modulo_q[k]) && ((*Result).second_ele[*indeks_res - 1] >= X.first_ele[k] - Q)){
            (*Result).second_ele[*indeks_res - 1] = maximum(X.second_ele[k],(*Result).second_ele[*indeks_res - 1]);
            primary_ele = k + 1;
        }
        else{
            while( ((*Result).modulo_q[*indeks_res - 1] == X.modulo_q[primary_ele]) && ((*Result).second_ele[*indeks_res - 1] >= X.first_ele[primary_ele] - Q) ){
                (*Result).second_ele[*indeks_res - 1] = maximum(X.second_ele[primary_ele],(*Result).second_ele[*indeks_res - 1]);
                ++primary_ele;
            }
        }
    }
    for(int i = primary_ele; i <= k; ++i){
        (*Result).modulo_q[*indeks_res + i - primary_ele] = X.modulo_q[i];
        (*Result).first_ele[*indeks_res + i - primary_ele] = X.first_ele[i];
        (*Result).second_ele[*indeks_res + i - primary_ele] = X.second_ele[i];
    }
    *indeks_res += (k - primary_ele +1);
}

zbior_ary ciag_arytmetyczny(int a, int q, int b){
    initiate_Q(q);
    zbior_ary Result;
    Result.modulo_q = (int *)malloc(sizeof(int));
    Result.first_ele = (int *)malloc(sizeof(int));
    Result.second_ele = (int *)malloc(sizeof(int));

    Result.len = 1;
    Result.modulo_q[0] = positive_modulo(a,Q);
    Result.first_ele[0] = a;
    Result.second_ele[0] = b;

    return Result;
}

zbior_ary singleton(int a){
    return ciag_arytmetyczny(a,Q,a);
}

zbior_ary suma(zbior_ary A,zbior_ary B){
    zbior_ary Result = create_empty_set(A.len,B.len);
    int finger_A=0,finger_B=0,last_to_copy,indeks_res=0;
    int a_1,a_2,b_1,b_2;//para liczb [a_1,a_2] reprezentuje przedzial zawierajacy liczby całkowite należące do ciągu arytmetycznego o różnicy Q, zachodzi a_1 <= a_2

    while( (finger_A < A.len) && (finger_B < B.len) ){ //petla skonczy się gdy skończą się elementy jednego ze zbiorów
        if( A.modulo_q[finger_A] == B.modulo_q[finger_B] ){
            
            interval(A,&a_1,&a_2,finger_A);//wczytanie odpowiednich przedzialów
            interval(B,&b_1,&b_2,finger_B);

            Result.modulo_q[indeks_res] = A.modulo_q[finger_A];
            if(a_1 <= b_1){
                add_interval(&Result,a_1,a_2,b_1,b_2,&finger_A,&finger_B,indeks_res);
            }
            else{
                add_interval(&Result,b_1,b_2,a_1,a_2,&finger_B,&finger_A,indeks_res);
            }
            if(indeks_res - 1 >= 0){
                if( (Result.modulo_q[indeks_res-1] == Result.modulo_q[indeks_res]) && (Result.second_ele[indeks_res-1] >= minimum(a_1,b_1) - Q) ){
                    Result.second_ele[indeks_res-1] = maximum(Result.second_ele[indeks_res],Result.second_ele[indeks_res-1]);
                    Result.modulo_q[indeks_res] = -1;
                    --indeks_res;
                }
            }
            ++indeks_res;
        }
        else{
            if( A.modulo_q[finger_A] > B.modulo_q[finger_B] ){
                last_to_copy = last_ele_with_modulo(B,finger_B);
                copy_ele(&Result,B,finger_B,last_to_copy,&indeks_res);
                finger_B = last_to_copy + 1;
            }
            else{
                last_to_copy = last_ele_with_modulo(A,finger_A);
                copy_ele(&Result,A,finger_A,last_to_copy,&indeks_res);
                finger_A = last_to_copy + 1;
            }
        }
    }
    if(finger_A < A.len){
        copy_ele(&Result,A,finger_A,A.len-1,&indeks_res);//copy the rest of elements in A
    }
    if(finger_B < B.len){
        copy_ele(&Result,B,finger_B,B.len-1,&indeks_res);//copy the rest of elements in B
    }
    //tutaj bedzie mialo miejsce przepisanie (preferencyjnie w funkcji)
    zbior_ary Thin_Result = free_unused_memory(&Result);
    return Thin_Result;
}