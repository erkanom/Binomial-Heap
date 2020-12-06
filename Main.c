
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int emax=0;
int MainTime=0;
int DeltaWt=0;
int whichNodeChange;

#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

typedef double ElementType;
#define Infinity (6000000000L)



#define MaxTrees (12)   /* Stores 2^9 -1 items */
#define Capacity (4095)

//  #define MaxTrees (14)   /* Stores 2^14 -1 items */
//  #define Capacity (16383)

//	#define MaxTrees (30)   /* Stores 2^30 -1 items */
//  #define Capacity (1073741823)

struct BinNode;
struct NodeInfo;
typedef struct BinNode *BinTree;
struct Collection;
typedef struct Collection *BinQueue;

BinQueue Initialize( void );
void Destroy( BinQueue H );
BinQueue MakeEmpty( BinQueue H );
BinQueue Insert(struct NodeInfo* Info , BinQueue H ,int time);
ElementType DeleteMin( BinQueue H , int q);
BinQueue Merge( BinQueue H1, BinQueue H2 );
int FindMin( BinQueue H );
int IsEmpty( BinQueue H );
int IsFull( BinQueue H );















struct NodeInfo{
    int name;
    int work;
    int workBackup;
    int time;
    int WT;
    struct BinNode* next;
   double pri;
   int alreadyInHeap;
  int done;
  int workLeft;
  int HowManyInsert;
  int FirstShowUp;
  int SamePriorityFactor;
};

double newPri(struct NodeInfo* Info ,int emax,int q){
 
    double newPri=0;
    double CalculationPri=0;

  CalculationPri =1.0/(exp(-(pow(((2.0*Info->workLeft)/(3.0*emax)),3.0))));
newPri=(Info->workLeft)*CalculationPri;

return newPri;
}







/* START: fig6_52.txt */
typedef struct BinNode *Position;

struct BinNode
{
    ElementType priority;

   struct NodeInfo* next;

    Position    LeftChild;
    Position    NextSibling;
};

struct Collection
{
    int CurrentSize;
    BinTree TheTrees[ MaxTrees ];
};

BinQueue
Initialize( void )
{
    BinQueue H;
    int i;

    H = malloc( sizeof( struct Collection ) );
    if( H == NULL )
        FatalError( "Out of space!!!" );
    H->CurrentSize = 0;
    for( i = 0; i < MaxTrees; i++ )
        H->TheTrees[ i ] = NULL;
    return H;
}

static void
DestroyTree( BinTree T )
{
    if( T != NULL )
    {
        DestroyTree( T->LeftChild );
        DestroyTree( T->NextSibling );
        free( T );
    }
}

void
Destroy( BinQueue H )
{
    int i;

    for( i = 0; i < MaxTrees; i++ )
        DestroyTree( H->TheTrees[ i ] );
}

BinQueue
MakeEmpty( BinQueue H )
{
    int i;

    Destroy( H );
    for( i = 0; i < MaxTrees; i++ )
        H->TheTrees[ i ] = NULL;
    H->CurrentSize = 0;

    return H;
}

/* Not optimized for O(1) amortized performance */
BinQueue
Insert( struct NodeInfo * Info , /*model_node_type m,*/ BinQueue H ,int time)
{
    BinTree NewNode;
    BinQueue OneItem;
    int i;

    NewNode = malloc( sizeof( struct BinNode ) );
    if( NewNode == NULL )
        FatalError( "Out of space!!!" );
    NewNode->LeftChild = NewNode->NextSibling = NULL;
    NewNode->priority =Info->pri ;
   NewNode->next=Info;
   Info->next=NewNode;
   Info->alreadyInHeap=1;
   if(Info->HowManyInsert==0)
       Info->FirstShowUp=time;
   Info->HowManyInsert++;






   /*NewNode->mn.p = m.p;
    NewNode->mn.sid.scalar = m.sid.scalar;
    NewNode->mn.aid.d3act = m.aid.d3act;
    NewNode->mn.nexts.scalar = m.nexts.scalar;
    NewNode->mn.reward = m.reward;
    NewNode->mn.last=m.last;
    for (i=0; i < max_act_const; i++) {
      NewNode->mn.from[i].s=m.from[i].s;
      NewNode->mn.from[i].a=m.from[i].a;
    }*/

    OneItem = Initialize( );
    OneItem->CurrentSize = 1;
    OneItem->TheTrees[ 0 ] = NewNode;

    return Merge( H, OneItem );
}

/* START: fig6_56.txt */
ElementType
DeleteMin( BinQueue H ,int q  ) {
    int i, j, k = 0;
    int MinTree;   /* The tree with the minimum priority */
    BinQueue DeletedQueue;
    Position DeletedTree, OldRoot;
    ElementType MinItem;
    //model_node_type MinItem;

    if (IsEmpty(H)) {
        Error("Empty binomial queue");
        MinItem = -Infinity;

        //  for (i=0; i < max_act_const; i++) {
        //    MinItem.from[i].s=-1;
        //    MinItem->mn.from[i].a=m.from[i].a;
        //  }
        return MinItem;
    }

    MinItem = Infinity;
    for (i = 0; i < MaxTrees; i++) {
        if (H->TheTrees[i] &&
            H->TheTrees[i]->priority < MinItem) {
            /* Update minimum */
            MinItem = H->TheTrees[i]->priority;

            MinTree = i;
        }
    }




    DeletedTree = H->TheTrees[ MinTree ];

    OldRoot = DeletedTree;
    DeletedTree = DeletedTree->LeftChild;






   OldRoot->next->alreadyInHeap=0;
    struct NodeInfo *tempInfo =OldRoot->next;
   if(OldRoot->next->workLeft==q){
       DeltaWt=q;
       whichNodeChange=OldRoot->next->name;
       OldRoot->next->alreadyInHeap=0;
       OldRoot->next->done=1;
       OldRoot->next->workLeft=0;
 MainTime+=q;
  } else
    if(OldRoot->next->workLeft>q){
        DeltaWt=q;
        whichNodeChange=OldRoot->next->name;
        OldRoot->next->workLeft-=q;
        OldRoot->next->alreadyInHeap=0;
        OldRoot->next->pri=newPri(OldRoot->next, emax,q);

       k=1;
       MainTime+=q;
    } else
    if(OldRoot->next->workLeft<q){
      DeltaWt=OldRoot->next->workLeft;
        whichNodeChange=OldRoot->next->name;
        MainTime+=OldRoot->next->workLeft;
        OldRoot->next->workLeft=0;
        OldRoot->next->done=1;
        OldRoot->next->alreadyInHeap=0;


    }


    free( OldRoot );


    DeletedQueue = Initialize( );
    DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
    for( j = MinTree - 1; j >= 0; j-- )
    {
        DeletedQueue->TheTrees[ j ] = DeletedTree;
        DeletedTree = DeletedTree->NextSibling;
        DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
    }

    H->TheTrees[ MinTree ] = NULL;
    H->CurrentSize -= DeletedQueue->CurrentSize + 1;

    Merge( H, DeletedQueue );
    if(k)
        Insert(tempInfo,H,MainTime);
    return MinItem;

}
/* END */

int
 FindMin( BinQueue H )
{
    int i;
    ElementType MinItem;

    if( IsEmpty( H ) )
    {
        Error( "Empty binomial queue" );
        MinItem=0;
        return MinItem;
    }

    MinItem = Infinity;
int x=-1;
    for( i = 0; i < MaxTrees; i++ )
    {
        if( H->TheTrees[ i ] &&
            H->TheTrees[ i ]->priority < MinItem ) {
            MinItem = H->TheTrees[ i ]->priority;
            x=H->TheTrees[i]->next->name;
        }
    }

    return x;
}

int
IsEmpty( BinQueue H )
{
    return H->CurrentSize == 0;
}

int IsFull( BinQueue H )
{
    return H->CurrentSize == Capacity;
}

/* START: fig6_54.txt */
/* Return the result of merging equal-sized T1 and T2 */
BinTree
CombineTrees( BinTree T1, BinTree T2 )
{
    if( T1->priority > T2->priority )
        return CombineTrees( T2, T1 );
    T2->NextSibling = T1->LeftChild;
    T1->LeftChild = T2;
    return T1;
}
/* END */

/* START: fig6_55.txt */
/* Merge two binomial queues */
/* Not optimized for early termination */
/* H1 contains merged result */

BinQueue
Merge( BinQueue H1, BinQueue H2 )
{
    BinTree T1, T2, Carry = NULL;
    int i, j;

    if( H1->CurrentSize + H2->CurrentSize > Capacity )
        Error( "Merge would exceed capacity" );

    H1->CurrentSize += H2->CurrentSize;
    for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
    {
        T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];

        switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
        {
            case 0: /* No trees */
            case 1: /* Only H1 */
                break;
            case 2: /* Only H2 */
                H1->TheTrees[ i ] = T2;
                H2->TheTrees[ i ] = NULL;
                break;
            case 4: /* Only Carry */
                H1->TheTrees[ i ] = Carry;
                Carry = NULL;
                break;
            case 3: /* H1 and H2 */
                Carry = CombineTrees( T1, T2 );
                H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                break;
            case 5: /* H1 and Carry */
                Carry = CombineTrees( T1, Carry );
                H1->TheTrees[ i ] = NULL;
                break;
            case 6: /* H2 and Carry */
                Carry = CombineTrees( T2, Carry );
                H2->TheTrees[ i ] = NULL;
                break;
            case 7: /* All three */
                H1->TheTrees[ i ] = Carry;
                Carry = CombineTrees( T1, T2 );
                H2->TheTrees[ i ] = NULL;
                break;
        }
    }
    return H1;
}

// by Borahan Tumer
BinTree printTree(BinTree p, BinTree *r, int i)
{
    BinTree t[20]={NULL}, q; int j;
    for ( j=0; j<i; j++ ) t[j]= r[j];
    i=0;
    if (p!=NULL) {
        printf("& %2.1lf ",p->priority);
        q=p->NextSibling;
        j=0;
        do {
            while (q!=NULL) {
                printf("%2.1lf ",q->priority);
                if (q->LeftChild != NULL) { r[i]=q->LeftChild; i++; }
                q=q->NextSibling;
            }
            q=t[j++];
        } while (q!=NULL);
    }
    else return NULL;
    //for (j=0; j<i; j++) t[j]=NULL;
    printf("\n");
    printTree(p->LeftChild, r, i);
}


//	/*
main()
{
    BinQueue H1, H2;
    BinTree p, r[20]={NULL};
    ElementType priority;
    char ch;
    int i;
    H1 = Initialize( );
    FILE *fp = fopen("./input.txt","r");
    FILE *fp2 = fopen("./input.txt","r");
    char term[20];
    char term1[20];
    int numberOfNode=0;
    int numberOfNode2=0;
    while( fgets(term, 100, fp) != NULL )
    {




        if(term[0]=='P'){

            numberOfNode++;
        }





    }


    fclose(fp);


struct NodeInfo Infos[numberOfNode];




    while( fgets(term1, 100, fp2) != NULL )
    {




if(term1[0]=='P'){

    char * token = strtok(term1, " ");
    // loop through the string to extract all other tokens
    int i = 0 ;
    while( token != NULL ) {
       if(i==0){
           Infos[numberOfNode2].name=numberOfNode2+1;
           Infos[numberOfNode2].SamePriorityFactor=0;
           Infos[numberOfNode2].WT=0;
           Infos[numberOfNode2].done=0;
           Infos[numberOfNode2].HowManyInsert=0;
           Infos[numberOfNode2].alreadyInHeap=0;
       }


        if(i==1){
            Infos[numberOfNode2].work=atoi(token);
            Infos[numberOfNode2].workBackup= Infos[numberOfNode2].work;
            Infos[numberOfNode2].pri=Infos[numberOfNode2].work;
            Infos[numberOfNode2].workLeft=Infos[numberOfNode2].work;
        }


        if(i==2)

            Infos[numberOfNode2].time=atoi(token);




        i++;
        token = strtok(NULL, " ");
    }

    numberOfNode2++;
}





    }

    fclose(fp2);
int m;

for(m=1;m<11;m++) {
    int q =m;
    int j;
    for (j = 0; j < numberOfNode; ++j) {
        if (Infos[j].work > emax)
            emax = Infos[j].work;
    }

    int control = 1;
    while (control) {
        int j;
        for (j = 0; j < numberOfNode; ++j) {
            control *= Infos[j].done;
        }
        if (control == 0)
            control++;
        else break;
        for (j = 0; j < numberOfNode; ++j) {
            if (Infos[j].time <= MainTime && Infos[j].done == 0 && Infos[j].work - Infos[j].workLeft == 0 &&
                Infos[j].alreadyInHeap == 0)
                Insert(&Infos[j], H1, MainTime);
        }

     int nextElementReal=FindMin(H1);
        int NextElementFake=-1;
       int Node1;
        int Node2;
        for ( Node1 = 0; Node1 <numberOfNode ; ++Node1) {
            for (Node2 = 0;Node2  <numberOfNode ; Node2++) {

               if(Infos[Node1].alreadyInHeap==1&&Infos[Node2].alreadyInHeap==1)
                   if(Infos[Node1].pri==Infos[Node2].pri)
                       if(Infos[Node1].name==nextElementReal||Infos[Node2].name==nextElementReal){

                           if(Infos[Node1].time>Infos[Node2].time&&nextElementReal==Infos[Node1].name){
                               struct BinNode * node1=Infos[Node1].next;
                               struct BinNode * node2=Infos[Node2].next;

                               Infos[Node1].next->next=&Infos[Node2];
                               Infos[Node2].next->next=&Infos[Node1];

                               Infos[Node1].next=node2;
                               Infos[Node2].next=node1;

                           }

                          /* if(Infos[Node2].time>Infos[Node1].time&&nextElementReal==Infos[Node2].name){
                               struct BinNode * node1=Infos[Node1].next;
                               struct BinNode * node2=Infos[Node2].next;

                               Infos[Node1].next->next=&Infos[Node2];
                               Infos[Node2].next->next=&Infos[Node1];

                               Infos[Node2].next=node1;
                               Infos[Node1].next=node2;

                           }

*/

                       }




            }
        }











        DeleteMin(H1, q);



        for (j = 0; j < numberOfNode; ++j) {
            if (Infos[j].alreadyInHeap == 1 && Infos[j].name != whichNodeChange)
                Infos[j].WT += DeltaWt;

        }


    }

    for (j = 0; j < numberOfNode; ++j) {
        if (Infos[j].FirstShowUp > Infos[j].time)
            Infos[j].WT += Infos[j].FirstShowUp - Infos[j].time;

    }
double TotalWT=0;

    printf("%s %d\n", "**************** q----->",m);
    for (j = 0; j < numberOfNode; ++j) {
TotalWT+=Infos[j].WT;
        printf("%s%d   %d\n","P", Infos[j].name,Infos[j].WT);

    }
printf("\nAWT = %lf / %d  = %lf\n",TotalWT,numberOfNode,(TotalWT/numberOfNode));

    for (j = 0; j < numberOfNode; ++j) {
        Infos[j].work = Infos[j].workBackup;
        Infos[j].FirstShowUp = 0;
        Infos[j].HowManyInsert = 0;
        Infos[j].alreadyInHeap = 0;
        Infos[j].done = 0;
        Infos[j].pri =Infos[j].work ;
        Infos[j].WT = 0;
        Infos[j].workLeft = Infos[j].work;

    }
     MainTime=0;
    DeltaWt=0;
    emax=0;

}









}
