# Rental Management System

## Overview
A console-based application for homeowners to manage multi-story properties with multiple flats and rooms. Built in C with SQLite for data storage, this system helps manage property structures, tenants, rent payments, and generates reports.

## Features
- **Property Structure Management**
  - Add, edit, view, and delete rooms
  - Add, edit, view, and delete flats
  - Assign rooms to flats
- **Tenant Management** (Coming Soon)
- **Rent & Utility Management** (Coming Soon)
- **Reporting** (Coming Soon)
- **Data Export** (Coming Soon)

## Getting Started

### Prerequisites
- GCC compiler
- SQLite3 development libraries

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/rental-management-system.git
   ```
2. Navigate to the project directory:
   ```bash
   cd rental-management-system
   ```

### Compilation
Compile the program using:
```bash
gcc rental_system.c -o rental_system -lsqlite3
```

### Running the Application
Execute the compiled binary:
```bash
./rental_system
```

## Usage
1. **Main Menu**:
   - Property Structure Management
   - Tenant Management
   - Rent and Utility Management
   - Reports
   - Data Export
   - Exit

2. **Property Structure Management**:
   - Room Management (Add, View, Edit, Delete rooms)
   - Flat Management (Add, View, Edit, Delete flats, Assign rooms to flats)

3. **Database**:
   - Automatically creates `rental_management.db` file
   - Stores rooms and flats information

## Database Schema
The application uses two main tables:

### Rooms Table
| Column       | Type    | Description               |
|-------------|---------|---------------------------|
| id          | INTEGER | Primary key, auto-increment |
| number      | TEXT    | Unique room identifier    |
| description | TEXT    | Room description          |

### Flats Table
| Column       | Type    | Description               |
|-------------|---------|---------------------------|
| id          | INTEGER | Primary key, auto-increment |
| number      | TEXT    | Unique flat identifier    |
| description | TEXT    | Flat description          |

## Future Development
- Tenant management with contact information
- Rent and utility tracking
- Payment recording and receipt generation
- Financial reporting and KPIs
- Data export to CSV format
