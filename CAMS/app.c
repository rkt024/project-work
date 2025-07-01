#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <ctype.h> // For toupper, isdigit, tolower

// Constants
#define MAX_STRING 256
#define MAX_APPOINTMENTS_PER_DAY 15
#define DB_NAME "clinic.db"

// Global database connection
sqlite3 *db;

// Utility function prototypes
void wait_for_enter();
void clear_screen(); // Clears the console screen
void clear_input_buffer();
void to_uppercase(char *str); // Converts a string to uppercase
void getString(char *input, int size, const char *message); // Gets a non-empty string input with a custom message
int getPositiveInt(const char *message); // Gets a positive integer input with a custom message
float getPositiveFloat(const char *message); // Gets a positive float input with a custom message
char getGender(const char *message); // Gets a valid gender input (M, F, O)
void getContactNumber(char *contact, const char *message); // Gets a valid contact number input (10 digits only)

// DB Function Prototypes
void connect_database();
void initialize_database(sqlite3 *db);
void execute_sql(sqlite3 *db, const char *sql);
int getRecordCount(const char *table_name, const char *id_column, int id_value);

// Menu Functions
void show_main_menu();
void receptionist_menu();
void patient_management_menu();
void doctor_management_menu();
void appointment_management_menu();

// Patient Management Prototypes
void add_patient();
void view_patients();
void edit_patient();
void delete_patient();

// Doctor Management Prototypes
void add_doc();
void view_docs();
void edit_doc();
void delete_doc();

// Appointment Management Prototypes
void schedule_appointment();
void view_appointments();
void edit_appointment();
void cancel_appointment();


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

// getRecordCount function
int getRecordCount(const char *table_name, const char *id_column, int id_value) {
    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM %s WHERE %s = %d;", table_name, id_column, id_value);

    sqlite3_stmt *stmt;
    int count = 0;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    }
    return count;
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
                appointment_management_menu(); 
                // printf("Appointment management is under construction.\n");
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

    getString(name, MAX_STRING, "Full Name: ");
    getString(address, MAX_STRING, "Address: ");
    getContactNumber(contact, "Contact Number (10 digits only): ");

    // FIXED: Assign gender correctly and null-terminate
    gender[0] = getGender("Enter your gender (M/F/O): ");
    gender[1] = '\0';  // Null-terminate to make it a proper string

    age = getPositiveInt("Age: ");
    weight = getPositiveFloat("Weight (kg): ");
/* 
    printf("Full Name: %s\n", name);
    printf("Address: %s\n", address);
    printf("Contact: %s\n", contact);
    printf("Gender: %s\n", gender);
    printf("Age: %d\n", age);
    printf("Weight: %.2f\n", weight); */

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

    getString(name, MAX_STRING, "Full Name: ");
    getString(address, MAX_STRING, "Address: ");
    getContactNumber(contact, "Contact Number (10 digits only): ");
    age = getPositiveInt("Age: ");
    weight = getPositiveFloat("Weight (kg): ");
    gender[10] = getGender("Gender (M/F/O): ");


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

    getString(name, MAX_STRING, "Full Name: ");
    getContactNumber(contact, "Contact Number (10 digits only): ");
    getString(specialization, MAX_STRING, "Specialization: ");

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
    getString(name, MAX_STRING, "Full Name: ");
    getString(specialization, MAX_STRING, "Specialization: "); 
    getContactNumber(contact, "Contact Number (10 digits only): ");

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

// Appointment Management
void appointment_management_menu() {
    int choice;
    while (1) {
        clear_screen();
        printf("\n=== APPOINTMENT MANAGEMENT ===\n");
        printf("1. Schedule Appointment\n");
        printf("2. Edit Appointment\n");
        printf("3. Delete Appointment\n");
        printf("4. View All Appointments\n");
        printf("0. Back\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: schedule_appointment(); break;
            case 2: edit_appointment(); break;
            case 3: cancel_appointment(); break;
            case 4: view_appointments(); break;
            case 0: return;
            default: 
                printf("Invalid choice!\n");
                wait_for_enter();
                break;
        }
    }
}

void schedule_appointment() {
    int patient_id, doctor_id;
    char appointment_date[11], appointment_time[6];
    int patient_exists, doctor_exists;

    clear_screen();
    printf("=== SCHEDULE APPOINTMENT ===\n");
    printf("Please enter the following details:\n");

    // Get and validate Patient ID
    do {
        patient_id = getPositiveInt("Enter Patient ID: ");
        patient_exists = getRecordCount("patients", "patient_id", patient_id);
        if (!patient_exists) {
            printf("Patient with ID %d does not exist. Please enter a valid Patient ID.\n", patient_id);
            wait_for_enter();
        }
    } while (!patient_exists);

    // Get and validate Doctor ID
    do {
        doctor_id = getPositiveInt("Enter Doctor ID: ");
        doctor_exists = getRecordCount("doctors", "doctor_id", doctor_id);
        if (!doctor_exists) {
            printf("Doctor with ID %d does not exist. Please enter a valid Doctor ID.\n", doctor_id);
            wait_for_enter();
        }
    } while (!doctor_exists);

    // Get appointment date (YYYY-MM-DD)
    getString(appointment_date, 15, "Enter Appointment Date (YYYY-MM-DD): ");
    // Get appointment time (HH:MM)
    getString(appointment_time, 6, "Enter Appointment Time (HH:MM): ");

/* 
    // --- Validation Checks ---
    // 1. Check if the doctor has reached the maximum appointments for the *specified day*
    char sql_count_daily[256];
    snprintf(sql_count_daily, sizeof(sql_count_daily),
             "SELECT COUNT(*) FROM appointments WHERE doctor_id = %d AND appointment_date = '%s';",
             doctor_id, appointment_date); // Use the entered date, not date('now')

    sqlite3_stmt *stmt_count_daily;
    int appointment_count = 0;
    if (sqlite3_prepare_v2(db, sql_count_daily, -1, &stmt_count_daily, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare daily appointment count statement: %s\n", sqlite3_errmsg(db));
        wait_for_enter(); // Add wait_for_enter for user to see the error
        return;
    }

    if (sqlite3_step(stmt_count_daily) == SQLITE_ROW) {
        appointment_count = sqlite3_column_int(stmt_count_daily, 0);
    }
    sqlite3_finalize(stmt_count_daily);

    if (appointment_count >= MAX_APPOINTMENTS_PER_DAY) {
        printf("Doctor with ID %d has reached the maximum appointments (%d) for %s.\n",
               doctor_id, MAX_APPOINTMENTS_PER_DAY, appointment_date);
        wait_for_enter();
        return;
    }

    // 2. Check for existing appointments at the *same date and time* for the same doctor (double-booking)
    char check_sql_slot[512];
    snprintf(check_sql_slot, sizeof(check_sql_slot),
             "SELECT COUNT(*) FROM appointments WHERE doctor_id = %d AND appointment_date = '%s' AND appointment_time = '%s';",
             doctor_id, appointment_date, appointment_time);

    sqlite3_stmt *stmt_check_slot;
    int existing_appointments_slot = 0;
    int rc_slot = sqlite3_prepare_v2(db, check_sql_slot, -1, &stmt_check_slot, NULL);

    if (rc_slot == SQLITE_OK) {
        if (sqlite3_step(stmt_check_slot) == SQLITE_ROW) {
            existing_appointments_slot = sqlite3_column_int(stmt_check_slot, 0);
        }
        sqlite3_finalize(stmt_check_slot);
    } else {
        fprintf(stderr, "Failed to prepare slot booking check statement: %s\n", sqlite3_errmsg(db));
        wait_for_enter();
        return;
    }

    if (existing_appointments_slot > 0) {
        printf("\nError: Doctor ID %d is already booked at %s on %s. Please choose a different time or date.\n",
               doctor_id, appointment_time, appointment_date);
        wait_for_enter();
        return; // Prevent scheduling if already booked
    }
 */

    // Prepare the SQL insert statement
    char sql_insert[512];
    snprintf(sql_insert, sizeof(sql_insert),
             "INSERT INTO appointments (patient_id, doctor_id, appointment_date, appointment_time) "
             "VALUES (%d, %d, '%s', '%s');",
             patient_id, doctor_id, appointment_date, appointment_time);

    execute_sql(db, sql_insert);

    // Check if the insertion was successful
    if (sqlite3_changes(db) > 0) {
        printf("\nAppointment scheduled successfully.\n");
    } else {
        printf("\nFailed to schedule appointment. This might be due to a database error or constraint violation.\n");
    }
    wait_for_enter();
}

void view_appointments() {
    clear_screen();
    printf("=== VIEW ALL APPOINTMENTS ===\n");
    const char *sql = "SELECT a.appointment_id, p.full_name AS patient_name, d.full_name AS doctor_name, "
                      "a.appointment_date, a.appointment_time "
                      "FROM appointments a "
                      "JOIN patients p ON a.patient_id = p.patient_id "
                      "JOIN doctors d ON a.doctor_id = d.doctor_id;";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    printf("\n%-5s %-25s %-25s %-15s %-5s\n", "ID", "Patient Name", "Doctor Name", "Date", "Time");
    printf("----- ------------------------- ------------------------- --------------- -----\n");

    // Loop through rows
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int appointment_id = sqlite3_column_int(stmt, 0);
        const char *patient_name = (const char*)sqlite3_column_text(stmt, 1);
        const char *doctor_name = (const char*)sqlite3_column_text(stmt, 2);
        const char *appointment_date = (const char*)sqlite3_column_text(stmt, 3);
        const char *appointment_time = (const char*)sqlite3_column_text(stmt, 4);

        printf("%-5d %-25s %-25s %-15s %-5s\n",
               appointment_id, patient_name, doctor_name, appointment_date, appointment_time);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error fetching data: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt); // Clean up the prepared statement
    printf("\nEnd of appointment list.\n");
    wait_for_enter();
}

void cancel_appointment() {
    clear_screen();
    printf("=== CANCEL APPOINTMENT ===\n");

    int appointment_id_to_cancel;

    // Ask for appointment ID
    printf("Enter the ID of the appointment you want to cancel: ");
    if (scanf("%d", &appointment_id_to_cancel) != 1) {
        printf("Invalid input. Please enter a number.\n");
        clear_input_buffer();
        wait_for_enter();
        return;
    }
    clear_input_buffer(); // Clear leftover input from stdin

    // Check if appointment exists
    char sql_check[256];
    snprintf(sql_check, sizeof(sql_check),
             "SELECT COUNT(*) FROM appointments WHERE appointment_id = %d;",
             appointment_id_to_cancel);

    sqlite3_stmt *stmt_check;
    if (sqlite3_prepare_v2(db, sql_check, -1, &stmt_check, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    int appointment_exists = 0;
    if (sqlite3_step(stmt_check) == SQLITE_ROW) {
        appointment_exists = sqlite3_column_int(stmt_check, 0);
    }
    sqlite3_finalize(stmt_check);

    if (!appointment_exists) {
        printf("Appointment with ID %d does not exist.\n", appointment_id_to_cancel);
        wait_for_enter();
        return;
    }

    char confirm = '\0';

    // Keep looping until user enters 'y' or 'n'
    while (confirm != 'y' && confirm != 'n') {
        printf("Are you sure you want to cancel appointment ID %d? (y/n): ", appointment_id_to_cancel);
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
        printf("Cancellation cancelled by user.\n");
        wait_for_enter();
        return;
    }

    // Prepare and execute the DELETE SQL statement
    char sql_delete[256];
    snprintf(sql_delete, sizeof(sql_delete),
             "DELETE FROM appointments WHERE appointment_id = %d;",
             appointment_id_to_cancel);

    sqlite3_stmt *stmt_delete;
    if (sqlite3_prepare_v2(db, sql_delete, -1, &stmt_delete, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_step(stmt_delete) != SQLITE_DONE) {
        fprintf(stderr, "Error deleting appointment: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Appointment ID %d has been successfully cancelled.\n", appointment_id_to_cancel);
    }

    sqlite3_finalize(stmt_delete);
    wait_for_enter();
}

void edit_appointment() {
    clear_screen();
    printf("=== Menu Under Construction ===\n");
    printf("This feature is currently under construction.\n");
    printf("Please check back later.\n");
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

void to_uppercase(char *str) {
    if (str == NULL) return; // Check for NULL pointer
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void getString(char *input, int size, const char *message) {
    while (1) {
        printf("%s", message);
        fgets(input, size, stdin);

        // Remove newline character if present
        input[strcspn(input, "\n")] = 0;

        to_uppercase(input); // Convert to uppercase
        if (strlen(input) == 0) {
            printf("Input cannot be empty. Please try again.\n");
        } else {
            break;
        }
    }
}

// Gets a positive integer from the user
int getPositiveInt(const char *message) {
    int input;

    while (1) {
        printf("%s", message);

        if (scanf("%d", &input) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            clear_input_buffer();
            continue;
        }

        clear_input_buffer(); // Clear leftover input from stdin

        if (input > 0) {
            return input;
        } else {
            printf("Please enter a positive number.\n");
        }
    }
}

float getPositiveFloat(const char *message) {
    float input;

    while (1) {
        printf("%s", message);

        if (scanf("%f", &input) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            clear_input_buffer();
            continue;
        }

        clear_input_buffer(); // Clear leftover input from stdin

        if (input > 0) {
            return input;
        } else {
            printf("Please enter a positive number.\n");
        }
    }
}

char getGender(const char *message) {
    char gender[10];

    while (1) {
        printf("%s", message);
        if (fgets(gender, sizeof(gender), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }

        gender[strcspn(gender, "\n")] = '\0';  // Remove newline

        if (strlen(gender) != 1) {
            printf("Invalid input. Please enter a single character: M, F, or O.\n");
            continue;
        }

        gender[0] = toupper((unsigned char)gender[0]);

        if (gender[0] == 'M' || gender[0] == 'F' || gender[0] == 'O') {
            return gender[0];
        } else {
            printf("Invalid input. Please enter M, F, or O only.\n");
        }
    }
}

// Function to get a valid 10-digit contact number
void getContactNumber(char *contact, const char *message) {
    while (1) {
        printf("%s", message);
        fgets(contact, 100, stdin);

        // Remove newline character if present
        contact[strcspn(contact, "\n")] = 0;

        // Check length
        if (strlen(contact) != 10) {
            printf("Contact must be exactly 10 digits long.\n");
            continue;
        }

        // Check for only digits
        int valid = 1;
        for (int i = 0; i < 10; i++) {
            if (!isdigit(contact[i])) {
                valid = 0;
                printf("Contact must contain only digits.\n");
                break;
            }
        }

        if (valid) {
            break;  // valid contact number
        }
    }
}


