#include <stdio.h>

#define MAX_STUDENTS 100

typedef struct
{
    int rollNo;
    char name[50];
    int marks[3];
    int totalMarks;
    float averageMarks;
    char grade;
} Student;

void inputStudentData(Student *student)
{
    printf("Enter Roll No, Name, Marks1, Marks2, Marks3:\n");
    scanf(" %d %s %d %d %d",
          &student->rollNo,
          student->name,
          &student->marks[0],
          &student->marks[1],
          &student->marks[2]);
}

void computeGrade(Student *student)
{
    if (student->averageMarks >= 85)
    {
        student->grade = 'A';
    }
    else if (student->averageMarks >= 70)
    {
        student->grade = 'B';
    }
    else if (student->averageMarks >= 50)
    {
        student->grade = 'C';
    }
    else if (student->averageMarks >= 35)
    {
        student->grade = 'D';
    }
    else
    {
        student->grade = 'F';
    }
}

void computePerformance(char grade)
{
    int countOfStars = 0;
    switch (grade)
    {
    case 'A':
        countOfStars = 5;
        break;
    case 'B':
        countOfStars = 4;
        break;
    case 'C':
        countOfStars = 3;
        break;
    case 'D':
        countOfStars = 2;
        break;
    default:
        break;
    }

    printf("Performance: ");
    for (int i = 0; i < countOfStars; i++)
        printf("*");
    printf("\n");
}

void printStudentReport(const Student *student)
{
    printf("\nRoll: %d\n", student->rollNo);
    printf("Name: %s\n", student->name);
    printf("Total: %d\n", student->totalMarks);
    printf("Average: %.2f\n", student->averageMarks);
    printf("Grade: %c\n", student->grade);
    computePerformance(student->grade);
}

void PrintRollNoRecursively(const Student *student, int index, int studentCount)
{
    if (index == studentCount){
        return;
    }
    printf("%d ", student[index].rollNo);
    PrintRollNoRecursively(student, index + 1, studentCount);
}

int main()
{
    int numOfStudents = 0;
    Student students[MAX_STUDENTS];
    printf("Enter the number of students: ");
    scanf("%d", &numOfStudents);
    for (int index = 0; index < numOfStudents; ++index)
    {
        printf("\nStudent %d\n", index + 1);
        inputStudentData(&students[index]);
        students[index].totalMarks = students[index].marks[0] +
                                     students[index].marks[1] +
                                     students[index].marks[2];
        students[index].averageMarks = students[index].totalMarks / 3.0;
    }
    
    printf("\n- Student Report -\n");
    for (int index = 0; index < numOfStudents; ++index)
    {
        computeGrade(&students[index]); 
        printStudentReport(&students[index]);
    }
    printf("\nList of Student Roll Numbers: ");
    PrintRollNoRecursively(students, 0, numOfStudents);
    printf("\n");

    return 0;
}
