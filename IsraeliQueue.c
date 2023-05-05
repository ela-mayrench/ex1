#include "IsraeliQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_FRIENDS 5
#define MAX_ENEMIES 3
#define FRIEND 1
#define NUTRAL 0 //if two people are not friends and not enemies
#define ENEMY (-1)
#define SAME 1


typedef struct PersonLine_t{
    void* item;
    int serial_num;
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

//our func
//returns the number of the friendship_funcs in the array
int CountFuncs(FriendshipFunction * friendship_func){
    int i=0;
    if(friendship_func!=NULL){
        while (friendship_func[i]!=NULL){
            i++;
        }
    }
    return i+1;
}

//given func:
IsraeliQueue IsraeliQueueCreate(FriendshipFunction * friendship_func, ComparisonFunction compare_func, int friendship_th, int rivalry_th){
    int friendship_func_num = CountFuncs(friendship_func);
    IsraeliQueue queue = (IsraeliQueue)malloc(sizeof(struct IsraeliQueue_t));
    if (queue == NULL){
        return NULL;
    }
    if(friendship_func!=NULL){
        if(friendship_func[0]!=NULL){
            queue->friendship_func = (FriendshipFunction*)malloc(sizeof(FriendshipFunction)*(friendship_func_num+1));
            int i=0;
            while (friendship_func[i]!=NULL){
                queue->friendship_func[i] = friendship_func[i];
                i++;
            }
            queue->friendship_func[i]=NULL;
        }
        else
            queue->friendship_func=NULL;
    }
    else
        queue->friendship_func=NULL;
    queue->compare_func = compare_func;
    queue->friendship_th = friendship_th;
    queue->rivalry_th = rivalry_th;
    queue->person = NULL;

    return queue;
}

//our func
PersonLine PersonLineCreate(void* item){
    PersonLine person = (PersonLine)malloc(sizeof(struct PersonLine_t));
    person->item = item;
    person->serial_num = 0;
    person->friends_num=0;
    person->enemy_num=0;
    person->next = NULL;

    return person;
}

//our func
void PersonLineDestroy(PersonLine line){
    while(line!=NULL){
        PersonLine temp = line;
        line=line->next;
        //free(temp->next);
        temp->next=NULL;
        free(temp);
    }
}


//given func
void IsraeliQueueDestroy(IsraeliQueue queue){
    if(queue->friendship_func!=NULL){
        //FriendshipFunction * temp = queue->friendship_func;
        free(queue->friendship_func);
    }
    PersonLineDestroy(queue->person);
    free(queue);
}


//our func
FriendshipFunction* CopyFuncArray(FriendshipFunction * friendship_func){
    int length = CountFuncs(friendship_func);
    FriendshipFunction * new_array = (FriendshipFunction*)malloc(sizeof(FriendshipFunction)*(length+1));
    if (new_array==NULL || friendship_func==NULL || friendship_func[0]==NULL){
        return NULL;
    }
    for(int i=0 ; i<length ; i++){
        new_array[i] = friendship_func[i];
    }
    new_array[length]=NULL;
    return new_array;
}

//given func
IsraeliQueue IsraeliQueueClone(IsraeliQueue q){
    //make new things to put in new queue
    FriendshipFunction* new_array = CopyFuncArray(q->friendship_func);
    IsraeliQueue new_queue = IsraeliQueueCreate(new_array, q->compare_func, q->friendship_th, q->rivalry_th);
    PersonLine original_tmp = q->person;
    //copy person line to new queue
    PersonLine new_person=PersonLineCreate(original_tmp->item);
    new_person->serial_num = original_tmp->serial_num;
    new_person->friends_num=original_tmp->friends_num; //new person points at the head of the cloned person line
    new_person->enemy_num=original_tmp->enemy_num;
    PersonLine new_tmp = new_person; //new temp points at the latest created person in the line
    original_tmp=original_tmp->next;
    while(original_tmp!=NULL){
        PersonLine created_person = PersonLineCreate(original_tmp->item);
        created_person->friends_num=original_tmp->friends_num; //new person points at the head of the cloned person line
        created_person->enemy_num=original_tmp->enemy_num;
        created_person->serial_num = original_tmp->serial_num;
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


//our func
int AreFriends(PersonLine person1, PersonLine person2, FriendshipFunction * friendship_func, int friendship_th, int rivalry_th){
    int func_num = 0;
    int sum = 0; //sums the friendship values of each friendship function
    int i=0;
    if(friendship_func!=NULL){
        while (friendship_func[i] != NULL){
            if (friendship_func[i](person1->item,person2->item)>friendship_th){ //if there is one func that says they're friends, end.
                return FRIEND;
            }
            else {
                sum+=friendship_func[i](person1->item,person2->item);
                func_num++;
            }
            i++; //continue to next func
        }
    }
    if(func_num==0 || friendship_func == NULL || friendship_func[0] == NULL)
    {
        return NUTRAL;
    }
    else{
        if ((sum/func_num)<rivalry_th){
            return ENEMY;
        }
        else {
            return NUTRAL;
        }
    }


}


//our func
//AddToSpot - adds person to line after person in "spot"
void AddToSpot(PersonLine new_person, PersonLine spot) {
    new_person->next = NULL;
    PersonLine temp = spot->next;
    spot->next = new_person;
    new_person->next = temp;
    temp = NULL;
}

//our func
int IsraeliQueueSize(IsraeliQueue queue){
    int length=0;
    if(queue!=NULL){
        PersonLine temp = queue->person;
        while(temp!= NULL){
            length++;
            temp=temp->next;
        }
        temp=NULL;
    }
    return length;
}

//our func
IsraeliQueueError IsraeliQueueInsert(IsraeliQueue queue, PersonLine new_person,bool update_serial){
    //if the line is empty, put new person in first place
    if (new_person == NULL) {
        return ISRAELI_QUEUE_ERROR;
    }

    int queue_length = IsraeliQueueSize(queue);
    if(update_serial){
        new_person->serial_num = queue_length+1;
    }


    if(queue->person==NULL){
        queue->person = new_person;
        return ISRAELIQUEUE_SUCCESS;
    }

    //else
    PersonLine temp = queue->person; //the person i'm comparing friendship with
    PersonLine spot=NULL;
    bool found_friend = false;
    //spot = NULL;
    while(temp->next!=NULL){
        int relationship = AreFriends(temp ,new_person,queue->friendship_func, queue->friendship_th, queue->rivalry_th);
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
    /*bool is_spot_last_item=false;
    if(temp!=NULL && spot !=NULL) {
        if (queue->compare_func(temp, spot) && temp->serial_num == spot->serial_num) {
            is_spot_last_item = true;
        }
        if (temp->next == NULL && AreFriends(temp, new_person, queue->friendship_func, queue->friendship_th, queue->rivalry_th) == FRIEND && is_spot_last_item) {
            if (temp->friends_num < MAX_FRIENDS) {
                temp->friends_num++;
                temp->next = new_person;
            } else {
                temp->next = new_person;
            }
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
    }*/
    if(temp!=NULL)
    {
        //if(spot!=NULL&& queue->compare_func(temp,spot)&&temp->serial_num==spot->serial_num){}
        if(temp->next == NULL && AreFriends(temp ,new_person,queue->friendship_func, queue->friendship_th, queue->rivalry_th) == FRIEND && spot==NULL){
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
                AddToSpot(new_person,spot);
                spot->friends_num++;
                temp=NULL;
                spot=NULL;
                return ISRAELIQUEUE_SUCCESS;
            }
        }
    }

    temp=NULL;
    spot=NULL;
    return ISRAELI_QUEUE_ERROR;
}

//given func
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void * item){
    PersonLine new_person = PersonLineCreate(item);
    return IsraeliQueueInsert(queue,new_person,1);
}

//given func
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction func){
    int length=0;//friendship function arr length
    if(queue->friendship_func!=NULL){
        while(queue->friendship_func[length]!=NULL){
            length++;
        }
    }
    FriendshipFunction* new_array = (FriendshipFunction*)malloc(sizeof(FriendshipFunction)*(length+2));
    if(new_array==NULL){
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    for(int i=0; i<length; i++){
        new_array[i]=queue->friendship_func[i];
    }
    new_array[length]=func;
    new_array[length+1]=NULL;
    if(queue->friendship_func!=NULL){
        free(queue->friendship_func);
    }
    queue->friendship_func = new_array;
    return ISRAELIQUEUE_SUCCESS;
}

//given func
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue queue, int new_threshold){
    queue->friendship_th=new_threshold;
    return ISRAELIQUEUE_SUCCESS;
}

//given func
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue queue, int new_threshold){
    queue->rivalry_th=new_threshold;
    return ISRAELIQUEUE_SUCCESS;
}



//our func
PersonLine IsraeliQueueRemove(IsraeliQueue queue){
    if (queue == NULL || queue->person == NULL){
        return NULL;
    }
    PersonLine temp = queue->person;
    queue->person = queue->person->next;
    temp->next = NULL;
    //the person is out of the line so its friends and enemy count becomes 0
    temp->friends_num=0;
    temp->enemy_num=0;
    return temp;

}

//given func
void* IsraeliQueueDequeue(IsraeliQueue queue){
    if(queue!=NULL){
        PersonLine removed = IsraeliQueueRemove(queue);
        if(removed!=NULL){
            return  removed->item;
        }
    }
    return NULL;

}

//given func
bool IsraeliQueueContains(IsraeliQueue queue, void * item){
    PersonLine person1 = PersonLineCreate(item);
    PersonLine temp = queue->person;
    while(temp!=NULL){
        if(queue->compare_func(temp->item,person1->item) == SAME)
            return true;
    }
    return false;
}

//build backwards queue
IsraeliQueue BuildBackwardsLine(IsraeliQueue queue){
    IsraeliQueue new_queue = IsraeliQueueClone(queue);
    PersonLine backwards_line;
    //
    PersonLine temp = IsraeliQueueRemove(new_queue);
    while (new_queue->person != NULL){
        backwards_line = temp;
        temp = IsraeliQueueRemove(new_queue);
        temp->next=backwards_line;
    }
    backwards_line = temp;
    temp=NULL;
    new_queue->person = backwards_line;
    return new_queue;
}

/*//finds the location of a person in queue, if not exists returns null
//person1 is in queue for sure - because it comes from the back of the existing line
//pay attention - this func is only used in RemoveFromQueue
PersonLine FindPersonToRemove(IsraeliQueue queue, PersonLine person1){
    //if the line is empty or person is empty
    if (queue == NULL || queue->person == NULL || person1 == NULL){
        return NULL;
    }
    PersonLine temp = queue->person; //temp compares the person to the queue
    while(temp != NULL){
        //if person is the same as temp
        if(queue->compare_func(person1->item, temp->item) && person1->serial_num == temp->serial_num){
            return temp; //the similar person pointer
        }
        //if person not the same as temp
        temp=temp->next;  //try to compare to the next person in queue
    }
    //person is not in queue
    temp=NULL;
    return NULL;
}*/

//accepts the queue and a pointer to person to remove
/*PersonLine RemoveFromQueue(IsraeliQueue queue, PersonLine person1){
    if (queue == NULL || queue->person == NULL || person1 == NULL){
        return NULL;
    }
    PersonLine spot = NULL;
    PersonLine temp = queue->person; //temp compares the person to the queue
    while(temp != NULL){
        //if person is the same as temp
        if(queue->compare_func(person1->item, temp->item) && person1->serial_num == temp->serial_num){
            spot = temp; //the similar person pointer
        }
        //if person not the same as temp
        temp=temp->next;  //try to compare to the next person in queue
    }
    //person is not in queue
    if(spot != NULL){
        PersonLine temp2 = spot->next; //save the pointer to after spot
        PersonLine before = queue->person; //pointer to find the person before the person to remove, found in loop
        while(!(queue->compare_func(before->next->item,spot->item) && before->next->serial_num == spot->serial_num)){
            before=before->next;
        }
        before->next=temp2;
        spot->next = NULL;
    }
    return spot;
}*/

//given func
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue queue){
    IsraeliQueue backwards_queue = BuildBackwardsLine(queue);
    while(backwards_queue->person != NULL){
        PersonLine improve_p = IsraeliQueueRemove(backwards_queue);
        /////////////////////////////////////////////////////////////////////////////////////////
        //Remove
        if (queue == NULL || queue->person == NULL || improve_p == NULL){
            return ISRAELI_QUEUE_ERROR;
        }
        PersonLine spot = NULL;
        PersonLine temp = queue->person; //temp compares the person to the queue
        while(temp != NULL){
            //if person is the same as temp
            if(queue->compare_func(improve_p->item, temp->item) && improve_p->serial_num == temp->serial_num){
                spot = temp; //the similar person pointer
                break;
            }
            //if person not the same as temp
            temp=temp->next;  //try to compare to the next person in queue
        }
        //person is not in queue
        if(spot != NULL){
            PersonLine temp2 = spot->next; //save the pointer to after spot
            PersonLine before = queue->person; //pointer to find the person before the person to remove, found in loop
            if((queue->compare_func(before->item,spot->item) && before->serial_num == spot->serial_num))
            {
                PersonLine first = before;
                queue->person=queue->person->next;
                first->next=NULL;
                //PersonLineDestroy(first);
            }
            else{
                while(before->next!=NULL && !(queue->compare_func(before->next->item,spot->item) && before->next->serial_num == spot->serial_num)){
                    before=before->next;
                }
                before->next=temp2;
            }

            spot->next = NULL;
        }
        PersonLine removed = spot;
        /////////////////////////////////////////////////////////////////////////////////////
        IsraeliQueueError error = IsraeliQueueInsert(queue,removed,0);
        if(error != ISRAELIQUEUE_SUCCESS){
            return error;
        }
    }
    return ISRAELIQUEUE_SUCCESS;
}

//given func
//
IsraeliQueue IsraeliQueueMerge(IsraeliQueue* arr_q,ComparisonFunction compare_func){
    int arr_q_size = 0; //the number of queues in the array

    //make new merged friendship functions array
    while (arr_q[arr_q_size] != NULL){
        arr_q_size++;
    }
    int merged_arr_func_size = 0; //the number of friendship funcs in the array of all queues

    //find merged_arr_func_size :
    for(int i = 0; i<arr_q_size; i++){ //for every queue
        int func_num = 0; //num of func in certain queue
        if(arr_q[i]->friendship_func!=NULL){
            while(arr_q[i]->friendship_func[func_num] != NULL){
                merged_arr_func_size++; //count all funcs in queue
                func_num++; //to mark when loop is ready for new iteration
            }
        }
    }

    FriendshipFunction* arr_func_merged = (FriendshipFunction*)malloc(sizeof(FriendshipFunction)*(merged_arr_func_size+1));
    int func_spot = 0; //the next spot to put function in merged_func_array
    for(int i = 0; i<arr_q_size; i++){ //for every queue
        int func_num = 0; //num of func in certain queue
        if(arr_q[i]->friendship_func!=NULL) {
            while (arr_q[i]->friendship_func[func_num] != NULL) {
                arr_func_merged[func_spot] = arr_q[i]->friendship_func[func_num];
                func_spot++; //count all funcs in queue
                func_num++; //to mark when loop is ready for new iteration
            }
        }
    }

    //find the merged_friendship_th
    int merged_friendship_th = 0;
    for(int i = 0; i<arr_q_size; i++){
        merged_friendship_th+= arr_q[i]->friendship_th;
    }
    merged_friendship_th = (merged_friendship_th/arr_q_size)+1;

    //find the merged_rivalry_th
    int merged_rivalry_th = 0;
    for(int i = 0; i<arr_q_size; i++){
        merged_rivalry_th*= arr_q[i]->rivalry_th;
    }
    merged_rivalry_th = abs(merged_rivalry_th);
    merged_rivalry_th = pow(merged_rivalry_th,1/arr_q_size)+1;

    //create the merged queue
    IsraeliQueue merged_queue = IsraeliQueueCreate(arr_func_merged,compare_func,merged_friendship_th,merged_rivalry_th);

    //create array of personline
    PersonLine* person_line_arr = (PersonLine*)malloc(sizeof(struct PersonLine_t)*arr_q_size); //person line from every queue
    for(int i=0 ; i < arr_q_size ; i++){
        person_line_arr[i] = arr_q[i]->person;
    }

    bool all_null = false; //are all the queues in the merged queue
    while(!all_null){
        for(int i=0; i<arr_q_size; i++){
            if(person_line_arr[i]!= NULL){
                person_line_arr[i]->enemy_num=0;
                person_line_arr[i]->enemy_num=0;
                person_line_arr[i]->serial_num=0;
                IsraeliQueueEnqueue(merged_queue, person_line_arr[i]->item);
                person_line_arr[i]=person_line_arr[i]->next;
            }
        }
        all_null = true;
        for(int i=0; i<arr_q_size; i++){
            if(person_line_arr[i]!=NULL){
                all_null = false;
            }
        }
    }
    free(person_line_arr);
    return merged_queue;


}

int FindPlaceInQueue(IsraeliQueue queue, void* item){
    PersonLine temp = queue->person;
    int num_in_line=0;
    while(temp!=NULL){
        if(queue->compare_func(temp->item,item) != SAME){
            num_in_line++;
            temp=temp->next;
        }
        else{
            break;
        }
    }
    return num_in_line;
}