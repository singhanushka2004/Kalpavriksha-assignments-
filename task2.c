#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 50
#define File "users.txt"
#define Temp_file "temp.txt"

enum CrudOperation {
    ADD_USER = 1,
    DISPLAY_USERS = 2,
    UPDATE_USER = 3,
    DELETE_USER = 4,
    EXIT_PROGRAM = 5
};

struct User {
    int id;
    char name[MAX_LEN];
    int age;
};

FILE* openFile(const char* filename, const char* mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        printf("Error opening file '%s' with mode '%s'!\n", filename, mode);
    }
}

void closeFile(FILE *file) {
    if (file != NULL) {
        if (fclose(file) != 0) {
            printf("Error closing file.\n");
        }
    }
}

void readUserData(struct User *user, int isNewUser) {
    if (isNewUser) {
        printf("Enter unique ID: ");
        scanf("%d", &user->id);
        printf("Enter Name: ");
        scanf("%s", user->name);
        printf("Enter Age: ");
        scanf("%d", &user->age); 
    } else {
        printf("User found (Current Name: %s, Current Age: %d).\n", user->name, user->age);
        printf("Enter NEW Name: ");
        scanf("%s", user->name);
        printf("Enter NEW Age: ");
        scanf("%d", &user->age); 
    }
}

void addUser() {
    FILE *file = openFile(File, "a");
    if (file == NULL) {
    } else {
        struct User newUser;
        readUserData(&newUser, 1);
        fprintf(file, "%d %s %d\n", newUser.id, newUser.name, newUser.age);
        closeFile(file);
        printf("\nUser added successfully!\n");
    }
}

void displayUsers() {
    FILE *file = openFile(File, "r");
    if (file == NULL) {
        printf("\nNo users found. File '%s' does not exist.\n", File);
    } else {
        struct User user;
        printf("ID\tName\t\tAge\n");
        while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) == 3) {
            printf("%d\t%s\t\t%d\n", user.id, user.name, user.age);
        }
        closeFile(file);
    }
}

void updateUser() {
    int targetID;
    printf("Enter the ID of the user to update: ");
    scanf("%d", &targetID); 
    FILE *originalFile = openFile(File, "r");
    FILE *tempFile = openFile(Temp_file, "w");
    
    if (originalFile == NULL || tempFile == NULL) {
        printf("Error: Could not open file.\n");
        closeFile(originalFile); 
        closeFile(tempFile);
        if (originalFile == NULL && tempFile != NULL) remove(Temp_file);
    } else {
        struct User user;
        int found = 0;
        while (fscanf(originalFile, "%d %s %d", &user.id, user.name, &user.age) == 3) {
            if (user.id == targetID) {
                readUserData(&user, 0);
                fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
                found = 1;
                printf("User ID %d updated successfully.\n", targetID);
            } else {
                fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
            }
        }
        closeFile(originalFile);
        closeFile(tempFile);
        if (found) {
            remove(File);
            rename(Temp_file, File); 
        } else {
            printf("User with ID %d not found or update failed.\n", targetID);
            remove(Temp_file);
        }
    }
} 

void deleteUser() {
    int targetID;
    printf("Enter the ID to delete: ");
    scanf("%d", &targetID);
    FILE *originalFile = openFile(File, "r");
    FILE *tempFile = openFile(Temp_file, "w");

    if (originalFile == NULL || tempFile == NULL) {
        printf("Error: Could not open file.\n");
        closeFile(originalFile);
        closeFile(tempFile);
        if (originalFile == NULL && tempFile != NULL) remove(Temp_file);
    } else {
        struct User user;
        int found = 0;
        while (fscanf(originalFile, "%d %s %d", &user.id, user.name, &user.age) == 3) {
            if (user.id == targetID) {
                found = 1;
            } else {
                fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
            }
        }
        closeFile(originalFile);
        closeFile(tempFile);
        if (found) {
            remove(File);
            rename(Temp_file, File);
            printf("User ID %d deleted successfully.\n", targetID);
        } else {
            printf("User with ID %d not found.\n", targetID);
            remove(Temp_file);
        }
    }
}

int main() {
    int choice = 0;
    while (choice <= 5) {
        printf("Choose an operation:\n");
        printf("1. Add User \n");
        printf("2. Display All Users \n");
        printf("3. Update User \n");
        printf("4. Delete User \n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        fflush(stdout); 
        scanf("%d", &choice);

        enum CrudOperation operation = (enum CrudOperation)choice;
        switch (operation) {
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
                printf("\nExit program.\n");
                break;
            default:
                printf("Invalid. Enter a number between 1 and 5.\n");
        }
        if (choice == 5) {
            break;
        }
    }
    return 0;
}