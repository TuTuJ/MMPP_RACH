#define   Min(a,b)  (((a)<(b))?(a):(b))
#define   Max(a,b)  (((a)>(b))?(a):(b))

#define MaxN 10

/* list */
typedef struct list_struct {
    int preamble; /*chosen preamble */
    int tries;    /* number of tries */
    int atime;    /* arrival time */
    int level;    /* original level */
    struct list_struct *next, *prev;
} list_type, *list;
list UE_list[3]={NULL}; /* CE level 0, 1, 2 */
list Preamble[48]={NULL};

typedef struct listA_struct {
    int preambleA; /*chosen preamble */
    int tries_A;  /* number of tries */
    int atime_A;    /* arrival time */
    int level_A;    /* original level */
    struct listA_struct *next_A, *prev_A;
} listA_type, *listA;
listA UE_listA[3]={NULL}; /* CE level 0, 1, 2 */
listA PreambleA[48]={NULL};

typedef struct listB_struct {
    int preambleB; /*chosen preamble */
    int tries_B;  /* number of tries */
    int atime_B;    /* arrival time */
    int level_B;    /* original level */
    struct listB_struct *next_B, *prev_B;
} listB_type, *listB;
listB UE_listB[3]={NULL}; /* CE level 0, 1, 2 */
listB PreambleB[48]={NULL};



long  seed;               /* seed for r.v. */
/* t-distribution table */
float t_dist[31] = {10,
12.706,4.303,3.182,2.776,2.571,2.447,2.365,2.306,2.262,2.228,
2.201,2.179,2.160,2.145,2.131,2.120,2.110,2.101,2.093,2.086,
2.080,2.074,2.069,2.064,2.060,2.056,2.052,2.048,2.045,2.042};

