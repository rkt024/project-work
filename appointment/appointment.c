#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#define DB_NAME "appointment.db"
#define get_input(variable, prompt) { \
    printf("%s", prompt); \
    fgets(variable, sizeof(variable), stdin); \
    variable[strcspn(variable, "\n")] = 0;  /* Remove newline character if present */ \
}

// Global database connection
sqlite3 *db;

// DB Function Prototypes
void connect_database();
void initialize_database();
void execute_sql(sqlite3 *db, const char *sql);

// Common Functions
void wait_for_enter();
void clear_screen();
void clear_input_buffer();

// Patient, Doctor, and Appointment Menus
void main_menu();
void patient_menu();
void doctor_menu();
void appointment_menu();

int main() {
    clear_screen();
    connect_database();
    initialize_database(db);
    main_menu();
    return 0;
}

// Functions

void wait_for_enter() {
    printf("Press Enter to continue...");
    getchar(); // Wait for Enter
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clear_input_buffer() {
    while (getchar() != '\n'); // Flush input buffer
}

// Initialize database
void connect_database() {
    char *err_msg = 0;
    int rc;

    rc = sqlite3_open(DB_NAME, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Database error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
}

// Function to execute SQL queries
void execute_sql(sqlite3 *db, const char *sql) {
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

// Function to initialize the database and create tables
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
        "appointment_date TEXT NOT NULL CHECK(appointment_date GLOB '????-??-??'), "
        "appointment_time TEXT NOT NULL CHECK(appointment_time GLOB '??:??'), "
        "FOREIGN KEY(patient_id) REFERENCES patients(patient_id) ON DELETE CASCADE, "
        "FOREIGN KEY(doctor_id) REFERENCES doctors(doctor_id) ON DELETE CASCADE"
        ");";

    execute_sql(db, sql);
}

// Main menu
void main_menu() {
    int choice;
    while(1) {
        printf("\n=== HOSPITAL APPOINTMENT SYSTEM ===\n");
        printf("1. Patient Management\n");
        printf("2. Doctor Management\n");
        printf("3. Appointment Management\n");
        printf("0. Exit\n");
        printf("Enter your Choice: ");
        scanf("%d", &choice);
        clear_input_buffer();

        switch(choice) {
            case 1: 
                clear_screen();
                patient_menu();
                break;
            case 2: 
                clear_screen();
                doctor_menu();
                break;
            case 3: 
                clear_screen();
                appointment_menu();
                break;
            case 0: 
                sqlite3_close(db);
                printf("Exiting program.....\n");
                wait_for_enter();
                exit(0);
            default: 
                printf("Invalid choice!\n");
        }
    }
}

void patient_menu() {
    int choice;
    while(1) {
        printf("\n=== PATIENT MANAGEMENT ===\n");
        printf("1. Add New Patient\n");
        printf("2. View Patients\n");
        printf("3. Edit Patient\n");
        printf("4. Delete Patient\n");
        printf("0. Back to Main Menu\n\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        clear_input_buffer();

        switch(choice) {
            case 1: 
                // Add patient functionality
                clear_screen();
                printf("Add New Patient...\n");
                wait_for_enter();
                break;
            case 2: 
                // View patients functionality
                clear_screen();
                printf("View Patients...\n");
                wait_for_enter();
                break;
            case 3: 
                // Edit patient functionality
                clear_screen();
                printf("Edit Patient...\n");
                wait_for_enter();
                break;
            case 4: 
                // Delete patient functionality
                clear_screen();
                printf("Delete Patient...\n");
                wait_for_enter();
                break;
            case 0: 
                clear_screen();
                return; // Go back to main menu
            default: 
                printf("Invalid choice!\n");
        }
    }
}

void doctor_menu() {
    int choice;
    while(1) {
        printf("\n=== DOCTOR MANAGEMENT ===\n");
        printf("1. Add New Doctor\n");
        printf("2. View Doctors\n");
        printf("3. Edit Doctor\n");
        printf("4. Delete Doctor\n");
        printf("0. Back to Main Menu\n\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        clear_input_buffer();

        switch(choice) {
            case 1: 
                // Add doctor functionality
                clear_screen();
                printf("Add New doctor...\n");
                wait_for_enter();
                break;
            case 2: 
                // View doctor functionality
                clear_screen();
                printf("View doctor ...\n");
                wait_for_enter();
                break;
            case 3: 
                // Edit doctor functionality
                clear_screen();
                printf("Edit doctor ...\n");
                wait_for_enter();
                break;
            case 4: 
                // Delete doctor functionality
                clear_screen();
                printf("Delete doctor...\n");
                wait_for_enter();
                break;
            case 0: 
                clear_screen();
                return; // Go back to main menu
            default: 
                printf("Invalid choice!\n");
        }
    }
}

void appointment_menu() {
    int choice;
    while(1) {
        printf("\n=== APPOINTMENT MANAGEMENT ===\n");
        printf("1. Add New Appointment\n");
        printf("2. View Appointments\n");
        printf("3. Edit Appointment\n");
        printf("4. Delete Appointment\n");
        printf("0. Back to Main Menu\n\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        clear_input_buffer();

        switch(choice) {
            case 1: 
                // Add appointment functionality
                clear_screen();
                printf("Add New appointment...\n");
                wait_for_enter();
                break;
            case 2: 
                // View appointment functionality
                clear_screen();
                printf("View appointments...\n");
                wait_for_enter();
                break;
            case 3: 
                // Edit appointment functionality
                clear_screen();
                printf("Edit appointment...\n");
                wait_for_enter();
                break;
            case 4: 
                // Delete appointment functionality
                clear_screen();
                printf("Delete appointment...\n");
                wait_for_enter();
                break;
            case 0: 
                clear_screen();
                return; // Go back to main menu
            default: 
                printf("Invalid choice!\n");
        }
    }
}
