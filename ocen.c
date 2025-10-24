#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "zbior_ary.h"
#include <limits.h>

void maluj(zbior_ary X){
  for(int i=0;i<X.len;++i){
    printf("poczatek = %d,koniec = %d,roznica = %d\n",X.first_ele[i],X.second_ele[i],X.modulo_q[i]);
  }
  printf("%d",X.len);
  printf("\n");
}

int main() {
  // A = {2, 7, 12}
  int __q=2;
  zbior_ary A = ciag_arytmetyczny(2, __q, 4);
  maluj(A);
  //assert(moc(A) == 3);
  //assert(ary(A) == 1);
  // B = {4, 9, 14, 19}
  zbior_ary B = ciag_arytmetyczny(11, __q, 23);
  maluj(B);
  // C = {1}
  zbior_ary C = ciag_arytmetyczny(32, __q, 42);
  maluj(C);
  // D = {1, 4, 9, 14, 19}
  zbior_ary D = suma(B, C);
  maluj(D);
  // E = {1, 2, 4, 7, 9, 12, 14, 19}
  zbior_ary E = suma(D, A);
  maluj(E);
  //assert(moc(E) == 8);
  // F = {14}
  zbior_ary F = ciag_arytmetyczny(-50, __q , -4);
  maluj(F);
  zbior_ary G = ciag_arytmetyczny(5, __q, 23);
  maluj(G);
  zbior_ary H = ciag_arytmetyczny(25, __q,39);
  maluj(H);
  zbior_ary I = suma(F,G);
  maluj(I);
  zbior_ary J = suma(H,I);
  maluj(J);
  zbior_ary K = suma(E,J);
  maluj(K);
  zbior_ary M = roznica(K,B);
  maluj(M); 
  // G = {1, 2, 4, 7, 9, 12, 19}
  //zbior_ary G = roznica(E, F);
  //assert(ary(G) == 4);
  //assert(nalezy(G, 4));
  //assert(!nalezy(G, 5));
  // H = {4, 9, 19};
  //zbior_ary H = iloczyn(G, B);
  //assert(moc(H) == 3);
  //assert(ary(H) == 2);
  // I = {}
  //zbior_ary I = iloczyn(H, A);
  //assert(moc(I) == 0);
  return 0;
}
