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

sql = "CREATE TABLE IF NOT EXISTS membership (s_id integer primary key,club_id integer, date integer,month integer, year integer,  foreign key(club_id) references Club(c_id) ON DELETE SET NULL);";
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
        sql="INSERT INTO Attendance VALUES(" + to_string(s[i].s_id) + ", " + to_string(c[i].c_id) + ", '" + condition[i] + "');";
        test=sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    }
}

void Generate_report() {
    sqlite3_stmt* stmt;
    cout<<"Students'department info\n";

    sql = "SELECT dept, COUNT(*) FROM Student GROUP BY dept;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {  
        string dept = (char*)sqlite3_column_text(stmt, 0);
        int count = sqlite3_column_int(stmt, 1);
        cout << dept << ": " << count << " students "<< endl;
    }
    sqlite3_finalize(stmt);

    cout<<"Amount of Events on a single day\n";

    sql = "SELECT date,month,year, COUNT(*) FROM Event GROUP BY date,month,year;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {  
        string date = (char*)sqlite3_column_text(stmt, 0);
        string month = (char*)sqlite3_column_text(stmt, 1);
        string year = (char*)sqlite3_column_text(stmt, 2);
        int count = sqlite3_column_int(stmt, 3);
        cout << date << "/" << month <<"/"<<year<<": "<< count << " events " << endl;
    }
    sqlite3_finalize(stmt);

    cout<<"Number of club in the university: ";

    sql = "SELECT COUNT(*) FROM Club;";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW) {  
        
        int count = sqlite3_column_int(stmt, 0);
        cout << count << " clubs" << endl;
    }
    sqlite3_finalize(stmt);


}

void Delete_Record() {
    int choice, id;
    cout << "\n--- Delete Menu ---\n";
    cout << "1) Delete Student\n";
    cout << "2) Delete Club\n";
    cout << "3) Delete Event\n";
    cout << "4) Delete Membership\n";
    cout << "5) Delete Attendance\n";
    cout << "Enter your choice: ";
    cin >> choice;
    
    switch(choice) {
        case 1:
            cout << "Enter Student ID: ";
            cin >> id;
            sql = "DELETE FROM Student WHERE s_id = " + to_string(id) + ";";
            break;
        case 2:
            cout << "Enter Club ID: ";
            cin >> id;
            sql = "DELETE FROM Club WHERE c_id = " + to_string(id) + ";";
            break;
        case 3:
            cout << "Enter Event ID: ";
            cin >> id;
            sql = "DELETE FROM Event WHERE event_id = " + to_string(id) + ";";
            break;
        case 4:
            cout << "Enter Student ID: ";
            cin >> id;
            sql = "DELETE FROM membership WHERE s_id = " + to_string(id) + ";";
            break;
        case 5:
            cout << "Enter Student ID: ";
            cin >> id;
            sql = "DELETE FROM Attendance WHERE s_id = " + to_string(id) + ";";
            break;
        default:
            cout << "Invalid choice!" << endl;
            return;
    }
    
    test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (test == SQLITE_OK)
        cout << "Record deleted successfully!" << endl;
    else
        cout << "Error: " << errMsg << endl;
}

void Update_Record() {
    int choice, id, new_int;
    string new_string;
    
    cout << "\n--- Update Menu ---\n";
    cout << "1) Update Student\n";
    cout << "2) Update Club\n";
    cout << "3) Update Event\n";
    cout << "4) Update Attendance Status\n";
    cout << "Enter your choice: ";
    cin >> choice;
    
    switch(choice) {
        case 1: { // Update Student
            cout << "Enter Student ID: ";
            cin >> id;
            cout << "Enter new name: ";
            cin >> new_string;
            cout << "Enter new department: ";
            string dept;
            cin >> dept;
            sql = "UPDATE Student SET name = '" + new_string + "', dept = '" + dept + 
                  "' WHERE s_id = " + to_string(id) + ";";
            break;
        }
        case 2: { // Update Club
            cout << "Enter Club ID: ";
            cin >> id;
            cout << "Enter new name: ";
            cin >> new_string;
            cout << "Enter new description: ";
            string desc;
            cin >> desc;
            cout << "Enter new leader ID: ";
            cin >> new_int;
            sql = "UPDATE Club SET name = '" + new_string + "', description = '" + desc + 
                  "', leader_id = " + to_string(new_int) + " WHERE c_id = " + to_string(id) + ";";
            break;
        }
        case 3: { // Update Event
            cout << "Enter Event ID: ";
            cin >> id;
            cout << "Enter new title: ";
            cin >> new_string;
            cout << "Enter new description: ";
            string desc;
            cin >> desc;
            cout << "Enter new date (day month year): ";
            int d, m, y;
            cin >> d >> m >> y;
            sql = "UPDATE Event SET title = '" + new_string + "', description = '" + desc + 
                  "', date = " + to_string(d) + ", month = " + to_string(m) + 
                  ", year = " + to_string(y) + " WHERE event_id = " + to_string(id) + ";";
            break;
        }
        case 4: { // Update Attendance
            int student_id, club_id;
            string status;
            cout << "Enter Student ID: ";
            cin >> student_id;
            cout << "Enter Club ID: ";
            cin >> club_id;
            cout << "Enter new status (Present/Absent/Late): ";
            cin >> status;
            sql = "UPDATE Attendance SET condition = '" + status + 
                  "' WHERE s_id = " + to_string(student_id) + 
                  " AND club_id = " + to_string(club_id) + ";";
            break;
        }
        default:
            cout << "Invalid choice!" << endl;
            return;
    }
    
    test = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (test == SQLITE_OK)
        cout << "Record updated successfully!" << endl;
    else
        cout << "Error: " << errMsg << endl;
}


int main(){

    int choice;
    char ch;
    bool trial=true;

    create_db();
    cout<<"Welcome to AASTU Club Managemnet System Service Menu\nPlease choose which one of the following services you want to get\n";
    
    while(trial){
    cout<<"1)Register students and clubs\n2)Create Event\n3)Join Club\n4)Record Attendance\n5)Generate Report\n6)Update a record\n7)delete a record\n8)Save and Exit"<<endl;
    cin>>choice;
    switch(choice){
        case 1:
        int n;
        cout<<"how many students do you want to register: ";
        cin>>n;
        Register(s,n);
        cout<<"would you like to reister clubs also(y/n)? ";
        cin>>ch;
        if(ch=='y'){
        cout<<"how many clubs do you want to register: ";
        cin>>n;
        Create_Club(c,n);}
        else if(ch=='n'){
        break;}
        else
        cout<<"invalid input";
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

        case 6:
        Update_Record();
        break;

        case 7:
        Delete_Record();
        break;

        case 8:
        cout<<"Thankyou for using our system, wishing you great time\n Good Bye;)";
        return 0;
        break;

        default:
        cout<<"invalid input please try again ";
        continue;
        break;
    }
    again:
    cout<<"would like to go again(y/n): ";
    cin>>ch;
    if(ch=='n'){
    trial=false;
    cout<<"Thankyou for using our system, wishing you great time\n Good Bye;)";
    }
    else if(ch!='y' and ch!='n'){
    cout<<"error in  valid input try again.\n";
    goto again;
    }

    }
    return 0;
}