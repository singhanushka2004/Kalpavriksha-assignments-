#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Product
{
    int id;
    char name[50];
    float price;
    int quantity;
} Product;

typedef enum MenuOptions
{
    ADD_PRODUCT = 1,
    DISPLAY_PRODUCTS,
    UPDATE_QUANTITY,
    SEARCH_BY_ID,
    SEARCH_BY_NAME,
    SEARCH_BY_PRICE,
    DELETE_PRODUCT,
    EXIT
} MenuOptions;

Product *inventory = NULL;
int productCount = 0;

void inputProductDetails(Product *ptr)
{
    printf("ID: ");
    scanf("%d", &ptr->id);
    printf("Name: ");
    scanf("%s", ptr->name);
    printf("Price: ");
    scanf("%f", &ptr->price);
    printf("Quantity: ");
    scanf("%d", &ptr->quantity);
}

void addProduct()
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

void displayProducts()
{
    printf("\n========= PRODUCT LIST =========\n");
    if (productCount == 0)
    {
        printf("The inventory is empty.\n");
        return;
    }
    for (int item = 0; item < productCount; item++)
    {
        printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               inventory[item].id,
               inventory[item].name,
               inventory[item].price,
               inventory[item].quantity);
    }
}

void updateQuantity()
{
    int targetId;
    int newQuantity;
    bool found = false;
    printf("Enter ID to update quantity:\n");
    scanf("%d", &targetId);

    for (int item = 0; item < productCount; item++)
    {
        if (inventory[item].id == targetId)
        {
            printf("Enter new Quantity: ");
            scanf("%d", &newQuantity);
            inventory[item].quantity =newQuantity;
            found = true;
            printf("Quantity updated successfully!\n");
            break;
        }
    }
    if (!found)
    {
        printf("Product with ID %d not found.\n", targetId);
    }
}

void searchById()
{
    int targetId;
    bool found = false;
    printf("Enter ID to search\n");
    scanf("%d", &targetId);

    for (int item = 0; item < productCount; item++)
    {
        if (inventory[item].id == targetId)
        {
            printf("Product Found:\n");
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[item].id,
                   inventory[item].name,
                   inventory[item].price,
                   inventory[item].quantity);
            found = true;
            break;
        }
    }
    if (!found)
    {
        printf("Product with ID %d not found.\n", targetId);
    }
}

void searchByName()
{
    char targetName[50];
    bool found = false;
    printf("Enter name to search (partial allowed)\n");
    scanf("%s", targetName);
    printf("Products Found:\n");

    for (int item = 0; item < productCount; item++)
    {
        if (strstr(inventory[item].name, targetName) != NULL)
        {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[item].id,
                   inventory[item].name,
                   inventory[item].price,
                   inventory[item].quantity);
            found = true;
        }
    }
    if (!found)
    {
        printf("product with name '%s' not found.\n", targetName);
    }
}

void searchByPrice()
{
    float minPrice;
    float maxPrice;
    bool found = false;
    printf("Enter minimum price:\n");
    scanf("%f", &minPrice);
    printf("Enter maximum price:\n");
    scanf("%f", &maxPrice);
    printf("Products in price range:\n");

    for (int item = 0; item < productCount; item++)
    {
        if (inventory[item].price >= minPrice &&
            inventory[item].price <= maxPrice)
        {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[item].id,
                   inventory[item].name,
                   inventory[item].price,
                   inventory[item].quantity);
            found = true;
        }
    }
    if (!found)
    {
        printf("No products found in the range $%.2f to $%.2f.\n", minPrice, maxPrice);
    }
}

void deleteProduct()
{
    int targetId;
    int targetIndex = -1;
    printf("Enter ID to delete:\n");
    scanf("%d", &targetId);

    for (int item = 0; item < productCount; item++)
    {
        if (inventory[item].id == targetId)
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
        printf("Product with ID %d not found.\n", targetId);
    }
}

void displayMenu()
{
    printf("\n========= INVENTORY MENU =========\n");
    printf("%d. Add New Product\n", ADD_PRODUCT);
    printf("%d. View All Products\n", DISPLAY_PRODUCTS);
    printf("%d. Update Quantity\n", UPDATE_QUANTITY);
    printf("%d. Search Product by ID\n", SEARCH_BY_ID);
    printf("%d. Search Product by Name\n", SEARCH_BY_NAME);
    printf("%d. Search Product by Price Range\n", SEARCH_BY_PRICE);
    printf("%d. Delete Product \n", DELETE_PRODUCT);
    printf("%d. Exit \n", EXIT);
}

int main()
{
    int initialCount;
    int choice;
    printf("Enter initial number of products: ");
    scanf("%d", &initialCount);
    productCount = initialCount;
    inventory = (Product *)calloc(productCount, sizeof(Product));

    for (int item = 0; item < productCount; item++)
    {
        printf("\nEnter details for product %d:\n", item + 1);
        inputProductDetails(&inventory[item]);
    }
    do
    {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case ADD_PRODUCT:
            addProduct();
            break;
        case DISPLAY_PRODUCTS:
            displayProducts();
            break;
        case UPDATE_QUANTITY:
            updateQuantity();
            break;
        case SEARCH_BY_ID:
            searchById();
            break;
        case SEARCH_BY_NAME:
            searchByName();
            break;
        case SEARCH_BY_PRICE:
            searchByPrice();
            break;
        case DELETE_PRODUCT:
            deleteProduct();
            break;
        case EXIT:
            printf("Memory released successfully. Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Enter a number between %d and %d.\n", ADD_PRODUCT, EXIT);
        }
    } while (choice != EXIT);

    free(inventory);
    inventory = NULL;
    return 0;
}
