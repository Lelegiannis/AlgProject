/* SOFTWARE DEVELOPMENT FOR ALGORITHMIC PROBLEMS
   ASSIGNMENT 2
   LELEGIANNIS IOANNIS: 1115201200090
   POULIDIS NIKOLAOS: 1115200000111 */
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

typedef struct ConfParams{
	int k;
	int hashFuncNum;
	int hashTableNum;
	int claransFrac;
	int claransIter;
}ConfParams;

void evalInput(char* inputFilePath)
{
	int size,i,token=0;
	char symbols[100],*line;
    FILE* inputFile;
    
	if((inputFile=fopen(inputFilePath,"r"))==NULL)
	{
		perror("Failed to open input file");
		exit(-1);
	}
	fscanf(inputFile,"%s",symbols);
	if(!strcmp(symbols,"@metric_space"))
	{
		fscanf(inputFile,"%s",symbols);
		if(!strcmp(symbols,"vector"))
		{
		    fscanf(inputFile,"%s",symbols);
		    if(!strcmp(symbols,"@metric"))
		    {
		    	if(!strcmp(symbols,"euclidean"))
		    	{
				    while(!feof(inputFile))
			        {
				        fscanf(inputFile,"%s",symbols);
				        size=strlen(symbols);
				        if((line=malloc((size+1)*sizeof(char)))==NULL)
				        {
					        printf("Error: System failure.\n");
					        return 1;
				        }
				        strcpy(line,symbols);
				        i=size+1;
				        line[i-1]= ;
				        while(getc(inputFile)!='\n')
				        {
						    fscanf(inputFile,"%s",symbols);
				            size=strlen(symbols);
				            if((line=realloc(line,(i+size+1)*sizeof(char)))==NULL)
				            {
				                printf("Error: System failure.\n");
					            return 1;
				            }
				            strcpy(line+i,symbols);
				            i+=size+1;
				            line[i-1]= ;
				        }
				        line[i-1]='\0';
				        euclidean_data_create(line);
				        free(line);
				    }
		    	}
		    	else if(!strcmp(symbols,"cosine"))
		    	{
				    while(!feof(inputFile))
			        {
				        fscanf(inputFile,"%s",symbols);
				        size=strlen(symbols);
				        if((line=malloc((size+1)*sizeof(char)))==NULL)
				        {
					        printf("Error: System failure.\n");
					        return 1;
				        }
				        strcpy(line,symbols);
				        i=size+1;
				        line[i-1]= ;
				        while(getc(inputFile)!='\n')
				        {
						    fscanf(inputFile,"%s",symbols);
				            size=strlen(symbols);
				            if((line=realloc(line,(i+size+1)*sizeof(char)))==NULL)
				            {
				                printf("Error: System failure.\n");
					            return 1;
				            }
				            strcpy(line+i,symbols);
				            i+=size+1;
				            line[i-1]= ;
				        }
				        line[i-1]='\0';
				        cosine_data_create(line);
				        free(line);
				    }
		    	}
		    	else
		    	{
		    		printf("Wrong input file.\n");
		    	}
			}
			else
			{
				while(!feof(inputFile))
			    {
			    	if(!token)
			    	{
			    		token=1;
			    	}
			    	else
			    	{
					    fscanf(inputFile,"%s",symbols);
					}
				    size=strlen(symbols);
				    if((line=malloc((size+1)*sizeof(char)))==NULL)
				    {
					    printf("Error: System failure.\n");
					    return 1;
				    }
				    strcpy(line,symbols);
				    i=size+1;
				    line[i-1]= ;
				    while(getc(inputFile)!='\n')
				    {
						fscanf(inputFile,"%s",symbols);
				        size=strlen(symbols);
				        if((line=realloc(line,(i+size+1)*sizeof(char)))==NULL)
				        {
				            printf("Error: System failure.\n");
					        return 1;
				        }
				        strcpy(line+i,symbols);
				        i+=size+1;
				        line[i-1]= ;
				    }
				    line[i-1]='\0';
				    euclidean_data_create(line);
				    free(line);
				}
			}
		}
		else if(!strcmp(symbols,"hamming"))
		{
			while(!feof(inputFile))
			{
				fscanf(inputFile,"%s",symbols);
				size=strlen(symbols);
				if((line=malloc((size+1)*sizeof(char)))==NULL)
				{
					printf("Error: System failure.\n");
					return 1;
				}
				strcpy(line,symbols);
				i=size;
				line[i]= ;
				fscanf(inputFile,"%s",symbols);
				size=strlen(symbols);
				if((line=realloc(line,(i+size+2)*sizeof(char)))==NULL)
				{
				    printf("Error: System failure.\n");
					return 1;
				}
				strcpy(line+i+1,symbols);
				hamming_data_create(line);
				free(line);
			}
		}
		else if(!strcmp(symbols,"matrix"))
		{
			fscanf(inputFile,"%s",symbols);
			if(!strcmp(symbols,"@items")
			{
			    while(!feof(inputFile))
			    {
				    fscanf(inputFile,"%s",symbols);
				    size=strlen(symbols);
				    if((line=malloc((size+1)*sizeof(char)))==NULL)
				    {
					    printf("Error: System failure.\n");
					    return 1;
				    }
				    strcpy(line,symbols);
				    i=size+1;
				    line[i-1]= ;
				    while(getc(inputFile)!='\n')
				    {
						fscanf(inputFile,"%s",symbols);
				        size=strlen(symbols);
				        if((line=realloc(line,(i+size+1)*sizeof(char)))==NULL)
				        {
				            printf("Error: System failure.\n");
					        return 1;
				        }
				        strcpy(line+i,symbols);
				        i+=size+1;
				        line[i-1]= ;
				    }
				    line[i-1]='\0';
				    matrix_data_create(line);
				    free(line);
				}
			}
			else
			{
				printf("Wrong input file.\n");
			}
		}
		else
		{
			printf("Wrong input file.\n");
		}
	}
	else
	{
		printf("Wrong input file.\n");
	}
	/*find metric*/
	/*read data to temp list*/
	/*create array and put data from list to array*/
	/*destroy list*/
}

ConfParams* evalConf(char* confFilePath,int n)
{
	int k=0,f=0,L=0,s=0,i=0;
	double max;
    FILE* confFile;
    ConfParams *params;
    
	if((confFile=fopen(confFilePath,"r"))==NULL)
	{
		perror("Failed to open configuration file");
		exit(-2);
	}
	if((params=malloc(sizeof(ConfParams)))==NULL)
	{
		perror("Failed to allocate memory for configuration parameters");
		return NULL;
	}
	fscanf(confFile,"number_of_clusters:%d",&k);
	if(!k)
	{
		printf("Wrong configuration file.\n");
		return NULL;
	}
	fscanf(confFile,"number_of_hash_functions:%d",&f);
	if(!f)
	{
	    f=4;
	}
	fscanf(confFile,"number_of_hash_tables:%d",&L);
	if(!L)
	{
		L=5;
	}
	fscanf(confFile,"clarans_set_fraction:%d",&s);
	if(!s)
	{
	    if((max=0.12*k*(n-k))>250)
	    {
	    	s=max;
	    }
	    else
	    {
	    	s=250;
	    }
	}
	fscanf(confFile,"clarans_iterations:%d",&i);
	if(!s)
	{
		i=2;
	}
	params->k=k;
	params->hashFuncNum=f;
	params->hashTableNum=L;
	params->claransFrac=s;
	params->claransIter=i;
	return params;
}

void aplly_kMedoids(int mode){
	/*CLARA*/
	if(mode & 0x8){
		/*CLARA*/
		return;
	}

	/*Initialization*/
	if(mode & 0x4){
		/*Concentrate(Park-Jun)*/
	}
	else{
		/*K-medoids++*/
	}

	/*Assignment*/
	if(mode & 0x2){
		/*LSH/DBH*/
	}
	else{
		/*PAM*/
	}

	/*Update*/
	if(mode & 0x1){
		/*CLARANS*/
	}
	else{
		/*Lloyd's*/
	}
}

void outputResults(char* outFilePath){
	FILE* outFile = fopen(outFilePath,"w");
	if(outFile == NULL){
		perror("Failed to open/create output file");
		exit(-3);
	}
}

int main(int argc,char* argv[]) 
{
	char* outFilePath = NULL;
	int complete = 0;
	ConfParams* confParams = NULL;

	/*Read command line arguements*/
	int i;
	for(i = 1;i < argc;i++){
		if(!strcmp(argv[i],"-d")){
			evalInput(argv[i]);
			/*create distance matrix*/
		}
		else if(!strcmp(argv[i],"-c")){
			confParams = evalConf(argv[i]);
		}
		else if(!strcmp(argv[i],"-o")){
			outFilePath = argv[i];
		}
		else if(!strcmp(argv[i],"-complete")){
			complete = 1;
		}
		else{
			fprintf(stderr,"Invalid parameters given. Call should be:\n")
			fprintf(stderr,"./medoids -d <input file> -c <configuration file> -o <output file>\n");
			return 0;
		}
	}

	if(confParams == NULL){
		fprintf(stderr,"Configuration data were not set.\nTerminating program.\n");
		return 0;
	}
	if(outFilePath == NULL){
		printf("Path for output file was not given.\n");
		printf("Seting default output file 'outFile'\n");
		outFilePath = "./outFile";
	}
	
	for(i=0;i<=9;i++){
		aplly_kMedoids(i);
	}

	outputResults(outFilePath);

	/*destroy distance matrix*/
	return 0;
}
