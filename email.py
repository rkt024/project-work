import sqlite3
import os
from datetime import datetime
import re
import csv

class TransactionManager:
    """
    A console-based transaction management system with SQLite database integration.
    """
    
    def __init__(self, db_name="email_records.db"):
        """Initialize the database connection and create table if not exists."""
        # Validate and sanitize database name
        if not re.fullmatch(r'[\w\-\.]+\.db', db_name):
            raise ValueError("Invalid database name")
        self.db_name = os.path.basename(db_name)  # Prevent path traversal
        
        self.bank_codes = {
            "CIT": "CITIZEN INVESTMENT TRUST",
            "EPF": "EMPLOYEE PROVIDENT FUND",
            "GBBL": "GARIMA BIKAS BANK LTD",
            "MNBBL": "MUKTINATH BIKASH BANK LTD",
            "NIMB": "NEPAL INVESTMENT MEGA BANK LTD",
            "PCBL": "PRIME COMMERCIAL BANK LTD",
            "SADBL": "SHANGRILA DEVELOPMENT BANK LTD"
        }
        self.init_database()
    
    def init_database(self):
        """Create the transactions table if it doesn't exist."""
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            # Create table with unique constraint on reference number
            cursor.execute('''
                CREATE TABLE IF NOT EXISTS transactions (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    ref_number TEXT NOT NULL UNIQUE,
                    bank_name TEXT NOT NULL,
                    date TEXT NOT NULL
                )
            ''')
            
            # Create indexes for faster queries
            cursor.execute('CREATE INDEX IF NOT EXISTS idx_ref_number ON transactions(ref_number)')
            cursor.execute('CREATE INDEX IF NOT EXISTS idx_date ON transactions(date)')
            
            conn.commit()
            conn.close()
            # print(f"Database '{self.db_name}' initialized successfully.")
        except sqlite3.Error as e:
            print(f"Error initializing database: {e}")
    
    def format_reference_number(self, number):
        """
        Convert input number to reference number format: RK + 7 digits with leading zeros.
        
        Args:
            number (str): Input number string
            
        Returns:
            str: Formatted reference number (e.g., "RK0004651")
        """
        try:
            # Convert to integer to validate it's a number
            num = int(number)
            
            # Validate number range
            if num < 0 or num > 9999999:
                raise ValueError("Number must be between 0-9999999")
            
            # Format with leading zeros to make it 7 digits
            formatted_num = f"{num:07d}"
            return f"RK{formatted_num}"
        except ValueError:
            raise ValueError("Invalid number format. Please enter a valid integer between 0-9999999")
    
    def get_bank_name(self, bank_code):
        """
        Convert bank code to full bank name.
        
        Args:
            bank_code (str): Bank short code
            
        Returns:
            str: Full bank name
        """
        bank_code = bank_code.upper().strip()
        if bank_code in self.bank_codes:
            return self.bank_codes[bank_code]
        else:
            raise ValueError(f"Invalid bank code. Supported codes: {', '.join(self.bank_codes.keys())}")
    
    def validate_date(self, date_str):
        """
        Validate date format (YYYY-MM-DD) and ensure it's not in the future.
        
        Args:
            date_str (str): Date string to validate
            
        Returns:
            bool: True if valid, False otherwise
        """
        try:
            date = datetime.strptime(date_str, '%Y-%m-%d')
            # Prevent future dates
            if date.date() > datetime.now().date():
                return False
            return True
        except ValueError:
            return False
    
    def get_current_date(self):
        """Get current date in YYYY-MM-DD format."""
        return datetime.now().strftime('%Y-%m-%d')
    
    def add_transaction(self):
        """Add a new transaction to the database."""
        print("\n=== ADD NEW TRANSACTION ===")
        
        try:
            # Get and format reference number
            while True:
                number_input = input("Enter a number (e.g., 453, 4561, 10245): ").strip()
                if number_input:
                    try:
                        ref_number = self.format_reference_number(number_input)
                        print(f"Generated reference number: {ref_number}")
                        break
                    except ValueError as e:
                        print(f"Error: {e}")
                else:
                    print("Please enter a valid number.")
            
            # Get bank name
            while True:
                print(f"\nSupported bank codes: {', '.join(self.bank_codes.keys())}")
                bank_code = input("Enter bank short code: ").strip()
                if bank_code:
                    try:
                        bank_name = self.get_bank_name(bank_code)
                        print(f"Bank name: {bank_name}")
                        break
                    except ValueError as e:
                        print(f"Error: {e}")
                else:
                    print("Please enter a valid bank code.")
            
            # Get date
            while True:
                date_choice = input("\nUse current date? (y/n): ").strip().lower()
                if date_choice in ['y', 'yes']:
                    transaction_date = self.get_current_date()
                    print(f"Using current date: {transaction_date}")
                    break
                elif date_choice in ['n', 'no']:
                    custom_date = input("Enter custom date (YYYY-MM-DD): ").strip()
                    if self.validate_date(custom_date):
                        transaction_date = custom_date
                        break
                    else:
                        print("Invalid date format or future date. Please use valid YYYY-MM-DD format.")
                else:
                    print("Please enter 'y' for yes or 'n' for no.")
            
            # Save to database
            self.save_transaction(ref_number, bank_name, transaction_date)
            
        except Exception as e:
            print(f"Error adding transaction: {e}")
    
    def save_transaction(self, ref_number, bank_name, date):
        """
        Save transaction to database.
        
        Args:
            ref_number (str): Reference number
            bank_name (str): Full bank name
            date (str): Transaction date
        """
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute('''
                INSERT INTO transactions (ref_number, bank_name, date)
                VALUES (?, ?, ?)
            ''', (ref_number, bank_name, date))
            
            conn.commit()
            conn.close()
            
            print(f"\n✓ Transaction saved successfully!")
            print(f"  Reference: {ref_number}")
            print(f"  Bank: {bank_name}")
            print(f"  Date: {date}")
            
        except sqlite3.IntegrityError:
            print(f"Error: Reference number {ref_number} already exists!")
        except sqlite3.Error as e:
            print(f"Error saving transaction: {e}")
    
    def view_records(self):
        """View records with filtering options."""
        while True:
            print("\n=== VIEW RECORDS ===")
            print("1. View all records")
            print("2. Filter by reference number")
            print("3. Filter by bank name")
            print("4. Filter by date")
            print("0. Back to Main Menu")
            
            choice = input("\nSelect option (0-4): ").strip()
            
            if choice == "1":
                self.display_all_records()
                input("\nPress Enter to continue...")
            elif choice == "2":
                self.filter_by_reference()
                input("\nPress Enter to continue...")
            elif choice == "3":
                self.filter_by_bank()
                input("\nPress Enter to continue...")
            elif choice == "4":
                self.filter_by_date()
                input("\nPress Enter to continue...")
            elif choice == "0":
                break
            else:
                print("Invalid option selected.")
    
    def display_records(self, records, title="Records"):
        """
        Display records in a formatted table sorted by reference number.
        
        Args:
            records (list): List of record tuples
            title (str): Title for the display
        """
        if not records:
            print(f"\nNo {title.lower()} found.")
            return
        
        # Sort records by reference number (ascending)
        records.sort(key=lambda x: x[1])
        
        print(f"\n=== {title.upper()} ===")
        print(f"{'ID':<5} {'Reference':<12} {'Bank Name':<30} {'Date':<12}")
        print("-" * 60)
        
        for record in records:
            print(f"{record[0]:<5} {record[1]:<12} {record[2]:<30} {record[3]:<12}")
        
        print(f"\nTotal records: {len(records)}")
    
    def display_all_records(self):
        """Display all records in the database sorted by reference number."""
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute("SELECT * FROM transactions ORDER BY ref_number ASC")
            records = cursor.fetchall()
            # print("Records fetched:", records)
            conn.close()
            
            self.display_records(records, "All Records")
            
        except sqlite3.Error as e:
            print(f"Error retrieving records: {e}")
    
    def filter_by_reference(self):
        """Filter records by reference number."""
        ref_number = input("Enter reference number to search: ").strip().upper()
        
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute("""
                SELECT * FROM transactions 
                WHERE UPPER(ref_number) LIKE ? 
                ORDER BY ref_number ASC
            """, (f"%{ref_number}%",))
            
            records = cursor.fetchall()
            conn.close()
            
            self.display_records(records, f"Records matching '{ref_number}'")
            
        except sqlite3.Error as e:
            print(f"Error filtering by reference: {e}")
    
    def filter_by_bank(self):
        """Filter records by bank name, partial name, or bank code."""
        print(f"\nAvailable bank codes: {', '.join(self.bank_codes.keys())}")
        # print(f"Available bank names: {', '.join(self.bank_codes.values())}")
        print("You can enter:")
        print("- Full bank name (e.g., 'PRIME COMMERCIAL BANK')")
        print("- Partial bank name (e.g., 'PRIME', 'COMMERCIAL')")
        print("- Bank code (e.g., 'PCBL', 'GBBL', 'EPF')")
        
        bank_input = input("\nEnter bank code, full name, or partial name: ").strip()
        
        if not bank_input:
            print("Please enter a valid input.")
            return
        
        # Check if input is a bank code first
        bank_input_upper = bank_input.upper()
        search_term = bank_input
        
        if bank_input_upper in self.bank_codes:
            # If it's a bank code, get the full name for exact match
            full_bank_name = self.bank_codes[bank_input_upper]
            search_condition = "UPPER(bank_name) = ?"
            search_param = full_bank_name.upper()
            display_term = f"bank code '{bank_input_upper}' ({full_bank_name})"
        else:
            # If it's not a bank code, use partial matching
            search_condition = "UPPER(bank_name) LIKE ?"
            search_param = f"%{bank_input_upper}%"
            display_term = f"'{bank_input}'"
        
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute(f"""
                SELECT * FROM transactions 
                WHERE {search_condition}
                ORDER BY ref_number ASC
            """, (search_param,))
            
            records = cursor.fetchall()
            conn.close()
            
            self.display_records(records, f"Records matching {display_term}")
            
        except sqlite3.Error as e:
            print(f"Error filtering by bank: {e}")
    
    def filter_by_date(self):
        """Filter records by specific date."""
        date_input = input("Enter date (YYYY-MM-DD): ").strip()
        
        if not self.validate_date(date_input):
            print("Invalid date format or future date. Please use valid YYYY-MM-DD format.")
            return
        
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute("""
                SELECT * FROM transactions 
                WHERE date = ? 
                ORDER BY ref_number ASC
            """, (date_input,))
            
            records = cursor.fetchall()
            conn.close()
            
            self.display_records(records, f"Records for {date_input}")
            
        except sqlite3.Error as e:
            print(f"Error filtering by date: {e}")
    
    def generate_reports(self):
        """Generate various reports."""
        while True:
            print("\n=== GENERATE REPORTS ===")
            print("1. Date range report")
            print("2. Bank-specific report")
            print("3. Reference number report")
            print("4. Summary report")
            print("0. Back to Main Menu")
            
            choice = input("\nSelect report type (0-4): ").strip()
            
            if choice == "1":
                self.date_range_report()
                input("\nPress Enter to continue...")
            elif choice == "2":
                self.bank_report()
                input("\nPress Enter to continue...")
            elif choice == "3":
                self.reference_report()
                input("\nPress Enter to continue...")
            elif choice == "4":
                self.summary_report()
                input("\nPress Enter to continue...")
            elif choice == "0":
                break
            else:
                print("Invalid option selected.")
    
    def date_range_report(self):
        """Generate report for a specific date range."""
        print("\n=== DATE RANGE REPORT ===")
        
        start_date = input("Enter start date (YYYY-MM-DD): ").strip()
        if not self.validate_date(start_date):
            print("Invalid start date format.")
            return
        
        end_date = input("Enter end date (YYYY-MM-DD): ").strip()
        if not self.validate_date(end_date):
            print("Invalid end date format.")
            return
        
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute("""
                SELECT * FROM transactions 
                WHERE date BETWEEN ? AND ? 
                ORDER BY ref_number ASC
            """, (start_date, end_date))
            
            records = cursor.fetchall()
            conn.close()
            
            self.display_records(records, f"Transactions from {start_date} to {end_date}")
            
        except sqlite3.Error as e:
            print(f"Error generating date range report: {e}")
    
    def bank_report(self):
        """Generate report for a specific bank."""
        print(f"\nSupported banks: {', '.join(self.bank_codes.values())}")
        bank_input = input("Enter bank name or code: ").strip()
        
        # Check if input is a bank code
        bank_name = bank_input.upper()
        if bank_name in self.bank_codes:
            bank_name = self.bank_codes[bank_name]
        else:
            bank_name = bank_input
        
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute("""
                SELECT * FROM transactions 
                WHERE UPPER(bank_name) LIKE ? 
                ORDER BY ref_number ASC
            """, (f"%{bank_name.upper()}%",))
            
            records = cursor.fetchall()
            conn.close()
            
            self.display_records(records, f"Transactions for '{bank_name}'")
            
        except sqlite3.Error as e:
            print(f"Error generating bank report: {e}")
    
    def reference_report(self):
        """Generate report for a specific reference number."""
        ref_number = input("Enter reference number: ").strip()
        
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute("""
                SELECT * FROM transactions 
                WHERE UPPER(ref_number) = ? 
                ORDER BY ref_number ASC
            """, (ref_number.upper(),))
            
            records = cursor.fetchall()
            conn.close()
            
            self.display_records(records, f"Transaction details for '{ref_number}'")
            
        except sqlite3.Error as e:
            print(f"Error generating reference report: {e}")
    
    def summary_report(self):
        """Generate a summary report of all transactions."""
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            # Total transactions
            cursor.execute("SELECT COUNT(*) FROM transactions")
            total_count = cursor.fetchone()[0]
            
            # Transactions by bank
            cursor.execute("""
                SELECT bank_name, COUNT(*) as count 
                FROM transactions 
                GROUP BY bank_name 
                ORDER BY count DESC
            """)
            bank_stats = cursor.fetchall()
            
            # Recent transactions
            cursor.execute("""
                SELECT * FROM transactions 
                ORDER BY date DESC, id DESC 
                LIMIT 5
            """)
            recent_transactions = cursor.fetchall()
            
            conn.close()
            
            print("\n=== SUMMARY REPORT ===")
            print(f"Total Transactions: {total_count}")
            
            print("\n--- Transactions by Bank ---")
            for bank, count in bank_stats:
                print(f"{bank}: {count} transaction(s)")
            
            print("\n--- Recent Transactions (Last 5) ---")
            if recent_transactions:
                print(f"{'ID':<5} {'Reference':<12} {'Bank Name':<25} {'Date':<12}")
                print("-" * 60)
                for record in recent_transactions:
                    print(f"{record[0]:<5} {record[1]:<12} {record[2]:<25} {record[3]:<12}")
            else:
                print("No transactions found.")
            
        except sqlite3.Error as e:
            print(f"Error generating summary report: {e}")
    
    def export_to_csv(self):
        """Export all transaction data to a CSV file."""
        print("\n=== EXPORT TO CSV ===")
        
        # Get filename from user
        filename = input("Enter filename (default: transactions.csv): ").strip()
        if not filename:
            filename = "transactions.csv"
        
        # Ensure filename ends with .csv
        if not filename.lower().endswith('.csv'):
            filename += '.csv'
        
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            # Get all records sorted by reference number
            cursor.execute("SELECT * FROM transactions ORDER BY ref_number ASC")
            records = cursor.fetchall()
            conn.close()
            
            if not records:
                print("No records found to export.")
                return
            
            # Write to CSV file
            with open(filename, 'w', newline='', encoding='utf-8') as csvfile:
                writer = csv.writer(csvfile)
                
                # Write header
                writer.writerow(['ID', 'Reference Number', 'Bank Name', 'Date'])
                
                # Write data
                for record in records:
                    writer.writerow(record)
            
            print(f"✓ Successfully exported {len(records)} records to {filename}")
            
        except PermissionError:
            print(f"Error: Permission denied. Could not write to {filename}")
        except Exception as e:
            print(f"Error exporting to CSV: {e}")
    
    def edit_record(self):
        """Edit an existing transaction record."""
        print("\n=== EDIT RECORD ===")
        
        # First, let user search for the record they want to edit
        while True:
            print("\nSearch for the record you want to edit:")
            print("1. Search by ID")
            print("2. Search by reference number")
            print("3. Show all records to select")
            print("0. Back to Main Menu")
            
            search_choice = input("\nSelect search option (0-3): ").strip()
            
            if search_choice == "0":
                return
            
            record_to_edit = None
            
            if search_choice == "1":
                record_to_edit = self.search_by_id()
            elif search_choice == "2":
                record_to_edit = self.search_by_ref_for_edit()
            elif search_choice == "3":
                record_to_edit = self.select_from_all_records()
            else:
                print("Invalid option selected.")
                continue
            
            if not record_to_edit:
                retry = input("\nNo record selected. Try again? (y/n): ").strip().lower()
                if retry not in ['y', 'yes']:
                    return
                continue
            
            # Show current record details
            print(f"\n=== CURRENT RECORD DETAILS ===")
            print(f"ID: {record_to_edit[0]}")
            print(f"Reference Number: {record_to_edit[1]}")
            print(f"Bank Name: {record_to_edit[2]}")
            print(f"Date: {record_to_edit[3]}")
            
            # Confirm edit
            confirm = input("\nDo you want to edit this record? (y/n): ").strip().lower()
            if confirm not in ['y', 'yes']:
                continue
            
            # Edit fields
            print("\n=== EDIT FIELDS ===")
            print("Leave blank to keep current value")
            
            # Edit reference number
            current_ref = record_to_edit[1]
            print(f"\nCurrent reference number: {current_ref}")
            new_ref_input = input("Enter new number (or press Enter to keep current): ").strip()
            
            if new_ref_input:
                try:
                    new_ref = self.format_reference_number(new_ref_input)
                    print(f"New reference number: {new_ref}")
                except ValueError as e:
                    print(f"Error: {e}")
                    new_ref = current_ref
            else:
                new_ref = current_ref
            
            # Edit bank
            current_bank = record_to_edit[2]
            print(f"\nCurrent bank: {current_bank}")
            print(f"Supported bank codes: {', '.join(self.bank_codes.keys())}")
            new_bank_input = input("Enter new bank code/name (or press Enter to keep current): ").strip()
            
            if new_bank_input:
                try:
                    # Check if it's a bank code
                    if new_bank_input.upper() in self.bank_codes:
                        new_bank = self.bank_codes[new_bank_input.upper()]
                    else:
                        # Assume it's a bank name (validate it exists in our supported banks)
                        new_bank_upper = new_bank_input.upper()
                        found_bank = None
                        for code, name in self.bank_codes.items():
                            if new_bank_upper in name.upper():
                                found_bank = name
                                break
                        
                        if found_bank:
                            new_bank = found_bank
                        else:
                            print(f"Bank not recognized. Keeping current bank: {current_bank}")
                            new_bank = current_bank
                    
                    print(f"New bank: {new_bank}")
                except Exception as e:
                    print(f"Error: {e}. Keeping current bank.")
                    new_bank = current_bank
            else:
                new_bank = current_bank
            
            # Edit date
            current_date = record_to_edit[3]
            print(f"\nCurrent date: {current_date}")
            new_date_input = input("Enter new date (YYYY-MM-DD) or press Enter to keep current: ").strip()
            
            if new_date_input:
                if self.validate_date(new_date_input):
                    new_date = new_date_input
                    print(f"New date: {new_date}")
                else:
                    print(f"Invalid date format or future date. Keeping current date: {current_date}")
                    new_date = current_date
            else:
                new_date = current_date
            
            # Show summary of changes
            print(f"\n=== SUMMARY OF CHANGES ===")
            print(f"ID: {record_to_edit[0]} (unchanged)")
            print(f"Reference: {current_ref} → {new_ref}")
            print(f"Bank: {current_bank} → {new_bank}")
            print(f"Date: {current_date} → {new_date}")
            
            # Final confirmation
            final_confirm = input("\nSave these changes? (y/n): ").strip().lower()
            if final_confirm in ['y', 'yes']:
                self.update_record(record_to_edit[0], new_ref, new_bank, new_date)
                input("\nPress Enter to continue...")
            else:
                print("Changes cancelled.")
            
            # Ask if user wants to edit another record
            another = input("\nEdit another record? (y/n): ").strip().lower()
            if another not in ['y', 'yes']:
                break
    
    def search_by_id(self):
        """Search for a record by ID."""
        try:
            record_id = input("Enter record ID: ").strip()
            if not record_id.isdigit():
                print("Please enter a valid numeric ID.")
                return None
            
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute("SELECT * FROM transactions WHERE id = ?", (int(record_id),))
            record = cursor.fetchone()
            conn.close()
            
            if record:
                return record
            else:
                print(f"No record found with ID: {record_id}")
                return None
                
        except sqlite3.Error as e:
            print(f"Error searching by ID: {e}")
            return None
    
    def search_by_ref_for_edit(self):
        """Search for a record by reference number for editing."""
        ref_input = input("Enter reference number: ").strip()
        
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute("""
                SELECT * FROM transactions 
                WHERE UPPER(ref_number) = ? 
                ORDER BY ref_number ASC
            """, (ref_input.upper(),))
            
            records = cursor.fetchall()
            conn.close()
            
            if not records:
                print(f"No record found with reference number: {ref_input}")
                return None
            elif len(records) == 1:
                return records[0]
            else:
                # Multiple records with same reference (shouldn't happen, but handle it)
                print(f"\nMultiple records found with reference {ref_input}:")
                for i, record in enumerate(records, 1):
                    print(f"{i}. ID: {record[0]}, Bank: {record[2]}, Date: {record[3]}")
                
                choice = input(f"\nSelect record (1-{len(records)}): ").strip()
                if choice.isdigit() and 1 <= int(choice) <= len(records):
                    return records[int(choice) - 1]
                else:
                    print("Invalid selection.")
                    return None
                    
        except sqlite3.Error as e:
            print(f"Error searching by reference: {e}")
            return None
    
    def select_from_all_records(self):
        """Show all records and let user select one to edit."""
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute("SELECT * FROM transactions ORDER BY ref_number ASC")
            records = cursor.fetchall()
            conn.close()
            
            if not records:
                print("No records found in database.")
                return None
            
            print(f"\n=== ALL RECORDS ===")
            print(f"{'#':<3} {'ID':<5} {'Reference':<12} {'Bank Name':<25} {'Date':<12}")
            print("-" * 65)
            
            for i, record in enumerate(records, 1):
                print(f"{i:<3} {record[0]:<5} {record[1]:<12} {record[2]:<25} {record[3]:<12}")
            
            choice = input(f"\nSelect record to edit (1-{len(records)}): ").strip()
            if choice.isdigit() and 1 <= int(choice) <= len(records):
                return records[int(choice) - 1]
            else:
                print("Invalid selection.")
                return None
                
        except sqlite3.Error as e:
            print(f"Error retrieving records: {e}")
            return None
    
    def update_record(self, record_id, ref_number, bank_name, date):
        """
        Update a record in the database.
        
        Args:
            record_id (int): ID of the record to update
            ref_number (str): New reference number
            bank_name (str): New bank name
            date (str): New date
        """
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            cursor.execute("""
                UPDATE transactions 
                SET ref_number = ?, bank_name = ?, date = ?
                WHERE id = ?
            """, (ref_number, bank_name, date, record_id))
            
            if cursor.rowcount > 0:
                conn.commit()
                print(f"\n✓ Record updated successfully!")
                print(f"  ID: {record_id}")
                print(f"  Reference: {ref_number}")
                print(f"  Bank: {bank_name}")
                print(f"  Date: {date}")
            else:
                print("No record was updated. Record might not exist.")
            
            conn.close()
            
        except sqlite3.IntegrityError:
            print(f"Error: Reference number {ref_number} already exists!")
        except sqlite3.Error as e:
            print(f"Error updating record: {e}")

    def view_latest_records(self):
        """Generate a summary report of all transactions."""
        try:
            conn = sqlite3.connect(self.db_name)
            cursor = conn.cursor()
            
            # Total transactions
            cursor.execute("SELECT COUNT(*) FROM transactions")
            total_count = cursor.fetchone()[0]
            
            # Transactions by bank
            cursor.execute("""
                SELECT bank_name, COUNT(*) as count 
                FROM transactions 
                GROUP BY bank_name 
                ORDER BY count DESC
            """)
            bank_stats = cursor.fetchall()
            
            # Recent transactions
            cursor.execute("""
                SELECT * FROM transactions 
                ORDER BY date DESC, id DESC 
                LIMIT 10
            """)
            recent_transactions = cursor.fetchall()
            
            conn.close()
            
            print("\n=== SUMMARY REPORT ===")
            print(f"Total Transactions: {total_count}")
            
            print("\n--- Transactions by Bank ---")
            for bank, count in bank_stats:
                print(f"{bank}: {count} transaction(s)")
            
            print("\n--- Recent Transactions (Last 10) ---")
            if recent_transactions:
                print(f"{'ID':<5} {'Reference':<12} {'Bank Name':<35} {'Date':<12}")
                print("-" * 60)
                for record in recent_transactions:
                    print(f"{record[0]:<5} {record[1]:<12} {record[2]:<35} {record[3]:<12}")
            else:
                print("No transactions found.")
            
        except sqlite3.Error as e:
            print(f"Error generating summary report: {e}")

    def run(self):
        """Main application loop."""
        print("=" * 50)
        print("    TRANSACTION MANAGEMENT SYSTEM")
        print("=" * 50)
        
        while True:
            print("\n=== MAIN MENU ===")
            print("1. Add Transaction")
            print("2. View Records")
            print("3. Edit Record")
            print("4. Generate Reports")
            print("5. Export to CSV")
            print("6. View latest 10 records")
            print("0. Exit")
            
            choice = input("\nSelect option (0-6): ").strip()
            
            if choice == "1":
                self.add_transaction()
            elif choice == "2":
                self.view_records()
            elif choice == "3":
                self.edit_record()
            elif choice == "4":
                self.generate_reports()
            elif choice == "5":
                self.export_to_csv()
                input("\nPress Enter to continue...")
            elif choice == "6":
                self.view_latest_records()
            elif choice == "0":
                print("\nThank you for using Transaction Management System!")
                break
            else:
                print("Invalid option. Please select 0-5.")
            
            input("\nPress Enter to continue...")


def main():
    """Main function to run the application."""
    try:
        app = TransactionManager()
        app.run()
    except KeyboardInterrupt:
        print("\n\nApplication terminated by user.")
    except Exception as e:
        print(f"\nUnexpected error: {e}")


if __name__ == "__main__":
    main()