#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <ctype.h>

// Constants
#define MAX_STRING 256
#define MAX_APPOINTMENTS_PER_DAY 15
#define DB_NAME "clinic.db"

// Global database connection
sqlite3 *db;

// User roles
typedef enum {
    ROLE_ADMIN = 1,
    ROLE_RECEPTIONIST = 2
} UserRole;

// Utility function prototypes
void wait_for_enter();
void clear_screen();
void clear_input_buffer();

// DB Function Prototypes
void connect_database();
void initialize_database(sqlite3 *db);
void execute_sql(sqlite3 *db, const char *sql);

// Menu Functions
void show_main_menu();
void receptionist_menu();

// Patient Management Prototypes
void patient_management_menu();
void add_patient();
void edit_patient();
void delete_patient();
void view_patients();


// Main Function
int main() {
    clear_screen();
    connect_database();
    initialize_database(db);

    show_main_menu();

    sqlite3_close(db);
    return 0;
}

// Connect Database
void connect_database() {
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Database error: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    // Enable foreign keys
    const char *sql = "PRAGMA foreign_keys = ON;";
    execute_sql(db, sql);
}

// Execute SQL Queries
void execute_sql(sqlite3 *db, const char *sql) {
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

// Initialize Database and Tables 
void initialize_database(sqlite3 *db) {
    const char *sql =
        "CREATE TABLE IF NOT EXISTS patients ("
        "patient_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "full_name TEXT NOT NULL, "
        "age INTEGER CHECK(age > 0), "
        "weight REAL CHECK(weight > 0), "
        "address TEXT, "
        "contact TEXT NOT NULL, "
        "gender TEXT CHECK(gender IN ('M','F','O'))"
        "); "

        "CREATE TABLE IF NOT EXISTS doctors ("
        "doctor_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "full_name TEXT NOT NULL, "
        "specialization TEXT NOT NULL, "
        "contact TEXT NOT NULL"
        "); "

        "CREATE TABLE IF NOT EXISTS appointments ("
        "appointment_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "patient_id INTEGER NOT NULL, "
        "doctor_id INTEGER NOT NULL, "
        "appointment_date TEXT NOT NULL CHECK(appointment_date GLOB '____-__-__'), "
        "appointment_time TEXT NOT NULL CHECK(appointment_time GLOB '__:__'), "
        "FOREIGN KEY(patient_id) REFERENCES patients(patient_id) ON DELETE CASCADE, "
        "FOREIGN KEY(doctor_id) REFERENCES doctors(doctor_id) ON DELETE CASCADE"
        ");";

    execute_sql(db, sql);
}

// Main Menu
void show_main_menu() {
    int choice;
    
    while(1) {
        clear_screen();
        printf("=================================================\n");
        printf("    CLINIC APPOINTMENT MANAGEMENT SYSTEM (CAGS)  \n");
        printf("==================================================\n\n");
        printf("1. Goto Receptionist Section\n");
        printf("2. Goto Admin Section\n");
        printf("0. Exit\n");
        printf("\nEnter your Choice: ");
        
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                receptionist_menu();
                break;
            case 2:
                printf("Admin section is under construction.\n");
                wait_for_enter();
                // admin_menu();
                break;
            case 0:
                printf("Exiting...\n");
                wait_for_enter();
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                wait_for_enter();
        }
    }
}

// Receptionist Menu
void receptionist_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== RECEPTIONIST MENU ===\n");
        printf("1. Appointment Management\n");
        printf("2. Patient Management\n");
        printf("3. Doctor Management\n");
        printf("0. Logout\n\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: 
                // appointment_management_menu(); 
                printf("Appointment management is under construction.\n");
                wait_for_enter();
                break;
            case 2: 
                patient_management_menu(); 
                break;
            case 3: 
                // doctor_management_menu(); 
                printf("Doctor management is under construction.\n");
                wait_for_enter();
                break;
            case 0: 
                return;
            default: 
                printf("Invalid choice!\n");
                wait_for_enter();
                break;
        }
    }
}

// Patient Management
void patient_management_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== PATIENT MANAGEMENT ===\n");
        printf("1. Add Patient\n");
        printf("2. Edit Patient\n");
        printf("3. Delete Patient\n");
        printf("4. View All Patients\n");
        printf("0. Back\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: add_patient(); break;
            case 2: edit_patient(); break;
            case 3: delete_patient(); break;
            case 4: view_patients(); break;
            case 0: return;
            default: 
                printf("Invalid choice!\n");
                wait_for_enter();
                break;
        }
    }
}

void add_patient() {
    char name[MAX_STRING], address[MAX_STRING], contact[MAX_STRING], gender[10];
    int age;
    float weight;

    clear_screen();
    printf("=== ADD NEW PATIENT ===\n");
    printf("Please enter the following details:\n");
    do {
        printf("Full Name: ");
        fgets(name, MAX_STRING, stdin);
        name[strcspn(name, "\n")] = 0;  // remove newline

        if (strlen(name) == 0) {
            printf("❌ Name cannot be empty.\n");
            wait_for_enter();
            return;
        }
    } while (strlen(name) == 0);
    
    do {
        printf("Age: "); 
        scanf("%d", &age); 
        clear_input_buffer();
        if (age <= 0) {
            printf("Age must be a positive integer.\n");
        }
    } while (age <= 0);
    
    do {
        printf("Weight (kg): "); 
        scanf("%f", &weight); 
        clear_input_buffer();
        if (weight <= 0) {
            printf("Weight must be a positive number.\n");
        }
    } while (weight <= 0);
    
    do {
        printf("Address: ");
        fgets(address, MAX_STRING, stdin);
        address[strcspn(address, "\n")] = 0;  // remove newline

        if (strlen(address) == 0) {
            printf("❌ Address cannot be empty.\n");
        }

    } while (strlen(address) == 0);


    do {
        printf("Contact Number (10 digits only): ");
        fgets(contact, MAX_STRING, stdin);
        contact[strcspn(contact, "\n")] = 0;

        if (strlen(contact) != 10) {
            printf("❌ Contact must be exactly 10 digits long.\n");
            continue;
        }

        int valid = 1;
        for (int i = 0; i < 10; i++) {
            if (!isdigit(contact[i])) {
                valid = 0;
                printf("❌ Contact must contain only digits.\n");
                break;
            }
        }

        if (valid) {
            break;  // valid contact, exit loop
        }

    } while (1);  // keep repeating until a valid contact is entered


    do {
        printf("Gender (M/F/O): ");
        fgets(gender, sizeof(gender), stdin);
        gender[strcspn(gender, "\n")] = 0;

        // Convert first character to uppercase
        gender[0] = toupper(gender[0]);

        if (strlen(gender) != 1 || (gender[0] != 'M' && gender[0] != 'F' && gender[0] != 'O')) {
            printf("❌ Invalid input. Please enter M, F, or O only.\n");
        }
    } while (strlen(gender) != 1 || (gender[0] != 'M' && gender[0] != 'F' && gender[0] != 'O'));


    char sql[512];
    snprintf(sql, sizeof(sql),
        "INSERT INTO patients (full_name, age, weight, address, contact, gender) "
        "VALUES ('%s', %d, %.2f, '%s', '%s', '%s');",
        name, age, weight, address, contact, gender);

    execute_sql(db, sql);
    printf("\n✅ Patient added successfully.\n");
    wait_for_enter();
}

void view_patients() {
    printf("Viewing all patients...\n");
    wait_for_enter();
}

void edit_patient() {
    printf("Editing patient details...\n");
    wait_for_enter();
}

void delete_patient() {
    printf("Deleting patient...\n");
    wait_for_enter();
}

// Utility Functions
void wait_for_enter() {
    printf("Press Enter to continue...");
    getchar();
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clear_input_buffer() {
    while (getchar() != '\n');
}
