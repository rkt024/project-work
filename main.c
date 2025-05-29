#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

// Function Prototypes
void clearScreen();
void waitForEnter();
void initializeDatabase(sqlite3 *db);
void showMainMenu(sqlite3 *db);
void propertyMenu(sqlite3 *db);
void roomManagementMenu(sqlite3 *db);
void flatManagementMenu(sqlite3 *db);

// Helper function to clear the input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("rental_management.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Initialize the database with tables
    initializeDatabase(db);

    // Main menu loop
    while (1) {
        showMainMenu(db);
    }

    sqlite3_close(db);  // Close the database connection at the end of the program
    return 0;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter() {
    printf("Press Enter to continue...");
    getchar();  // Wait for user input (press Enter)
}

void initializeDatabase(sqlite3 *db) {
    // SQL queries to create tables if they don't exist
    const char *create_rooms = "CREATE TABLE IF NOT EXISTS rooms (" 
                               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                               "number TEXT NOT NULL UNIQUE,"
                               "description TEXT);";
    
    const char *create_flats = "CREATE TABLE IF NOT EXISTS flats (" 
                               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                               "number TEXT NOT NULL UNIQUE,"
                               "description TEXT);";
    
    // Execute SQL queries to create tables
    int rc = sqlite3_exec(db, create_rooms, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return; // Stop further execution if there is an error
    }
    
    rc = sqlite3_exec(db, create_flats, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return; // Stop further execution if there is an error
    }
}

void showMainMenu(sqlite3 *db) {
    clearScreen();
    printf("\n=== Rental Management System ===\n\n");
    printf("1. Property Structure Management\n");
    printf("2. Tenant Management\n");
    printf("3. Rent and Utility Management\n");
    printf("4. Reports\n");
    printf("5. Data Export (CSV)\n");
    printf("0. Exit\n");
    
    printf("\nEnter your choice [0-5]: ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter a number.\n");
        waitForEnter();
        return;
    }
    clearInputBuffer();

    switch (choice) {
        case 0:
            printf("Exiting program...\n");
            sqlite3_close(db);  // Close the database connection
            exit(0);  // Exit the program cleanly
        case 1:
            propertyMenu(db);
            break;
        case 2:
            printf("Tenant Management - Coming soon!\n");
            waitForEnter();
            break;
        case 3:
            printf("Rent and Utility Management - Coming soon!\n");
            waitForEnter();
            break;
        case 4:
            printf("Reports - Coming soon!\n");
            waitForEnter();
            break;
        case 5:
            printf("Data Export - Coming soon!\n");
            waitForEnter();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            waitForEnter();
    }
}

void propertyMenu(sqlite3 *db) {
    int choice;

    while (1) {
        clearScreen();
        printf("\n=== PROPERTY STRUCTURE MANAGEMENT ===\n\n");
        printf("1. Room Management\n");
        printf("2. Flat Management\n");
        printf("0. Return to Main Menu\n");
        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number between 0 and 2.\n");
            waitForEnter();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                roomManagementMenu(db);
                break;
            case 2:
                flatManagementMenu(db);
                break;
            case 0:
                return;
            default:
                printf("Invalid choice. Please enter a number between 0 and 2.\n");
                waitForEnter();
        }
    }
}

void roomManagementMenu(sqlite3 *db) {
    int choice;

    while (1) {
        clearScreen();
        printf("\n=== ROOM MANAGEMENT ===\n\n");
        printf("1. Add New Room\n");
        printf("2. View All Rooms\n");
        printf("3. Edit Room\n");
        printf("4. Delete Room\n");
        printf("0. Return to Previous Menu\n");
        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number.\n");
            waitForEnter();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                printf("Add New Room - Coming soon!\n");
                waitForEnter();
                break;
            case 2:
                printf("View All Rooms - Coming soon!\n");
                waitForEnter();
                break;
            case 3:
                printf("Edit Room - Coming soon!\n");
                waitForEnter();
                break;
            case 4:
                printf("Delete Room - Coming soon!\n");
                waitForEnter();
                break;
            case 0:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                waitForEnter();
        }
    }
}

void flatManagementMenu(sqlite3 *db) {
    int choice;

    while (1) {
        clearScreen();
        printf("\n=== FLAT MANAGEMENT ===\n\n");
        printf("1. Add New Flat\n");
        printf("2. View All Flats\n");
        printf("3. Edit Flat\n");
        printf("4. Delete Flat\n");
        printf("5. Assign Rooms to Flat\n");
        printf("0. Return to Previous Menu\n");
        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number.\n");
            waitForEnter();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                printf("Add New Flat - Coming soon!\n");
                waitForEnter();
                break;
            case 2:
                printf("View All Flats - Coming soon!\n");
                waitForEnter();
                break;
            case 3:
                printf("Edit Flat - Coming soon!\n");
                waitForEnter();
                break;
            case 4:
                printf("Delete Flat - Coming soon!\n");
                waitForEnter();
                break;
            case 5:
                printf("Assign Rooms to Flat - Coming soon!\n");
                waitForEnter();
                break;
            case 0:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                waitForEnter();
        }
    }
}
