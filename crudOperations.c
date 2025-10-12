#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LEN 50
#define FILE_NAME "users.txt"
#define TEMP_FILE "temp.txt"

enum CrudOperation
{
    ADD_USER = 1,
    DISPLAY_USERS = 2,
    UPDATE_USER = 3,
    DELETE_USER = 4,
    EXIT_PROGRAM = 5
};

struct User
{
    int id;
    char name[MAX_LEN];
    int age;
};

FILE *openFile(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (file == NULL)
    {
        printf("Error opening file '%s' with mode '%s'!\n", filename, mode);
    }
    return file;
}

void closeFile(FILE *file)
{
    if (file != NULL)
    {
        if (fclose(file) != 0)
        {
            printf("Error closing file.\n");
        }
    }
}

void readUserData(struct User *user, int isNewUser)
{
    if (isNewUser)
    {
        printf("Enter unique ID: ");
        scanf("%d", &user->id);
    }
    else
    {
        printf("User found (Current Name: %s, Current Age: %d).\n", user->name, user->age);
        printf("Enter a new name and age.\n");
    }

    printf("Enter Name (no spaces): ");
    scanf("%s", user->name);
    printf("Enter Age: ");
    scanf("%d", &user->age);
}

void addUser()
{
    FILE *file = openFile(FILE_NAME, "a");

    if (file != NULL)
    {
        struct User newUser;
        readUserData(&newUser, 1);
        fprintf(file, "%d %s %d\n", newUser.id, newUser.name, newUser.age);
        closeFile(file);
        printf("\nUser added. \n");
    }
}

void displayUsers()
{
    FILE *file = openFile(FILE_NAME, "r");
    if (file == NULL)
    {
        printf("\nNo users found. File '%s' does not exist.\n", FILE_NAME);
    }
    else
    {
        struct User user;
        printf("ID\tName\t\tAge\n");
        while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) == 3)
        {
            printf("%d\t%s\t\t%d\n", user.id, user.name, user.age);
        }
        closeFile(file);
    }
}

void updateUser()
{
    int targetID;
    printf("Enter the ID of the user to update: ");
    scanf("%d", &targetID);

    FILE *originalFile = openFile(FILE_NAME, "r");
    if (originalFile == NULL)
    {
        printf("Error: Original file not found.\n");
        return;
    }

    FILE *tempFile = openFile(TEMP_FILE, "w");

    if (tempFile == NULL)
    {
        printf("Error: Could not open temporary file.\n");
        closeFile(originalFile);
        return;
    }

    struct User user;
    int found = 0;
    while (fscanf(originalFile, "%d %s %d", &user.id, user.name, &user.age) == 3)
    {
        if (user.id == targetID)
        {
            readUserData(&user, 0);
            fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
            found = 1;
        }
        else
        {
            fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
        }
    }

    closeFile(originalFile);
    closeFile(tempFile);

    if (found)
    {
        remove(FILE_NAME);
        rename(TEMP_FILE, FILE_NAME);
        printf("\nUser ID %d updated successfully.\n", targetID);
    }
    else
    {
        printf("\nUser with ID %d not found or update failed.\n", targetID);
        remove(TEMP_FILE);
    }
}

void deleteUser()
{
    int targetID;
    printf("Enter the ID to delete: ");
    scanf("%d", &targetID);

    FILE *originalFile = openFile(FILE_NAME, "r");
    if (originalFile == NULL)
    {
        printf("Error: Original file not found.\n");
        return;
    }

    FILE *tempFile = openFile(TEMP_FILE, "w");
    if (tempFile == NULL)
    {
        printf("Error: Could not open temporary file.\n");
        closeFile(originalFile);
        return;
    }

    struct User user;
    int found = 0;
    while (fscanf(originalFile, "%d %s %d", &user.id, user.name, &user.age) == 3)
    {
        if (user.id == targetID)
        {
            found = 1;
        }
        else
        {
            fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
        }
    }

    closeFile(originalFile);
    closeFile(tempFile);

    if (found)
    {
        remove(FILE_NAME);
        rename(TEMP_FILE, FILE_NAME);
        printf("\nUser ID %d deleted successfully.\n", targetID);
    }
    else
    {
        printf("\nUser with ID %d not found.\n", targetID);
        remove(TEMP_FILE);
    }
}

int main()
{
    int choice = 0;
    bool start = true;

    while (start)
    {
        printf("\n--- User Management Dashboard ---\n");
        printf("1. Add User \n");
        printf("2. Display All Users \n");
        printf("3. Update User \n");
        printf("4. Delete User \n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        fflush(stdout);

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }

        enum CrudOperation operation = (enum CrudOperation)choice;

        switch (operation)
        {
        case ADD_USER:
            addUser();
            break;
        case DISPLAY_USERS:
            displayUsers();
            break;
        case UPDATE_USER:
            updateUser();
            break;
        case DELETE_USER:
            deleteUser();
            break;
        case EXIT_PROGRAM:
            start = false;
            printf("\nExiting program.\n");
            break;
        default:
            printf("Invalid. Enter a number between 1 and 5.\n");
        }
    }
    return 0;
}
