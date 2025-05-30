/*
 * Simple Rental Management System
 * Console-based application for managing multi-story house rentals
 * Features: SQLite database, tenant management, payment tracking, basic reporting
 */


#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <ctype.h>

// Function Prototypes
void clearScreen();
void waitForEnter();
void initializeDatabase(sqlite3 *db);
void showMainMenu(sqlite3 *db);
void propertyMenu(sqlite3 *db);
void flatManagementMenu(sqlite3 *db);

// Function prototypes for room management
void roomManagementMenu(sqlite3 *db);
void addNewRoom(sqlite3 *db);
void viewAllRooms(sqlite3 *db);
void editRoom(sqlite3 *db);
void deleteRoom(sqlite3 *db);

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
    // clearScreen();

    printf("==============================================\n");
    printf("    SIMPLE RENTAL MANAGEMENT SYSTEM\n");
    printf("    Multi-Story House Management\n");
    printf("==============================================\n\n");

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
/* 
// Room Management Functions
void roomManagementMenu(sqlite3 *db) {
    int choice;

    while (1) {
        clearScreen();
        printf("\n==============================================\n");
        printf("           ROOM MANAGEMENT\n");
        printf("==============================================\n");
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
        // printf("After scanf, choice is: %d\n", choice);  // Debugging print
        clearInputBuffer();

        switch (choice) {
            case 1:
                addNewRoom(db);
                break;
            case 2:
                // printf("Calling viewAllRooms...\n");
                viewAllRooms(db);
                break;
            case 3:
                editRoom(db);
                break;
            case 4:
                deleteRoom(db);
                break;
            case 0:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                waitForEnter();
        }
    }
}
 */
// Replace your roomManagementMenu function with this debug version:

void roomManagementMenu(sqlite3 *db) {
    int choice;

    while (1) {
        clearScreen();
        printf("\n==============================================\n");
        printf("           ROOM MANAGEMENT\n");
        printf("==============================================\n");
        printf("1. Add New Room\n");
        printf("2. View All Rooms\n");
        printf("3. Edit Room\n");
        printf("4. Delete Room\n");
        printf("0. Return to Previous Menu\n");
        printf("\nEnter your choice: ");

        /* int scanf_result = scanf("%d", &choice);
        printf("DEBUG: scanf returned: %d\n", scanf_result);
        printf("DEBUG: choice value is: %d\n", choice); */
        
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number.\n");
            waitForEnter();
            continue;
        }
        clearInputBuffer();


        switch (choice) {
            case 1:
                addNewRoom(db);
                break;
            case 2:
                viewAllRooms(db);
                break;
            case 3:
                editRoom(db);
                break;
            case 4:
                deleteRoom(db);
                break;
            case 0:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                waitForEnter();
        }
        
        waitForEnter();
    }
}

// Add new room to the database
void addNewRoom(sqlite3 *db) {
    clearScreen();
    printf("\n=== ADD NEW ROOM ===\n\n");
    
    char number[50];
    char description[200];
    
    printf("Enter room number: ");
    fgets(number, sizeof(number), stdin);
    number[strcspn(number, "\n")] = 0; // Remove newline
    
    printf("Enter room description (optional): ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = 0;
    
    // Prepare SQL statement
    char sql[500];
    if (strlen(description) > 0) {
        sprintf(sql, "INSERT INTO rooms (number, description) VALUES ('%s', '%s');", 
                number, description);
    } else {
        sprintf(sql, "INSERT INTO rooms (number) VALUES ('%s');", number);
    }
    
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        printf("\nFailed to add room!\n");
    } else {
        printf("\nRoom added successfully!\n");
    }
    
    // waitForEnter();
}

void viewAllRooms(sqlite3 *db) {
    clearScreen();
    printf("\n=== ALL ROOMS ===\n\n");
    
    
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT id, number, description FROM rooms ORDER BY number;";
    
    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        waitForEnter();
        return;
    }
    
    printf("%-5s %-15s %-30s\n", "ID", "Room Number", "Description");
    printf("------------------------------------------------\n");
    
    int count = 0;
    while (1) {
        int result = sqlite3_step(stmt);
        
        if (result == SQLITE_ROW) {
            // Get column values
            int id = sqlite3_column_int(stmt, 0);
            const char *number = (const char*)sqlite3_column_text(stmt, 1);
            const char *description = (const char*)sqlite3_column_text(stmt, 2);
            
            // Handle NULL values
            if (number == NULL) number = "-";
            if (description == NULL) description = "-";
            
            printf("%-5d %-15s %-30s\n", id, number, description);
            count++;
        } else if (result == SQLITE_DONE) {
            break;  // All rows processed
        } else {
            // Error occurred
            fprintf(stderr, "Error fetching data: %s\n", sqlite3_errmsg(db));
            break;
        }
    }
    
    if (count == 0) {
        printf("No rooms found in database.\n");
    } else {
        printf("\nTotal rooms: %d\n", count);
    }
    
    // Finalize the statement to release resources
    sqlite3_finalize(stmt);
}

// Edit existing room information
void editRoom(sqlite3 *db) {
    clearScreen();
    printf("\n=== EDIT ROOM ===\n\n");
    
    // First show all rooms
    viewAllRooms(db);
    
    int room_id;
    printf("\nEnter room ID to edit: ");
    if (scanf("%d", &room_id) != 1) {
        clearInputBuffer();
        printf("Invalid room ID.\n");
        waitForEnter();
        return;
    }
    clearInputBuffer();
    
    char new_number[50];
    char new_description[200];
    
    printf("Enter new room number (leave blank to keep current): ");
    fgets(new_number, sizeof(new_number), stdin);
    new_number[strcspn(new_number, "\n")] = 0;
    
    printf("Enter new description (leave blank to keep current): ");
    fgets(new_description, sizeof(new_description), stdin);
    new_description[strcspn(new_description, "\n")] = 0;
    
    // Prepare SQL statement based on what fields are being updated
    char sql[500];
    int updates = 0;
    
    if (strlen(new_number) > 0 || strlen(new_description) > 0) {
        strcpy(sql, "UPDATE rooms SET ");
        
        if (strlen(new_number) > 0) {
            strcat(sql, "number = '");
            strcat(sql, new_number);
            strcat(sql, "'");
            updates++;
        }
        
        if (strlen(new_description) > 0) {
            if (updates) strcat(sql, ", ");
            strcat(sql, "description = '");
            strcat(sql, new_description);
            strcat(sql, "'");
            updates++;
        }
        
        strcat(sql, " WHERE id = ");
        char id_str[10];
        sprintf(id_str, "%d;", room_id);
        strcat(sql, id_str);
        
        char *err_msg = 0;
        int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
        
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
            printf("\nFailed to update room!\n");
        } else {
            printf("\nRoom information updated successfully!\n");
        }
    } else {
        printf("No changes made.\n");
    }
    
    // waitForEnter();
}

// Delete a room from the database
void deleteRoom(sqlite3 *db) {
    clearScreen();
    printf("\n=== DELETE ROOM ===\n\n");
    
    // First show all rooms
    viewAllRooms(db);
    
    int room_id;
    printf("\nEnter room ID to delete: ");
    if (scanf("%d", &room_id) != 1) {
        clearInputBuffer();
        printf("Invalid room ID.\n");
        waitForEnter();
        return;
    }
    clearInputBuffer();
    
    // Confirm deletion
    char confirm;
    printf("Are you sure you want to delete this room? (y/n): ");
    scanf(" %c", &confirm);
    clearInputBuffer();
    
    if (tolower(confirm) != 'y') {
        printf("Deletion canceled.\n");
        waitForEnter();
        return;
    }
    
    char sql[100];
    sprintf(sql, "DELETE FROM rooms WHERE id = %d;", room_id);
    
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        printf("\nFailed to delete room!\n");
    } else {
        printf("\nRoom deleted successfully!\n");
    }
    
    // waitForEnter();
}


// Flat Management Functions
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
