/*
	Taksinomhsh Selection Sort p > 2
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	Syleopoulos Anastasios
*/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h> // sleep

#define _MAXNUMBER 10000 /* megistos arithmos stoixeiou */

/************** FUNCS ****************/
// gemizei pinaka me thn rand()
void FilltheArray(int *pin, int N){  
	int i;

	for(i=0;i<N;i++)
		pin[i]=rand()%_MAXNUMBER;
}
// ektupwnei ton pinaka
void Myprint(int *pin, int N, char *str){
	int i;
	
	printf("\n%s\n",str);	
	for(i=0;i<N;i++)
		printf("%d |",pin[i]);	
	printf("\n");
}

/***************** MAIN **********************/
int main(int argc,char *argv[]){
	int myid, numprocs;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	
	int i,j;  // deiktes gia for
	int temp; // metavliti gia thn taksinomhsh
	int N, part; // N: megethos pinaka | part: megethos kommatiou
	int *thesi;  // pinakas deiktwn
	int *A, *As; // A: mh-taksinomhmenos pinakas | As: taksinomhmenos pinakas
	int *Aw;     // pinakas megethous part opou tha efarmostei o selection sort
	double ta,tt; // ta: arxh xronou | tt: telos xronou

	/********** MASTER ***********/
	if(myid == 0){
		printf("Selection Sort: Parallhlo programma gia p > 2\n");
		printf("Plhktrologiste to megethos tou pinaka: ");
		scanf("%d",&N);

		A = malloc(N*sizeof(int));
		As = malloc(N*sizeof(int));
		if(A==NULL || As==NULL){
			printf("\nAdunamia desmeushs mnhmhs!\nTelos programmatos!\n");
			exit(1);
		}
		FilltheArray(A,N); // gemisma pinaka me stoixeia
		
		part = N/numprocs;  // upologismos kommatiou
		if(N%numprocs!=0){
			printf("\nO arithmos komvwn(numprocs) den einai pollaplasio tou megethous(N) tou pinaka!\nTelos programmatos!\n");
			exit(1);
		}
		printf("O arithmos komvwn(numprocs): %d To megethos(N): %d To kommati(part): %d\n",numprocs,N,part);		
			
		ta = MPI_Wtime(); // arxizei h xronometrish		

		thesi=malloc(numprocs*sizeof(int));
		if(thesi==NULL){
			printf("\nAdunamia desmeushs mnhmhs!\nTelos programmatos!\n");
			exit(1);
		}
		for(i=0;i<numprocs;i++)  // Anathesh timwn ston pinaka(thesi) me thn arxikh thesh tou stoixeiou kathe komvou
			thesi[i]=i*part;
		
		//Myprint(A,N,"Mh-taksinomhmenos pinakas(A)");  // ektypwsh A	
	}
	
	MPI_Bcast(&part,1,MPI_INT,0,MPI_COMM_WORLD); // Apostolh se olous tous komvous to part
	Aw=malloc(part*sizeof(int));	             // Desmeush mnhmhs megethous part gia ton pinaka Aw  
	MPI_Scatter(&A[0],part,MPI_INT,&Aw[0],part,MPI_INT,0,MPI_COMM_WORLD); // Diamoirash tou mh-taksinomhmenou pinaka(A) stous pinakes Aw
/*

	// Ektypwsh twn mh-taksinomimenwn kommatiwn apo kathe komvo
	char name1[51];
	sleep(myid*1);
	sprintf(name1,"~Komvos %d -> not-sorted pinakas(Aw)",myid);
	Myprint(Aw,part,name1);
	sleep(numprocs-myid);
*/

/************ Selection Sort *************/ 
 
	//Efagmogh ths taksinomishs ston pinaka Aw
	for(i=0;i<part;i++){
		for(j=i+1;j<part;j++){
			if(Aw[i]>Aw[j]){
				temp=Aw[i];
				Aw[i]=Aw[j];
				Aw[j]=temp;
			}
		}
	}

/****************************************/

/*
	// Ektypwsh twn taksinomimenwn kommatiwn apo kathe komvo
	if(myid==0)
		printf("\n******** SELECTION SORT DONE ********\n");
	char name2[51];
	sleep(myid*1);
	sprintf(name2,">Komvos %d -> sorted pinakas(Aw)",myid);
	Myprint(Aw,part,name2);
	sleep(1);	
*/

	MPI_Gather(&Aw[0],part,MPI_INT,&A[0],part,MPI_INT,0,MPI_COMM_WORLD); // syllogh stoixeiwn apo tous Aw kai apothukeush ston pinaka A
		
	// Enwsh kommatiwn kai oristikh taksinomhsh tou pinaka
	/********** MASTER ***********/
	if(myid==0){
		//Myprint(A,N,"O pinakas(A) meta thn GATHER");  // ektypwsh A meta thn gather
		//Myprint(thesi,numprocs,"pinakas deiktwn(thesi)"); // ektypwsh thesi
		int tp;  // tp: deikths thesis komvou
		int check;  // anathesh timis pou endexetai na einai h mikroterh 
		int done; // elegxos an exei ginei eyresh tou mikroterou (done=1)

		/* 
	 	   Gia kathe thesi tou taksinomhmenou pinaka(As) vriskei 
	       to mikrotero stoixeio tou pinaka Aw tou kathe komvou. 
		   Anathetei thn timh ston pinaka(As) kai auksanei ton deikth 
	       thesis apo ton komvo pou vrike to stoixeio kata ena.  
		*/ 
	
		for(i=0;i<N;i++){  
			tp=0;
			done=0;
			while(done!=1){     
				if(thesi[tp]<(tp+1)*part){  
					check = thesi[tp];
					for(j=tp+1;j<numprocs;j++){
						if(A[check]>A[thesi[j]] && thesi[j] < (j+1)*part){
							check = thesi[j];
							tp=j;
						}
					}
				done=1;								
				}else{
					tp++;
				}
			}
			As[i]=A[check];
			thesi[tp]++;
		}
	
		tt = MPI_Wtime(); // telos xronometrisis
		printf("\nO xronos ekteleshs: %.16f\n",tt-ta);
		//Myprint(As,N,"O taksinomhmenos pinakas(As)");	
	}
	
	MPI_Finalize();
	return 0;
}