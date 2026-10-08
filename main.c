#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FOODS 100
#define MAX_NAME 50
#define MAX_CATEGORY 30
#define MAX_ORDERS 100

/* =========================
   STRUCTURES
   ========================= */

typedef struct {
    int id;
    char name[MAX_NAME];
    char category[MAX_CATEGORY];
    float price;
} Food;


/* Linked List Node for Cart */
typedef struct CartNode {
    int foodId;
    int quantity;
    struct CartNode *next;
} CartNode;


/* Order structure */
typedef struct {
    int orderId;
    char customerName[50];
    float totalAmount;
    char status[30];
} Order;


/* Queue Node */
typedef struct QueueNode {
    Order order;
    struct QueueNode *next;
} QueueNode;


/* =========================
   GLOBAL VARIABLES
   ========================= */

Food foods[MAX_FOODS];

int foodCount = 8;

int nextFoodId = 9;
int nextOrderId = 1001;


/* Initial food menu */
void initializeFoods()
{
    foods[0] = (Food){1, "Burger", "Fast Food", 150};
    foods[1] = (Food){2, "Pizza", "Fast Food", 250};
    foods[2] = (Food){3, "Sandwich", "Snacks", 120};
    foods[3] = (Food){4, "French Fries", "Snacks", 100};
    foods[4] = (Food){5, "Pasta", "Italian", 180};
    foods[5] = (Food){6, "Biryani", "Indian", 220};
    foods[6] = (Food){7, "Cold Drink", "Beverage", 60};
    foods[7] = (Food){8, "Ice Cream", "Dessert", 90};
}


/* =========================
   FILE HANDLING
   ========================= */

void saveFoods()
{
    FILE *file = fopen("foods.dat", "wb");

    if (file == NULL) {
        printf("\nUnable to save food data.\n");
        return;
    }

    fwrite(&foodCount, sizeof(int), 1, file);
    fwrite(&foods, sizeof(Food), foodCount, file);

    fclose(file);
}


void loadFoods()
{
    FILE *file = fopen("foods.dat", "rb");

    if (file == NULL) {
        initializeFoods();
        saveFoods();
        return;
    }

    fread(&foodCount, sizeof(int), 1, file);
    fread(&foods, sizeof(Food), foodCount, file);

    fclose(file);

    nextFoodId = 1;

    for (int i = 0; i < foodCount; i++) {
        if (foods[i].id >= nextFoodId) {
            nextFoodId = foods[i].id + 1;
        }
    }
}


/* =========================
   DISPLAY MENU
   ========================= */

void displayFoods()
{
    if (foodCount == 0) {
        printf("\nNo food available.\n");
        return;
    }

    printf("\n");
    printf("============================================================\n");
    printf("                     FOOD MENU\n");
    printf("============================================================\n");

    printf("%-5s %-25s %-20s %-10s\n",
           "ID", "Food Name", "Category", "Price");

    printf("------------------------------------------------------------\n");

    for (int i = 0; i < foodCount; i++) {

        printf("%-5d %-25s %-20s Rs. %.2f\n",
               foods[i].id,
               foods[i].name,
               foods[i].category,
               foods[i].price);
    }

    printf("============================================================\n");
}


/* =========================
   SEARCH FOOD
   ========================= */

void searchFood()
{
    char searchName[MAX_NAME];
    int found = 0;

    printf("\nEnter food name to search: ");
    scanf(" %[^\n]", searchName);

    printf("\nSearch Results:\n");

    for (int i = 0; i < foodCount; i++) {

        if (strstr(foods[i].name, searchName) != NULL) {

            printf("\nID       : %d", foods[i].id);
            printf("\nName     : %s", foods[i].name);
            printf("\nCategory : %s", foods[i].category);
            printf("\nPrice    : Rs. %.2f\n", foods[i].price);

            found = 1;
        }
    }

    if (!found) {
        printf("\nFood not found.\n");
    }
}


/* =========================
   SORT FOOD BY PRICE
   ========================= */

void sortFoodByPrice()
{
    Food temp;

    for (int i = 0; i < foodCount - 1; i++) {

        for (int j = 0; j < foodCount - i - 1; j++) {

            if (foods[j].price > foods[j + 1].price) {

                temp = foods[j];
                foods[j] = foods[j + 1];
                foods[j + 1] = temp;
            }
        }
    }

    printf("\nFood sorted by price successfully.\n");

    displayFoods();
}


/* =========================
   FIND FOOD BY ID
   ========================= */

int findFoodIndex(int id)
{
    for (int i = 0; i < foodCount; i++) {

        if (foods[i].id == id) {
            return i;
        }
    }

    return -1;
}


/* =========================
   CART - LINKED LIST
   ========================= */

void addToCart(CartNode **head)
{
    int foodId;
    int quantity;

    displayFoods();

    printf("\nEnter Food ID: ");
    scanf("%d", &foodId);

    int index = findFoodIndex(foodId);

    if (index == -1) {
        printf("\nInvalid Food ID.\n");
        return;
    }

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    if (quantity <= 0) {
        printf("\nInvalid quantity.\n");
        return;
    }


    /* Check if food already exists */
    CartNode *current = *head;

    while (current != NULL) {

        if (current->foodId == foodId) {

            current->quantity += quantity;

            printf("\nQuantity updated in cart.\n");

            return;
        }

        current = current->next;
    }


    /* Create new node */

    CartNode *newNode =
        (CartNode *)malloc(sizeof(CartNode));

    if (newNode == NULL) {
        printf("\nMemory allocation failed.\n");
        return;
    }

    newNode->foodId = foodId;
    newNode->quantity = quantity;
    newNode->next = NULL;


    /* Insert at beginning */

    newNode->next = *head;
    *head = newNode;

    printf("\n%s added to cart successfully.\n",
           foods[index].name);
}


/* =========================
   VIEW CART
   ========================= */

float calculateCartTotal(CartNode *head)
{
    float total = 0;

    CartNode *current = head;

    while (current != NULL) {

        int index = findFoodIndex(current->foodId);

        if (index != -1) {

            total +=
                foods[index].price *
                current->quantity;
        }

        current = current->next;
    }

    return total;
}


void viewCart(CartNode *head)
{
    if (head == NULL) {

        printf("\nYour cart is empty.\n");
        return;
    }

    printf("\n");
    printf("============================================================\n");
    printf("                         YOUR CART\n");
    printf("============================================================\n");

    printf("%-20s %-10s %-10s %-10s\n",
           "Food", "Price", "Quantity", "Subtotal");

    printf("------------------------------------------------------------\n");

    CartNode *current = head;

    while (current != NULL) {

        int index = findFoodIndex(current->foodId);

        if (index != -1) {

            float subtotal =
                foods[index].price *
                current->quantity;

            printf("%-20s Rs.%-7.2f %-10d Rs.%.2f\n",
                   foods[index].name,
                   foods[index].price,
                   current->quantity,
                   subtotal);
        }

        current = current->next;
    }

    printf("------------------------------------------------------------\n");

    printf("Total Amount: Rs. %.2f\n",
           calculateCartTotal(head));

    printf("============================================================\n");
}


/* =========================
   REMOVE FROM CART
   ========================= */

void removeFromCart(CartNode **head)
{
    if (*head == NULL) {

        printf("\nCart is empty.\n");
        return;
    }

    int foodId;

    viewCart(*head);

    printf("\nEnter Food ID to remove: ");
    scanf("%d", &foodId);

    CartNode *current = *head;
    CartNode *previous = NULL;

    while (current != NULL) {

        if (current->foodId == foodId) {

            if (previous == NULL) {
                *head = current->next;
            }
            else {
                previous->next = current->next;
            }

            free(current);

            printf("\nFood removed from cart.\n");

            return;
        }

        previous = current;
        current = current->next;
    }

    printf("\nFood not found in cart.\n");
}


/* =========================
   FREE CART
   ========================= */

void freeCart(CartNode **head)
{
    CartNode *current = *head;

    while (current != NULL) {

        CartNode *temp = current;

        current = current->next;

        free(temp);
    }

    *head = NULL;
}


/* =========================
   QUEUE
   ========================= */

QueueNode *front = NULL;
QueueNode *rear = NULL;


void enqueue(Order order)
{
    QueueNode *newNode =
        (QueueNode *)malloc(sizeof(QueueNode));

    if (newNode == NULL) {
        printf("\nMemory allocation failed.\n");
        return;
    }

    newNode->order = order;
    newNode->next = NULL;


    if (rear == NULL) {

        front = rear = newNode;
    }
    else {

        rear->next = newNode;
        rear = newNode;
    }
}


Order dequeue()
{
    Order emptyOrder = {0, "", 0, ""};

    if (front == NULL) {

        return emptyOrder;
    }

    QueueNode *temp = front;

    Order order = temp->order;

    front = front->next;

    if (front == NULL) {
        rear = NULL;
    }

    free(temp);

    return order;
}


/* =========================
   ORDER PROCESSING
   ========================= */

void placeOrder(CartNode **cart)
{
    if (*cart == NULL) {

        printf("\nYour cart is empty.\n");

        return;
    }

    char customerName[50];

    printf("\nEnter your name: ");
    scanf(" %[^\n]", customerName);

    float total = calculateCartTotal(*cart);

    Order order;

    order.orderId = nextOrderId++;

    strcpy(order.customerName, customerName);

    order.totalAmount = total;

    strcpy(order.status, "Order Placed");


    /* Add order to queue */

    enqueue(order);


    /* Display bill */

    printf("\n");
    printf("============================================================\n");
    printf("                     ORDER CONFIRMED\n");
    printf("============================================================\n");

    printf("Order ID      : %d\n", order.orderId);
    printf("Customer Name : %s\n", order.customerName);

    printf("\nOrdered Items:\n");

    CartNode *current = *cart;

    while (current != NULL) {

        int index = findFoodIndex(current->foodId);

        if (index != -1) {

            float subtotal =
                foods[index].price *
                current->quantity;

            printf("%-20s x %-3d = Rs. %.2f\n",
                   foods[index].name,
                   current->quantity,
                   subtotal);
        }

        current = current->next;
    }

    printf("\nTotal Amount: Rs. %.2f\n",
           total);

    printf("Status      : %s\n",
           order.status);

    printf("============================================================\n");

    printf("\nOrder placed successfully!\n");


    /* Clear cart */

    freeCart(cart);
}


/* =========================
   VIEW QUEUE
   ========================= */

void viewOrders()
{
    if (front == NULL) {

        printf("\nNo pending orders.\n");
        return;
    }

    QueueNode *current = front;

    printf("\n");
    printf("============================================================\n");
    printf("                     PENDING ORDERS\n");
    printf("============================================================\n");

    while (current != NULL) {

        printf("\nOrder ID : %d",
               current->order.orderId);

        printf("\nCustomer : %s",
               current->order.customerName);

        printf("\nAmount   : Rs. %.2f",
               current->order.totalAmount);

        printf("\nStatus   : %s\n",
               current->order.status);

        current = current->next;
    }

    printf("============================================================\n");
}


/* =========================
   PROCESS NEXT ORDER
   ========================= */

void processNextOrder()
{
    if (front == NULL) {

        printf("\nNo orders to process.\n");
        return;
    }

    Order order = dequeue();

    printf("\n");
    printf("============================================================\n");
    printf("                    ORDER PROCESSED\n");
    printf("============================================================\n");

    printf("Order ID : %d\n", order.orderId);
    printf("Customer : %s\n", order.customerName);
    printf("Amount   : Rs. %.2f\n", order.totalAmount);

    printf("Status   : Preparing\n");

    printf("============================================================\n");
}


/* =========================
   ADMIN - ADD FOOD
   ========================= */

void addFood()
{
    if (foodCount >= MAX_FOODS) {

        printf("\nFood storage is full.\n");
        return;
    }

    Food newFood;

    newFood.id = nextFoodId++;

    printf("\nEnter Food Name: ");
    scanf(" %[^\n]", newFood.name);

    printf("Enter Category: ");
    scanf(" %[^\n]", newFood.category);

    printf("Enter Price: ");
    scanf("%f", &newFood.price);

    if (newFood.price <= 0) {

        printf("\nInvalid price.\n");
        return;
    }

    foods[foodCount] = newFood;

    foodCount++;

    saveFoods();

    printf("\nFood added successfully.\n");
}


/* =========================
   ADMIN - DELETE FOOD
   ========================= */

void deleteFood()
{
    if (foodCount == 0) {

        printf("\nNo food available.\n");
        return;
    }

    int id;

    displayFoods();

    printf("\nEnter Food ID to delete: ");
    scanf("%d", &id);

    int index = findFoodIndex(id);

    if (index == -1) {

        printf("\nFood not found.\n");
        return;
    }


    for (int i = index; i < foodCount - 1; i++) {

        foods[i] = foods[i + 1];
    }

    foodCount--;

    saveFoods();

    printf("\nFood deleted successfully.\n");
}


/* =========================
   ADMIN - UPDATE FOOD
   ========================= */

void updateFood()
{
    displayFoods();

    int id;

    printf("\nEnter Food ID to update: ");
    scanf("%d", &id);

    int index = findFoodIndex(id);

    if (index == -1) {

        printf("\nFood not found.\n");
        return;
    }

    printf("\nEnter new Food Name: ");
    scanf(" %[^\n]", foods[index].name);

    printf("Enter new Category: ");
    scanf(" %[^\n]", foods[index].category);

    printf("Enter new Price: ");
    scanf("%f", &foods[index].price);

    saveFoods();

    printf("\nFood updated successfully.\n");
}


/* =========================
   ADMIN LOGIN
   ========================= */

int adminLogin()
{
    char username[30];
    char password[30];

    printf("\n");
    printf("====================================\n");
    printf("           ADMIN LOGIN\n");
    printf("====================================\n");

    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 &&
        strcmp(password, "1234") == 0) {

        printf("\nLogin successful!\n");

        return 1;
    }

    printf("\nInvalid username or password.\n");

    return 0;
}


/* =========================
   ADMIN MENU
   ========================= */

void adminMenu()
{
    if (!adminLogin()) {
        return;
    }

    int choice;

    do {

        printf("\n");
        printf("====================================\n");
        printf("            ADMIN PANEL\n");
        printf("====================================\n");

        printf("1. View Food Menu\n");
        printf("2. Add Food\n");
        printf("3. Update Food\n");
        printf("4. Delete Food\n");
        printf("5. View Pending Orders\n");
        printf("6. Process Next Order\n");
        printf("7. Logout\n");

        printf("====================================\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                displayFoods();
                break;

            case 2:
                addFood();
                break;

            case 3:
                updateFood();
                break;

            case 4:
                deleteFood();
                break;

            case 5:
                viewOrders();
                break;

            case 6:
                processNextOrder();
                break;

            case 7:
                printf("\nAdmin logged out.\n");
                break;

            default:
                printf("\nInvalid choice.\n");
        }

    } while (choice != 7);
}


/* =========================
   CUSTOMER MENU
   ========================= */

void customerMenu()
{
    CartNode *cart = NULL;

    int choice;

    do {

        printf("\n");
        printf("============================================\n");
        printf("        FOOD ORDERING MANAGEMENT SYSTEM\n");
        printf("============================================\n");

        printf("1. View Food Menu\n");
        printf("2. Search Food\n");
        printf("3. Sort Food By Price\n");
        printf("4. Add Food To Cart\n");
        printf("5. View Cart\n");
        printf("6. Remove Food From Cart\n");
        printf("7. Place Order\n");
        printf("8. Back To Main Menu\n");

        printf("============================================\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                displayFoods();
                break;

            case 2:
                searchFood();
                break;

            case 3:
                sortFoodByPrice();
                break;

            case 4:
                addToCart(&cart);
                break;

            case 5:
                viewCart(cart);
                break;

            case 6:
                removeFromCart(&cart);
                break;

            case 7:
                placeOrder(&cart);
                break;

            case 8:
                freeCart(&cart);
                printf("\nReturning to main menu...\n");
                break;

            default:
                printf("\nInvalid choice.\n");
        }

    } while (choice != 8);
}


/* =========================
   MAIN FUNCTION
   ========================= */

int main()
{
    loadFoods();

    int choice;

    do {

        printf("\n");
        printf("================================================\n");
        printf("             FOODIE EXPRESS\n");
        printf("       FOOD ORDERING MANAGEMENT SYSTEM\n");
        printf("================================================\n");

        printf("1. Customer\n");
        printf("2. Admin\n");
        printf("3. Exit\n");

        printf("================================================\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                customerMenu();
                break;

            case 2:
                adminMenu();
                break;

            case 3:
                printf("\nThank you for using Foodie Express!\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while (choice != 3);

    return 0;
}