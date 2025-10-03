#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 50
#define File "users.txt"
#define Temp_file "temp.txt"
struct User {
    int id;
    char name[MAX_LEN];
    int age;
};
//adding new user
void addUser() {
    FILE *file = fopen(File, "a");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    struct User newUser;
    printf("Enter unique ID: ");
    scanf("%d", &newUser.id);
    printf("Enter Name: ", MAX_LEN - 1);
    scanf("%s", newUser.name); 
    printf("Enter Age: ");
    scanf("%d", &newUser.age); 
    fprintf(file, "%d %s %d\n", newUser.id, newUser.name, newUser.age);
    fclose(file);
    printf("\nUser added successfully!\n");
}
//display user details
void displayUsers() {
    FILE *file = fopen(File, "r");
    if (file == NULL) {
        printf("\nNo users found. File '%s' does not exist.\n", File);
        return;
    }
    struct User user;
    printf("ID\tName\t\tAge\n");
    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        printf("%d\t%s\t\t%d\n", user.id, user.name, user.age);
    }
    fclose(file);
}
//modify
void updateUser() {
    int targetID;
    printf("Enter the ID of the user to update: ");
    scanf("%d", &targetID); 
    FILE *originalFile = fopen(File, "r");
    FILE *tempFile = fopen(Temp_file, "w");
    if (originalFile == NULL || tempFile == NULL) {
        printf("Error: Could not open file.\n");
        if (originalFile) fclose(originalFile);
        if (tempFile) fclose(tempFile);
        return;
    }
    struct User user;
    int found = 0;
    while (fscanf(originalFile, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        if (user.id == targetID) {
            printf("User found (Current Name: %s, Current Age: %d).\n", user.name, user.age);
            printf("Enter NEW Name: ");
            scanf("%s", user.name);
            printf("Enter NEW Age: ");
            scanf("%d", &user.age); 
            fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
            found = 1;
            printf("User ID %d updated successfully.\n", targetID);
        } else {
            fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
        }
    }
    fclose(originalFile);
    fclose(tempFile);
    // Finalizing the update
    if (found) {
        remove(File);
        rename(Temp_file, File); 
    } else {
        printf("User with ID %d not found or update failed.\n", targetID);
        remove(Temp_file);
    }
}
//delete 
void deleteUser() {
    int targetID;
    printf("Enter the ID to delete: ");
    scanf("%d", &targetID);
    FILE *originalFile = fopen(File, "r");
    FILE *tempFile = fopen(Temp_file, "w");
    if (originalFile == NULL || tempFile == NULL) {
        printf("Error: Could not open file.\n");
        if (originalFile) fclose(originalFile);
        if (tempFile) fclose(tempFile);
        return;
    }
    struct User user;
    int found = 0;
    while (fscanf(originalFile, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        if (user.id == targetID) {
            found = 1;
        } else {
            fprintf(tempFile, "%d %s %d\n", user.id, user.name, user.age);
        }
    }
    fclose(originalFile);
    fclose(tempFile);
    if (found) {
        remove(File);
        rename(Temp_file, File);
        printf("User ID %d deleted successfully.\n", targetID);
    } else {
        printf("User with ID %d not found.\n", targetID);
        remove(Temp_file);
    }
}
//dashboard
int main() {
    int choice=0;
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
        switch (choice) {
            case 1:
                addUser();
                break;
            case 2:
                displayUsers();
                break;
            case 3:
                updateUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                printf("\nExiting program!\n");
                break;
            default:
                printf("Invalid choice.Enter a number between 1 and 5.\n");
        }
                if (choice == 5) {
            break;
        }
    }
    return 0;
}
