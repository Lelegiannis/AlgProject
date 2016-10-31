#ifndef HASH_GEN_H
#define HASH_GEN_H
#include <stdint.h>

typedef struct HashDesc *HashDescriptor;

typedef struct EuclideanData{
	int id;
} EuclideanData;

/*FOR ALL METRICS*/
int hash_apply(HashDescriptor hd,void* x);
/*Aplly function described by HashDescriptor on element x and return the ouput*/


/*FOR HAMMING METRIC*/
HashDescriptor hamming_hash_create(int d,int k);
/*Creates a hamming function g as described by theory, return NULL on failure*/

int hamming_is_equal(HashDescriptor a,HashDescriptor b);
/*if HashDescritor function a is the same as HashDescriptor function b returns 1 else returns 0*/

void hamming_hash_destroy(HashDescriptor hd);
/*Destory a hamming HashDescriptor and make it NULL*/


/*FOR EUCLIDEAN METRIC*/
HashDescriptor euclidean_hash_create(int d,int k,int n);
/*Creates an euclidean function g as described by theory, NULL on failure
  n: number of total elements in query. Used to determine TableSize. See theory for more info*/

void euclidean_hash_destroy(HashDescriptor hd);
/*Destroy an euclidean HashDescriptor and make it NULL*/


/*FOR COSINE METRIC*/
HashDescriptor cosine_hash_create();
/*Creates a cosine function g as described by theory, NULL on failure*/

void cosine_hash_destroy(HashDescriptor hd);
/*Destroy a cosine HashDescriptor and make it NULL*/
#endif
