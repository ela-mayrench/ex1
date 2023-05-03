#include "IsraeliQueue.h"
#include "HackEnrollment.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Student_t {
    char* id;
    int credits;
    int GPA;
    char* first_name;
    char* last_name;
    char* city;
    char* department;
};

struct Hacker_t {
    char* id;
    char** course_num;
    char** friends_id;
    char** enemies_id;
};

struct Course_t{
    int course_num;
    int course_size;
    IsraeliQueue course_queue;
};

struct EnrollmentSystem_t{
    Student** student_arr; 
    Hacker** hacker_arr;
    Course** course_arr;
};





Student CreateStudent(char* id, int credits, int GPA,char* first_name,char* last_name,char* city, char* department ){
    Student student = (Student)malloc(sizeof(Student));
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
}

Course CreateCourse(int course_num, int course_size){
    Course course = (Course)malloc(sizeof(Course));
    course->course_num=course_num;
    course->course_size=course_size;
    course->course_queue = NULL;
}

Hacker CreateHacker(char* id, char** course_num, char** friends_id, char** enemies_id){
    Hacker hacker = (Hacker)malloc(sizeof(Hacker));
    int id_len = strlen(id);
  //
    student->id = (char*)malloc(sizeof(char)*id_len);
 
    hacker->id = id;
    hacker->course_num=course_num;///check!! (if need malloc)
    hacker->friends_id=friends_id;///check!!
    hacker->enemies_id=enemies_id;///check!! (if need malloc)
}

char** SplitStr(char* line){
    int len = strlen(line);//length of the string
    int word_counter=1;
    //counts number of words in string
    for (int i = 0; i < len; ++i) {
        if(line[i]==' '){
            word_counter++;
        }
    }
    char** arr = malloc(sizeof(char*)*word_counter);//create an array of words the size of word_counter
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
            begin=i;
        }
    }
    return arr;
}

Student FromLineToStudent (char* line){
    char** arr = SplitStr(line);
    Student student = CreateStudent(arr[0],atoi(arr[1]), atoi(arr[2]),arr[3],arr[4],arr[5],arr[6]);
    return student;
}

Course FromLineToCourse (char* line){
    char** arr = SplitStr(line);
    Course course = CreateCourse(atoi(arr[0]),atoi(arr[1]));
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
    Student* student_arr = (Student*)malloc(sizeof(Student)*row_num);
    //
    char line[max_row_length];
    int i=0;


    while(fgets(line,max_row_length,students) !=NULL){
        student_arr[i] = FromLineToStudent(line);
        i++;
        for (int j = 0; j < max_row_length; ++j) {
            line[j]='\0';
        }
    }
    return student_arr;
}

Hacker* ReadHackerFile(FILE* hackers){
    //checks the longest row length in the given file and how many rows are in it
    int row_num = FindRowNum(hackers);
    int max_row_length = FindMaxRowLength(hackers);

    //every hacker has 4 rowes to describe him
    int hacker_num = row_num/4;
    Hacker* hacker_arr = (Hacker*)malloc(sizeof(Hacker)*hacker_num);
    
    char line[max_row_length];
    int i=0;
    for(int curr_hacker=0; curr_hacker<hacker_num;curr_hacker++){
        for(int hacker_line=0;hacker_line<4;hacker_line++){
            fgets(line,max_row_length,hackers);
            switch(hacker_line){
                case 0:
                    hacker_arr[curr_hacker]->id = line;
                    break;
                case 1:
                    hacker_arr[curr_hacker]->course_num = SplitStr(line);
                    break;
                case 2:
                    hacker_arr[curr_hacker]->friends_id = SplitStr(line);
                    break;
                case 3:
                    hacker_arr[curr_hacker]->enemies_id = SplitStr(line);
                    break;
            }
        }


    }
    return hacker_arr;
}

Course* ReadCourseFile(FILE* courses_file){
    //checks the longest row length in the given file and how many rows are in it

    int row_num = FindRowNum(courses_file);
    int max_row_length = FindMaxRowLength(courses_file);
    Course* course_arr = (Course*)malloc(sizeof(Course)*row_num);
    //
    char line[max_row_length];
    int i=0;

    while(fgets(line,max_row_length,courses_file) !=NULL){
        student_arr[i] = FromLineToCourse(line);
        i++;
        for (int j = 0; j < max_row_length; ++j) {
            line[j]='\0';
        }
    }
    return course_arr;
}


EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){
    Student* student_arr=ReadStudentFile(students);
    Course* course_arr=ReadCourseFile(courses);
    Hacker* hacker_arr=ReadHackerFile(hackers);
}
