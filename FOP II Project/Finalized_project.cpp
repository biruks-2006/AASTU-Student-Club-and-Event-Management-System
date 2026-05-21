#include <iostream>
#include <sqlite3.h>
#include <string>
#include <limits>
using namespace std;

sqlite3* db;
char* errMsg = nullptr;

struct Student {
    int s_id;
    string name, dept;
};
struct Club {
    int c_id, leader_id;
    string name, description;
};
struct Event {
    int event_id, club_id, date, month, year;
    string title, description;
};
struct Membership {
    int s_id, club_id, date, month, year;
};

string escape_sql(const string& s) {
    string result;
    for (char c : s) {
        if (c == '\'') result += "''";   // SQLite escapes ' as ''
        else result += c;
    }
    return result;
}

bool record_exists(const string& table, const string& col, int id) {
    sqlite3_stmt* stmt;
    string sql = "SELECT 1 FROM " + table + " WHERE " + col + " = " + to_string(id) + " LIMIT 1;";
    bool found = false;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW)
            found = true;
        sqlite3_finalize(stmt);
    }
    return found;
}

bool composite_exists(const string& table,
                      const string& col1, int id1,
                      const string& col2, int id2) {
    sqlite3_stmt* stmt;
    string sql = "SELECT 1 FROM " + table +
                 " WHERE " + col1 + " = " + to_string(id1) +
                 " AND "   + col2 + " = " + to_string(id2) + " LIMIT 1;";
    bool found = false;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW)
            found = true;
        sqlite3_finalize(stmt);
    }
    return found;
}

bool exec_sql(const string& sql) {
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL Error: " << (errMsg ? errMsg : "unknown") << endl;
        sqlite3_free(errMsg);
        errMsg = nullptr;
        return false;
    }
    return true;
}

void flush_cin() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void create_db() {
    if (sqlite3_open("AASTU_CLUB_MGT.db", &db) != SQLITE_OK) {
        cerr << "Failed to open database: " << sqlite3_errmsg(db) << endl;
        return;
    }
    cout << "Database opened successfully!" << endl;
    exec_sql("PRAGMA foreign_keys = ON;");

    exec_sql("CREATE TABLE IF NOT EXISTS Student ("
             "s_id INTEGER PRIMARY KEY, "
             "name TEXT NOT NULL, "
             "dept TEXT NOT NULL);");

    exec_sql("CREATE TABLE IF NOT EXISTS Club ("
             "c_id INTEGER PRIMARY KEY, "
             "leader_id INTEGER, "
             "name TEXT NOT NULL, "
             "description TEXT, "
             "FOREIGN KEY(leader_id) REFERENCES Student(s_id) ON DELETE SET NULL);");

    exec_sql("CREATE TABLE IF NOT EXISTS Event ("
             "event_id INTEGER PRIMARY KEY, "
             "club_id INTEGER, "
             "title TEXT NOT NULL, "
             "description TEXT, "
             "date INTEGER NOT NULL, "
             "month INTEGER NOT NULL, "
             "year INTEGER NOT NULL, "
             "FOREIGN KEY(club_id) REFERENCES Club(c_id) ON DELETE SET NULL);");

    exec_sql("CREATE TABLE IF NOT EXISTS Membership ("
             "s_id INTEGER, "
             "club_id INTEGER, "
             "date INTEGER NOT NULL, "
             "month INTEGER NOT NULL, "
             "year INTEGER NOT NULL, "
             "PRIMARY KEY(s_id, club_id), "
             "FOREIGN KEY(s_id) REFERENCES Student(s_id) ON DELETE CASCADE, "
             "FOREIGN KEY(club_id) REFERENCES Club(c_id) ON DELETE CASCADE);");

    exec_sql("CREATE TABLE IF NOT EXISTS Attendance ("
             "club_id INTEGER, "
             "s_id INTEGER, "
             "condition TEXT NOT NULL, "
             "PRIMARY KEY(club_id, s_id), "
             "FOREIGN KEY(club_id) REFERENCES Club(c_id) ON DELETE CASCADE, "
             "FOREIGN KEY(s_id) REFERENCES Student(s_id) ON DELETE CASCADE);");
}

void Register() {
    int n;
    cout << "How many students do you want to register? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        Student a;
        cout << "Enter Student " << i + 1 << " ID, Name and Department: ";
        cin >> a.s_id >> a.name >> a.dept;

        if (record_exists("Student", "s_id", a.s_id)) {
            cout << "  [!] Student ID " << a.s_id << " already exists. Skipping.\n";
            continue;
        }

        string sql = "INSERT INTO Student VALUES (" +
                     to_string(a.s_id) + ", '" +
                     escape_sql(a.name) + "', '" +
                     escape_sql(a.dept) + "');";

        if (exec_sql(sql))
            cout << "  [+] Student " << a.name << " registered.\n";
    }
}

void Create_Club() {
    int n;
    cout << "How many clubs do you want to create? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        Club c;
        cout << "Enter Club " << i + 1 << " ID, Leader ID, Name and Description: ";
        cin >> c.c_id >> c.leader_id >> c.name >> c.description;

        if (record_exists("Club", "c_id", c.c_id)) {
            cout << "  [!] Club ID " << c.c_id << " already exists. Skipping.\n";
            continue;
        }
        if (!record_exists("Student", "s_id", c.leader_id)) {
            cout << "  [!] Leader ID " << c.leader_id << " does not exist. Skipping.\n";
            continue;
        }

        string sql = "INSERT INTO Club VALUES (" +
                     to_string(c.c_id) + ", " +
                     to_string(c.leader_id) + ", '" +
                     escape_sql(c.name) + "', '" +
                     escape_sql(c.description) + "');";

        if (exec_sql(sql))
            cout << "  [+] Club " << c.name << " created.\n";
    }
}

void Create_Event() {
    int n;
    cout << "How many events do you want to create? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        Event e;
        cout << "Enter Event " << i + 1
             << " ID, Club ID, Date, Month, Year, Title and Description: ";
        cin >> e.event_id >> e.club_id >> e.date >> e.month >> e.year
            >> e.title >> e.description;

        if (record_exists("Event", "event_id", e.event_id)) {
            cout << "  [!] Event ID " << e.event_id << " already exists. Skipping.\n";
            continue;
        }
        if (!record_exists("Club", "c_id", e.club_id)) {
            cout << "  [!] Club ID " << e.club_id << " does not exist. Skipping.\n";
            continue;
        }
        if (e.date < 1 || e.date > 31 || e.month < 1 || e.month > 12 || e.year < 2000) {
            cout << "  [!] Invalid date. Skipping.\n";
            continue;
        }

        string sql = "INSERT INTO Event VALUES (" +
                     to_string(e.event_id) + ", " +
                     to_string(e.club_id)  + ", '" +
                     escape_sql(e.title)   + "', '" +
                     escape_sql(e.description) + "', " +
                     to_string(e.date)  + ", " +
                     to_string(e.month) + ", " +
                     to_string(e.year)  + ");";

        if (exec_sql(sql))
            cout << "  [+] Event \"" << e.title << "\" created.\n";
    }
}

void Join_Club() {
    int n;
    cout << "How many memberships do you want to register? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        Membership m;
        cout << "Enter Member " << i + 1
             << " Student ID, Club ID, Date, Month and Year: ";
        cin >> m.s_id >> m.club_id >> m.date >> m.month >> m.year;

        if (!record_exists("Student", "s_id", m.s_id)) {
            cout << "  [!] Student ID " << m.s_id << " does not exist. Skipping.\n";
            continue;
        }
        if (!record_exists("Club", "c_id", m.club_id)) {
            cout << "  [!] Club ID " << m.club_id << " does not exist. Skipping.\n";
            continue;
        }
        if (composite_exists("Membership", "s_id", m.s_id, "club_id", m.club_id)) {
            cout << "  [!] Student " << m.s_id
                 << " is already a member of Club " << m.club_id << ". Skipping.\n";
            continue;
        }

        string sql = "INSERT INTO Membership VALUES (" +
                     to_string(m.s_id)   + ", " +
                     to_string(m.club_id) + ", " +
                     to_string(m.date)   + ", " +
                     to_string(m.month)  + ", " +
                     to_string(m.year)   + ");";

        if (exec_sql(sql))
            cout << "  [+] Student " << m.s_id << " joined Club " << m.club_id << ".\n";
    }
}

void Record_Attendance() {
    int n;
    cout << "How many attendance records do you want to enter? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        int s_id, club_id;
        string condition;
        cout << "Enter " << i + 1 << " Student ID, Club ID and Status (present/absent): ";
        cin >> s_id >> club_id >> condition;

        if (!record_exists("Student", "s_id", s_id)) {
            cout << "  [!] Student ID " << s_id << " does not exist. Skipping.\n";
            continue;
        }
        if (!record_exists("Club", "c_id", club_id)) {
            cout << "  [!] Club ID " << club_id << " does not exist. Skipping.\n";
            continue;
        }
        if (!composite_exists("Membership", "s_id", s_id, "club_id", club_id)) {
            cout << "  [!] Student " << s_id
                 << " is not a member of Club " << club_id << ". Skipping.\n";
            continue;
        }
        if (composite_exists("Attendance", "s_id", s_id, "club_id", club_id)) {
            cout << "  [!] Attendance already recorded for Student " << s_id
                 << " in Club " << club_id << ". Use Update to change it.\n";
            continue;
        }

        string sql = "INSERT INTO Attendance VALUES (" +
                     to_string(club_id)         + ", " +
                     to_string(s_id)            + ", '" +
                     escape_sql(condition)       + "');";

        if (exec_sql(sql))
            cout << "  [+] Attendance recorded.\n";
    }
}

void Update_student() {
    int id;
    string newName, newDept;
    cout << "Enter Student ID to update: ";
    cin >> id;

    if (!record_exists("Student", "s_id", id)) {
        cout << "  [!] Student ID " << id << " does not exist.\n";
        return;
    }

    cout << "Enter new Name: ";
    cin >> newName;
    cout << "Enter new Department: ";
    cin >> newDept;

    string sql = "UPDATE Student SET name='" + escape_sql(newName) +
                 "', dept='" + escape_sql(newDept) +
                 "' WHERE s_id=" + to_string(id) + ";";

    if (exec_sql(sql))
        cout << "  [+] Student updated successfully.\n";
}

void Update_club() {
    int id, newLeaderID;
    string newName, newDesc;
    cout << "Enter Club ID to update: ";
    cin >> id;

    if (!record_exists("Club", "c_id", id)) {
        cout << "  [!] Club ID " << id << " does not exist.\n";
        return;
    }

    cout << "Enter new Leader ID: ";
    cin >> newLeaderID;

    if (!record_exists("Student", "s_id", newLeaderID)) {
        cout << "  [!] Leader ID " << newLeaderID << " does not exist.\n";
        return;
    }

    cout << "Enter new Name: ";
    cin >> newName;
    cout << "Enter new Description: ";
    cin >> newDesc;

    string sql = "UPDATE Club SET leader_id=" + to_string(newLeaderID) +
                 ", name='" + escape_sql(newName) +
                 "', description='" + escape_sql(newDesc) +
                 "' WHERE c_id=" + to_string(id) + ";";

    if (exec_sql(sql))
        cout << "  [+] Club updated successfully.\n";
}

void Update_event() {
    int id, newDate, newMonth, newYear;
    string newTitle, newDesc;
    cout << "Enter Event ID to update: ";
    cin >> id;

    if (!record_exists("Event", "event_id", id)) {
        cout << "  [!] Event ID " << id << " does not exist.\n";
        return;
    }

    cout << "Enter new Title: ";
    cin >> newTitle;
    cout << "Enter new Description: ";
    cin >> newDesc;
    cout << "Enter new Date Month Year: ";
    cin >> newDate >> newMonth >> newYear;

    if (newDate < 1 || newDate > 31 || newMonth < 1 || newMonth > 12 || newYear < 2000) {
        cout << "  [!] Invalid date.\n";
        return;
    }

    string sql = "UPDATE Event SET title='" + escape_sql(newTitle) +
                 "', description='" + escape_sql(newDesc) +
                 "', date=" + to_string(newDate) +
                 ", month=" + to_string(newMonth) +
                 ", year=" + to_string(newYear) +
                 " WHERE event_id=" + to_string(id) + ";";

    if (exec_sql(sql))
        cout << "  [+] Event updated successfully.\n";
}

void Update_attendance() {
    int sID, clubID;
    string newCondition;
    cout << "Enter Student ID: ";
    cin >> sID;
    cout << "Enter Club ID: ";
    cin >> clubID;

    if (!composite_exists("Attendance", "s_id", sID, "club_id", clubID)) {
        cout << "  [!] No attendance record found for Student " << sID
             << " in Club " << clubID << ".\n";
        return;
    }

    cout << "Enter new Attendance Status (present/absent): ";
    cin >> newCondition;

    string sql = "UPDATE Attendance SET condition='" + escape_sql(newCondition) +
                 "' WHERE s_id=" + to_string(sID) +
                 " AND club_id=" + to_string(clubID) + ";";

    if (exec_sql(sql))
        cout << "  [+] Attendance updated successfully.\n";
}

void Delete_student() {
    int id;
    char confirm;
    cout << "Enter Student ID to delete: ";
    cin >> id;

    if (!record_exists("Student", "s_id", id)) {
        cout << "  [!] Student ID " << id << " does not exist.\n";
        return;
    }

    cout << "Are you sure? This will also remove all their memberships and attendance. (y/n): ";
    cin >> confirm;
    if (confirm != 'y') { cout << "  Delete cancelled.\n"; return; }

    if (exec_sql("DELETE FROM Student WHERE s_id=" + to_string(id) + ";"))
        cout << "  [+] Student deleted successfully.\n";
}

void Delete_club() {
    int id;
    char confirm;
    cout << "Enter Club ID to delete: ";
    cin >> id;

    if (!record_exists("Club", "c_id", id)) {
        cout << "  [!] Club ID " << id << " does not exist.\n";
        return;
    }

    cout << "Are you sure? This will also remove all events, memberships and attendance for this club. (y/n): ";
    cin >> confirm;
    if (confirm != 'y') { cout << "  Delete cancelled.\n"; return; }

    if (exec_sql("DELETE FROM Club WHERE c_id=" + to_string(id) + ";"))
        cout << "  [+] Club deleted successfully.\n";
}

void Delete_event() {
    int id;
    char confirm;
    cout << "Enter Event ID to delete: ";
    cin >> id;

    if (!record_exists("Event", "event_id", id)) {
        cout << "  [!] Event ID " << id << " does not exist.\n";
        return;
    }

    cout << "Are you sure you want to delete this event? (y/n): ";
    cin >> confirm;
    if (confirm != 'y') { cout << "  Delete cancelled.\n"; return; }

    if (exec_sql("DELETE FROM Event WHERE event_id=" + to_string(id) + ";"))
        cout << "  [+] Event deleted successfully.\n";
}

void Delete_membership() {
    int sID, clubID;
    char confirm;
    cout << "Enter Student ID: ";
    cin >> sID;
    cout << "Enter Club ID: ";
    cin >> clubID;

    if (!composite_exists("Membership", "s_id", sID, "club_id", clubID)) {
        cout << "  [!] No membership found for Student " << sID
             << " in Club " << clubID << ".\n";
        return;
    }

    cout << "Are you sure you want to remove this member from the club? (y/n): ";
    cin >> confirm;
    if (confirm != 'y') { cout << "  Delete cancelled.\n"; return; }

    if (exec_sql("DELETE FROM Membership WHERE s_id=" + to_string(sID) +
                 " AND club_id=" + to_string(clubID) + ";"))
        cout << "  [+] Membership deleted successfully.\n";
}

void Delete_attendance() {
    int sID, clubID;
    char confirm;
    cout << "Enter Student ID: ";
    cin >> sID;
    cout << "Enter Club ID: ";
    cin >> clubID;

    if (!composite_exists("Attendance", "s_id", sID, "club_id", clubID)) {
        cout << "  [!] No attendance record found for Student " << sID
             << " in Club " << clubID << ".\n";
        return;
    }

    cout << "Are you sure you want to delete this attendance record? (y/n): ";
    cin >> confirm;
    if (confirm != 'y') { cout << "  Delete cancelled.\n"; return; }

    if (exec_sql("DELETE FROM Attendance WHERE s_id=" + to_string(sID) +
                 " AND club_id=" + to_string(clubID) + ";"))
        cout << "  [+] Attendance record deleted successfully.\n";
}


void report_students() {
    sqlite3_stmt* stmt;
    cout << "║       STUDENT REPORT        ║\n";
    cout << "ID\tName\t\tDept\n";
    cout << "─────────────────────────────\n";

    sqlite3_prepare_v2(db, "SELECT s_id, name, dept FROM Student;", -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << sqlite3_column_int(stmt, 0)  << "\t"
             << (char*)sqlite3_column_text(stmt, 1) << "\t\t"
             << (char*)sqlite3_column_text(stmt, 2) << "\n";
    }
    sqlite3_finalize(stmt);

    cout << "\nStudents Per Department\n─────────────────────────────\n";
    sqlite3_prepare_v2(db, "SELECT dept, COUNT(*) FROM Student GROUP BY dept;", -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW)
        cout << (char*)sqlite3_column_text(stmt, 0) << ": "
             << sqlite3_column_int(stmt, 1) << " students\n";
    sqlite3_finalize(stmt);
}

void report_clubs() {
    sqlite3_stmt* stmt;
    cout << "║         CLUB REPORT         ║\n";
    cout << "ID\tLeader ID\tName\t\tDescription\n";
    cout << "─────────────────────────────────────\n";

    sqlite3_prepare_v2(db, "SELECT c_id, leader_id, name, description FROM Club;", -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << sqlite3_column_int(stmt, 0)  << "\t"
             << sqlite3_column_int(stmt, 1)  << "\t\t"
             << (char*)sqlite3_column_text(stmt, 2) << "\t\t"
             << (char*)sqlite3_column_text(stmt, 3) << "\n";
    }
    sqlite3_finalize(stmt);

    cout << "\nTotal Clubs: ";
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM Club;", -1, &stmt, nullptr);
    if (sqlite3_step(stmt) == SQLITE_ROW)
        cout << sqlite3_column_int(stmt, 0) << "\n";
    sqlite3_finalize(stmt);
}

void report_events() {
    sqlite3_stmt* stmt;
    cout << "║         EVENT REPORT        ║\n";
    cout << "ID\tClub ID\tTitle\t\tDate\n";
    cout << "─────────────────────────────────────\n";

    sqlite3_prepare_v2(db,
        "SELECT event_id, club_id, title, date, month, year FROM Event;",
        -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << sqlite3_column_int(stmt, 0) << "\t"
             << sqlite3_column_int(stmt, 1) << "\t"
             << (char*)sqlite3_column_text(stmt, 2) << "\t\t"
             << sqlite3_column_int(stmt, 3) << "/"   // FIX: read as int, stored as int
             << sqlite3_column_int(stmt, 4) << "/"
             << sqlite3_column_int(stmt, 5) << "\n";
    }
    sqlite3_finalize(stmt);

    cout << "\nEvents Per Day\n─────────────────────────────\n";
    sqlite3_prepare_v2(db,
        "SELECT date, month, year, COUNT(*) FROM Event GROUP BY date, month, year;",
        -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << sqlite3_column_int(stmt, 0) << "/"
             << sqlite3_column_int(stmt, 1) << "/"
             << sqlite3_column_int(stmt, 2) << ": "
             << sqlite3_column_int(stmt, 3) << " event(s)\n";
    }
    sqlite3_finalize(stmt);
}

void report_memberships() {
    sqlite3_stmt* stmt;
    cout << "║      MEMBERSHIP REPORT      ║\n";
    cout << "Student ID\tClub ID\t\tJoin Date\n";
    cout << "─────────────────────────────────────\n";

    sqlite3_prepare_v2(db,
        "SELECT s_id, club_id, date, month, year FROM Membership;",
        -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << sqlite3_column_int(stmt, 0) << "\t\t"
             << sqlite3_column_int(stmt, 1) << "\t\t"
             << sqlite3_column_int(stmt, 2) << "/"
             << sqlite3_column_int(stmt, 3) << "/"
             << sqlite3_column_int(stmt, 4) << "\n";
    }
    sqlite3_finalize(stmt);
}

void report_attendance() {
    sqlite3_stmt* stmt;
    cout << "║      ATTENDANCE REPORT      ║\n";
    cout << "Club ID\tStudent ID\tStatus\n";
    cout << "─────────────────────────────────────\n";

    sqlite3_prepare_v2(db,
        "SELECT club_id, s_id, condition FROM Attendance;",
        -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cout << sqlite3_column_int(stmt, 0) << "\t"
             << sqlite3_column_int(stmt, 1) << "\t\t"
             << (char*)sqlite3_column_text(stmt, 2) << "\n";
    }
    sqlite3_finalize(stmt);
}

void Update_menu() {
    int choice;
    cout << "║        UPDATE  MENU         ║\n";
    cout << "1) Update Student\n";
    cout << "2) Update Club\n";
    cout << "3) Update Event\n";
    cout << "4) Update Attendance\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1: Update_student();    break;
        case 2: Update_club();       break;
        case 3: Update_event();      break;
        case 4: Update_attendance(); break;
        default: cout << "  [!] Invalid choice.\n"; break;
    }
}

void Delete_menu() {
    int choice;
    cout << "║        DELETE  MENU         ║\n";
    cout << "1) Delete Student\n";
    cout << "2) Delete Club\n";
    cout << "3) Delete Event\n";
    cout << "4) Delete Membership\n";
    cout << "5) Delete Attendance Record\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1: Delete_student();    break;
        case 2: Delete_club();       break;
        case 3: Delete_event();      break;
        case 4: Delete_membership(); break;
        case 5: Delete_attendance(); break;
        default: cout << "  [!] Invalid choice.\n"; break;
    }
}

void Generate_report() {
    int choice;
    cout << "║REPORT  MENU║\n";
    cout << "1) Student Report\n";
    cout << "2) Club Report\n";
    cout << "3) Event Report\n";
    cout << "4) Membership Report\n";
    cout << "5) Attendance Report\n";
    cout << "6) Full Report\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1: report_students();   break;
        case 2: report_clubs();      break;
        case 3: report_events();     break;
        case 4: report_memberships(); break;
        case 5: report_attendance(); break;
        case 6:
            report_students();
            report_clubs();
            report_events();
            report_memberships();
            report_attendance();
            cout << "END OF REPORT\n";
            break;
        default:
            cout << "  [!] Invalid choice.\n";
            break;
    }
}


int main() {
    create_db();

    cout << "\nWelcome to AASTU Club Management System\n";

    bool running = true;
    while (running) {
        cout << "\n╔══════════════════════════════════╗\n";
        cout << "║          MAIN  MENU               ║\n";
        cout << "╚══════════════════════════════════╝\n";
        cout << "1) Register Students\n";
        cout << "2) Create Club\n";
        cout << "3) Create Event\n";
        cout << "4) Join Club\n";
        cout << "5) Record Attendance\n";
        cout << "6) Update\n";
        cout << "7) Delete\n";
        cout << "8) Generate Report\n";
        cout << "9) Save and Exit\n";
        cout << "Enter your choice: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: Register();           break;
            case 2: Create_Club();        break;
            case 3: Create_Event();       break;
            case 4: Join_Club();          break;
            case 5: Record_Attendance();  break;
            case 6: Update_menu();        break;
            case 7: Delete_menu();        break;
            case 8: Generate_report();    break;
            case 9:
                cout << "Thank you for using AASTU Club Management System. Goodbye!\n";
                sqlite3_close(db);
                return 0;
            default:
                cout << "  [!] Invalid choice. Please try again.\n";
                break;
        }

        // Ask to continue — no goto needed
        char ch;
        bool valid = false;
        while (!valid) {
            cout << "Would you like to return to the main menu? (y/n): ";
            cin >> ch;
            if (ch == 'y')      valid = true;
            else if (ch == 'n') { valid = true; running = false; }
            else cout << "  [!] Invalid input. Enter y or n.\n";
        }
    }

    sqlite3_close(db);
    return 0;
}