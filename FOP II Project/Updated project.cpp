#include <iostream>
#include <sqlite3.h>
using namespace std;
sqlite3* db;
int test;
string sql;
char* errMsg = nullptr;

struct student{
    int s_id;
    string name,dept;
}s[100];

struct club{
    int c_id,leader_id;
    string name, description;
}c[100];

struct event{
    int event_id,club_id,date,month,year;
    string title,description;
}e[100];

struct membership{
    int s_id,club_id,date,month,year;
}m[100];

void create_db(){
test=sqlite3_open("AASTU_CLUB_MGT.db",&db);
if (test == 0) 
    cout << "Database created successfully!" <<endl;

sql = "CREATE TABLE IF NOT EXISTS Student (s_id integer primary key, name text, dept text);";
test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

sql = "CREATE TABLE IF NOT EXISTS Club (c_id integer primary key,leader_id integer, name text, description text, foreign key(leader_id) references Student(s_id) ON DELETE SET NULL);";
test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

sql = "CREATE TABLE IF NOT EXISTS Event (event_id integer primary key,club_id integer, title text, description text, date integer,month integer, year integer, foreign key(club_id) references Club(c_id) ON DELETE SET NULL);";
test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

sql = "CREATE TABLE IF NOT EXISTS membership (s_id integer, club_id integer, date integer, month integer, year integer, PRIMARY KEY(s_id, club_id), foreign key(club_id) references Club(c_id));";
test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

sql = "CREATE TABLE IF NOT EXISTS Attendance (club_id INTEGER, s_id INTEGER, condition TEXT, PRIMARY KEY (club_id, s_id), FOREIGN KEY (club_id) REFERENCES Club(c_id) ON DELETE SET NULL, FOREIGN KEY (s_id) REFERENCES Student(s_id) ON DELETE SET NULL);";
test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
}

void Register(student a[], int size){
    for(int i=0;i<size;i++){
    cout<<"Enter Student "<<i+1<<" ID, Name and Department ";
    cin>>a[i].s_id>>a[i].name>>a[i].dept;
    }
    for(int i=0;i<size;i++){
        sql = "INSERT INTO Student VALUES (" + to_string(a[i].s_id) + ", '" + a[i].name + "', '" + a[i].dept + "');";
        test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
}
    }


void Create_Club(club c[], int size){
    for(int i=0;i<size;i++){
        cout<<"enter club "<<i+1<<" ID, Leadr ID, Name and Description";
        cin>>c[i].c_id>>c[i].leader_id>>c[i].name>>c[i].description;
    }
    for(int i=0;i<size;i++){
        sql = "INSERT INTO Club VALUES (" + to_string(c[i].c_id) + ", " + to_string(c[i].leader_id) + ", '" + c[i].name + "', '" + c[i].description + "');";
        test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
}
    }


void Create_event(event e[], int size){
    for(int i=0;i<size;i++){
        cout<<"Enter Event"<<i+1<<" ID, Club_ID, Date, Month, Year, Title and Description respectively: ";
        cin>>e[i].event_id>>e[i].club_id>>e[i].date>>e[i].month>>e[i].year>>e[i].title>>e[i].description;
    }
    
    for(int i=0;i<size;i++){
        sql = "INSERT INTO Event VALUES (" + to_string(e[i].event_id) + ", " + to_string(e[i].club_id) + ", '" + e[i].title +"', '" + e[i].description + "', " + to_string(e[i].date) + ", " + to_string(e[i].month) + ", " + to_string(e[i].year)+");";
        test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        
    }
}

void Join_club(membership m[],int size){
    for(int i=0;i<size;i++){
        cout<<"Enter Member "<<i+1<<" ID, Club_ID, Date, Month and Year of entry respectively: ";
        cin>>m[i].s_id>>m[i].club_id>>m[i].date>>m[i].month>>m[i].year;
    }
    for(int i=0;i<size;i++){
        sql = "INSERT INTO membership VALUES (" + to_string(m[i].s_id) + ", " + to_string(m[i].club_id) + ", " + to_string(m[i].date) + ", " + to_string(m[i].month) + ", " + to_string(m[i].year) + ");";
        test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    }
}
void Record_Attendance(student s[], club c[], int size){
    string condition[100];
    for(int i=0;i<size;i++){
        cout<<"Enter Member "<<i+1<<" Student ID, Club ID and Presence status respectively: ";
        cin>>s[i].s_id>>c[i].c_id>>condition[i];
        sql = "INSERT INTO Attendance VALUES(" + to_string(c[i].c_id) + ", " + to_string(s[i].s_id) + ", '" + condition[i] + "');";
        test=sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    }
}

// ═══════════════════════════════════
//         UPDATE FUNCTIONS
// ═══════════════════════════════════

void Update_student(){
    int id;
    string newName, newDept;
    cout << "Enter Student ID to update: ";
    cin >> id;
    cout << "Enter new Name: ";
    cin >> newName;
    cout << "Enter new Department: ";
    cin >> newDept;
    sql = "UPDATE Student SET name='" + newName + "', dept='" + newDept + "' WHERE s_id=" + to_string(id) + ";";
    test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
}

void Update_club(){
    int id, newLeaderID;
    string newName, newDesc;
    cout << "Enter Club ID to update: ";
    cin >> id;
    cout << "Enter new Leader ID: ";
    cin >> newLeaderID;
    cout << "Enter new Name: ";
    cin >> newName;
    cout << "Enter new Description: ";
    cin >> newDesc;
    sql = "UPDATE Club SET leader_id=" + to_string(newLeaderID) + ", name='" + newName + "', description='" + newDesc + "' WHERE c_id=" + to_string(id) + ";";
    test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
}

void Update_event(){
    int id;
    string newTitle, newDesc;
    int newDate, newMonth, newYear;
    cout << "Enter Event ID to update: ";
    cin >> id;
    cout << "Enter new Title: ";
    cin >> newTitle;
    cout << "Enter new Description: ";
    cin >> newDesc;
    cout << "Enter new Date Month Year: ";
    cin >> newDate >> newMonth >> newYear;
    sql = "UPDATE Event SET title='" + newTitle + "', description='" + newDesc + "', date=" + to_string(newDate) + ", month=" + to_string(newMonth) + ", year=" + to_string(newYear) + " WHERE event_id=" + to_string(id) + ";";
    test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
}

void Update_attendance(){
    int sID, clubID;
    string newCondition;
    cout << "Enter Student ID: ";
    cin >> sID;
    cout << "Enter Club ID: ";
    cin >> clubID;
    cout << "Enter new Attendance Status: ";
    cin >> newCondition;
    sql = "UPDATE Attendance SET condition='" + newCondition + "' WHERE s_id=" + to_string(sID) + " AND club_id=" + to_string(clubID) + ";";
    test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

}

// ═══════════════════════════════════
//         DELETE FUNCTIONS
// ═══════════════════════════════════

void Delete_student(){
    int id;
    char confirm;
    cout << "Enter Student ID to delete: ";
    cin >> id;
    cout << "Are you sure you want to delete this student?(y/n): ";
    cin >> confirm;
    if(confirm == 'y'){
        sql = "DELETE FROM Student WHERE s_id=" + to_string(id) + ";";
        test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    }
    else
        cout << "Delete cancelled." << endl;
}

void Delete_club(){
    int id;
    char confirm;
    cout << "Enter Club ID to delete: ";
    cin >> id;
    cout << "Are you sure you want to delete this club?(y/n): ";
    cin >> confirm;
    if(confirm == 'y'){
        sql = "DELETE FROM Club WHERE c_id=" + to_string(id) + ";";
        test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    }
    else
        cout << "Delete cancelled." << endl;
}

void Delete_event(){
    int id;
    char confirm;
    cout << "Enter Event ID to delete: ";
    cin >> id;
    cout << "Are you sure you want to delete this event?(y/n): ";
    cin >> confirm;
    if(confirm == 'y'){
        sql = "DELETE FROM Event WHERE event_id=" + to_string(id) + ";";
        test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    }
    else
        cout << "Delete cancelled." << endl;
}

void Delete_membership(){
    int sID, clubID;
    char confirm;
    cout << "Enter Student ID: ";
    cin >> sID;
    cout << "Enter Club ID: ";
    cin >> clubID;
    cout << "Are you sure you want to remove this member from club?(y/n): ";
    cin >> confirm;
    if(confirm == 'y'){
        sql = "DELETE FROM membership WHERE s_id=" + to_string(sID) + " AND club_id=" + to_string(clubID) + ";";
        test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    }
    else
        cout << "Delete cancelled." << endl;
}

void Delete_attendance(){
    int sID, clubID;
    char confirm;
    cout << "Enter Student ID: ";
    cin >> sID;
    cout << "Enter Club ID: ";
    cin >> clubID;
    cout << "Are you sure you want to delete this attendance record?(y/n): ";
    cin >> confirm;
    if(confirm == 'y'){
        sql = "DELETE FROM Attendance WHERE s_id=" + to_string(sID) + " AND club_id=" + to_string(clubID) + ";";
        test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    }
    else
        cout << "Delete cancelled." << endl;
}

// ═══════════════════════════════════
//       UPDATE AND DELETE MENU
// ═══════════════════════════════════

void Update_menu(){
    int choice;
    cout << "\n╔════════════════════════════╗\n";
    cout << "║       UPDATE  MENU          ║\n";
    cout << "╚════════════════════════════╝\n";
    cout << "1) Update Student\n";
    cout << "2) Update Club\n";
    cout << "3) Update Event\n";
    cout << "4) Update Attendance\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch(choice){
        case 1: Update_student();  break;
        case 2: Update_club();     break;
        case 3: Update_event();    break;
        case 4: Update_attendance(); break;
        default: cout << "Invalid choice." << endl; break;
    }
}

void Delete_menu(){
    int choice;
    cout << "\n╔════════════════════════════╗\n";
    cout << "║       DELETE  MENU          ║\n";
    cout << "╚════════════════════════════╝\n";
    cout << "1) Delete Student\n";
    cout << "2) Delete Club\n";
    cout << "3) Delete Event\n";
    cout << "4) Delete Membership\n";
    cout << "5) Delete Attendance Record\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch(choice){
        case 1: Delete_student();    break;
        case 2: Delete_club();       break;
        case 3: Delete_event();      break;
        case 4: Delete_membership(); break;
        case 5: Delete_attendance(); break;
        default: cout << "Invalid choice." << endl; break;
    }
}


void Generate_report() {
    sqlite3_stmt* stmt;
    int choice;

    cout << "\n╔════════════════════════════╗\n";
    cout << "║       REPORT  MENU          ║\n";
    cout << "╚════════════════════════════╝\n";
    cout << "1) Student Report\n";
    cout << "2) Club Report\n";
    cout << "3) Event Report\n";
    cout << "4) Membership Report\n";
    cout << "5) Attendance Report\n";
    cout << "6) Full Report\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch(choice){

        // ═══════════════════════════════════
        //           STUDENT REPORT
        // ═══════════════════════════════════
        case 1:
        cout << "\n╔════════════════════════════╗\n";
        cout << "║       STUDENT REPORT        ║\n";
        cout << "╚════════════════════════════╝\n";
        cout << "ID\tName\t\tDept\n";
        cout << "─────────────────────────────\n";

        sql = "SELECT s_id, name, dept FROM Student;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id      = sqlite3_column_int(stmt, 0);
            string name = (char*)sqlite3_column_text(stmt, 1);
            string dept = (char*)sqlite3_column_text(stmt, 2);
            cout << id << "\t" << name << "\t\t" << dept << endl;
        }
        sqlite3_finalize(stmt);

        // students per department summary
        cout << "\nStudents Per Department\n";
        cout << "─────────────────────────────\n";
        sql = "SELECT dept, COUNT(*) FROM Student GROUP BY dept;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string dept = (char*)sqlite3_column_text(stmt, 0);
            int count   = sqlite3_column_int(stmt, 1);
            cout << dept << ": " << count << " students" << endl;
        }
        sqlite3_finalize(stmt);
        break;

        // ═══════════════════════════════════
        //            CLUB REPORT
        // ═══════════════════════════════════
        case 2:
        cout << "\n╔════════════════════════════╗\n";
        cout << "║         CLUB REPORT         ║\n";
        cout << "╚════════════════════════════╝\n";
        cout << "ID\tLeader ID\tName\t\tDescription\n";
        cout << "─────────────────────────────────────\n";

        sql = "SELECT c_id, leader_id, name, description FROM Club;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int c_id      = sqlite3_column_int(stmt, 0);
            int leader_id = sqlite3_column_int(stmt, 1);
            string name   = (char*)sqlite3_column_text(stmt, 2);
            string desc   = (char*)sqlite3_column_text(stmt, 3);
            cout << c_id << "\t" << leader_id << "\t\t" << name << "\t\t" << desc << endl;
        }
        sqlite3_finalize(stmt);

        // total clubs
        cout << "\nTotal Clubs: ";
        sql = "SELECT COUNT(*) FROM Club;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            cout << count << " clubs" << endl;
        }
        sqlite3_finalize(stmt);
        break;

        // ═══════════════════════════════════
        //           EVENT REPORT
        // ═══════════════════════════════════
        case 3:
        cout << "\n╔════════════════════════════╗\n";
        cout << "║         EVENT REPORT        ║\n";
        cout << "╚════════════════════════════╝\n";
        cout << "ID\tClub ID\tTitle\t\tDate\n";
        cout << "─────────────────────────────────────\n";

        sql = "SELECT event_id, club_id, title, date, month, year FROM Event;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int event_id = sqlite3_column_int(stmt, 0);
            int club_id  = sqlite3_column_int(stmt, 1);
            string title = (char*)sqlite3_column_text(stmt, 2);
            string date  = (char*)sqlite3_column_text(stmt, 3);
            string month = (char*)sqlite3_column_text(stmt, 4);
            string year  = (char*)sqlite3_column_text(stmt, 5);
            cout << event_id << "\t" << club_id << "\t" << title << "\t\t" << date << "/" << month << "/" << year << endl;
        }
        sqlite3_finalize(stmt);

        // events per day summary
        cout << "\nEvents Per Day\n";
        cout << "─────────────────────────────\n";
        sql = "SELECT date, month, year, COUNT(*) FROM Event GROUP BY date, month, year;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string date  = (char*)sqlite3_column_text(stmt, 0);
            string month = (char*)sqlite3_column_text(stmt, 1);
            string year  = (char*)sqlite3_column_text(stmt, 2);
            int count    = sqlite3_column_int(stmt, 3);
            cout << date << "/" << month << "/" << year << ": " << count << " events" << endl;
        }
        sqlite3_finalize(stmt);
        break;

        // ═══════════════════════════════════
        //        MEMBERSHIP REPORT
        // ═══════════════════════════════════
        case 4:
        cout << "\n╔════════════════════════════╗\n";
        cout << "║      MEMBERSHIP REPORT      ║\n";
        cout << "╚════════════════════════════╝\n";
        cout << "Student ID\tClub ID\t\tJoin Date\n";
        cout << "─────────────────────────────────────\n";

        sql = "SELECT s_id, club_id, date, month, year FROM membership;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int s_id     = sqlite3_column_int(stmt, 0);
            int club_id  = sqlite3_column_int(stmt, 1);
            string date  = (char*)sqlite3_column_text(stmt, 2);
            string month = (char*)sqlite3_column_text(stmt, 3);
            string year  = (char*)sqlite3_column_text(stmt, 4);
            cout << s_id << "\t\t" << club_id << "\t\t" << date << "/" << month << "/" << year << endl;
        }
        sqlite3_finalize(stmt);
        break;

        // ═══════════════════════════════════
        //        ATTENDANCE REPORT
        // ═══════════════════════════════════
        case 5:
        cout << "\n╔════════════════════════════╗\n";
        cout << "║      ATTENDANCE REPORT      ║\n";
        cout << "╚════════════════════════════╝\n";
        cout << "Club ID\tStudent ID\tStatus\n";
        cout << "─────────────────────────────────────\n";

        sql = "SELECT club_id, s_id, condition FROM Attendance;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int club_id       = sqlite3_column_int(stmt, 0);
            int s_id          = sqlite3_column_int(stmt, 1);
            string condition  = (char*)sqlite3_column_text(stmt, 2);
            cout << club_id << "\t" << s_id << "\t\t" << condition << endl;
        }
        sqlite3_finalize(stmt);
        break;

        // ═══════════════════════════════════
        //           FULL REPORT
        // ═══════════════════════════════════
        case 6:
        // simply call all cases one by one
        // student
        cout << "\n╔════════════════════════════╗\n";
        cout << "║       STUDENT REPORT        ║\n";
        cout << "╚════════════════════════════╝\n";
        cout << "ID\tName\t\tDept\n";
        cout << "─────────────────────────────\n";
        sql = "SELECT s_id, name, dept FROM Student;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id      = sqlite3_column_int(stmt, 0);
            string name = (char*)sqlite3_column_text(stmt, 1);
            string dept = (char*)sqlite3_column_text(stmt, 2);
            cout << id << "\t" << name << "\t\t" << dept << endl;
        }
        sqlite3_finalize(stmt);
        cout << "\nStudents Per Department\n";
        cout << "─────────────────────────────\n";
        sql = "SELECT dept, COUNT(*) FROM Student GROUP BY dept;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string dept = (char*)sqlite3_column_text(stmt, 0);
            int count   = sqlite3_column_int(stmt, 1);
            cout << dept << ": " << count << " students" << endl;
        }
        sqlite3_finalize(stmt);

        // club
        cout << "\n╔════════════════════════════╗\n";
        cout << "║         CLUB REPORT         ║\n";
        cout << "╚════════════════════════════╝\n";
        cout << "ID\tLeader ID\tName\t\tDescription\n";
        cout << "─────────────────────────────────────\n";
        sql = "SELECT c_id, leader_id, name, description FROM Club;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int c_id      = sqlite3_column_int(stmt, 0);
            int leader_id = sqlite3_column_int(stmt, 1);
            string name   = (char*)sqlite3_column_text(stmt, 2);
            string desc   = (char*)sqlite3_column_text(stmt, 3);
            cout << c_id << "\t" << leader_id << "\t\t" << name << "\t\t" << desc << endl;
        }
        sqlite3_finalize(stmt);
        cout << "\nTotal Clubs: ";
        sql = "SELECT COUNT(*) FROM Club;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int count = sqlite3_column_int(stmt, 0);
            cout << count << " clubs" << endl;
        }
        sqlite3_finalize(stmt);

        // event
        cout << "\n╔════════════════════════════╗\n";
        cout << "║         EVENT REPORT        ║\n";
        cout << "╚════════════════════════════╝\n";
        cout << "ID\tClub ID\tTitle\t\tDate\n";
        cout << "─────────────────────────────────────\n";
        sql = "SELECT event_id, club_id, title, date, month, year FROM Event;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int event_id = sqlite3_column_int(stmt, 0);
            int club_id  = sqlite3_column_int(stmt, 1);
            string title = (char*)sqlite3_column_text(stmt, 2);
            string date  = (char*)sqlite3_column_text(stmt, 3);
            string month = (char*)sqlite3_column_text(stmt, 4);
            string year  = (char*)sqlite3_column_text(stmt, 5);
            cout << event_id << "\t" << club_id << "\t" << title << "\t\t" << date << "/" << month << "/" << year << endl;
        }
        sqlite3_finalize(stmt);

        // membership
        cout << "\n╔════════════════════════════╗\n";
        cout << "║      MEMBERSHIP REPORT      ║\n";
        cout << "╚════════════════════════════╝\n";
        cout << "Student ID\tClub ID\t\tJoin Date\n";
        cout << "─────────────────────────────────────\n";
        sql = "SELECT s_id, club_id, date, month, year FROM membership;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int s_id     = sqlite3_column_int(stmt, 0);
            int club_id  = sqlite3_column_int(stmt, 1);
            string date  = (char*)sqlite3_column_text(stmt, 2);
            string month = (char*)sqlite3_column_text(stmt, 3);
            string year  = (char*)sqlite3_column_text(stmt, 4);
            cout << s_id << "\t\t" << club_id << "\t\t" << date << "/" << month << "/" << year << endl;
        }
        sqlite3_finalize(stmt);

        // attendance
        cout << "\n╔════════════════════════════╗\n";
        cout << "║      ATTENDANCE REPORT      ║\n";
        cout << "╚════════════════════════════╝\n";
        cout << "Club ID\tStudent ID\tStatus\n";
        cout << "─────────────────────────────────────\n";
        sql = "SELECT club_id, s_id, condition FROM Attendance;";
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int club_id      = sqlite3_column_int(stmt, 0);
            int s_id         = sqlite3_column_int(stmt, 1);
            string condition = (char*)sqlite3_column_text(stmt, 2);
            cout << club_id << "\t" << s_id << "\t\t" << condition << endl;
        }
        sqlite3_finalize(stmt);

        cout << "\n─────────────────────────────────────\n";
        cout << "           END OF REPORT\n";
        cout << "─────────────────────────────────────\n";
        break;

        default:
        cout << "Invalid choice please try again." << endl;
        break;
    }
}

int main(){

    int choice;
    char ch;
    bool trial=true;

    create_db();
    cout<<"Welcome to AASTU Club Managemnet System Service Menu\nPlease choose which one of the following services you want to get\n";

    while(trial){
    cout<<"1)Register students and clubs\n2)Create Event\n3)Join Club\n4)Record Attendance\n5)Generate Report\n)6Update\n7)Delete\n8)Save and Exit"<<endl;
    cin>>choice;
    switch(choice){
        case 1:
        int n;
        cout<<"how many students do you want to register: ";
        cin>>n;
        Register(s,n);
        cout<<"would you like to register clubs also(y/n)? ";
        cin>>ch;
        if(ch=='y'){
        cout<<"how many clubs do you want to register: ";
        cin>>n;
        Create_Club(c,n);}
        else if(ch=='n'){
        break;}
        else
        cout<<"invalid input.";
        break;
        
        case 2:
        cout<<"how many events do you want to create: ";
        cin>>n;
        Create_event(e,n);
        break;

        case 3:
        cout<<"how many memebers do you want to register: ";
        cin>>n;
        Join_club(m,n);
        break;

        case 4:
        cout<<"how many members you want to record attendance of: ";
        cin>>n;
        Record_Attendance(s,c,n);
        break;
        
        case 5:
        cout<<"====Report====\n";
        Generate_report();
        break;
       
       // add these two cases
        case 6:
        cout << "====Update====\n";
        Update_menu();
        break;

        case 7:
        cout << "====Delete====\n";
        Delete_menu();
        break;
        
        case 8:
        cout<<"Thankyou for using our system, wishing you great time\n Good Bye;)";
        sqlite3_close(db);  // properly close database
        return 0;
        break;
    }
    again:
    cout<<"would you like to go again(y/n): ";
    cin>>ch;
    if(ch=='n')
    trial=false;
    else if(ch!='y' and ch!='n'){
    cout<<"error invalid input try again.\n";
    goto again;
    }
    }
    return 0;
}
