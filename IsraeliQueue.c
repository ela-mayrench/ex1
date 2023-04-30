#include "IsraeliQueue.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_FRIENDS 5
#define MAX_ENEMIES 3
#define FRIEND 1 
#define NUTRAL 0 //if two people are not friends and not enemies
#define ENEMY (-1)


typedef struct PersonLine_t{
    int id;
    int friends_num;
    int enemy_num;
    struct PersonLine_t* next;
}*PersonLine;

struct IsraeliQueue_t {
    FriendshipFunction* friendship_func;
    ComparisonFunction compare_func;
    int friendship_th;
    int rivalry_th;
    PersonLine person;
};
/*
//declaring new funcs :
PersonLine PersonLineCreate(int id);
void PersonLineDestroy(PersonLine line);
int CountFuncs(FriendshipFunction * friendship_func);
FriendshipFunction* CopyFuncArray(FriendshipFunction * friendship_func);
int AreFriends(PersonLine* person1, PersonLine* person2, FriendshipFunction * friendship_func, int friendship_th, int rivalry_th);
void AddToSpot(void * new_person, PersonLine* spot);
IsraeliQueue FindPersonInQueue(IsraeliQueue queue, void * person1);
PersonLine BuildBackwardsLine(IsraeliQueue queue);
PersonLine RemoveFromQueue(IsraeliQueue queue, PersonLine person1);
//end of declaration
*/

IsraeliQueue IsraeliQueueCreate(FriendshipFunction * friendship_func, ComparisonFunction compare_func, int friendship_th, int rivalry_th){
    IsraeliQueue queue = (IsraeliQueue)malloc(sizeof(IsraeliQueue));
    if (queue == NULL){
        return NULL;
    }
    queue->friendship_func = (FriendshipFunction*)malloc(sizeof(FriendshipFunction*));
 
    queue->friendship_func = friendship_func;
    queue->compare_func = compare_func;
    queue->friendship_th = friendship_th;
    queue->rivalry_th = rivalry_th;
    queue->person = NULL;

    return queue;
}

PersonLine PersonLineCreate(int id){
    PersonLine person = (PersonLine)malloc(sizeof(PersonLine*));
    person->id = id;
    person->friends_num=0;
    person->enemy_num=0;
    person->next = NULL;
    
    return person;
}

void PersonLineDestroy(PersonLine line){
    while(line!=NULL){
        PersonLine temp = line;
        line=line->next;
        free(temp->next);
        free(temp);
    }
}

//returns the length of the friendship_func array
int CountFuncs(FriendshipFunction * friendship_func){
    int i=0;
    for (; friendship_func[i]!=NULL; i++);
    return i+1;
}


FriendshipFunction* CopyFuncArray(FriendshipFunction * friendship_func){
    int length = CountFuncs(friendship_func);
    FriendshipFunction * new_array = malloc(sizeof(FriendshipFunction)*length);
    if (new_array==NULL){
        return NULL;
    }
    for(int i=0 ; i<length ; i++){
        new_array[i] = friendship_func[i];
    }
    return new_array;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q){
    FriendshipFunction* new_array = CopyFuncArray(q->friendship_func);
    IsraeliQueue new_queue = IsraeliQueueCreate(new_array, q->compare_func, q->friendship_th, q->rivalry_th);
    PersonLine original_tmp = q->person;
    PersonLine new_person=PersonLineCreate(original_tmp->id);
    new_person->friends_num=original_tmp->friends_num; //new person points at the head of the cloned person line
    new_person->enemy_num=original_tmp->enemy_num;
    PersonLine new_tmp = new_person; //new temp points at the latest created person in the line
    original_tmp=original_tmp->next;
    while(original_tmp!=NULL){
        PersonLine created_person = PersonLineCreate(original_tmp->id);
        created_person->friends_num=original_tmp->friends_num; //new person points at the head of the cloned person line
        created_person->enemy_num=original_tmp->enemy_num;
        new_tmp->next = created_person; 
        new_tmp=new_tmp->next;
        original_tmp=original_tmp->next;
    }
    new_queue->person=new_person;
    new_person=NULL;
    original_tmp=NULL;
    new_tmp=NULL;
    return new_queue;
}

void IsraeliQueueDestroy(IsraeliQueue queue){
    free(queue->friendship_func);
    PersonLineDestroy(queue->person);
    free(queue);
}

int AreFriends(PersonLine person1, PersonLine person2, FriendshipFunction * friendship_func, int friendship_th, int rivalry_th){
    int func_num = 0;
    int sum = 0; //sums the friendship values of each friendship function
    int i=0;
    while (friendship_func[i] != NULL){
        if (friendship_func[i](person1,person2)>friendship_th){ //if there is one func that says they're friends, end.
            return FRIEND;
        }
        else {
            sum+=friendship_func[i](person1,person2);
            func_num++;
        }
        i++; //continue to next func
    }
    if (sum/func_num<rivalry_th){
        return ENEMY;
    }
    else {
        return NUTRAL;
    }
    
}

//AddToSpot - adds person to line after person in "spot"
void AddToSpot(PersonLine new_person, PersonLine spot){
    PersonLine temp = spot->next;
    spot->next = new_person;
    new_person->next = temp;
    temp=NULL;
}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void * new_person){
    //if the line is empty, put new person in first place
    if(queue->person==NULL){
        queue->person = new_person;
        return ISRAELIQUEUE_SUCCESS;
    }

    //else
    PersonLine temp = queue->person; //the person i'm comparing friendship with
    PersonLine spot;
    while(temp->next!=NULL){
        int relationship = AreFriends(temp ,new_person,queue->friendship_func, queue->friendship_th, queue->rivalry_th);
        bool found_friend = false;
        spot = NULL;
        //find first friend & check if the friend has spots available
        if (relationship == FRIEND && temp->friends_num<MAX_FRIENDS && !found_friend){
            spot = temp; //potential person to put person after
            found_friend = true;
        }
        else if (relationship == NUTRAL || temp->friends_num>=MAX_FRIENDS || found_friend){
            temp = temp->next;
            continue;
        }
        //find next enemy
        else if (relationship == ENEMY){
            if(temp->enemy_num < MAX_ENEMIES){ //enemy can block
                found_friend = false;
                temp->enemy_num++;
                spot = NULL;
            }
            else{
                temp = temp->next;
                continue;
            }
        }
        temp = temp->next;
    }
    if(temp->next == NULL && AreFriends(temp ,new_person,queue->friendship_func, queue->friendship_th, queue->rivalry_th) == FRIEND){
        if(temp->friends_num<MAX_FRIENDS){
            temp->friends_num++;
            temp->next = new_person;
        }
        else{
            temp->next = new_person;
        }

    }
    else{
        if(spot==NULL){
            temp->next = new_person;
            temp=NULL;
            spot=NULL;
            return ISRAELIQUEUE_SUCCESS;
        }
        else{
            AddToSpot(spot,new_person);
            temp=NULL;
            spot=NULL;
            return ISRAELIQUEUE_SUCCESS;
        }
    }
    temp=NULL;
    spot=NULL;
    return ISRAELI_QUEUE_ERROR;
}

IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction func){
    int length=0;//friendship function arr length
    while(queue->friendship_func!=NULL){
        length++;
    }
    FriendshipFunction* new_array = malloc(sizeof(FriendshipFunction*)*length+1);
    if(new_array==NULL){
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    for(int i=0; i<length; i++){
        new_array[i]=queue->friendship_func[i];
    }
    new_array[length]=func;
    free(queue->friendship_func);
    queue->friendship_func = new_array;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queue, int new_threshold){
    queue->friendship_th=new_threshold;
    return ISRAELIQUEUE_SUCCESS;
}

IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue queue, int new_threshold){
    queue->rivalry_th=new_threshold;
    return ISRAELIQUEUE_SUCCESS;
}

int IsraeliQueueSize(IsraeliQueue queue){
    PersonLine temp = queue->person;
    int length=0;
    while(temp->next!= NULL){
        length++;
        temp=temp->next;
    }
    temp=NULL;
    return length;
}

void* IsraeliQueueDequeue(IsraeliQueue queue){
    if (queue == NULL || queue->person == NULL){
        return NULL;
    }
    PersonLine temp = queue->person;
    temp->next = NULL;    
    queue->person = queue->person->next;
    //the person is out of the line so its friends and enemy count becomes 0
    temp->friends_num=0;
    temp->enemy_num=0;
    return temp;
    
}

//finds the location of a person in queue, if not exists returns null
PersonLine FindPersonInQueue(IsraeliQueue queue, PersonLine person1){
    if (queue == NULL || queue->person == NULL || person1 == NULL){
        return NULL;
    }
    PersonLine temp = queue->person; //temp cmopares the person to the queue
    while(temp != NULL){
        //if person is the same as temp
        if(queue->compare_func(person1, temp)){
            return temp; //the similar person pointer
        }
        //if person not the same as temp
        temp=temp->next;  //try to compare to the next person in queue
    }
    //person is not in queue
    temp=NULL;
    return NULL;
}

bool IsraeliQueueContains(IsraeliQueue queue, void * person1){
    if(FindPersonInQueue(queue,person1)==NULL){
        return false;
    }
    else
        return true;
}

//build backwards queue
PersonLine BuildBackwardsLine(IsraeliQueue queue){
    IsraeliQueue new_queue = IsraeliQueueClone(queue);
    PersonLine backwards_line;
    PersonLine temp = IsraeliQueueDequeue(new_queue);
    while (new_queue->person != NULL){
        backwards_line = temp;
        temp = IsraeliQueueDequeue(new_queue);
        temp->next=backwards_line;
    }
    backwards_line = temp;
    temp=NULL;
    IsraeliQueueDestroy(new_queue);
    return backwards_line;
}

//accepts the queue and a pointer to person (not in queue) to remove
PersonLine RemoveFromQueue(IsraeliQueue queue, PersonLine person1){
    PersonLine spot = FindPersonInQueue(queue,person1); //points at the person to remove location
    if(spot != NULL){
        PersonLine temp = spot->next; //save the pointer to after spot
        spot->next = NULL;
        PersonLine before = queue->person; //pointer to find the person before the person to remove, found in loop
        while(!queue->compare_func(before->next,spot)){
            before=before->next;
        }
        before->next=temp;
    }
    //before=NULL;
    return spot;
}

IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue queue){
   PersonLine backwards_line = BuildBackwardsLine(queue);
   while(backwards_line != NULL){
        PersonLine improve_p = IsraeliQueueDequeue(backwards_line);
        PersonLine removed = RemoveFromQueue(queue,improve_p);
        IsraeliQueueError error = IsraeliQueueEnqueue(queue,removed);
        if(error != ISRAELIQUEUE_SUCCESS){
            return error;
        }
   }
   return ISRAELIQUEUE_SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////
int Func0(PersonLine person1,PersonLine person2){
    return 1;
}

int Func1(PersonLine person1,PersonLine person2){
    return 10;
}

int Func2(PersonLine person1,PersonLine person2){
    return 29;
}

int Func3(PersonLine person1,PersonLine person2){
    return 6;
}

int compare_func(PersonLine person1 ,PersonLine person2){
    if(person1->id == person2->id){
        return 1;
    }
    return 0;
}

int main(){
    int length = 4;
    FriendshipFunction** friendship_func=malloc(sizeof(FriendshipFunction)*length);
    friendship_func[0]=Func0;
    friendship_func[1]=Func1;
    friendship_func[2]=Func2;
    friendship_func[3]=Func3;

    int friendship_th = 10;
    int rivalry_th = 3;

    IsraeliQueue queue = IsraeliQueueCreate(friendship_func,compare_func,friendship_th, rivalry_th);
    PersonLine line = PersonLineCreate(123456);
    IsraeliQueueEnqueue(queue,line);
    PersonLine line2 = PersonLineCreate(123457);
    IsraeliQueueEnqueue(queue,line2);
    PersonLine line3 = PersonLineCreate(123458);
    IsraeliQueueEnqueue(queue,line3);
    return 0;
}