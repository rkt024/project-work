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

// Utility function prototypes
void wait_for_enter();
void clear_screen();
void clear_input_buffer();
void to_uppercase(char *str);

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
void view_patients();
void edit_patient();
void delete_patient();

// Doctor Management Prototypes
void doctor_management_menu();
void add_doc();
void view_docs();
void edit_doc();
void delete_doc();

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
                doctor_management_menu(); 
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
            printf("Name cannot be empty.\n");
            wait_for_enter();
            return;
        }

        // Convert to uppercase
        to_uppercase(name);

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
            printf("Address cannot be empty.\n");
            wait_for_enter();
            return;
        }

        // Convert to uppercase
        to_uppercase(address);

    } while (strlen(address) == 0);


    do {
        printf("Contact Number (10 digits only): ");
        fgets(contact, MAX_STRING, stdin);
        contact[strcspn(contact, "\n")] = 0;

        if (strlen(contact) != 10) {
            printf("Contact must be exactly 10 digits long.\n");
            continue;
        }

        int valid = 1;
        for (int i = 0; i < 10; i++) {
            if (!isdigit(contact[i])) {
                valid = 0;
                printf("Contact must contain only digits.\n");
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
            printf("Invalid input. Please enter M, F, or O only.\n");
        }
    } while (strlen(gender) != 1 || (gender[0] != 'M' && gender[0] != 'F' && gender[0] != 'O'));


    char sql[512];
    snprintf(sql, sizeof(sql),
        "INSERT INTO patients (full_name, age, weight, address, contact, gender) "
        "VALUES ('%s', %d, %.2f, '%s', '%s', '%s');",
        name, age, weight, address, contact, gender);

    execute_sql(db, sql);
    printf("\nPatient added successfully.\n");
    wait_for_enter();
}

void view_patients() {
    clear_screen();
    printf("=== VIEW ALL PATIENTS ===\n");
    const char *sql = "SELECT * FROM patients;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    printf("\n%-5s %-25s %-5s %-10s %-25s %-15s %-8s\n", "ID", "Full Name", "Age", "Weight", "Address", "Contact", "Gender");
    printf("----- ------------------------- ----- ---------- ------------------------- --------------- --------\n");

    // Loop through rows
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int patient_id = sqlite3_column_int(stmt, 0);
        const char *full_name = (const char*)sqlite3_column_text(stmt, 1);
        int age = sqlite3_column_int(stmt, 2);
        double weight = sqlite3_column_double(stmt, 3);
        const char *address = (const char*)sqlite3_column_text(stmt, 4);
        const char *contact = (const char*)sqlite3_column_text(stmt, 5);
        const char *gender = (const char*)sqlite3_column_text(stmt, 6);

        printf("%-5d %-25s %-5d %-10.2f %-25s %-15s %-8s\n",
               patient_id, full_name, age, weight, address, contact, gender);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error fetching data: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt); // Clean up the prepared statement
    printf("\nEnd of patient list.\n");
    wait_for_enter();
}

void edit_patient() {
    clear_screen();
    printf("=== EDIT PATIENT DETAILS ===\n");

    int patient_id_to_edit;
    printf("Enter the ID of the patient you want to edit: ");
    if (scanf("%d", &patient_id_to_edit) != 1) {
        printf("Invalid input. Please enter a number.\n");
        clear_input_buffer();
        wait_for_enter();
        return;
    }
    clear_input_buffer(); // Clear buffer after scanf

    char name[MAX_STRING], address[MAX_STRING], contact[MAX_STRING], gender[10];
    int age;
    float weight;

    // Check if patient exists
    char sql_check[256];
    snprintf(sql_check, sizeof(sql_check),
             "SELECT COUNT(*) FROM patients WHERE patient_id = %d;", patient_id_to_edit);
    sqlite3_stmt *stmt_check;
    if (sqlite3_prepare_v2(db, sql_check, -1, &stmt_check, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    int patient_exists = 0;
    if (sqlite3_step(stmt_check) == SQLITE_ROW) {
        patient_exists = sqlite3_column_int(stmt_check, 0);
    }
    sqlite3_finalize(stmt_check);

    if (!patient_exists) {
        printf("Patient with ID %d does not exist.\n", patient_id_to_edit);
        wait_for_enter();
        return;
    }

    printf("\nEnter NEW details for Patient ID %d:\n", patient_id_to_edit);

    do {
        printf("Full Name: ");
        fgets(name, MAX_STRING, stdin);
        name[strcspn(name, "\n")] = 0;  // remove newline

        if (strlen(name) == 0) {
            printf("Name cannot be empty.\n");
            wait_for_enter();
            return;
        }

        // Convert to uppercase
        to_uppercase(name);

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
            printf("Address cannot be empty.\n");
            wait_for_enter();
            return;
        } else {
            to_uppercase(address);
        }

    } while (strlen(address) == 0);


    do {
        printf("Contact Number (10 digits only): ");
        fgets(contact, MAX_STRING, stdin);
        contact[strcspn(contact, "\n")] = 0;

        if (strlen(contact) != 10) {
            printf("Contact must be exactly 10 digits long.\n");
            continue;
        }

        int valid = 1;
        for (int i = 0; i < 10; i++) {
            if (!isdigit(contact[i])) {
                valid = 0;
                printf("Contact must contain only digits.\n");
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
            printf("Invalid input. Please enter M, F, or O only.\n");
        }
    } while (strlen(gender) != 1 || (gender[0] != 'M' && gender[0] != 'F' && gender[0] != 'O'));

    // Prepare the SQL update statement
    char sql_update[1024]; // Using a larger buffer for the SQL query
    snprintf(sql_update, sizeof(sql_update),
             "UPDATE patients SET full_name = '%s', age = %d, weight = %.2f, address = '%s', contact = '%s', gender = '%s' WHERE patient_id = %d;",
             name, age, weight, address, contact, gender, patient_id_to_edit);

    execute_sql(db, sql_update);

    // Check if any rows were affected to confirm if the patient ID existed
    if (sqlite3_changes(db) > 0) {
        printf("\nPatient details updated successfully for ID %d.\n", patient_id_to_edit);
    } else {
        printf("\nPatient with ID %d not found or no changes were made.\n", patient_id_to_edit);
    }
    wait_for_enter();
}

void delete_patient() {
    clear_screen();
    printf("=== DELETE PATIENT DETAILS ===\n");

    int patient_id_to_delete;

    // Ask for patient ID
    printf("Enter the ID of the patient you want to delete: ");
    if (scanf("%d", &patient_id_to_delete) != 1) {
        printf("Invalid input. Please enter a number.\n");
        clear_input_buffer();
        wait_for_enter();
        return;
    }
    clear_input_buffer(); // Clear leftover input from stdin

    // Check if patient exists
    char sql_check[256];
    snprintf(sql_check, sizeof(sql_check),
             "SELECT COUNT(*) FROM patients WHERE patient_id = %d;",
             patient_id_to_delete);

    sqlite3_stmt *stmt_check;
    if (sqlite3_prepare_v2(db, sql_check, -1, &stmt_check, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    int patient_exists = 0;
    if (sqlite3_step(stmt_check) == SQLITE_ROW) {
        patient_exists = sqlite3_column_int(stmt_check, 0);
    }
    sqlite3_finalize(stmt_check);

    if (!patient_exists) {
        printf("Patient with ID %d does not exist.\n", patient_id_to_delete);
        wait_for_enter();
        return;
    }

    char confirm = '\0';

    // Keep looping until user enters 'y' or 'n'
    while (confirm != 'y' && confirm != 'n') {
        printf("Are you sure you want to delete patient ID %d? (y/n): ", patient_id_to_delete);
        confirm = getchar();

        // Clear the input buffer to remove extra characters or newline
        while (getchar() != '\n');

        // Convert to lowercase to accept 'Y' or 'N'
        confirm = tolower((unsigned char)confirm);

        if (confirm != 'y' && confirm != 'n') {
            printf("Invalid input. Please enter 'y' or 'n'.\n");
        }
    }

    // Now handle 'y' and 'n'
    if (confirm == 'n') {
        printf("Deletion cancelled by user.\n");
        wait_for_enter();
        return;
    }

    // Prepare and execute the DELETE SQL statement
    char sql_delete[256];
    snprintf(sql_delete, sizeof(sql_delete),
             "DELETE FROM patients WHERE patient_id = %d;",
             patient_id_to_delete);

    execute_sql(db, sql_delete);

    // Confirm whether the deletion was successful
    if (sqlite3_changes(db) > 0) {
        printf("\nPatient details deleted successfully for ID %d.\n", patient_id_to_delete);
    } else {
        printf("\nPatient with ID %d not found or no changes were made.\n", patient_id_to_delete);
    }

    wait_for_enter();
}

void doctor_management_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== Doctor MANAGEMENT ===\n");
        printf("1. Add Doctor\n");
        printf("2. Edit Doctor\n");
        printf("3. Delete Doctor\n");
        printf("4. View All Doctors\n");
        printf("0. Back\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: add_doc(); break;
            case 2: edit_doc(); break;
            case 3: delete_doc(); break;
            case 4: view_docs(); break;
            case 0: return;
            default: 
                printf("Invalid choice!\n");
                wait_for_enter();
                break;
        }
    }
}

void add_doc() {
    char name[MAX_STRING], specialization[MAX_STRING], contact[MAX_STRING];

    clear_screen();
    printf("=== ADD NEW DOCTOR ===\n");
    printf("Please enter the following details:\n");

    do {
        printf("Full Name: ");
        fgets(name, MAX_STRING, stdin);
        name[strcspn(name, "\n")] = 0;  // remove newline

        if (strlen(name) == 0) {
            printf("Name cannot be empty.\n");
            wait_for_enter();
            return;
        }

        // Convert to uppercase
        to_uppercase(name);

    } while (strlen(name) == 0);

    do {
        printf("Specialization: ");
        fgets(specialization, MAX_STRING, stdin);
        specialization[strcspn(specialization, "\n")] = 0;  // remove newline

        if (strlen(specialization) == 0) {
            printf("Specialization cannot be empty.\n");
            wait_for_enter();
            return;
        }

        // Convert to uppercase
        to_uppercase(specialization);

    } while (strlen(specialization) == 0);

    do {
        printf("Contact Number (10 digits only): ");
        fgets(contact, MAX_STRING, stdin);
        contact[strcspn(contact, "\n")] = 0;

        if (strlen(contact) != 10) {
            printf("Contact must be exactly 10 digits long.\n");
            continue;
        }

        int valid = 1;
        for (int i = 0; i < 10; i++) {
            if (!isdigit(contact[i])) {
                valid = 0;
                printf("Contact must contain only digits.\n");
                break;
            }
        }

        if (valid) {
            break;  // valid contact, exit loop
        }

    } while (1);  // keep repeating until a valid contact is entered

    char sql[512];
    snprintf(sql, sizeof(sql),
             "INSERT INTO doctors (full_name, specialization, contact) "
             "VALUES ('%s', '%s', '%s');",
             name, specialization, contact);

    execute_sql(db, sql);
    printf("\nDoctor added successfully.\n");
    wait_for_enter();
}

void view_docs() {
    clear_screen();
    printf("=== VIEW ALL DOCTORS ===\n");
    const char *sql = "SELECT * FROM doctors;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    printf("\n%-5s %-25s %-20s %-15s\n", "ID", "Full Name", "Specialization", "Contact");
    printf("----- ------------------------- -------------------- ---------------\n");

    // Loop through rows
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int doctor_id = sqlite3_column_int(stmt, 0);
        const char *full_name = (const char*)sqlite3_column_text(stmt, 1);
        const char *specialization = (const char*)sqlite3_column_text(stmt, 2);
        const char *contact = (const char*)sqlite3_column_text(stmt, 3);

        printf("%-5d %-25s %-20s %-15s\n",
               doctor_id, full_name, specialization, contact);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error fetching data: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt); // Clean up the prepared statement
    printf("\nEnd of doctor list.\n");
    wait_for_enter();
}

void edit_doc() {
    clear_screen();
    printf("=== EDIT DOCTOR DETAILS ===\n");

    int doctor_id_to_edit;
    printf("Enter the ID of the doctor you want to edit: ");
    if (scanf("%d", &doctor_id_to_edit) != 1) {
        printf("Invalid input. Please enter a number.\n");
        clear_input_buffer();
        wait_for_enter();
        return;
    }
    clear_input_buffer();

    char name[MAX_STRING], specialization[MAX_STRING], contact[MAX_STRING];
    char current_name[MAX_STRING], current_specialization[MAX_STRING], current_contact[MAX_STRING];

    // Check if doctor exists and get current details
    char sql_fetch[256];
    snprintf(sql_fetch, sizeof(sql_fetch),
             "SELECT full_name, specialization, contact FROM doctors WHERE doctor_id = %d;",
             doctor_id_to_edit);
    sqlite3_stmt *stmt_fetch;

    if (sqlite3_prepare_v2(db, sql_fetch, -1, &stmt_fetch, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_step(stmt_fetch) == SQLITE_ROW) {
        strncpy(current_name, (const char *)sqlite3_column_text(stmt_fetch, 0), MAX_STRING);
        strncpy(current_specialization, (const char *)sqlite3_column_text(stmt_fetch, 1), MAX_STRING);
        strncpy(current_contact, (const char *)sqlite3_column_text(stmt_fetch, 2), MAX_STRING);
    } else {
        printf("Doctor with ID %d does not exist.\n", doctor_id_to_edit);
        sqlite3_finalize(stmt_fetch);
        wait_for_enter();
        return;
    }

    sqlite3_finalize(stmt_fetch);

    printf("\nEnter NEW details for Doctor ID %d (press Enter to keep current value):\n", doctor_id_to_edit);

    // Full Name
    printf("Full Name [%s]: ", current_name);
    fgets(name, MAX_STRING, stdin);
    name[strcspn(name, "\n")] = 0;
    if (strlen(name) == 0) {
        strncpy(name, current_name, MAX_STRING);
    }
    to_uppercase(name);

    // Specialization
    printf("Specialization [%s]: ", current_specialization);
    fgets(specialization, MAX_STRING, stdin);
    specialization[strcspn(specialization, "\n")] = 0;
    if (strlen(specialization) == 0) {
        strncpy(specialization, current_specialization, MAX_STRING);
    }
    to_uppercase(specialization);

    // Contact
    printf("Contact [%s]: ", current_contact);
    fgets(contact, MAX_STRING, stdin);
    contact[strcspn(contact, "\n")] = 0;
    if (strlen(contact) == 0) {
        strncpy(contact, current_contact, MAX_STRING);
    }
    to_uppercase(contact);

    // Update doctor details
    char sql_update[512];
    snprintf(sql_update, sizeof(sql_update),
             "UPDATE doctors SET full_name = '%s', specialization = '%s', contact = '%s' WHERE doctor_id = %d;",
             name, specialization, contact, doctor_id_to_edit);
    sqlite3_stmt *stmt_update;

    if (sqlite3_prepare_v2(db, sql_update, -1, &stmt_update, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare update: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_step(stmt_update) != SQLITE_DONE) {
        fprintf(stderr, "Error updating doctor: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Doctor details updated successfully.\n");
    }

    sqlite3_finalize(stmt_update);
    wait_for_enter();
}

void delete_doc() {
    clear_screen();
    printf("=== DELETE DOCTOR DETAILS ===\n");

    int doctor_id_to_delete;

    // Ask for doctor ID
    printf("Enter the ID of the doctor you want to delete: ");
    if (scanf("%d", &doctor_id_to_delete) != 1) {
        printf("Invalid input. Please enter a number.\n");
        clear_input_buffer();
        wait_for_enter();
        return;
    }
    clear_input_buffer(); // Clear leftover input from stdin

    // Check if doctor exists
    char sql_check[256];
    snprintf(sql_check, sizeof(sql_check),
             "SELECT COUNT(*) FROM doctors WHERE doctor_id = %d;",
             doctor_id_to_delete);

    sqlite3_stmt *stmt_check;
    if (sqlite3_prepare_v2(db, sql_check, -1, &stmt_check, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    int doctor_exists = 0;
    if (sqlite3_step(stmt_check) == SQLITE_ROW) {
        doctor_exists = sqlite3_column_int(stmt_check, 0);
    }
    sqlite3_finalize(stmt_check);

    if (!doctor_exists) {
        printf("Doctor with ID %d does not exist.\n", doctor_id_to_delete);
        wait_for_enter();
        return;
    }

    char confirm = '\0';

    // Keep looping until user enters 'y' or 'n'
    while (confirm != 'y' && confirm != 'n') {
        printf("Are you sure you want to delete doctor ID %d? (y/n): ", doctor_id_to_delete);
        confirm = getchar();

        // Clear the input buffer to remove extra characters or newline
        while (getchar() != '\n');

        // Convert to lowercase to accept 'Y' or 'N'
        confirm = tolower((unsigned char)confirm);

        if (confirm != 'y' && confirm != 'n') {
            printf("Invalid input. Please enter 'y' or 'n'.\n");
        }
    }

    // Now handle 'y' and 'n'
    if (confirm == 'n') {
        printf("Deletion cancelled by user.\n");
        wait_for_enter();
        return;
    }

    // Prepare and execute the DELETE SQL statement
    char sql_delete[256];
    snprintf(sql_delete, sizeof(sql_delete),
             "DELETE FROM doctors WHERE doctor_id = %d;",
             doctor_id_to_delete);

    sqlite3_stmt *stmt_delete;
    if (sqlite3_prepare_v2(db, sql_delete, -1, &stmt_delete, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
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

void to_uppercase(char *str) {
    if (str == NULL) return; // Check for NULL pointer
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}
