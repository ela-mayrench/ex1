#include "IsraeliQueue.h"
#include "HackEnrollment.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* NOTES:
1) make sure you destroy all memory allocations
2)if in the FriendshipFunction i accept a bad person value what to return?
3) Are Hackers for sure in the student file?
4)is it enough for the hacker to be enrolled to 2 courses only, or we need to enroll him to all the courses he wants.
5)if course has "size=7", hacker in number 7 is ok?
*/


struct Student_t {
    char* id;
    int credits;
    int GPA;
    char* first_name;
    char* last_name;
    char* city;
    char* department;
    Hacker student_as_hacker;
};

struct Hacker_t {
    char* id;
    char** course_num;
    char** friends_id;
    char** enemies_id;
    Student hacker_as_student;
};

struct Course_t{
    char* course_num;
    int course_size;
    IsraeliQueue course_queue;
};

struct EnrollmentSystem_t{
    //all null terminated arrays
    Student* student_arr;
    Hacker* hacker_arr;
    Course* course_arr;
    bool flag;
};



//this is the compare function
int CompareById(void* s1,void* s2){
    Student student1=(Student)s1;
    Student student2=(Student)s2;

    if (strcmp(student1->id,student2->id)==0)
    {
        return 1;
    }
    return 0;
}


//creates :
Student CreateStudent(char* id, int credits, int GPA,char* first_name,char* last_name,char* city, char* department ){
    Student student = (Student)malloc(sizeof(struct Student_t));
    int id_len = strlen(id);
    int first_name_len = strlen(first_name);
    int last_name_len = strlen(last_name);
    int city_len = strlen(city);
    int department_len = strlen(department);

    student->id = (char*)malloc(sizeof(char)*id_len);
    student->first_name = (char*)malloc(sizeof(char)*first_name_len);
    student->last_name = (char*)malloc(sizeof(char)*last_name_len);
    student->city = (char*)malloc(sizeof(char)*city_len);
    student->department = (char*)malloc(sizeof(char)*department_len);

    student->id = id;
    student->credits=credits;
    student->GPA=GPA;
    student->first_name=first_name;
    student->last_name=last_name;
    student->city=city;
    student->department=department;
    student->student_as_hacker=NULL;
    return student;
}

Course CreateCourse(char* course_num, int course_size){
    Course course = (Course)malloc(sizeof(struct Course_t));
    course->course_num = (char*)malloc(sizeof(char)*strlen(course_num));

    strcpy(course->course_num,course_num);
    course->course_size=course_size;
    //course->course_queue = IsraeliQueueCreate(NULL,CompareById,0,0);
    course->course_queue=NULL;
    return course;
}

Hacker CreateHacker(char* id, char** course_num, char** friends_id, char** enemies_id){
    Hacker hacker = (Hacker)malloc(sizeof(struct Hacker_t));
    int id_len = strlen(id);
    int num_of_courses = 0;
    int num_of_friends = 0;
    int num_of_enemies = 0;
    for (; course_num[num_of_courses]!= NULL ; num_of_courses++);
    for (; friends_id[num_of_friends]!= NULL ; num_of_friends++);
    for (; enemies_id[num_of_enemies]!= NULL ; num_of_enemies++);

    hacker->id = (char*)malloc(sizeof(char)*id_len);
    hacker->course_num=(char**)malloc(sizeof(char*)*(num_of_courses+1));
    hacker->friends_id=(char**)malloc(sizeof(char*)*(num_of_friends+1));
    hacker->enemies_id=(char**)malloc(sizeof(char*)*(num_of_enemies+1));
    hacker->hacker_as_student=NULL;

    //copy array to Hacker struct
    strcpy(hacker->id,id);

    for(int i=0; i<num_of_courses; i++){
        hacker->course_num[i] = (char*)malloc(sizeof(char)*strlen(course_num[i]));
        strcpy(hacker->course_num[i],course_num[i]);
    }
    hacker->course_num[num_of_courses]=NULL;//null terminated array

    for(int i=0; i<num_of_friends; i++){
        hacker->friends_id[i] = (char*)malloc(sizeof(char)*strlen(friends_id[i]));
        strcpy(hacker->friends_id[i],friends_id[i]);
    }
    hacker->friends_id[num_of_friends]=NULL;//null terminated array

    for(int i=0; i<num_of_enemies; i++){
        hacker->enemies_id[i] = (char*)malloc(sizeof(char)*strlen(enemies_id[i]));
        strcpy(hacker->enemies_id[i],enemies_id[i]);
    }
    hacker->enemies_id[num_of_enemies]=NULL;//null terminated array

    return hacker;
}



//destroys:
void DestroyStudent(Student student){
    free(student->id);
    free(student->first_name);
    free(student->last_name);
    free(student->city);
    free(student->department);
    free(student);
}

void DestroyCourse(Course course){
    free(course->course_num);
    IsraeliQueueDestroy(course->course_queue);
    free(course);
}

void DestroyHacker(Hacker hacker){
    free(hacker->id);
    int num_of_courses = 0;
    int num_of_friends = 0;
    int num_of_enemies = 0;
    for (; hacker->course_num[num_of_courses]!= NULL ; num_of_courses++);
    for (; hacker->friends_id[num_of_friends]!= NULL ; num_of_friends++);
    for (; hacker->enemies_id[num_of_enemies]!= NULL ; num_of_enemies++);
    for(int i=0; i<num_of_courses; i++){
        free(hacker->course_num[i]);
    }
    free(hacker->course_num);

    for(int i=0; i<num_of_friends; i++){
        free(hacker->friends_id[i]);
    }
    free(hacker->friends_id);

    for(int i=0; i<num_of_enemies; i++){
        free(hacker->enemies_id[i]);
    }
    free(hacker->enemies_id);

    //DestroyStudent(hacker->hacker_as_student);

    free(hacker);

}



int CountWordsInLine(char* line){
    int len = strlen(line);//length of the string
    int word_counter=1;
    //counts number of words in string
    for (int i = 0; i < len; ++i) {
        if(line[i]==' '){
            word_counter++;
        }
    }
    return word_counter;
}

//returns null terminated array of a line
char** SplitStr(char* line){
    int len = strlen(line);//length of the string
    int word_counter = CountWordsInLine(line);
    if(strcmp(line,"\n")==0){
        char** arr = malloc(sizeof(char*));
        arr[0] = NULL;
        return arr;
    }
    char** arr = malloc(sizeof(char*)*(word_counter+1));//create an array of words the size of word_counter
    int begin=0;//beginning of every word
    int put=0;//where to put each word in word array
    for (int i = 0; i < len; ++i) {
        if(line[i]==' ' || line[i]=='\n'){
            int size = i-begin;
            char * temp = malloc(sizeof(char)*(size));//current word
            int loc=0;//where to put each letter in current word
            for (int j = begin; j < i; ++j) {
                temp[loc]=line[j];
                loc++;
            }
            temp[i-begin]='\0';
            arr[put]=temp;
            put++;
            begin=i+1;
        }
    }
    arr[word_counter] = NULL;
    return arr;
}

Student FromLineToStudent (char* line){
    char** arr = SplitStr(line);
    Student student = CreateStudent(arr[0],atoi(arr[1]), atoi(arr[2]),arr[3],arr[4],arr[5],arr[6]);
    return student;
}

Course FromLineToCourse (char* line){
    char** arr = SplitStr(line);
    Course course = CreateCourse(arr[0],atoi(arr[1])); //dont forget!! remove the atoi and build a cast function
    return course;
}

/*Hacker From4LineToHacker (char* line){
    char** arr = SplitStr(line);
    Hacker hacker = CreateHacker(atoi(arr[0]),atoi(arr[1]));
    return hacker;
}
*/

int FindRowNum (FILE* temp_file){
    FILE * file = temp_file;
    int row_num=0;
    char ch = getc(file);
    while (ch != EOF){
        while(ch!='\n'){
            ch = getc(file);
        }
        ch = getc(file);
        row_num++;
    }
    rewind(file);
    return row_num;
}

int FindMaxRowLength(FILE* temp_file) {
    FILE * file = temp_file;
    int max_row_length=0;
    int current_row_length=0;
    char ch = getc(file);
    while (ch != EOF){
        current_row_length=0;
        while(ch!='\n'){
            current_row_length++;
            ch = getc(file);
        }
        ch = getc(file);
        if(current_row_length>max_row_length){
            max_row_length=current_row_length;
        }
    }
    rewind(file);
    return max_row_length+2;
}

Student* ReadStudentFile(FILE* students){
    //checks the longest row length in the given file and how many rows are in it

    int row_num = FindRowNum(students);
    int max_row_length = FindMaxRowLength(students);
    Student* student_arr = (Student*)malloc(sizeof(struct Student_t)*(row_num+1));
    //
    char line[max_row_length];
    int i=0;


    while(fgets(line,max_row_length,students) !=NULL){
        student_arr[i] = FromLineToStudent(line);
        i++;
        //resets line before we put in a new line
        for (int j = 0; j < max_row_length; ++j) {
            line[j]='\0';
        }
    }
    student_arr[row_num] = NULL;

    return student_arr;
}

Hacker* ReadHackerFile(FILE* hackers){
    //checks the longest row length in the given file and how many rows are in it
    int row_num = FindRowNum(hackers);
    int max_row_length = FindMaxRowLength(hackers);

    //every hacker has 4 rowes to describe him
    int hacker_num = row_num/4;
    Hacker* hacker_arr = (Hacker*)malloc(sizeof(struct Hacker_t)*(hacker_num+1));

    char line[max_row_length];
    for(int curr_hacker=0; curr_hacker<hacker_num;curr_hacker++){
        char* new_id = (char*)malloc(sizeof(char)*max_row_length);
        char** new_course_num;
        char** new_friends_id;
        char** new_enemies_id;
        for(int hacker_line=0;hacker_line<4;hacker_line++){
            fgets(line,max_row_length,hackers);
            switch(hacker_line){
                case 0:{
                    char** temp_line = SplitStr(line);
                    strcpy(new_id,temp_line[0]);
                    free(temp_line);
                    break;
                }
                case 1:
                {
                    new_course_num = SplitStr(line);
                    break;
                }

                case 2:
                    new_friends_id = SplitStr(line);
                    break;
                case 3:
                    new_enemies_id = SplitStr(line);
                    break;
            }
        }
        Hacker new_hacker = CreateHacker(new_id,new_course_num,new_friends_id,new_enemies_id);
        hacker_arr[curr_hacker]=new_hacker;
        free(new_id);
        free(new_course_num);
        free(new_friends_id);
        free(new_enemies_id);
    }
    hacker_arr[hacker_num] = NULL;
    return hacker_arr;
}

Course* ReadCourseFile(FILE* courses_file){
    //checks the longest row length in the given file and how many rows are in it

    int row_num = FindRowNum(courses_file);
    int max_row_length = FindMaxRowLength(courses_file);
    Course* course_arr = (Course*)malloc(sizeof(struct Course_t)*(row_num+1));
    //
    char line[max_row_length];
    int i=0;

    while(fgets(line,max_row_length,courses_file) !=NULL){
        course_arr[i] = FromLineToCourse(line);
        i++;
        for (int j = 0; j < max_row_length; ++j) {
            line[j]='\0';
        }
    }
    course_arr[row_num] = NULL;
    return course_arr;
}


Student FindStudentInSys(EnrollmentSystem sys, char* student_id){
    if(sys == NULL){
        return NULL;
    }
    int i = 0;
    while(sys->student_arr!=NULL){
        Student temp_student = sys->student_arr[i];
        if(strcmp(temp_student->id,student_id)==0){
            return temp_student;
        }
        i++;
    }
    return NULL;
}

Course FindCourseInSys(EnrollmentSystem sys, char* course_num){
    if(sys == NULL){
        return NULL;
    }
    int i = 0;
    while(sys->course_arr!=NULL){
        Course temp_course = sys->course_arr[i];
        if(strcmp(temp_course->course_num,course_num)==0){
            return temp_course;
        }
        i++;
    }
    return NULL;

}

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){
    EnrollmentSystem sys = (EnrollmentSystem)malloc(sizeof(struct EnrollmentSystem_t));

    Student* student_arr=ReadStudentFile(students);
    Course* course_arr=ReadCourseFile(courses);
    Hacker* hacker_arr=ReadHackerFile(hackers);
    sys->flag = false;
    //for each hacker, finds the student that is him
    int i=0;
    while (hacker_arr[i]!=NULL){
        int j=0;
        while (student_arr[j]!=NULL){
            if(strcmp(hacker_arr[i]->id,student_arr[j]->id)==0){
                hacker_arr[i]->hacker_as_student=student_arr[j];
                student_arr[j]->student_as_hacker=hacker_arr[i];
                break;
            }
            j++;
        }
        i++;
    }

    sys->student_arr=student_arr;
    sys->course_arr=course_arr;
    sys->hacker_arr=hacker_arr;
    return sys;
}




void DestroyEnrollmentSystem(EnrollmentSystem sys){
    for (int i=0; sys->student_arr[i]!=NULL; i++){
        DestroyStudent(sys->student_arr[i]);
    }
    free(sys->student_arr);

    for (int i=0; sys->course_arr[i]!=NULL; i++){
        DestroyCourse(sys->course_arr[i]);
    }
    free(sys->course_arr);

    for (int i=0; sys->hacker_arr[i]!=NULL; i++){
        DestroyHacker(sys->hacker_arr[i]);
    }
    free(sys->hacker_arr);

    free(sys);
}

void SetFlag(EnrollmentSystem sys, bool flag){
    sys->flag = flag;
}


EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues){
    //int row_num = FindRowNum(queues); //the number of courses in file
    int max_row_length = FindMaxRowLength(queues); //the course + the number of students

    char line[max_row_length];
    char** line_in_arr;

    //for every line in file
    while(fgets(line,max_row_length,queues) !=NULL){
        int words_in_line = CountWordsInLine(line);
        line_in_arr = SplitStr(line);

        //for course
        Course curr_course = FindCourseInSys(sys,line_in_arr[0]);
        if(curr_course == NULL){
            return NULL;
        }

        else{
            curr_course->course_queue= IsraeliQueueCreate(NULL,CompareById,20,0);
            //for every student in course, add to course queue
            for (int i = 1; i < words_in_line; i++) //start in i=1 because, in arr[0] there is the course number
            {
                Student curr_student = FindStudentInSys(sys,line_in_arr[i]);
                if(curr_student==NULL){
                    return NULL;
                }
                else{
                    int error = IsraeliQueueEnqueue(curr_course->course_queue,curr_student);
                    if(error!=ISRAELIQUEUE_SUCCESS){
                        return NULL;
                    }
                }
            }
        }
    }
    return sys;
}

void MakeWordLower(char word[]){
    int len = strlen(word);

    for (int i = 0; i < len; ++i) {
        int letter = word[i];
        if(letter<'a'){
            letter+=('a'-'A');
            word[i] = (char)letter;
        }

    }
}





int FindNameDistancehAsciiValue(char* name_one, int name_one_len ,char* name_two,int name_two_len, bool flag){
    if (flag){
        MakeWordLower(name_one);
        MakeWordLower(name_two);
    }

    int sum=0;
    bool end = false;
    if(name_one_len>=name_two_len){
        for (int i = 0; i < name_one_len; i++)
        {
            if(!end){
                if(name_two[i]!='\0')
                {
                    sum+=abs(name_one[i]-name_two[i]);
                }
                else{
                    end = true;
                    sum+=abs(name_one[i]);
                }
            }
            else{
                sum+=abs(name_one[i]);
            }
        }

    }
    else{
        for (int i = 0; i < name_two_len; i++)
        {
            if(!end){
                if(name_one[i]!='\0')
                {
                    sum+=abs(name_two[i]-name_one[i]);
                }
                else{
                    end = true;
                    sum+=abs(name_two[i]);
                }
            }
            else{
                sum+=abs(name_two[i]);
            }

        }

    }
    return sum;
}

////////////
//Frindship Functions For Hackers

//first FriendshipFunction - from the hacker file
int HackerFileFriendshipFunction(void* s, void* h){
    Student hacker = (Student)h;
    Student student = (Student)s;
    if (hacker != NULL && student != NULL){
        int i = 0;
        if (hacker->student_as_hacker->friends_id[0] != NULL) {
            char **temp_friend = hacker->student_as_hacker->friends_id;
            if (temp_friend != NULL) {
                while (temp_friend[i] != NULL) {
                    if (strcmp(student->id, temp_friend[i]) == 0) {
                        return 20;
                    }
                    i++;
                }
            }
        }


        if(hacker->student_as_hacker->enemies_id[0] != NULL) {
            i=0;
            char **temp_enemy = hacker->student_as_hacker->enemies_id;
            if (temp_enemy != NULL) {
                while (temp_enemy[i] != NULL) {
                    if (strcmp(student->id, temp_enemy[i]) == 0) {
                        return -20;
                    }
                    i++;
                }
            }
        }
    }

    return 0;

}


//second FriendshipFunction - math on ASCII values in names
int NameDisFriendshipFunctionFlag(void* h, void* s){
    Student hacker = (Student)h;
    Student student = (Student)s;
    char* hacker_first_name = hacker->first_name;
    int h_first_name_len = strlen(hacker_first_name);
    char* hacker_last_name = hacker->last_name;
    int h_last_name_len = strlen(hacker_last_name);
    char* student_first_name = student->first_name;
    int s_first_name_len = strlen(student_first_name);
    char* student_last_name = student->last_name;
    int s_last_name_len = strlen(student_last_name);
    int sum_first = FindNameDistancehAsciiValue(hacker_first_name,h_first_name_len,student_first_name,s_first_name_len,1);
    int sum_last = FindNameDistancehAsciiValue(hacker_last_name,h_last_name_len,student_last_name,s_last_name_len,1);
    return sum_first+sum_last;
}
//second FriendshipFunction - math on ASCII values in names
int NameDisFriendshipFunctionNoFlag(void* h, void* s){
    Student hacker = (Student)h;
    Student student = (Student)s;
    char* hacker_first_name = hacker->first_name;
    int h_first_name_len = strlen(hacker_first_name);
    char* hacker_last_name = hacker->last_name;
    int h_last_name_len = strlen(hacker_last_name);
    char* student_first_name = student->first_name;
    int s_first_name_len = strlen(student_first_name);
    char* student_last_name = student->last_name;
    int s_last_name_len = strlen(student_last_name);
    int sum_first = FindNameDistancehAsciiValue(hacker_first_name,h_first_name_len,student_first_name,s_first_name_len,0);
    int sum_last = FindNameDistancehAsciiValue(hacker_last_name,h_last_name_len,student_last_name,s_last_name_len,0);
    return sum_first+sum_last;
}

//third FriendshipFunction - according to ids
int IdFriendshipFunction(void* h, void* s){
    Student hacker = (Student)h;
    Student student = (Student)s;

    return abs(atoi(student->id) - atoi(hacker->id));
}
//end of friendship functions
///////////////

char* CourseQueueToString(Course course){
    int course_size = IsraeliQueueSize(course->course_queue);
    int line_str_len=strlen(course->course_num);
    IsraeliQueue temp_queue1 = IsraeliQueueClone(course->course_queue);
    for (int i = 0; i < course_size; i++){
        Student curr_student = IsraeliQueueDequeue(temp_queue1);
        line_str_len+=strlen(curr_student->id);
        line_str_len++; //another spot for ' ' (space)
    }
    //line_str_len++;//another spot for \n
    IsraeliQueueDestroy(temp_queue1);

    char* str_line = (char*)malloc(sizeof(char)*line_str_len);
    IsraeliQueue temp_queue2 = IsraeliQueueClone(course->course_queue);
    strcpy(str_line,course->course_num);
    int str_location=strlen(course->course_num);
    str_line[str_location]=' ';
    str_location++;
    for (int i = 0; i < course_size; i++){
        Student curr_student = IsraeliQueueDequeue(temp_queue2);
        strcpy(str_line+str_location,curr_student->id);
        str_location+=strlen(curr_student->id);
        str_line[str_location]=' ';
        str_location++;
    }

    str_line[line_str_len]='\0';
    //str_line[line_str_len]='\n';
    IsraeliQueueDestroy(temp_queue2);
    return str_line;
}


void hackEnrollment(EnrollmentSystem sys, FILE* out){
    int i=0;
    Hacker* hacker_arr=sys->hacker_arr;
    //this while go over the hackers
    while (hacker_arr[i]!=NULL){
        int j=0;
        //this while go over the list of wanted courses for curr hacker and enqueue him to the course_queue.
        while (hacker_arr[i]->course_num[j]!=NULL)
        {
            Course curr_course=FindCourseInSys(sys,hacker_arr[i]->course_num[j]);
            if(curr_course->course_queue == NULL){
                curr_course->course_queue = IsraeliQueueCreate(NULL,CompareById,20,0);
            }

            //adding friendship functions to every course queue
            IsraeliQueueAddFriendshipMeasure(curr_course->course_queue, HackerFileFriendshipFunction);
            if (sys->flag == true){
                IsraeliQueueAddFriendshipMeasure(curr_course->course_queue, NameDisFriendshipFunctionFlag);
            }
            else{
                IsraeliQueueAddFriendshipMeasure(curr_course->course_queue, NameDisFriendshipFunctionNoFlag);
            }

            IsraeliQueueAddFriendshipMeasure(curr_course->course_queue, IdFriendshipFunction);
            /*if(curr_course==NULL){
            }*/
            //else{
            int error=IsraeliQueueEnqueue(curr_course->course_queue,hacker_arr[i]->hacker_as_student);
            if (error!=ISRAELIQUEUE_SUCCESS){

            }
                j++;
            //}
        }
        i++;
    }


    //again a while for the hackers and a while for the courses, this time we check each hacker location in the waiting list
    //to see if all hackers made it to their desired course
    //NOTE : check if \n is working correctly
    int k=0;
    bool hacker_success=true;
    while (hacker_arr[k]!=NULL){
        int j=0;
        int success_num=0;
        //this while go over the list of wanted courses for curr hacker and enqueue him to the course_queue.
        while (hacker_arr[k]->course_num[j]!=NULL && hacker_success)
        {
            int place_in_line=0;
            Course curr_course=FindCourseInSys(sys,hacker_arr[k]->course_num[j]);
            /*if(curr_course==NULL){

            }*/
            //else{
            //try to find hacker in course
            IsraeliQueue temp_course_queue = IsraeliQueueClone(curr_course->course_queue);
            Student temp_student = IsraeliQueueDequeue(temp_course_queue);
            while (temp_student!=NULL)
            {
                if(strcmp(temp_student->id,hacker_arr[k]->hacker_as_student->id)!=0)
                {
                    place_in_line++;
                }
                else
                {
                    if(place_in_line<curr_course->course_size)
                    {
                        success_num++;
                    }
                    break;
                }
                temp_student = IsraeliQueueDequeue(temp_course_queue);
            }
            //}
            j++;
            IsraeliQueueDequeue(temp_course_queue);
        }

        int num_of_courses2success = 2;
        if (j<2){
            num_of_courses2success = j;
        }
        if (success_num<num_of_courses2success){
            hacker_success = false;
            break;
        }
        k++;
    }

    if(!hacker_success){
        char* output1= "Cannot satisfy constraints for";
        char* output2= hacker_arr[k]->id;
        fputs(output1,out);
        fputc(' ',out);
        fputs(output2,out);
        fputc('\n',out);

        //write Cannot satisfy constraints for <student ID>
    }
    else{
        //write to OUT queue
        int i=0;
        Course* course_arr=sys->course_arr;
        while(course_arr[i]!=NULL){
            if(course_arr[i]->course_queue!=NULL){
                char* line = CourseQueueToString(course_arr[i]);
                fputs(line,out);
                fputc('\n',out);

            }
            i++;
        }

    }

}