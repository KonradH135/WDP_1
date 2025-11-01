/*
autor:       Konrad Hałaszko
nr. albumu:  479234
zadanie:     Zbiory arytmetyczne
*/

#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>//to be deleted
#include "zbior_ary.h"

int Q;

void initiate_Q(int q){
    Q = q;
}

int positive_modulo(int a,int b){// function returns (a mod b) >= 0
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

zbior_ary create_empty_set(int len_1,int len_2){// creates empty zbior_ary of size (len_1 + len_2)
    zbior_ary Tem;
    Tem.len = len_1 + len_2;
    Tem.modulo_q = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));
    Tem.first_ele = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));
    Tem.last_ele = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));

    // seting modulo_q = -1 helps to identify which memory cells are unused

    for(int i = 0;i < Tem.len;++i){
        Tem.modulo_q[i] = -1;
    }

    return Tem;
}

zbior_ary free_unused_memory(zbior_ary *Result){
    zbior_ary Tem;
    int finger_Tem = 0;
    Tem.len = 0;

    // function calculates the number of used memory cells
    // creates new zbior_ary Tem with that much memory allocated
    // copies all elements from Results to Tem
    // then memory used in Result is free

    for(int i = 0;i < (*Result).len ; ++i){
        if( (*Result).modulo_q[i] != -1){
            ++Tem.len;
        }
    }

    Tem.modulo_q = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));
    Tem.first_ele = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));
    Tem.last_ele = (int *)malloc( (long unsigned int)Tem.len * sizeof(int));

    for(int i = 0; i < (*Result).len; ++i){
        if((*Result).modulo_q[i] != -1){
            Tem.modulo_q[finger_Tem] = (*Result).modulo_q[i];
            Tem.first_ele[finger_Tem] = (*Result).first_ele[i];
            Tem.last_ele[finger_Tem] = (*Result).last_ele[i];
            ++finger_Tem;
        }
    }

    free((*Result).modulo_q);free((*Result).first_ele);free((*Result).last_ele);

    return Tem;
}

void interval(zbior_ary X,int *x_1,int *x_2,int finger){
    // function initiates x_1,x_2, helps to avoid repeated fragments in code
    *x_1 = X.first_ele[finger];
    *x_2 = X.last_ele[finger];
}

void add_interval(zbior_ary *Result,int x_1,int x_2,int y_1,int y_2,int *finger_X,int *finger_Y,int index_res){// it is ensured that x_1 <= y_1

    // the sum of intervals is added to Result

    if( ( ( (long long)x_2 + (long long)Q ) < (long long)y_1 ) || (y_2 <= x_2) ){// disjoint intervals or (y_1,y_2) is contained within (x_1,x_2)
        (*Result).first_ele[index_res] = x_1;
        (*Result).last_ele[index_res] = x_2;
        if(y_2 <= x_2){// (y_1,y_2) contained within (x_1,x_2)
            (*finger_Y)++;
        }
    }   
    else{// ( (x_2 >= y_1 - Q) && (y_2 >= x_2) )// intervals intersect
        (*Result).first_ele[index_res] = x_1;
        (*Result).last_ele[index_res] = y_2;
        (*finger_Y)++;
    }
    (*finger_X)++;
}

int last_ele_with_modulo(zbior_ary X, int finger_X){ // modulo = X.modulo_q[finger_X]

    // function searches for the last element with modulo_q = modulo

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


zbior_ary ciag_arytmetyczny(int a, int q, int b){
    initiate_Q(q);
    zbior_ary Result;

    //  each zbior_ary holds elements satisfying the following conditions:
    //  modulo_q[i] <= modulo_q[i+1], i.e., modulo_q is non-decreasing
    //  if modulo_q[i] == modulo_q[i+1], then first_ele[i] < first_ele[i+1]

    Result.modulo_q = (int *)malloc(sizeof(int));
    Result.first_ele = (int *)malloc(sizeof(int));
    Result.last_ele = (int *)malloc(sizeof(int));

    Result.len = 1;
    Result.modulo_q[0] = positive_modulo(a,Q);
    Result.first_ele[0] = a;
    Result.last_ele[0] = b;

    return Result;
}

zbior_ary singleton(int a){
    return ciag_arytmetyczny(a,Q,a);
}

void copy_ele_sum(zbior_ary *Result,zbior_ary X,int p,int k,int *index_res){//  copies elements from index p to k of set X to Result

    // the function checks whether the last added element to Result has a non-empty intersection with the newly added one
    // meaning, whether the previous interval should be extended instead of adding a new one

    int primary_ele = p;
    if(*index_res > 0){
        // we look for a possible need of connection
        if(((*Result).modulo_q[*index_res - 1] == X.modulo_q[k]) && ((*Result).last_ele[*index_res - 1] >= X.first_ele[k] - Q)){
            (*Result).last_ele[*index_res - 1] = maximum(X.last_ele[k],(*Result).last_ele[*index_res - 1]);
            primary_ele = k + 1;
        }
        else{
            while( ((*Result).modulo_q[*index_res - 1] == X.modulo_q[primary_ele]) && ((*Result).last_ele[*index_res - 1] >= X.first_ele[primary_ele] - Q) ){
                (*Result).last_ele[*index_res - 1] = maximum(X.last_ele[primary_ele],(*Result).last_ele[*index_res - 1]);
                ++primary_ele;
            }
        }
    }

    //we copy elements as we know they do not intersect anymore
    for(int i = primary_ele; i <= k; ++i){
        (*Result).modulo_q[*index_res + i - primary_ele] = X.modulo_q[i];
        (*Result).first_ele[*index_res + i - primary_ele] = X.first_ele[i];
        (*Result).last_ele[*index_res + i - primary_ele] = X.last_ele[i];
    }
    *index_res += (k - primary_ele +1);
}

void addition_the_same_modulo(zbior_ary A,zbior_ary B,zbior_ary *Result, int *finger_A, int *finger_B,int *index_res){
    // the pair of numbers [a_1, a_2] represents an interval containing integers belonging to an arithmetic sequence with difference Q, where a_1 <= a_2
    int a_1,a_2,b_1,b_2;

    interval(A,&a_1,&a_2,*finger_A);// loading the appropriate intervals
    interval(B,&b_1,&b_2,*finger_B);

    // adding the intervals to the Result

    (*Result).modulo_q[*index_res] = A.modulo_q[*finger_A];
    if(a_1 <= b_1){
        add_interval(Result,a_1,a_2,b_1,b_2,finger_A,finger_B,*index_res);
    }
    else{
        add_interval(Result,b_1,b_2,a_1,a_2,finger_B,finger_A,*index_res);
    }

    // eventual extension of the interval at (index_res - 1) by the newly added interval, if they overlap

    if(*index_res - 1 >= 0){// condition for the existence of the previous interval
        if( ((*Result).modulo_q[*index_res-1] == (*Result).modulo_q[*index_res]) && ((*Result).last_ele[*index_res-1] >= minimum(a_1,b_1) - Q) ){
            // equal modulo_q + intervals overlap

            (*Result).last_ele[*index_res-1] = maximum((*Result).last_ele[*index_res],(*Result).last_ele[*index_res-1]);
            (*Result).modulo_q[*index_res] = -1;
            --(*index_res);
        }
    }
    ++(*index_res);
}

zbior_ary suma(zbior_ary A,zbior_ary B){
    zbior_ary Result = create_empty_set(A.len,B.len);
    int finger_A=0,finger_B=0,index_res=0;// fingers are pointers on elements in A,B , index_res - first free memory cell to use

    while( (finger_A < A.len) && (finger_B < B.len) ){ // loop ends when elements of one of the sets run out
        if( A.modulo_q[finger_A] == B.modulo_q[finger_B] ){
            addition_the_same_modulo(A,B,&Result,&finger_A,&finger_B,&index_res);
        }
        else{
            // copying elements from the appropriate zbior_ary to the result, as the modulo_q do not match
            int last_to_copy; //index of last element to copy from A
            if( A.modulo_q[finger_A] > B.modulo_q[finger_B] ){
                last_to_copy = last_ele_with_modulo(B,finger_B);
                copy_ele_sum(&Result,B,finger_B,last_to_copy,&index_res);
                finger_B = last_to_copy + 1;
            }
            else{
                last_to_copy = last_ele_with_modulo(A,finger_A);
                copy_ele_sum(&Result,A,finger_A,last_to_copy,&index_res);
                finger_A = last_to_copy + 1;
            }
        }
    }

    if(finger_A < A.len){
        copy_ele_sum(&Result,A,finger_A,A.len-1,&index_res);// copying the remaining elements in A
    }
    if(finger_B < B.len){
        copy_ele_sum(&Result,B,finger_B,B.len-1,&index_res);// copying the remaining elements in B
    }

    zbior_ary Thin_Result = free_unused_memory(&Result);
    return Thin_Result;
}

void copy_ele_roz(zbior_ary A,zbior_ary *Result, int finger_A, int down_limit,int last_to_copy, int *index_res){
    // elements are being copied to Result from A
    //down_limit is important only when modulo_q is the same as modulo_q of A[finger_A]

    for(int i = finger_A; i <= last_to_copy; ++i){
        if(down_limit <= A.last_ele[i] || A.modulo_q[i] != A.modulo_q[finger_A]){
            (*Result).modulo_q[*index_res] = A.modulo_q[i];
            if(A.modulo_q[i]==A.modulo_q[finger_A]){
                (*Result).first_ele[*index_res] = maximum(A.first_ele[i],down_limit);
            }
            else{
                (*Result).first_ele[*index_res] = A.first_ele[i];
            }
            (*Result).last_ele[*index_res] = A.last_ele[i];
            ++(*index_res);
        }
    }
}

void subtraction_the_same_modulo(zbior_ary A,zbior_ary B,zbior_ary *Result,int *index_res, int *finger_A, int *finger_B, int *down_limit){
    // the pair of numbers [a_1, a_2] represents an interval containing integers belonging to an arithmetic sequence with difference Q, where a_1 <= a_2
    int a_1,a_2,b_1,b_2;

    interval(A,&a_1,&a_2,*finger_A);// loading the appropriate intervals
    interval(B,&b_1,&b_2,*finger_B);

    if( a_2 <= b_2 ){
        if( a_1 < b_1 ){// the interval from A has a piece cut off from the beginning, or nothing is cut off
            (*Result).modulo_q[*index_res] = A.modulo_q[*finger_A];
            (*Result).first_ele[*index_res] = maximum(*down_limit,a_1);
            (*Result).last_ele[*index_res] = minimum(a_2,b_1-Q);
            ++(*index_res);
        }
        ++(*finger_A);
    }
    else{
        if( a_1 < b_1 ){//  the interval from B is contained within the interval from A and cuts out the "middle", the "begining" of A that was not cut off is added to the result
            (*Result).modulo_q[*index_res] = A.modulo_q[*finger_A];
            (*Result).first_ele[*index_res] = maximum(*down_limit,a_1);
            (*Result).last_ele[*index_res] = b_1 - Q;
            ++(*index_res);
        }
        *down_limit = b_2 + Q;
        ++(*finger_B);
    }
}

zbior_ary roznica(zbior_ary A,zbior_ary B){
    zbior_ary Result = create_empty_set(A.len,B.len);
    int finger_A = 0,finger_B = 0,index_res = 0,down_limit = INT_MIN;

    // We consider intervals in ascending order of modulo_q, then the first elements of the intervals
    // down_limit is defined for the modulo_q as the end of the last considered cutting interval (from B) plus Q, i.e., the smallest legal value
    // for intervals from A not yet considered

    while( (finger_A < A.len) && (finger_B < B.len) ){
        if(A.modulo_q[finger_A] == B.modulo_q[finger_B]){
            subtraction_the_same_modulo(A,B,&Result,&index_res,&finger_A,&finger_B,&down_limit);
        }
        else{
            int last_to_copy;
            if(A.modulo_q[finger_A] < B.modulo_q[finger_B]){
                // we copy elements from A with the given modulo_q because no element from B will cut them off anymore

                last_to_copy = last_ele_with_modulo(A,finger_A);
                copy_ele_roz(A,&Result,finger_A,down_limit,last_to_copy,&index_res);
                finger_A = last_to_copy + 1;
            }
            else{
                ++finger_B;
            }
            down_limit = INT_MIN;
        }
    }

    if(finger_A < A.len){// copying the rest of the elements of A to the result, considering down_limit
        int last_to_copy = A.len-1;
        copy_ele_roz(A,&Result,finger_A,down_limit,last_to_copy,&index_res);
    }
    
    zbior_ary Thin_Result = free_unused_memory(&Result);
    return Thin_Result;
}

// function calulates the intersection of two intervals
void product_the_same_modulo(zbior_ary A, zbior_ary B, zbior_ary *Result, int *finger_A, int *finger_B, int *index_res){
    // the pair of numbers [a_1, a_2] represents an interval containing integers belonging to an arithmetic sequence with difference Q, where a_1 <= a_2
    int a_1,a_2,b_1,b_2;

    interval(A,&a_1,&a_2,*finger_A);// loading the appropriate intervals
    interval(B,&b_1,&b_2,*finger_B);

    if(a_2 <= b_2){
        if(b_1 <= a_2){
            (*Result).modulo_q[*index_res] = A.modulo_q[*finger_A];
            (*Result).first_ele[*index_res] = maximum(a_1,b_1);
            (*Result).last_ele[*index_res] = a_2;
            ++(*index_res);
        }
        ++(*finger_A);
    }
    else{
        if(a_1 <= b_2){
            (*Result).modulo_q[*index_res] = A.modulo_q[*finger_A];
            (*Result).first_ele[*index_res] = maximum(a_1,b_1);
            (*Result).last_ele[*index_res] = b_2;
            ++(*index_res);
        }
        ++(*finger_B);
    }
}

zbior_ary iloczyn(zbior_ary A,zbior_ary B){
    zbior_ary Result = create_empty_set(A.len,B.len);

    int finger_A = 0,finger_B = 0,index_res = 0;

    // iloczyn is a simpler version of the roznica 
    // what would be cut off from A by B is added to the result
    // 
    while( (finger_A < A.len) && (finger_B < B.len) ){
        if(A.modulo_q[finger_A] == B.modulo_q[finger_B]){
            product_the_same_modulo(A,B,&Result,&finger_A,&finger_B,&index_res);
        }
        else{
            if(A.modulo_q[finger_A] < B.modulo_q[finger_B]){
                ++finger_A;
            }
            else{
                ++finger_B;
            }
        }
    }
    zbior_ary Thin_Result = free_unused_memory(&Result);
    return Thin_Result;
}


unsigned ary(zbior_ary A){
    // number is calculated automatically with other functions
    return (unsigned)A.len;
}

unsigned moc(zbior_ary A){
    long long res = 0ll,tem;
    for(int i=0;i<A.len;++i){
        // funcion calculates the number of elements in arithmetic series for all arithmetic series
        tem = (long long)(((long long)A.last_ele[i]-(long long)A.first_ele[i])/(long long)Q + 1ll);
        res+=tem;
    }
    return (unsigned)res;
}

bool nalezy(zbior_ary A, int b){

    int q=positive_modulo(b,Q);
    int p=0,k=A.len-1,mid;

    // classic bin-search with a slight modification
    // first, we look for the correct modulo_q
    // then, for the interval where the element could be found

    while(p<k){
        mid=(p+k+1)/2;
        if(A.modulo_q[mid] < q){// too small q
            p = mid + 1;
        }
        else{
            if(A.modulo_q[mid] > q){// too large q
                k = mid - 1;
            }
            else{// correct q
                // we look for the largest (i): A.first_ele[i] <= b
                if(A.first_ele[mid] > b){
                    k = mid - 1;
                }
                else{
                    p = mid;
                }
            }
        }
    }

    // checking if something was found

    if(p == k){
        if( (A.modulo_q[p] == q) && (A.first_ele[p] <= b) && (A.last_ele[p] >= b) ){
            return true;
        }
    }
    
    return false;
}

// functions below are NOT part of the solution, they are a testing tool

void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void print(zbior_ary A){
    int MOC=moc(A);
    int* tab=(int *)malloc(sizeof(int*)*(unsigned long long)MOC);
    int indeks = 0;
    for(int i=0;i<A.len;++i){
        for(int j=A.first_ele[i];j<=A.last_ele[i];j+=Q){
            tab[indeks]=j;
            indeks++;
        }
    }
    sort(tab,MOC);
    for(int i=0;i<MOC;++i){
        printf("%d ",tab[i]);
    }
    printf("\n");
}