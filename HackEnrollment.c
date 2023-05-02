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
    int course_num;
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


Student** ReadStudentFile(FILE* students){
    //checks the longest row length in the given file and how many rows are in it
    fopen(students,"r");
    int row_num=0;
    int max_row_length=0;
    int current_row_length=0;
    while (getc(students) != EOF){ 
        current_row_length=0;
        while(getc(students)!="\n"){
            current_row_length++;
        }
        row_num++;
        if(current_row_length>max_row_length){
            max_row_length=current_row_length;
        }
    }
    fclose(students);
    
    Student** student_arr = (Student**)malloc(sizeof(Student*)*row_num);
    //
    fopen(students,"r");
    char* buffer[max_row_length];
    while(fgets(buffer,max_row_length,students)!=NULL){
        //split buffer into words
        
    }
    
    
}
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){
}
