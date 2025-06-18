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

// DB Function Prototypes
void connect_database();
void initialize_database();
void execute_sql(sqlite3 *db, const char *sql);

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
void initialize_database();
void execute_sql(sqlite3 *db, const char *sql);

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


int main() {
    clear_screen();
    connect_database();
    initialize_database(db);

    // Show the main menu after login
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
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
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

// Function to execute SQL queries
void execute_sql(sqlite3 *db, const char *sql) {
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void show_main_menu() {
    int choice;
    
    // Loop for main menu
    while(1) {
    	clear_screen();
    	printf("=================================================\n");
    	printf("    CLINIC APPOINTMENT MANAGEMENT SYSTEM (CAGS)  \n");
    	printf("==================================================\n\n");
        printf("1. Goto Admin Section\n");
        printf("2. Goto Receptionist Section\n");
        printf("0. Exit\n");
        printf("\nEnter your Choice: ");
        
        // Read and clear input buffer after reading
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
        }
    }
}

//===============================================================================
// Admin Functions

void admin_menu() {
    int choice;
    
    // Loop for admin menu
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
            case 0 : return;  // Logout
            default: 
                printf("Invalid choice!\n"); 
                wait_for_enter(); 
                break;
        }
    }
}

void view_system_data_menu() {
    int choice;

    // Loop for system data menu
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
            case 0 : return;  // Back to Admin Menu
            default: 
                printf("Invalid choice!\n"); 
                wait_for_enter(); 
                break;
        }
    }
}

void view_doctors() {
    // Placeholder for viewing doctors
    printf("Viewing all doctors...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void view_patients() {
    // Placeholder for viewing patients
    printf("Viewing all patients...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void view_appointments() {
    // Placeholder for viewing appointments
    printf("Viewing all appointments...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void generate_reports_menu() {
    int choice;

    // Loop for report generation menu
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
            case 0 : return;  // Back to Admin Menu
            default: 
                printf("Invalid choice!\n"); 
                wait_for_enter(); 
                break;
        }
    }
}

void generate_daily_report() {
    // Placeholder for generating daily report
    printf("Generating daily doctor-wise report...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void generate_patient_list_by_doctor() {
    // Placeholder for generating patient list by doctor
    printf("Generating patient list by doctor...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void generate_appointment_trends() {
    // Placeholder for generating appointment trends
    printf("Generating appointment trends...\n");
    // Actual implementation would go here
    wait_for_enter();
}

//===============================================================================
// Receptionist Functions

void receptionist_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== RECEPTIONIST MENU ===\n");
        printf("1. Doctor Management\n");
        printf("2. Patient Management\n");
        printf("3. Appointment Management\n");
        printf("0. Logout\n\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: doctor_management_menu(); break;
            case 2: patient_management_menu(); break;
            case 3: appointment_management_menu(); break;
            case 0: return;
            default: 
                printf("Invalid choice!\n"); 
                wait_for_enter(); 
                break;
        }
    }
}

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

void add_doctor() {
    // Placeholder for adding a doctor
    printf("Adding a new doctor...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void edit_doctor() {
    // Placeholder for editing a doctor
    printf("Editing a doctor...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void delete_doctor() {
    // Placeholder for deleting a doctor
    printf("Deleting a doctor...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void view_doctors() {
    // Placeholder for viewing doctors
    printf("Viewing all doctors...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void patient_management_menu() {
    char choice;
    while (1) {
        clear_screen();
        printf("\n=== PATIENT MANAGEMENT ===\n");
        printf("1. Add Patient\n");
        printf("2. Edit Patient\n");
        printf("3. Delete Patient\n");
        printf("4. View All Patients\n");
        printf("0. Back\n\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: add_patient(); break;
            case 2: view_patients(); break;
            case 0: return;
            default: 
                printf("Invalid choice!\n"); 
                wait_for_key(); 
                break;
        }
    }
}

void add_patient() {
    // Placeholder for adding a patient
    printf("Adding a new patient...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void edit_patient() {
    // Placeholder for editing a patient
    printf("Editing a patient...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void delete_patient() {
    // Placeholder for deleting a patient
    printf("Deleting a patient...\n");
    // Actual implementation would go here
    wait_for_enter();
}

void view_patients() {
    // Placeholder for viewing patients
    printf("Viewing all patients...\n");
    // Actual implementation would go here
    wait_for_enter();
}


// ================================================================================
// Appointment Management Functions

void appointment_management_menu() {
    char ch;
    while (1) {
        clear_screen();
        printf("\n=== APPOINTMENT MANAGEMENT ===\n");
        printf("g. Schedule Appointment\n");
        printf("h. Edit Appointment\n");
        printf("i. Delete Appointment\n");
        printf("j. View All Appointments\n");
        printf("k. Lookup by Token Number\n");
        printf("0. Back\n");
        printf("Enter your choice: ");
        scanf(" %c", &ch);

        switch (ch) {
            case 'g': case 'G': schedule_appointment(); break;
            case 'h': case 'H': edit_appointment(); break;
            case 'i': case 'I': delete_appointment(); break;
            case 'j': case 'J': view_appointments(); break;
            case 'k': case 'K': lookup_appointment_by_token(); break;
            case '0': return;
            default: 
                printf("Invalid choice!\n"); 
                wait_for_key(); 
                break;
        }
    }
}



// ================================================================================

// Utility Functions
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

