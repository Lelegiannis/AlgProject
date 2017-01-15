#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#include "Clustering.h"
#include "data.h"
#define LSH_L 5
#define LSH_K 4

/*calculates 2^num*/
unsigned int two_pow(unsigned int num){
	unsigned int i;
	unsigned int retVal = 1;
	for(i=0;i<num;i++) retVal*=2;
	return retVal;
}

static unsigned short neighborsNum;

/*NN-LSH*/
static HashTable* lsh_tables = NULL;
static char m; //metric

void lsh_init(Ratings ratings,char metric)
{
	if(lsh_tables) lsh_terminate();
	lsh_tables = malloc(LSH_L*sizeof(HashTable));
	if(lsh_tables == NULL){
		perror("Failed to allocate memory for lsh tables array");
		return;
	}

	normalizeRatings(ratings,metric);

	if(metric == 'h'){
		unsigned int d = ratings_calculateHammingDim(ratings);
		unsigned int i;
		for(i=0;i<LSH_L;i++){
			HashDescriptor g = hamming_hash_create(d,LSH_K);
			lsh_tables[i] = hashTable_create(two_pow(LSH_K),g);
		}

		User currentUser;
		while(currentUser = getNextUser(ratings)){
			Data data = hammingData_create(currentUser);
			if(data == NULL){
				fprintf(stderr,"lsh initialization cannot continue\n");
				exit(-1);
			}

			for(i=0;i<LSH_L;i++)
				hashTable_insert(lsh_tables[i],data);
		}
	}
	else if(metric == 'e'){
		unsigned int d = ratings_calculateEuclideanDim(ratings);
		unsigned int n = ratings_getNumberOfUsers(ratings);
		unsigned int i;
		for(i=0;i<LSH_L;i++){
			HashDescriptor g = euclidean_hash_create(d,LSH_K,n);
			lsh_tables[i] = hashTable_create(n/2,g);
		}

		User currentUser;
		while(currentUser = getNextUser(ratings)){
			Data data = euclideanData_create(currentUser);
			if(data == NULL){
				fprintf(stderr,"lsh initialization connot continue\n");
				exit(-1);
			}

			for(i=0;i<LSH_L;i++)
				hashTable_insert(lsh_tables[i],data);
		}
	}
	else if(metric == 'c'){
		//euclidean and cosine dims are the same
		unsigned int d = ratings_calculateEuclideanDim(ratings);
		unsigned int i;
		for(i=0;i<LSH_L;i++){
			HashDescriptor g = cosine_hash_create(d,LSH_K);
			lsh_tables[i] = hashTable_create(two_pow(LSH_K),g);
		}

		User currentUser;
		while(currentUser = getNextUser(ratings)){
			Data data = cosineData_create(currentUser);
			if(data == NULL){
				fprintf(stderr,"lsh initialization connot continue\n");
				exit(-1);
			}

			for(i=0;i<LSH_L;i++)
				hashTable_insert(lsh_tables[i],data);
		}
	}
	else{
		fprintf(stderr,"Metric %c is not supported\n",metric);
		exit(-1);
	}

	neighborsNum = ratings_getNeighborsNum(ratings);
	m = metric;
}

unsigned int* lsh_getRecommendedItems(User user){
	unsigned int* retVal = malloc(sizeof(unsigned int)*5);
	if(retVal == NULL){
		perror("Failed to allocate memory for top 5 ratings");
		return NULL;
	}

	Data current;
	if(m == 'h')
		current = hammingData_create(user);
	else if(m == 'e')
		current = euclideanData_create(user);
	else if(m == 'c')
		current = cosineData_create(user);

	
}

void lsh_terminate(){
	unsigned int i;
	Data data;
	for(i=0;i<LSH_L;i++){
		while(data = hashTable_getAll(lsh_tables[i])){
			data_destroy(data);
		}
		hashTable_destroy(lsh_tables[i]);
	}
	free(lsh_tables);
}

void clustering_init(Ratings ratings,char metric)
{
	unsigned int n,i,j,start=2,end=n/2,m,*recommended;
	unsigned short k[2],token=0;
	double s[2],sim,sum=0.0,z=0.0;
	Medoids *medoids[2];
	Assignment *assignment[2];
	User user;
	int8_t *vector,*flags;
	
	normalizeRatings(ratings,metric);
	if((medoids[0]=realloc(NULL,sizeof(Medoids)))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	n=ratings_getNumberOfUsers(ratings);
	k[0]=n/ratings_getNumberOfNeighbors(ratings);
	if((medoids[0]->m=realloc(NULL,k[0]*sizeof(unsigned int)))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	medoids[0]->k=k[0];
	for(i=0;i<=k[0]-1;i++)
	{
		do
		{
		    medoids[0]->m[i]=integerUniform(n)+1;
		    for(j=0;j<=i-1;j++)
		    {
		    	if(medoids[0]->m[i]==medoids[0]->m[j])
		    	{
		    		j=i+1;
		    		break;
		    	}
		    }
		}
		while(j==i+1);
	}
	assignment[0]=PAM(medoids[0],n,metric);
	s[0]=silhouette(metric,n,assignment[0]);
	k[1]=(start+k[0])/2;
	do
	{
	    if((medoids[1]=realloc(NULL,sizeof(Medoids)))==NULL)
	    {
		    printf("Error: System failure.\n");
		    exit(1);
	    }
	    if((medoids[1]->m=realloc(NULL,k[1]*sizeof(unsigned int)))==NULL)
	    {
		    printf("Error: System failure.\n");
		    exit(1);
	    }
	    medoids[1]->k=k[1];
	    for(i=0;i<=k[1]-1;i++)
	    {
		    do
		    {
		        medoids[1]->m[i]=integerUniform(n)+1;
		        for(j=0;j<=i-1;j++)
		        {
		    	    if(medoids[1]->m[i]==medoids[1]->m[j])
		    	    {
		    		    j=i+1;
		    		    break;
		    	    }
		        }
		    }
		    while(j==i+1);
	    }
	    assignment[1]=PAM(medoids[1],n,metric);
	    s[1]=silhouette(metric,n,assignment[1]);
	    if(s[1]>s[0])
	    {
	    	if(!token)
	    	{
			    end=k[0];
			}
			else
			{
				start=k[0];
				token=0;
			}
	    	k[0]=k[1];
	    	k[1]=(start+k[0])/2;
	    	free(medoids[0]->m);
	    	free(medoids[0]);
	    	free(assignment[0]);
	    	medoids[0]=medoids[1];
	    	assignment[0]=assignment[1];
	    }
	    else
	    {
		    free(medoids[1]->m);
	    	free(medoids[1]);
	    	free(assignment[1]);
	    	if(!token)
	    	{
	    	    k[1]=(k[0]+end)/2;
	    	    token=1;
	    	}
	    	else
	    	{
	    		break;
	    	}
	    }
	}
	while(start<=end+1);
	for(i=0;i<=n-1;i++)
	{
		user=ratings_getUser(ratings,i+1);
		vector=user_getRatingsVector(user);
		flags=user_getRatingFlags(user);
		m=ratings_getNumberOfItems(ratings);
		for(j=0;j<=m-1;j++)
		{
			if(flags[j])
			{
				for(start=0;start<=n-1;start++)
				{
					if(assignment[0]->first[i]==assignment[0]->first[start])
					{
						sim=1-user_cosineDistance(i+1,start+1);
						sum+=sim*vector[j];
						if(sim>=0)
						{
							z+=sim;
						}
						else
						{
							z+=-sim;
						}
					}
				}
				z=1/z;
				vector[j]=z*sum;
			}
		}
		recommended=clustering_getRecommendedItems(user);
	}
}

unsigned int* clustering_getRecommendedItems(User user){

}

void clustering_terminate(){

}
