CREATE TABLE IF NOT EXISTS patients (
    patient_id INTEGER PRIMARY KEY AUTOINCREMENT,
    full_name TEXT NOT NULL,
    age INTEGER CHECK(age > 0),
    weight REAL CHECK(weight > 0),
    address TEXT,
    contact TEXT NOT NULL,
    gender TEXT CHECK(gender IN ('M','F','O'))
);

CREATE TABLE IF NOT EXISTS doctors (
    doctor_id INTEGER PRIMARY KEY AUTOINCREMENT,
    full_name TEXT NOT NULL,
    specialization TEXT NOT NULL,
    contact TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS appointments (
    appointment_id INTEGER PRIMARY KEY AUTOINCREMENT,
    patient_id INTEGER NOT NULL,
    doctor_id INTEGER NOT NULL,
    appointment_date TEXT NOT NULL CHECK(appointment_date GLOB '????-??-??'),
    appointment_time TEXT NOT NULL CHECK(appointment_time GLOB '??:??'),
    FOREIGN KEY(patient_id) REFERENCES patients(patient_id) ON DELETE CASCADE,
    FOREIGN KEY(doctor_id) REFERENCES doctors(doctor_id) ON DELETE CASCADE
);