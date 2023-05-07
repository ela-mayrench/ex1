#include "HackEnrollment.h"

int main(int argc, char** argv){
    int index=0;
    bool flag = false;
    
    if(argc>5){
        index = 1;
        flag = true;
    }

    FILE* students_file = fopen(argv[0+index],"r");
    FILE* courses_file = fopen(argv[1+index],"r");
    FILE* hackers_file = fopen(argv[2+index],"r");
    FILE* queues_file = fopen(argv[3+index],"r");
    FILE* target_file = fopen(argv[4+index],"w");

    EnrollmentSystem sys = createEnrollment(students_file,courses_file,hackers_file);
    SetFlag(sys,flag);

    fclose(students_file);
    fclose(courses_file);
    fclose(hackers_file);

    sys = readEnrollment(sys,queues_file);

    fclose(queues_file);

    hackEnrollment(sys,target_file);
    fclose(target_file);
    
    //need to do: 1)destroy sys 2)use i

return 0;
}