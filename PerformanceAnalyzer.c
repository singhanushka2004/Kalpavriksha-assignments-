#include <stdio.h>

#define MAX_STUDENTS 100

struct Student
{
    int rollNo;
    char name[50];
    int marks[3];
    int totalMarks;
    float averageMarks;
    char grade;
};

void inputStudentData(struct Student *student)
{
    printf("Enter Roll No, Name, Marks1, Marks2, Marks3:\n");
    scanf(" %d %s %d %d %d",
          &student->rollNo,
          student->name,
          &student->marks[0],
          &student->marks[1],
          &student->marks[2]);
}

void computeGrade(struct Student *student)
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
    for (int starIndex = 0; starIndex < countOfStars; starIndex++)
    {
        printf("*");
    }
    printf("\n");
}

void printStudentReport(const struct Student *student)
{
    printf("\n");
    printf("Roll: %d\n", student->rollNo);
    printf("Name: %s\n", student->name);
    printf("Total: %d\n", student->totalMarks);
    printf("Average: %f\n", student->averageMarks);
    printf("Grade: %c\n", student->grade);

    computePerformance(student->grade);
}

void PrintRollNoRecursively(const struct Student *student, int index, int studentCount)
{
    if (index == studentCount)
    {
        return;
    }

    printf("%d ", student[index].rollNo);
    PrintRollNoRecursively(student, index + 1, studentCount);
}

int main()
{
    int noOfStudents = 0;

    struct Student students[MAX_STUDENTS];

    printf("Enter the number of students: ");
    scanf("%d", &noOfStudents);

    for (int index = 0; index < noOfStudents; ++index)
    {
        printf("\n Student %d ", index + 1);

        inputStudentData(&students[index]);

        students[index].totalMarks = students[index].marks[0] + students[index].marks[1] + students[index].marks[2];
        students[index].averageMarks = students[index].totalMarks / 3.0;

        computeGrade(&students[index]);
    }

    printf("\n-Student Report-");
    for (int index = 0; index < noOfStudents; ++index)
    {
        printStudentReport(&students[index]);
    }

    printf("\nList of Student Roll Numbers : ");
    PrintRollNoRecursively(students, 0, noOfStudents);
    printf("\n");

    return 0;
}
