#include <stdbool.h>
#include <stdio.h>

typedef struct Student_t* Student;
typedef struct Hacker_t* Hacker;
typedef struct Course_t* Course;
typedef struct EnrollmentSystem_t * EnrollmentSystem;

//create
Student CreateStudent(char* id, int credits, int GPA,char* first_name,char* last_name,char* city, char* department );
Course CreateCourse(char* course_num, int course_size);
Hacker CreateHacker(char* id, char** course_num, char** friends_id, char** enemies_id);
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

//destroy
void DestroyStudent(Student student);
void DestroyCourse(Course course);
void DestroyHacker(Hacker hacker);
void DestroyEnrollmentSystem(EnrollmentSystem sys);

int CompareById(void* s1,void* s2);


//fine management
int CountWordsInLine(char* line);
char** SplitStr(char* line);
Student FromLineToStudent (char* line);
Course FromLineToCourse (char* line);
int FindRowNum (FILE* temp_file);
int FindMaxRowLength(FILE* temp_file);
void MakeWordLower(char word[]);
Student* ReadStudentFile(FILE* students);
Hacker* ReadHackerFile(FILE* hackers);
Course* ReadCourseFile(FILE* courses_file);
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);
char* CourseQueueToString(Course course);

//find in sys
Student FindStudentInSys(EnrollmentSystem sys, char* student_id);
Course FindCourseInSys(EnrollmentSystem sys, char* course_num);

//friendship functions etc
int FindNameDistancehAsciiValue(char* name_one, int name_one_len ,char* name_two,int name_two_len, bool flag);
int HackerFileFriendshipFunction(void* h, void* s);
int NameDisFriendshipFunctionFlag(void* h, void* s);
int NameDisFriendshipFunctionNoFlag(void* h, void* s);
int IdFriendshipFunction(void* h, void* s);

//hack enrollment
void hackEnrollment(EnrollmentSystem sys, FILE* out);
void SetFlag(EnrollmentSystem sys, bool flag);
