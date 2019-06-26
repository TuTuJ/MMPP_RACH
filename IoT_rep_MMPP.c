#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "IoT1.h"



double ACB_parameter = 1;
double e = 2.71828;

void free_list(int i) {
    list ptr;
    ptr = UE_list[i];
    while (ptr!=NULL) {
        UE_list[i]=ptr->next;
        free(ptr);
        ptr = UE_list[i];
    }
    UE_list[i]=NULL;
}

void free_listA(int i) {
    listA ptrA;
    ptrA = UE_listA[i];
    while (ptrA!=NULL) {
        UE_listA[i]=ptrA->next_A;
        free(ptrA);
        ptrA = UE_listA[i];
    }
    UE_listA[i]=NULL;
}

void free_listB(int i) {
    listB ptrB;
    ptrB = UE_listB[i];
    while (ptrB!=NULL) {
        UE_listB[i]=ptrB->next_B;
        free(ptrB);
        ptrB = UE_listB[i];
    }
    UE_listB[i]=NULL;
}

void insert_list(int i, int preamble, int tries, int atime, int level)
{
    list ptr;

    ptr = (list) malloc(sizeof(list_type));
    if (ptr==NULL){
        printf("PANIC: No free memory\n"); exit(1);
    }
    else {
        ptr->preamble=preamble;
        ptr->tries=tries;
        ptr->atime=atime;
        ptr->level=level;
        ptr->next=NULL;
        ptr->prev=NULL;
    }

    if (UE_list[i]==NULL) {
        UE_list[i]=ptr;
    }
    else {
        ptr->next = UE_list[i];
        UE_list[i]->prev = ptr;
        UE_list[i]=ptr;
    }
    if (preamble>=0)
        Preamble[preamble] = ptr;
}

void insert_listA(int i, int preamble, int tries, int atime, int level)
{
    listA ptrA;

    ptrA = (listA) malloc(sizeof(listA_type));
    if (ptrA==NULL){
        printf("PANIC: No free memory\n"); exit(1);
    }
    else {
        ptrA->preambleA=preamble;
        ptrA->tries_A=tries;
        ptrA->atime_A=atime;
        ptrA->level_A=level;
        ptrA->next_A=NULL;
        ptrA->prev_A=NULL;
    }

    if (UE_listA[i]==NULL) {
        UE_listA[i]=ptrA;
    }
    else {
        ptrA->next_A = UE_listA[i];
        UE_listA[i]->prev_A = ptrA;
        UE_listA[i]=ptrA;
    }
    if (preamble>=0)
        PreambleA[preamble] = ptrA;
}

void insert_listB(int i, int preamble, int tries, int atime, int level)
{
    listB ptrB;

    ptrB = (listB) malloc(sizeof(listB_type));
    if (ptrB==NULL){
        printf("PANIC: No free memory\n"); exit(1);
    }
    else {
        ptrB->preambleB=preamble;
        ptrB->tries_B=tries;
        ptrB->atime_B=atime;
        ptrB->level_B=level;
        ptrB->next_B=NULL;
        ptrB->prev_B=NULL;
    }

    if (UE_listB[i]==NULL) {
        UE_listB[i]=ptrB;
    }
    else {
        ptrB->next_B = UE_listB[i];
        UE_listB[i]->prev_B = ptrB;
        UE_listB[i]=ptrB;
    }
    if (preamble>=0)
        PreambleB[preamble] = ptrB;
}

void delete_list(int i, list ptr)
{
    list pptr, nptr;

    if (ptr==NULL) {
        printf("PANIC: list element does not exist when deleting\n");
        printf("level=%d\n",i);
        exit(1);
    }
    else {
        pptr = ptr->prev;
        nptr = ptr->next;
        if (pptr==NULL) {
            /* list head */
            UE_list[i]=nptr;
            if (nptr!=NULL)
                nptr->prev=NULL;
        }
        else if (nptr==NULL) {
            /* list tail */
            pptr->next=NULL; /* we already check that pptr is not NULL */
        }
        else {
            pptr->next = nptr;
            nptr->prev = pptr;
        }
    }
    free(ptr);
}

void delete_listA(int i, listA ptrA)
{
    listA pptr, nptr;

    if (ptrA==NULL) {
        printf("PANIC: A list element does not exist when deleting\n");
        printf("level=%d\n",i);
        exit(1);
    }
    else {
        pptr = ptrA->prev_A;
        nptr = ptrA->next_A;
        if (pptr==NULL) {
            /* list head */
            UE_listA[i]=nptr;
            if (nptr!=NULL)
                nptr->prev_A=NULL;
        }
        else if (nptr==NULL) {
            /* list tail */
            pptr->next_A=NULL; /* we already check that pptr is not NULL */
        }
        else {
            pptr->next_A = nptr;
            nptr->prev_A = pptr;
        }
    }
    free(ptrA);
}

void delete_listB(int i, listB ptrB)
{
    listB pptr, nptr;

    if (ptrB==NULL) {
        printf("PANIC: B list element does not exist when deleting\n");
        printf("level=%d\n",i);
        exit(1);
    }
    else {
        pptr = ptrB->prev_B;
        nptr = ptrB->next_B;
        if (pptr==NULL) {
            /* list head */
            UE_listB[i]=nptr;
            if (nptr!=NULL)
                nptr->prev_B=NULL;
        }
        else if (nptr==NULL) {
            /* list tail */
            pptr->next_B=NULL; /* we already check that pptr is not NULL */
        }
        else {
            pptr->next_B = nptr;
            nptr->prev_B = pptr;
        }
    }
    free(ptrB);
}


double uniform()          /* return double prec. r.n. uniform in [0,1] */
{
  long ix = seed;
  long k = ix/127773;
  ix = 16807*(ix - k*127773) - k*2836;
  if (ix < 0) ix += 2147483647;
  seed = ix;
  return((double) ix * 4.656612875e-10);
}

double expo(float rate)
{
   double x;

   x = uniform();
   return( -log(x)/rate);
}

int Poisson(float rate, double etime)
{
    int n=0;
    double tsum;

    tsum=expo(rate);
    while (tsum<etime) {
        n++;
        tsum+=expo(rate);
    }
    return n;

}

int ACB(int device){      
		double out_device = 0;
		double x;
		int g;
		for(g = 0; g < device; g++){
			if(uniform() <= ACB_parameter){
			out_device++;
			}	
		}
		return out_device;
	}
	
	//Proposed ACB
	double Proposed_ACB(double a, double b){
		
		if(a > 1 || b > 1){
			ACB_parameter = (1/((a+b)/2));
//			ACB_parameter = 0.1;
		}else{
			ACB_parameter = 1;
		}
		return ACB_parameter;
	}

void main()
{
    int  i,j,h, N;
    int MMPP_state[3];
	float MMPP_time[3];
    int  clock;         /* system time */
    int  stop;          /* simulation time */
    int  success=0;     /* number of successful transmission */
  	int successA, successB;
    int leftover=0;
    int left_A=0, left_B=0;
  	float lambda[3][2];  // ON-OFF source or MMPP with 2 states
    float transrate[3][2]; //MMPP transition rate 0->1 and 1->0
    int Preamble_Number[3]={48, 24, 24};
    int Preamble_count[48];
    int PreambleA_count[48], PreambleB_count[48];
    int total_Preamble, dy_preambleA, dy_preambleB;
    int preambleA, preambleB;
    int device_Emergency, device_Regular;
    int preamble_choose;
    int preambleA_choose, preambleB_choose;
    int emptyA,emptyB;
    int tmp_deviceA, tmp_deviceB; 
    int deviceA_barred, deviceB_barred;
    double x0, y0;
    double lambda_Emergency, lambda_Regular;
    double lambda_Emergency_1, lambda_Regular_1;
   	double Pb_Em, Pb_Reg;
	double get_lambda_Em, get_lambda_Reg;
	double get_Pb_Em, get_Pb_Reg;
	double Pb_G_Em = 0.01, Pb_G_Reg = 0.1;
    float sum_success[3]={0};   // successful rate
    float sum_success1[3]={0};  // number of success of one level
    float sum_success2[3]={0};  // number of success of cross level
    float sum_delay[3]={0};
    
    float sum_success1A[3]={0};  // number of success of one level
    float sum_success2A[3]={0};  // number of success of cross level
    float sum_delayA[3]={0};
    
    float sum_success1B[3]={0};  // number of success of one level
    float sum_success2B[3]={0};  // number of success of cross level
    float sum_delayB[3]={0};
    
    float sum_throughput[3]={0};
    float sum_arrival[3]={0};
    int no_arrival[3]={0};
    long total_arrival[3]={0L};
    int rep_no[3];
    long max_try[3]={0L};
    int atime;
    int level, olevel;
    list ptr, next_ptr;
    listA ptrA, next_ptrA;
    listB ptrB, next_ptrB;
    int run, nruns;
    float cv, width;
    float tmp1;
    double tmp;
    double sum_avg_delay[3]={0};
    double vsum_avg_delay[3]={0};
    double sum_avg_success_rach[3]={0};
    double sum_avg_success_ue[3]={0};
    double sum_avg_success_uea[3]={0};
    double sum_avg_throughput[3]={0};
    double vsum_avg_success_rach[3]={0};
    double vsum_avg_success_ue[3]={0};
    double vsum_avg_success_uea[3]={0};
    double vsum_avg_throughput[3]={0};
    double sum_avg_arrival[3]={0};
    double vsum_avg_arrival[3]={0};

    /* initialize */
    printf("Input the seed: ");
    scanf("%ld",&seed);
    printf("Input the simulation time (slots): ");
    scanf("%d",&stop);
    printf("Input number of runs: ");
    scanf("%d",&nruns);
    for (i=0; i<3; i++) {
        printf("Input arrival rate of CE level %d: ", i);
        scanf("%f",&lambda[i][0]);
        scanf("%f",&lambda[i][1]);
        printf("Input transition rate of CE level %d: ", i);
	    scanf("%f",&transrate[i][0]);
        scanf("%f",&transrate[i][1]);
        printf("Maximum tries: ");
        scanf("%d",&rep_no[i]);
    }


    for (run=0; run<nruns; run++) {
        /* Start the system simulation */

        for (i=0; i<3; i++) {
            no_arrival[i]=0;
            total_arrival[i]=0;
            max_try[i]=0;
            sum_delay[i]=0;
            sum_success[i]=0;
            sum_success1[i]=0;
            sum_success2[i]=0;
            sum_delayA[i]=0;
            sum_success1A[i]=0;
            sum_success2A[i]=0;
            sum_delayB[i]=0;
            sum_success1B[i]=0;
            sum_success2B[i]=0;
            sum_throughput[i]=0;
            sum_arrival[i]=0;
            free_list(i);
            free_listA(i);
            free_listB(i);
            MMPP_state[i]=0;
            MMPP_time[i]=expo(transrate[i][0]);
        }
        
		//ORIGINAL Method & ACB_Scheme////////////////////////////////
        for (clock=1; clock<(stop+1); clock++) {  //each clock is 40ms
            for (level=0; level<3; level++) {
            
				ACB_parameter = (0.7 +0.6*uniform());  // Add Other paper ACB parameter
                if (((level==1)&&((clock%2)!=0))||((level==2)&&((clock%3)!=0)))
                    continue;

                for (i=0; i<48; i++) {
                    Preamble[i]=NULL;
                    Preamble_count[i]=0;
                }
                leftover=0;
 				if (clock>MMPP_time[level]) {
                    MMPP_state[level] = (MMPP_state[level]+1) % 2;
                    MMPP_time[level] = clock +expo(transrate[level][MMPP_state[level]]);
                }
                
                /* Leftover UEs re-choose preamble */
                ptr=UE_list[level];
                while (ptr!=NULL) {
                    next_ptr = ptr->next;   //we might delete ptr, so save this first
                    ptr->tries++;
                    if (ptr->tries > rep_no[level]) {
                        if (level==2)
                            max_try[ptr->level]++;

                        /* Change CE level to level+1 */
                        if (level<2) {
                            insert_list(level+1, -1, 0, ptr->atime, ptr->level);
                            total_arrival[level+1]++;
                        }
                        delete_list(level, ptr);
                    }
                    else {
                        ptr->preamble = (int) floor(uniform()*Preamble_Number[level]);
                        Preamble_count[ptr->preamble]++;
                        Preamble[ptr->preamble] = ptr;
                        leftover++;
                    }
                    ptr=next_ptr;
                }
                /* new arrival */
                N=Poisson(lambda[level][MMPP_state[level]], (double)(level+1.0));
                
				N = ACB(N);
                //N=Poisson(lambda[level], 1.0);
                total_arrival[level]+=N;
                sum_arrival[level] += (N+leftover);
                for (i=0; i<N; i++) {
                    preamble_choose=(int) floor(uniform()*Preamble_Number[level]);
                    insert_list(level, preamble_choose, 1, clock, level);
                    Preamble_count[preamble_choose]++;
                }

                /* check which UE transmits successfully */
                success=0;
                for (i=0; i<Preamble_Number[level]; i++) {
                    if (Preamble_count[i]==1) {
                        success++;
                        sum_success1[level]++;
                        sum_success2[Preamble[i]->level]++;
                        sum_delay[Preamble[i]->level] += (((float)clock)-Preamble[i]->atime);
                        delete_list(level, Preamble[i]);
                    }
                }
                if ((N+leftover)>0)
                    sum_success[level]+=(float)success/(N+leftover);
                else
                    no_arrival[level]++;

                sum_throughput[level]+= (float)success;
            }
        }
		//ORIGINAL Method & ACB_Scheme////////////////////////////////
		
		//Proposed Method////////////////////////////////
//        for (clock=1; clock<(stop+1); clock++) {  //each clock is 40ms
//            for (level=0; level<3; level++) {
//                if (((level==1)&&((clock%2)!=0))||((level==2)&&((clock%3)!=0)))
//                    continue;
//
//                for (i=0; i<48; i++) {
//                    Preamble[i]=NULL;
//                    Preamble_count[i]=0;
//                    PreambleA[i]=NULL;
//                    PreambleB[i]=NULL;
//                    PreambleA_count[i]=0;
//                    PreambleB_count[i]=0;
//                }
//                leftover=0;
//                left_A =0;
//                left_B=0;
//                
//				if (clock>MMPP_time[level]) {
//                    MMPP_state[level] = (MMPP_state[level]+1) % 2;
//                    MMPP_time[level] = clock +expo(transrate[level][MMPP_state[level]]);
//                } 
//                
//				total_Preamble = Preamble_Number[level];
//				preambleA = total_Preamble/2;
//				preambleB = total_Preamble/2;
//				dy_preambleA = preambleA;
//				dy_preambleB = preambleB;
//
//			     /* Leftover UEs re-choose preamble */
//                ptrA=UE_listA[level];
//                ptrB=UE_listB[level];
//                
//                while (ptrA!=NULL) {
//                    next_ptrA = ptrA->next_A;   //we might delete ptr, so save this first
//                    ptrA->tries_A++;
//                    if (ptrA->tries_A > rep_no[level]) {
//                        if (level==2)
//                        max_try[ptrA->level_A]++;
//
//                        /* Change CE level to level+1 */
//                        if (level<2) {
//                            insert_listA(level+1, -1, 0, ptrA->atime_A, ptrA->level_A);
//                            total_arrival[level+1]++;
//                        }
//                        delete_listA(level, ptrA);
//                    }
//                    else {
//                        ptrA->preambleA = (int) floor(uniform()*dy_preambleA);
//                        PreambleA_count[ptrA->preambleA]++;
//                        PreambleA[ptrA->preambleA] = ptrA;
//                        left_A++;
//                    }
//                    ptrA=next_ptrA;
//                }
//                
//                while (ptrB!=NULL) {
//                    next_ptrB = ptrB->next_B;   //we might delete ptr, so save this first
//                    ptrB->tries_B++;
//                    if (ptrB->tries_B > rep_no[level]) {
//                        if (level==2)
//                        max_try[ptrB->level_B]++;
//                       
//                        /* Change CE level to level+1 */
//                        if (level<2) {
//                            insert_listB(level+1, -1, 0, ptrB->atime_B, ptrB->level_B);
//                            total_arrival[level+1]++;
//                        }
//                        delete_listB(level, ptrB);
//                    }
//                    else {
//                        ptrB->preambleB = (int) floor(uniform()*dy_preambleB);
//                        PreambleB_count[ptrB->preambleB]++;
//                        PreambleB[ptrB->preambleB] = ptrB;
//                        left_B++;
//                    }
//                    ptrB=next_ptrB;
//                }
//                /* new arrival */
//                N=Poisson(lambda[level][MMPP_state[level]], (double)(level+1.0));
//          		N = ACB(N);  
//                total_arrival[level]+=N;
//                sum_arrival[level] += (N+left_A+left_B);
//     
//     			//Partial incoming devices into two types 
//                if(N%2 == 0){
//               	 device_Emergency = N/2;
//               	 device_Regular = N/2;	
//				}else{
//				 device_Emergency = (N/2) + 1;
//               	 device_Regular = N/2;	
//				}
//
//                
//                for (i=0; i<device_Emergency; i++) {
//                    preambleA_choose=(int) floor(uniform()*dy_preambleA);
//                    insert_listA(level, preambleA_choose, 1, clock, level);
//                    PreambleA_count[preambleA_choose]++;
//                }
//                
//                for (i=0; i<device_Regular; i++) {
//                    preambleB_choose=(int) floor(uniform()*dy_preambleB);
//                    insert_listB(level, preambleB_choose, 1, clock, level);
//                    PreambleB_count[preambleB_choose]++;
//                }
//
//                /* check which UE transmits successfully */
//                success=0;
//                successA=0;
//                successB=0;
//                emptyA =0;
//                emptyB=0;
//                for (i=0; i<dy_preambleA; i++) {
//                    if (PreambleA_count[i]==1) {
//                        successA++;
//                        sum_success1A[level]++;
//                        sum_success2A[PreambleA[i]->level_A]++;
//                        sum_delayA[PreambleA[i]->level_A] += (((float)clock)-PreambleA[i]->atime_A);
//                        delete_listA(level, PreambleA[i]);
//                    }
//                    if(PreambleA_count[i]==0){
//                    	emptyA++;
//					}
//                }
//                 for (i=0; i<dy_preambleB; i++) {
//                    if (PreambleB_count[i]==1) {
//                        successB++;
//                        sum_success1B[level]++;
//                        sum_success2B[PreambleB[i]->level_B]++;
//                        sum_delayB[PreambleB[i]->level_B] += (((float)clock)-PreambleB[i]->atime_B);
//                        delete_listB(level, PreambleB[i]);
//                    }
//                    if(PreambleB_count[i]==0){
//                    	emptyB++;
//					}
//                }
//                
//                success = (successA + successB);
//                leftover = (left_A+left_B);
//
//                if ((N+leftover)>0)
//                    sum_success[level]+=(float)success/(leftover+N);
//                else
//                    no_arrival[level]++;
//
//                sum_throughput[level]+= (float)success;
//                
//                
//            // Do partial preamble allocation start//////////////////////////////////////////////////
//      	    x0 = (double)emptyA/dy_preambleA;
//      		y0 = (double)emptyB/dy_preambleB;
//			lambda_Emergency_1 = -log(x0);
//			lambda_Regular_1 = -log(y0);
//			Pb_Em = 0;
//			Pb_Reg = 0;
//			
//			Pb_Em = 1-pow(e,-lambda_Emergency_1)-lambda_Emergency_1*pow(e,-lambda_Emergency_1);
//			Pb_Reg = 1-pow(e,-lambda_Regular_1)-lambda_Regular_1*pow(e,-lambda_Regular_1);
//	
//		
//			if(Pb_Em <= Pb_G_Em && Pb_Reg <= Pb_G_Reg){
//				for(h = 1 ; h <= total_Preamble; h++){
//					get_lambda_Em = (lambda_Emergency_1*preambleA)/(h);
//					get_lambda_Reg = (lambda_Regular_1*preambleB)/(h);
//					get_Pb_Em = 1 - pow(e,-get_lambda_Em) - (get_lambda_Em*pow(e,-get_lambda_Em));
//					get_Pb_Reg = 1 - pow(e,-get_lambda_Reg) - (get_lambda_Em*pow(e,-get_lambda_Reg));
//					if(get_Pb_Em > Pb_G_Em || get_Pb_Reg > Pb_G_Reg){
//						dy_preambleA = preambleA;
//						dy_preambleB = total_Preamble - dy_preambleA;
//						break;
//					}
//				}
//
//			}else{
//				for(h=total_Preamble; h > 1; h--){
//					get_lambda_Em = (lambda_Emergency_1*preambleA)/(h);
//					get_lambda_Reg = (lambda_Regular_1*preambleB)/(h);
//					get_Pb_Em = 1 - pow(e,-get_lambda_Em) - (get_lambda_Em*pow(e,-get_lambda_Em));
//					get_Pb_Reg = 1 - pow(e,-get_lambda_Reg) - (get_lambda_Em*pow(e,-get_lambda_Reg));
//					if(get_Pb_Em <= Pb_G_Em && get_Pb_Reg <= Pb_G_Reg){
//						dy_preambleA = h;
//						dy_preambleB = total_Preamble - h;
//						break;
//					}
//					
//				}
//		
//			}
//		
//			// Do partial preamble allocation end/////////////////////////////////////////////////////////////////
//                
//            Proposed_ACB(lambda_Emergency_1, lambda_Regular_1);  // use dynamic ACB
//                
//                
//            }
//        }
		//Proposed Method////////////////////////////////
		
		
		

        printf("Finish run %d\n", run);
        for (level=0; level<3; level++) {
        	
        	//For Proposed
//        	sum_success1[level] = sum_success1A[level] + sum_success1B[level];
//        	sum_success2[level] = sum_success2A[level] + sum_success2B[level];
//        	sum_delay[level] = sum_delayA[level] + sum_delayB[level];
        	//For Proposed
        	
            tmp = (1.0+level)*sum_success[level]/(stop-no_arrival[level]);
            sum_avg_success_rach[level] += tmp;
            vsum_avg_success_rach[level] += tmp*tmp;

            tmp = sum_success1[level]/total_arrival[level];
            sum_avg_success_ue[level] += tmp;
            vsum_avg_success_ue[level] += tmp*tmp;

            tmp = (1.0+level)*sum_throughput[level]/(stop-no_arrival[level]);
            sum_avg_throughput[level] += tmp;
            vsum_avg_throughput[level] += tmp*tmp;

            tmp = (1.0+level)*sum_arrival[level]/stop;
            sum_avg_arrival[level] += tmp;
            vsum_avg_arrival[level] += tmp*tmp;

            tmp = sum_delay[level]/sum_success2[level];
            sum_avg_delay[level] += tmp;
            vsum_avg_delay[level] += tmp*tmp;

            tmp = sum_success2[level]/total_arrival[level];
            tmp = (double) sum_success2[level]/ (sum_success2[level]+max_try[level]);
            sum_avg_success_uea[level] += tmp;
            vsum_avg_success_uea[level] += tmp*tmp;
        }

    }
    

    /* print out results */
    cv = t_dist[nruns-1];

    for (level=0; level<3; level++) {

        sum_avg_arrival[level] = sum_avg_arrival[level] / nruns;
        vsum_avg_arrival[level] = (vsum_avg_arrival[level]-(nruns*sum_avg_arrival[level]*sum_avg_arrival[level]))/(nruns-1);
        width = cv * sqrt(vsum_avg_arrival[level]) / sqrt((double)nruns);
        printf("average arrival of CE level %d per 40 ms is %f +- %f\n",level, sum_avg_arrival[level],width);

        sum_avg_throughput[level] = sum_avg_throughput[level] / nruns;
        vsum_avg_throughput[level] = (vsum_avg_throughput[level]-(nruns*sum_avg_throughput[level]*sum_avg_throughput[level]))/(nruns-1);
        width = cv * sqrt(vsum_avg_throughput[level]) / sqrt((double)nruns);
        printf("average throughput of CE level %d is %f +- %f\n",level, sum_avg_throughput[level],width);

        sum_avg_success_rach[level] = sum_avg_success_rach[level] / nruns;
        vsum_avg_success_rach[level] = (vsum_avg_success_rach[level]-(nruns*sum_avg_success_rach[level]*sum_avg_success_rach[level]))/(nruns-1);
        width = cv * sqrt(vsum_avg_success_rach[level]) / sqrt((double)nruns);
        printf("average successful probability of CE level %d of each RACH cycle is %f +- %f\n",level, sum_avg_success_rach[level],width);

        sum_avg_success_ue[level] = sum_avg_success_ue[level] / nruns;
        vsum_avg_success_ue[level] = (vsum_avg_success_ue[level]-(nruns*sum_avg_success_ue[level]*sum_avg_success_ue[level]))/(nruns-1);
        width = cv * sqrt(vsum_avg_success_ue[level]) / sqrt((double)nruns);
        printf("average successful probability of CE level %d of UEs is %f +- %f\n",level, sum_avg_success_ue[level],width);

        sum_avg_success_uea[level] = sum_avg_success_uea[level] / nruns;
        vsum_avg_success_uea[level] = (vsum_avg_success_uea[level]-(nruns*sum_avg_success_uea[level]*sum_avg_success_uea[level]))/(nruns-1);
        width = cv * sqrt(vsum_avg_success_uea[level]) / sqrt((double)nruns);
        printf("Overall average successful probability of CE level %d of UEs is %f +- %f\n",level, sum_avg_success_uea[level],width);

        sum_avg_delay[level] = sum_avg_delay[level] / nruns;
        vsum_avg_delay[level] = (vsum_avg_delay[level]-(nruns*sum_avg_delay[level]*sum_avg_delay[level]))/(nruns-1);
        width = cv * sqrt(vsum_avg_delay[level]) / sqrt((double)nruns);
        printf("average delay of CE level %d is %f *40ms +- %f\n",level, sum_avg_delay[level],width);
    }


}
