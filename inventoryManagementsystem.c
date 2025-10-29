#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Product
{
    int productID;
    char productName[50];
    float productPrice;
    int productQuantity;
} Product;

Product *inventory = NULL;
int productCount = 0;

void inputProductDetails(Product *p)
{
    printf("Product ID: ");
    scanf("%d", &p->productID);
    printf("Product Name: ");
    scanf("%s", p->productName);
    printf("Product Price: ");
    scanf("%f", &p->productPrice);
    printf("Product Quantity: ");
    scanf("%d", &p->productQuantity);
}

void displayProduct(const Product *p)
{
    printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
           p->productID, p->productName, p->productPrice, p->productQuantity);
}

void addNewProduct()
{
    printf("\nEnter new product details:\n");
    productCount++;
    Product *newInventory = (Product *)realloc(inventory, productCount * sizeof(Product));
    if (newInventory == NULL)
    {
        printf("ERROR: Failed to allocate memory for the new product.\n");
        productCount--;
        return;
    }
    inventory = newInventory;
    inputProductDetails(&inventory[productCount - 1]);
    printf("Product added successfully!\n");
}

void viewAllProducts()
{
    printf("\n========= PRODUCT LIST =========\n");
    if (productCount == 0)
    {
        printf("The inventory is empty.");
        return;
    }
    for (int item = 0; item < productCount; item++)
    {
        displayProduct(&inventory[item]);
    }
}

void updateQuantity()
{
    int targetID;
    int newQuantity;
    int found = 0;
    printf("Enter Product ID to update quantity:\n");
    scanf("%d", &targetID);
    for (int item = 0; item < productCount; item++)
    {
        if (inventory[item].productID == targetID)
        {
            printf("Enter new Quantity: ");
            scanf("%d", &newQuantity);
            inventory[item].productQuantity = newQuantity;
            found = 1;
            printf("Quantity updated successfully!\n");
            break;
        }
    }
    if (!found)
    {
        printf("Product with ID %d not found.\n", targetID);
    }
}

void searchById()
{
    int targetID;
    int found = 0;
    printf("Enter Product ID to search\n");
    scanf("%d", &targetID);
    for (int item = 0; item < productCount; item++)
    {
        if (inventory[item].productID == targetID)
        {
            printf("Product Found:\n");
            displayProduct(&inventory[item]);
            found = 1;
            break;
        }
    }
    if (!found)
    {
        printf("Product with ID %d not found.\n", targetID);
    }
}

void searchByName()
{
    char targetName[50];
    int found = 0;
    printf("Enter name to search (partial allowed)\n");
    scanf("%s", &targetName);
    printf("Products Found:\n");
    for (int item = 0; item < productCount; item++)
    {
        if (strstr(inventory[item].productName, targetName) != NULL)
        {
            displayProduct(&inventory[item]);
            found = 1;
        }
    }
    if (!found)
    {
        printf("product with name '%s' not found.\n", targetName);
    }
}

void searchByPrice()
{
    float minimumPrice;
    float maximumPrice;
    int found = 0;
    printf("Enter minimum price:\n");
    scanf("%f", &minimumPrice);
    printf("Enter maximum price:\n");
    scanf("%f", &maximumPrice);
    printf("Products in price range:\n");
    for (int item = 0; item < productCount; item++)
    {
        if (inventory[item].productPrice >= minimumPrice &&
            inventory[item].productPrice <= maximumPrice)
        {
            displayProduct(&inventory[item]);
            found = 1;
        }
    }
    if (!found)
    {
        printf("No products found in the range $%.2f to $%.2f.\n", minimumPrice, maximumPrice);
    }
}

void deleteProduct()
{
    int targetID;
    int targetIndex = -1;
    printf("Enter Product ID to delete:\n");
    scanf("%d", &targetID);
    for (int item = 0; item < productCount; item++)
    {
        if (inventory[item].productID == targetID)
        {
            targetIndex = item;
            break;
        }
    }
    if (targetIndex != -1)
    {
        for (int i = targetIndex; i < productCount - 1; i++)
        {
            inventory[i] = inventory[i + 1];
        }
        productCount--;
        if (productCount == 0)
        {
            free(inventory);
            inventory = NULL;
        }
        else
        {
            Product *newInventory = (Product *)realloc(inventory, productCount * sizeof(Product));
            if (newInventory != NULL)
            {
                inventory = newInventory;
            }
        }
        printf("Product deleted successfully!\n");
    }
    else
    {
        printf("Product with ID %d not found.\n", targetID);
    }
}

void displayMenu()
{

    printf("\n========= INVENTORY MENU =========\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product \n");
    printf("8. Exit \n");
}

int main()
{
    int initialCount = 0;
    int choice;
    printf("Enter initial number of products: ");
    scanf("%d", &initialCount);
    productCount = initialCount;
    inventory = (Product *)calloc(productCount, sizeof(Product));
    for (int item = 0; item < productCount; item++) {
        printf("\nEnter details for product %d:\n", item + 1);
        inputProductDetails(&inventory[item]);
    }
        do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addNewProduct();
            break;
        case 2:
            viewAllProducts();
            break;
        case 3:
            updateQuantity();
            break;
        case 4:
            searchById();
            break;
        case 5:
            searchByName();
            break;
        case 6:
            searchByPrice();
            break;
        case 7:
            deleteProduct();
            break;
        case 8:
            printf("Memory released successfully. Exiting program.\n");
            break;
        default:
            printf("Invalid choice.enter a number between 1 and 8.\n");
        }
    } while (choice != 8);
    free(inventory);
    inventory = NULL;
    return 0;
}

