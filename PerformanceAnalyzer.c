#include <stdio.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50

struct Student
{
    int rollNo;
    char name[MAX_NAME_LENGTH];
    int marks1;
    int marks2;
    int marks3;
    int totalMarks;
    float averageMarks;
    char grade;
};

void input_student_data(struct Student *s)
{
    printf("Enter Roll No, Name, Marks1, Marks2, Marks3 : \n");
    scanf("%d %s %d %d %d",
          &s->rollNo,
          s->name,
          &s->marks1,
          &s->marks2,
          &s->marks3);
}

int calculateTotal(int marks1, int marks2, int marks3)
{
    int total = marks1 + marks2 + marks3;
    return total;
}

float calculateAverage(float total)
{
    return total / 3.0;
}

char calculateGrade(float average)
{
    if (average >= 85)
    {
        return 'A';
    }
    else if (average >= 70)
    {
        return 'B';
    }
    else if (average >= 50)
    {
        return 'C';
    }
    else if (average >= 35)
    {
        return 'D';
    }
    else
    {
        return 'F';
    }
}

void PerformancePattern(int stars)
{
    for (int i = 0; i < stars; i++)
    {
        printf("*");
    }
    printf("\n");
}

void PrintStudentReport(const struct Student *s)
{
    printf("\n");
    printf("Roll: %d\n", s->rollNo);
    printf("Name: %s\n", s->name);
    printf("Total: %d\n", s->totalMarks);
    printf("Average: %.2f\n", s->averageMarks);
    printf("Grade: %c\n", s->grade);

    if (s->grade != 'F')
    {
        int stars = 0;
        switch (s->grade)
        {
        case 'A':
            stars = 5;
            break;
        case 'B':
            stars = 4;
            break;
        case 'C':
            stars = 3;
            break;
        case 'D':
            stars = 2;
            break;
        default:
            break;
        }

        printf("Performance: ");
        PerformancePattern(stars);
    }
}

void PrintRollNoRecursively(const struct Student *students, int index, int count)
{
    if (index == count)
    {
        return;
    }

    printf("%d ", students[index].rollNo);
    PrintRollNoRecursively(students, index + 1, count);
}

int main(void)
{
    int n;
    printf("Enter the number of students: ");
    scanf("%d", &n);

    struct Student students[n];
    for (int i = 0; i < n; ++i)
    {
        printf("\n Student %d ", i + 1);
        input_student_data(&students[i]);

        students[i].totalMarks = calculateTotal(students[i].marks1, students[i].marks2, students[i].marks3);
        students[i].averageMarks = calculateAverage(students[i].totalMarks);
        students[i].grade = calculateGrade(students[i].averageMarks);
    }

    printf("\n-- Student Report --");
    for (int i = 0; i < n; ++i)
    {
        PrintStudentReport(&students[i]);
    }

    printf("\nList of Roll Numbers : ");
    PrintRollNoRecursively(students, 0, n);
    printf("\n");

    return 0;
}
