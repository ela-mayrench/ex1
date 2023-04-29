#include "IsraeliQueue.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_FRIENDS 5
#define MAX_ENEMIES 3
#define FRIEND 1 
#define NUTRAL 0 //if two people are not friends and not enemies
#define ENEMY (-1)


typedef int (*FriendshipFunction)(void*,void*);
typedef int (*ComparisonFunction)(void*,void*);

typedef struct PersonLine *{
    int id;
    int friends_num;
    int enemy_num;
    PersonLine* next;
}PersonLine;

typedef struct IsraeliQueue_t * {
    FriendshipFunction* friendship_func;
    ComparisonFunction compare_func;
    int friendship_th;
    int rivalry_th;
    PersonLine person;
}IsraeliQueue;

IsraeliQueue IsraeliQueueCreate(FriendshipFunction * friendship_func, ComparisonFunction compare_func, int friendship_th, int rivalry_th){
    struct IsraeliQueue queue = (struct IsraeliQueue*)malloc(sizeof(struct IsraeliQueue));
    if (queue == NULL){
        return NULL;
    }
    queue->friendship_func = malloc(sizeof(FriendshipFunction*));
    queue->compare_func = malloc(sizeof(ComparisonFunction));
    queue->friendship_th = malloc(sizeof(int));
    queue->rivalry_th = malloc(sizeof(int));
    
    queue->friendship_func = friendship_func;
    queue->compare_func = compare_func;
    queue->friendship_th = friendship_th;
    queue->rivalry_th = rivalry_th;
    queue->person = NULL;

    return queue;
}

PersonLine PersonLineCreate(int id){
    struct PersonLine person = (struct PersonLine*)malloc(sizeof(struct PersonLine));
    person->id = malloc(sizeof(int));
    person->friends_num = malloc(sizeof(int));
    person->enemy_num = malloc(sizeof(int));

    person->id = id;
    person->friends_num=0;
    person->enemy_num=0;
    person->next = NULL;
    
    return person;
}

IsraeliQueue IsraeliQueueClone(IsraeliQueue q){
    struct IsraeliQueue new_queue = (struct IsraeliQueue*)malloc(sizeof(struct IsraeliQueue));
    new_queue->friendship_func = malloc(sizeof(FriendshipFunction*));
    new_queue->compare_func = malloc(sizeof(ComparisonFunction));
    new_queue->friendship_th = malloc(sizeof(int));
    new_queue->rivalry_th = malloc(sizeof(int));
    new_queue->person = malloc(sizeof(PersonLine));

    new_queue->friendship_func = q->friendship_func;
    new_queue->compare_func = q->compare_func;
    new_queue->friendship_th = q->friendship_th;
    new_queue->rivalry_th = q->rivalry_th;
    new_queue->person = q->person;

    return new_queue;
}

void IsraeliQueueDestroy(IsraeliQueue queue){
    free(queue->friendship_func);
    free(queue->compare_func);
    free(queue->friendship_th);
    free(queue->rivalry_th);
    free(queue->person);
    free(queue);
}

int AreFriends(PersonLine* person1, PersonLine* person2, FriendshipFunction * friendship_func, int friendship_th, int rivalry_th){
    int func_num = 0;
    int sum = 0; //sums the friendship values of each friendship function
    while (friendship_func != NULL){
        if (friendship_func(person1,person2)>friendship_th){ //if there is one func that says they're friends, end.
            return FRIEND;
        }
        else {
            sum+=friendship_func(person1,person2);
            func_num++;
        }
        friendship_func++; //continue to next func
    }
    if (sum/func_num<rivalry_th){
        return ENEMY;
    }
    else {
        return NUTRAL;
    }
    
}

//AddToSpot - adds person to line after person in "spot"
void AddToSpot(void * new_person, PersonLine* spot){
    PersonLine* temp = spot->next;
    spot->next = new_person;
    new_person->next = temp;

}

IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void * new_person){
    //if the line is empty, put new person in first place
    if(queue->person==NULL){
        queue->person = new_person;
        return ISRAELIQUEUE_SUCCESS;
    }

    //else
    PersonLine* temp = queue->person; //the person i'm comparing friendship with
    while(temp->next!=NULL){
        int relationship = AreFriends(temp ,new_person,queue->friendship_func, queue->friendship_th, queue->rivalry_th);
        bool found_friend = false;
        PersonLine* spot = NULL;
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
        return ISRAELIQUEUE_SUCCESS;
        }
        else{
            AddToSpot(spot,new_person);
            return ISRAELIQUEUE_SUCCESS;
        }
    }
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
    PersonLine* temp = queue->person;
    int length=0;
    while(temp->next!= NULL){
        length++;
        temp=temp->next;
    }
    return length;
}

void* IsraeliQueueDequeue(IsraeliQueue queue){
    if (queue == NULL || queue->person == NULL){
        return NULL;
    }
    PersonLine* temp = queue->person;
    temp->next = NULL;    
    queue->person = queue->person->next;
    //the personis out of the line so its friends and enemy count becomes 0
    temp->friends_num=0;
    temp->enemy_num=0;
    return temp;
    
}

//finds the location of a person in queue, if not exists returns null
IsraeliQueue FindPersonInQueue(IsraeliQueue queue, void * person1){
    if (queue == NULL || queue->person == NULL || person1 == NULL){
        return NULL;
    }
    while(queue->person->next != NULL){
        //if person is in queue
        if(queue->compare_func(person1, queue->person)){
            return queue->person;
        }
    }
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
    PersonLine* backwards_line;
    PersonLine* temp = IsraeliQueueDequeue(new_queue);
    while (new_queue->person != NULL){
        backwards_line = temp;
        temp = IsraeliQueueDequeue(new_queue);
        temp->next=backwards_line;
    }
    backwards_line = temp;
    return backwards_line;
}

//accepts the queue and a pointer to person (not in queue) to remove
PersonLine RemoveFromQueue(IsraeliQueue queue, PersonLine person1){
    IsraeliQueue spot = FindPersonInQueue(queue,person1); //points at the person to remove location
    if(spot != NULL){
        IsraeliQueue temp = spot->next; //save the pointer to after spot
        spot->next = NULL;
        PersonLine before = queue->person; //pointer to find the person before the person to remove, found in loop
        while(!queue->compare_func(before->next,spot)){
            before=before->next;
        }
        before->next=temp;
    }
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
