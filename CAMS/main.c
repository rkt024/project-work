#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

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

/* 
// DB Function Prototypes
void connect_database();
void initialize_database(sqlite3 *db);
void execute_sql(sqlite3 *db, const char *sql);
 */

// Menu Functions
void show_main_menu();
void receptionist_menu();
void admin_menu();
void generate_reports_menu();
void view_system_data_menu();

// Admin Functions
void view_doctors();
void view_patients();
void view_appointments();
void generate_daily_report();
void generate_patient_list_by_doctor();
void generate_appointment_trends();

// Receptionist Functions
void doctor_management_menu();
void add_doctor();
void edit_doctor();
void delete_doctor();

void patient_management_menu();
void add_patient();
void edit_patient();
void delete_patient();

void appointment_management_menu();
void schedule_appointment();
void edit_appointment();
void delete_appointment();

/* 
// Connect Database
void connect_database() {
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Database error: %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
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
        "appointment_date TEXT NOT NULL CHECK(appointment_date GLOB '????-??-??'), "
        "appointment_time TEXT NOT NULL CHECK(appointment_time GLOB '??:??'), "
        "FOREIGN KEY(patient_id) REFERENCES patients(patient_id) ON DELETE CASCADE, "
        "FOREIGN KEY(doctor_id) REFERENCES doctors(doctor_id) ON DELETE CASCADE"
        ");";

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
*/

// Main Menu
void show_main_menu() {
    int choice;
    
    while(1) {
        clear_screen();
        printf("=================================================\n");
        printf("    CLINIC APPOINTMENT MANAGEMENT SYSTEM (CAGS)  \n");
        printf("==================================================\n\n");
        printf("1. Goto Admin Section\n");
        printf("2. Goto Receptionist Section\n");
        printf("0. Exit\n");
        printf("\nEnter your Choice: ");
        
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1:
                admin_menu();
                break;
            case 2:
                receptionist_menu();
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

// Admin Menu
void admin_menu() {
    int choice;
    
    while (1) {
        clear_screen();
        printf("\n=== ADMIN MENU ===\n\n");
        printf("1. View All System Data\n");
        printf("2. Generate Reports\n");
        printf("0. Logout\n");
        printf("\nEnter your choice: ");
        
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch (choice) {
            case 1 : view_system_data_menu(); break;
            case 2 : generate_reports_menu(); break;
            case 0 : return;
            default: 
                printf("Invalid choice!\n"); 
                wait_for_enter(); 
                break;
        }
    }
}

// View System Data Menu
void view_system_data_menu() {
    int choice;

    while (1) {
        clear_screen();
        printf("\n=== VIEW SYSTEM DATA ===\n\n");
        printf("1. View All Doctors\n");
        printf("2. View All Patients\n");
        printf("3. View All Appointments\n");
        printf("0. Back to Admin Menu\n");
        printf("\nEnter your choice: ");
        
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch (choice) {
            case 1 : view_doctors(); break;
            case 2 : view_patients(); break;
            case 3 : view_appointments(); break;
            case 0 : return;
            default: 
                printf("Invalid choice!\n"); 
                wait_for_enter(); 
                break;
        }
    }
}

// Admin Function Placeholders
void view_doctors() {
    printf("Viewing all doctors...\n");
    wait_for_enter();
}

void view_patients() {
    printf("Viewing all patients...\n");
    wait_for_enter();
}

void view_appointments() {
    printf("Viewing all appointments...\n");
    wait_for_enter();
}

// Reports Menu
void generate_reports_menu() {
    int choice;

    while (1) {
        clear_screen();
        printf("\n=== GENERATE REPORTS ===\n\n");
        printf("1. Daily Doctor-wise Report\n");
        printf("2. Patient List by Doctor\n");
        printf("3. Appointment Trends\n");
        printf("0. Back to Admin Menu\n");
        printf("\nEnter your choice: ");
        
        scanf("%d", &choice);
        clear_input_buffer();
        
        switch (choice) {
            case 1 : generate_daily_report(); break;
            case 2 : generate_patient_list_by_doctor(); break;
            case 3 : generate_appointment_trends(); break;
            case 0 : return;
            default: 
                printf("Invalid choice!\n"); 
                wait_for_enter(); 
                break;
        }
    }
}

void generate_daily_report() {
    printf("Generating daily doctor-wise report...\n");
    wait_for_enter();
}

void generate_patient_list_by_doctor() {
    printf("Generating patient list by doctor...\n");
    wait_for_enter();
}

void generate_appointment_trends() {
    printf("Generating appointment trends...\n");
    wait_for_enter();
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
            case 1: appointment_management_menu(); break;
            case 2: patient_management_menu(); break;
            case 3: doctor_management_menu(); break;
            case 0: return;
            default: 
                printf("Invalid choice!\n");
                wait_for_enter();
                break;
        }
    }
}

// Appointment Management
void appointment_management_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== APPOINTMENT MANAGEMENT ===\n");
        printf("1. Schedule Appointment\n");
        printf("2. Edit Appointment\n");
        printf("3. Delete Appointment\n");
        printf("4. View Appointment\n");
        printf("0. Back\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: schedule_appointment(); break;
            case 2: edit_appointment(); break;
            case 3: delete_appointment(); break;
            case 4: view_appointments(); break;
            case 0: return;
            default: 
                printf("Invalid choice!\n");
                wait_for_enter();
                break;
        }
    }
}

// Doctor Management
void doctor_management_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== DOCTOR MANAGEMENT ===\n");
        printf("1. Add Doctor\n");
        printf("2. Edit Doctor\n");
        printf("3. Delete Doctor\n");
        printf("4. View All Doctors\n");
        printf("0. Back\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: add_doctor(); break;
            case 2: edit_doctor(); break;
            case 3: delete_doctor(); break;
            case 4: view_doctors(); break;
            case 0: return;
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

// Receptionist Function Placeholders
void add_doctor() {
    printf("Adding new doctor...\n");
    wait_for_enter();
}

void edit_doctor() {
    printf("Editing doctor details...\n");
    wait_for_enter();
}

void delete_doctor() {
    printf("Deleting doctor...\n");
    wait_for_enter();
}

void schedule_appointment() {
    printf("Scheduling appointment...\n");
    wait_for_enter();
}

void edit_appointment() {
    printf("Editing appointment...\n");
    wait_for_enter();
}

void delete_appointment() {
    printf("Deleting appointment...\n");
    wait_for_enter();
}

void add_patient() {
    printf("Adding new patient...\n");
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

// Main Function
int main() {
    clear_screen();
    // connect_database();
    // initialize_database(db);

    show_main_menu();

    // sqlite3_close(db);
    return 0;
}
