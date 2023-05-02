#include <stdbool.h>

typedef struct Student_t Student;
typedef struct Hacker_t Hacker;
typedef struct Course_t Course;
typedef struct EnrollmentSystem_t * EnrollmentSystem;

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);
void hackEnrollment(EnrollmentSystem sys, FILE* out);


