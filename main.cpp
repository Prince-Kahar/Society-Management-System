#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

ostream &line(ostream &out)
{
    out << "========================================";
    return out;
}

ostream &ModuleLines(ostream &out)
{
    out << "----------------------------------------";
    return out;
}

string months[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

ifstream fout;
ofstream fin, fin2;

string File1 = "Resident.txt";
string File3 = "TempResident.txt";
string File4 = "Maintence.txt";
string File5 = "TempMaintence.txt";
string File6 = "Complaint.txt";
string File7 = "TempComplaint.txt";
string File8 = "DeletedResident.txt";

void Enter_To_Continue();
bool checkMobileNUmberValidation(long long int);
bool checkPasswordValidation(string);
void loginPage();
void LoadData();

void ResidentLogin();
bool CheckLogin(long long int, string, int &);
void Resident_Registration_Page();

void AdminLogin();
void ViewMaintenanceMenu();
void search(int);

class Maintenace
{
protected:
    int month;
    int year;
    float amount;
    string status;

public:
    Maintenace()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        month = ltm->tm_mon + 1;
        year = ltm->tm_year + 1900;

        amount = 1500;

        status = "Pending";
    }
};

class Compaint
{
protected:
    string complaintTitle, complaintDescription;
    int complaintID;
};

class Resident : public Maintenace, public Compaint
{
protected:
    static int ResidentID;
    static int CountResident;

    int id, FlatNo;
    long long int MobileNo;

    string Name, Password, rePassword;
    char WingCharacter;

public:
    void ResidentDashobard(int);
    void CreateAccount();

    friend void LoadData();
    friend bool CheckLogin(long long int, string, int &);

    void ViewProfile(int);
    void UpdateProfile(int);
    void UpdateMobileNumber(long long int, long long int);
    void UpdatePassword(string, string);

    void viewMaintenece(int);

    void SubmitComplaint(int);
    void ViewComplaintStatus(int);

    string GetName()
    {
        return Name;
    }
} r[10000], r1;

int Resident::ResidentID = 100;

void Resident::ResidentDashobard(int Logged_In_Number)
{
    int choose;

Dashboard:
    clearScreen();

    cout << "+========================================+" << endl;
    cout << "|           RESIDENT DASHBOARD           |" << endl;
    cout << "+========================================+" << endl;
    cout << "|  1. View Profile                       |" << endl;
    cout << "|  2. Update Profile                     |" << endl;
    cout << "|  3. View Maintenance Details           |" << endl;
    cout << "|  4. Submit Complaint                   |" << endl;
    cout << "|  5. View Complaint Status              |" << endl;
    cout << "|  0. Logout                             |" << endl;
    cout << "+----------------------------------------+" << endl;
    cout << "|  Enter Your Choice:                    |" << endl;
    cout << "+========================================+" << endl;
    cin >> choose;

    switch (choose)
    {
    case 1:
        clearScreen();
        ViewProfile(Logged_In_Number);
        break;

    case 2:
        clearScreen();
        UpdateProfile(Logged_In_Number);
        break;

    case 3:
        clearScreen();
        viewMaintenece(Logged_In_Number);
        break;

    case 4:
        clearScreen();
        cout << "Submit Complaint Selected..." << endl;
        SubmitComplaint(Logged_In_Number);
        break;

    case 5:
        clearScreen();
        cout << "View Complaint Status Selected..." << endl;
        ViewComplaintStatus(Logged_In_Number);
        break;

    case 0:
        clearScreen();
        cout << "Logging Out..." << endl;
        break;

    default:
        cout << "Invalid Choice! Please Try Again." << endl;
    }

    if (choose != 0)
    {
        cin.ignore();
        Enter_To_Continue();
        goto Dashboard;
    }
}

void Resident::CreateAccount()
{
    int Logged_In_Number = -1;

    cout << "Enter Your Name: ";
    cin.ignore();
    getline(cin, Name);

    cout << "Enter Your Wing Charater: ";
    cin >> WingCharacter;

Flat:
    cout << "Enter Your Flat Number: ";
    if (!(cin >> FlatNo))
    {
        cout << "Invalid input! Please enter digits only.\n";

        cin.clear();
        cin.ignore(10000, '\n');
        goto Flat;
    }

mobile:
    cout << "Enter Your Mobile Number: ";
    if (!(cin >> MobileNo))
    {
        cout << "Invalid input! Please enter digits only.\n";

        cin.clear();
        cin.ignore(10000, '\n');
        goto mobile;
    }

    if (!checkMobileNUmberValidation(MobileNo))
    {
        cout << "Invalid Mobile Number! Please Enter 10 Digit Number." << endl;
        goto mobile;
    }

    cin.ignore();

pass:
    cout << "Enter Your Password: ";
    getline(cin, Password);

    if (!checkPasswordValidation(Password))
    {
        cout << " Password must contain at least 8 characters, including an uppercase letter, a lowercase letter, a number, and a special character (!, @, #, $) ... " << endl
             << "Reenter Your Password " << endl
             << endl;

        goto pass;
    }

    cout << "Renter Your Password: ";
    getline(cin, rePassword);

    if (Password == rePassword)
    {
        fin.open(File1, ios::app);
        fin2.open(File4, ios::app);

        if (!fin || !fin2)
        {
            cout << "File is Not Found \n";
            return;
        }
        else
        {
            fin << "ResidentID: " << ResidentID << endl
                << "Name: " << Name << endl
                << "Wing Character: " << WingCharacter << endl
                << "Flat Number: " << FlatNo << endl
                << "Mobile Number: " << MobileNo << endl
                << "Password: " << Password << endl
                << endl;

            fin2 << "Resident ID: " << ResidentID << endl
                 << "Month: " << months[month] << endl
                 << "Year: " << year << endl
                 << "Amount: " << amount << endl
                 << "Status: " << status << endl
                 << endl;
        }

        id = ResidentID;
        ResidentID++;

        fin.close();
        fin2.close();

        cout << endl;
        cout << "+========================================+" << endl;
        cout << "|        ACCOUNT ADDED SUCCESSFULLY      |" << endl;
        cout << "+========================================+" << endl;

        Enter_To_Continue();

        LoadData();

        if (CheckLogin(MobileNo, Password, Logged_In_Number))
        {
            clearScreen();

            cout << "+========================================+" << endl;
            cout << "|          LOGIN SUCCESSFUL              |" << endl;
            cout << "+========================================+" << endl;
            cout << "|                                        |" << endl;
            cout << "|  Welcome, "
                 << r[Logged_In_Number].GetName() << "!" << endl;
            cout << "|                                        |" << endl;
            cout << "+========================================+" << endl;

            Enter_To_Continue();

            r1.ResidentDashobard(Logged_In_Number);
        }

        clearScreen();
    }
    else
    {
        cout << "Password Do not match ... " << endl
             << endl;
        goto pass;
    }
}

void Resident::ViewProfile(int Logged_In_Number)
{
    cout << "+========================================+" << endl;
    cout << "|               VIEW PROFILE             |" << endl;
    cout << "+========================================+" << endl;

    cout << "| Resident ID    : " << r[Logged_In_Number].id << endl
         << "| Name           : " << r[Logged_In_Number].Name << endl
         << "| Wing Character : " << r[Logged_In_Number].WingCharacter << endl
         << "| Flat Number    : " << r[Logged_In_Number].FlatNo << endl
         << "| Mobile No      : " << r[Logged_In_Number].MobileNo << endl;

    cout << "+========================================+" << endl;

    Enter_To_Continue();
}

void Resident::UpdateProfile(int Logged_In_Number)
{
    int choice;
    long long int New_Mobile;

    cout << "+========================================+" << endl;
    cout << "|              UPDATE PROFILE            |" << endl;
    cout << "+========================================+" << endl;
    cout << "|  1. Mobile Number                      |" << endl;
    cout << "|  2. Password                           |" << endl;
    cout << "|  3. Nothing Update                     |" << endl;
    cout << "+----------------------------------------+" << endl;
    cout << "|  Enter Your Choice:                    |" << endl;
    cout << "+========================================+" << endl;
    cin >> choice;

    if (choice == 1)
    {
    mobile:
        cout << " Enter Your New Mobile Number: ";
        if (!(cin >> New_Mobile))
        {
            cout << "Invalid input! Please enter digits only.\n";

            cin.clear();
            cin.ignore(10000, '\n');
            goto mobile;
        }

        if (!checkMobileNUmberValidation(New_Mobile))
        {
            cout << "Invalid Mobile Number! Please Enter 10 Digit Number." << endl;
            goto mobile;
        }
        else
        {
            UpdateMobileNumber(r[Logged_In_Number].MobileNo, New_Mobile);
            r[Logged_In_Number].MobileNo = New_Mobile;
        }
    }

    else if (choice == 2)
    {
        string new_password, renew_password;
    password:
        cout << "Enter Your New Password: ";
        cin >> new_password;

        if (!checkPasswordValidation(new_password))
        {
            cout << " Password must contain at least 8 characters, including an uppercase letter, a lowercase letter, a number, and a special character (!, @, #, $) ... " << endl
                 << "Reenter Your Password " << endl
                 << endl;

            goto password;
        }

        cout << "Renter Your New Password: ";
        cin >> renew_password;

        if (new_password == renew_password)
        {
            UpdatePassword(r[Logged_In_Number].Password, new_password);
            r[Logged_In_Number].Password = new_password;
        }
    }
    else if (choice == 3)
    {
        cout << "Nothing to be Updated" << endl;
    }
    else
    {
        cout << "You have chhose Wrong Operations" << endl;
    }
}

void Resident::UpdateMobileNumber(long long int Old_Mobile, long long int New_Mobile)
{
    string lines;
    fout.open(File1);
    fin.open(File3);

    if (!fout || !fin)
    {
        cout << "File Open Error!" << endl;
        return;
    }

    while (getline(fout, lines))
    {
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);

        long long int Temp_Mobile = stoll(lines.substr(15));

        if (Temp_Mobile == Old_Mobile)
            fin << "Mobile Number: " << New_Mobile << endl;
        else
            fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;
    }

    fout.close();
    fin.close();

    std::remove("Resident.txt");
    std::rename("TempResident.txt", "Resident.txt");

    cout << "Mobile Number Updated Successfully!" << endl;
}

void Resident::UpdatePassword(string Old_Password, string New_Password)
{
    string lines;
    fout.open(File1);
    fin.open(File3);

    if (!fout || !fin)
    {
        cout << "File Open Error!" << endl;
        return;
    }

    while (getline(fout, lines))
    {
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        string temp_Password = lines.substr(10);

        if (temp_Password == Old_Password)
            fin << "Password: " << New_Password << endl;
        else
            fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;
    }

    fout.close();
    fin.close();

    std::remove("Resident.txt");
    std::rename("TempResident.txt", "Resident.txt");

    cout << "Password Updated Successfully!" << endl;
}

void Resident::viewMaintenece(int Logged_In_Number)
{
    cout << endl;
    cout << line << endl;
    cout << "          VIEW MAINTENANECE" << endl;
    cout << line << endl;

    cout << "Resident ID  : " << r[Logged_In_Number].id << endl
         << "Month        : " << r[Logged_In_Number].month << endl
         << "Year         : " << r[Logged_In_Number].year << endl
         << "Amount       : " << r[Logged_In_Number].amount << endl
         << "Status       : " << r[Logged_In_Number].status << endl
         << ModuleLines << endl;

    Enter_To_Continue();
}

void Resident::SubmitComplaint(int Logged_In_Number)
{
    int ComplaintID = 1;
    string line;

    fout.open(File6);

    if (fout)
    {
        while (getline(fout, line))
        {
            if (line.find("Complaint ID: ") == 0)
            {
                int id = stoi(line.substr(14));

                if (id >= ComplaintID)
                    ComplaintID = id + 1;
            }
        }

        fout.close();
    }
    else
    {
        fout.clear();
    }

    cout << "+========================================+" << endl;
    cout << "|             SUBMIT COMPLAINT           |" << endl;
    cout << "+========================================+" << endl;

    cout << "\nEnter Complaint Title: ";
    cin.ignore();
    getline(cin, complaintTitle);

    cout << "Enter Complaint Description: ";
    getline(cin, complaintDescription);

    time_t now = time(0);
    tm *ltm = localtime(&now);

    fin.open(File6, ios::app);

    if (!fin)
    {
        cout << "Unable to open Complaint.txt!" << endl;
        return;
    }

    fin << "Complaint ID: " << ComplaintID << endl;
    fin << "Resident ID: " << r[Logged_In_Number].id << endl;
    fin << "Title: " << complaintTitle << endl;
    fin << "Description: " << complaintDescription << endl;
    fin << "Date: " << ltm->tm_mday << "-" << ltm->tm_mon + 1 << "-" << ltm->tm_year + 1900 << endl;
    fin << "Status: Pending" << endl
        << endl;

    fin.close();

    cout << "\nComplaint submitted successfully!" << endl;
    cout << "Your Complaint ID is: " << ComplaintID << endl;
}

void Resident::ViewComplaintStatus(int Logged_In_Number)
{
    fout.open(File6);

    if (!fout)
    {
        cout << "\nNo complaint record found!" << endl;
        return;
    }

    string line;
    string ComplaintID, ResidentID;
    string Title, Description, Date, Status;

    bool found = false;

    while (getline(fout, line))
    {
        if (line.find("Complaint ID: ") == 0)
            ComplaintID = line.substr(14);

        else if (line.find("Resident ID: ") == 0)
            ResidentID = line.substr(13);

        else if (line.find("Title: ") == 0)
            Title = line.substr(7);

        else if (line.find("Description: ") == 0)
            Description = line.substr(13);

        else if (line.find("Date: ") == 0)
            Date = line.substr(6);

        else if (line.find("Status: ") == 0)
            Status = line.substr(8);

        else if (line.empty() && !ComplaintID.empty())
        {
            if (ResidentID == to_string(r[Logged_In_Number].id))
            {
                found = true;

                cout << "+================================+" << endl;
                cout << "| Complaint ID: " << ComplaintID << endl;
                cout << "| Title: " << Title << endl;
                cout << "| Description: " << Description << endl;
                cout << "| Date: " << Date << endl;
                cout << "| Status: " << Status << endl;
                cout << "+================================+" << endl;
            }

            ComplaintID = "";
            ResidentID = "";
            Title = "";
            Description = "";
            Date = "";
            Status = "";
        }
    }

    if (!ComplaintID.empty() &&
        ResidentID == to_string(r[Logged_In_Number].id))
    {
        found = true;

        cout << "+================================+" << endl;
        cout << "| Complaint ID: " << ComplaintID << endl;
        cout << "| Title: " << Title << endl;
        cout << "| Description: " << Description << endl;
        cout << "| Date: " << Date << endl;
        cout << "| Status: " << Status << endl;
        cout << "+================================+" << endl;
    }

    if (!found)
    {
        cout << "\nYou have not submitted any complaints yet." << endl;
    }

    fout.close();
}

bool checkMobileNUmberValidation(long long int MobileNumber)
{
    if (MobileNumber < 1000000000LL || MobileNumber > 9999999999LL)
        return false;
    else
        return true;
}

bool checkPasswordValidation(string Password)
{
    bool UpperCase = false;
    bool LowerCase = false;
    bool Numeric = false;
    bool Special = false;

    if (Password.length() < 8)
        return false;

    for (int i = 0; i < Password.length(); i++)
    {
        if (Password[i] >= 'A' && Password[i] <= 'Z')
            UpperCase = true;
        else if (Password[i] >= 'a' && Password[i] <= 'z')
            LowerCase = true;
        else if (Password[i] >= '0' && Password[i] <= '9')
            Numeric = true;
        else if (Password[i] == '!' || Password[i] == '@' || Password[i] == '#' || Password[i] == '$')
            Special = true;
        else
            return false;
    }

    return UpperCase && LowerCase && Numeric && Special;
}

class Admin : public Resident
{
public:
    void AdminDashboard();
    void ViewAllResidents();

    void SearchResident();
    friend void search(int);

    void AddResident();

    void UpdateResident();
    void UpdateResidentMobileNumber(long long int, long long int);
    void UpdateResidentPassword(string, string);
    void updateResidentName(string, string);

    void DeleteResident();

    void maintenaceMangment();
    void AddMaintenanceRecord();

    void ViewMaintenance();
    void ViewPendingMaintenance();
    void ViewPaidMaintenance();

    void UpdateMaintenanceStatus();

    void ComplaintManagement();
    void ViewAllComplaints();
    void ViewPendingComplaints();
    void UpdateComplaintStatus();
} a[10000], a1;

void Admin::AdminDashboard()
{
    int choice;

    do
    {
        clearScreen();

        cout << "+========================================+" << endl;
        cout << "|             ADMIN DASHBOARD            |" << endl;
        cout << "+========================================+" << endl;
        cout << "|  1. View All Residents                 |" << endl;
        cout << "|  2. Search Resident                    |" << endl;
        cout << "|  3. Add Resident                       |" << endl;
        cout << "|  4. Update Resident                    |" << endl;
        cout << "|  5. Delete Resident                    |" << endl;
        cout << "|  6. Maintenance Management             |" << endl;
        cout << "|  7. Complaint Management               |" << endl;
        cout << "|  8. View All Complaints                |" << endl;
        cout << "|  0. Logout                             |" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "|  Enter Your Choice:                    |" << endl;
        cout << "+========================================+" << endl;
        cin >> choice;

        switch (choice)
        {
        case 1:
            clearScreen();
            cout << line << " View All Residents " << line << endl;
            ViewAllResidents();
            break;

        case 2:
            cout << line << " Search Resident " << line << endl;
            SearchResident();
            break;

        case 3:
            clearScreen();
            AddResident();
            break;

        case 4:
            clearScreen();
            UpdateResident();
            break;

        case 5:
            cout << line << " Delete Residents " << line << endl;
            DeleteResident();
            break;

        case 6:
            cout << "\nView Maintenance";
            maintenaceMangment();
            break;

        case 7:
            cout << "\nView Complaints";
            ComplaintManagement();
            break;

        case 8:
            cout << "\nComplaint Status";
            ViewAllComplaints();
            break;

        case 0:
            cout << "\nAdmin Logged Out Successfully!" << endl;
            break;

        default:
            cout << "\nInvalid Choice!" << endl;
        }

        cin.get();
        Enter_To_Continue();

    } while (choice != 0);
}

void Admin::ViewAllResidents()
{
    string lines;
    int i = 0;
    fout.open(File1);
    int count_resident = 0;

    if (!fout)
    {
        fin.open(File1);
        fin.close();
    }

    while (getline(fout, lines))
    {
        if (i >= 10000)
        {
            cout << "Maximum Resident Limit Reached!" << endl;
            break;
        }

        cout << lines << endl;

        getline(fout, lines);
        cout << lines << endl;

        getline(fout, lines);
        cout << lines << endl;

        getline(fout, lines);
        cout << lines << endl;

        getline(fout, lines);
        cout << lines << endl;

        getline(fout, lines);
        cout << lines << endl;

        getline(fout, lines);
        cout << endl;

        count_resident++;
        i++;
    }
    fout.close();
}

void Admin::SearchResident()
{
    int choice;

    do
    {
        clearScreen();

        cout << "+========================================+" << endl;
        cout << "|             SEARCH RESIDENT            |" << endl;
        cout << "+========================================+" << endl;
        cout << "|  1. Search By Resident ID              |" << endl;
        cout << "|  2. Search By Mobile Number            |" << endl;
        cout << "|  3. Search By Flat Number              |" << endl;
        cout << "|  0. Back                               |" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "|  Enter Your Choice:                    |" << endl;
        cout << "+========================================+" << endl;
        cin >> choice;

        switch (choice)
        {
        case 1:
            clearScreen();
            cout << "\nSearch By Resident ID Selected..." << endl;
            search(choice);
            break;

        case 2:
            clearScreen();
            cout << "\nSearch By Mobile Number Selected..." << endl;
            search(choice);
            break;

        case 3:
            clearScreen();
            cout << "\nSearch By Flat Number Selected..." << endl;
            search(choice);
            break;

        case 0:
            break;

        default:
            cout << "\nInvalid Choice! Please Try Again." << endl;
        }

        if (choice != 0)
        {
            cin.ignore();
            Enter_To_Continue();
        }

    } while (choice != 0);
}

void Admin::AddResident()
{
    cout << "+========================================+" << endl;
    cout << "|               ADD RESIDENT             |" << endl;
    cout << "+========================================+" << endl;

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, Name);

    cout << "Enter Wing Character: ";
    cin >> WingCharacter;

Flat:
    cout << "Enter Flat Number: ";
    if (!(cin >> FlatNo))
    {
        cout << "Invalid input! Please enter digits only.\n";

        cin.clear();
        cin.ignore(10000, '\n');
        goto Flat;
    }

mobile:
    cout << "Enter Mobile Number: ";
    if (!(cin >> MobileNo))
    {
        cout << "Invalid input! Please enter digits only.\n";

        cin.clear();
        cin.ignore(10000, '\n');
        goto mobile;
    }

    if (!checkMobileNUmberValidation(MobileNo))
    {
        cout << "Invalid Mobile Number! Please Enter 10 Digit Number." << endl;
        goto mobile;
    }

    cin.ignore();

pass:
    cout << "Enter Password: ";
    getline(cin, Password);

    if (!checkPasswordValidation(Password))
    {
        cout << " Password must contain at least 8 characters, including an uppercase letter, a lowercase letter, a number, and a special character (!, @, #, $) ... " << endl
             << "Reenter Your Password " << endl
             << endl;

        goto pass;
    }

    cout << "Renter Password: ";
    getline(cin, rePassword);

    if (Password == rePassword)
    {
        ofstream fin;

        fin.open(File1, ios::app);

        if (!fin)
        {
            cout << "File is Not Found \n";
            return;
        }
        else
        {
            fin << "ResidentID: " << ResidentID << endl
                << "Name: " << Name << endl
                << "Wing Character: " << WingCharacter << endl
                << "Flat Number: " << FlatNo << endl
                << "Mobile Number: " << MobileNo << endl
                << "Password: " << Password << endl
                << endl;
        }

        id = ResidentID;
        ResidentID++;

        fin.close();

        cout << line << " Account Added Successfully " << line << endl;

        cin.ignore();
        Enter_To_Continue();
    }
    else
    {
        cout << "Password Do not match ... " << endl
             << endl;
        goto pass;
    }
}

void Admin::UpdateResident()
{
    int choice, id, i;
    bool found;
    string lines;

    cout << "Enter Resident Id: ";
    cin >> id;

update_menu:
    clearScreen();

    cout << "+========================================+" << endl;
    cout << "|              UPDATE RESIDENT           |" << endl;
    cout << "+========================================+" << endl;
    cout << "|  1. Mobile Number                      |" << endl;
    cout << "|  2. Password                           |" << endl;
    cout << "|  3. Name                               |" << endl;
    cout << "|  4. Nothing Update                     |" << endl;
    cout << "+----------------------------------------+" << endl;
    cout << "|  Enter Your Choice:                    |" << endl;
    cout << "+========================================+" << endl;
    cin >> choice;
    
    i = 0;
    found = false;
    
    fout.open(File1);

    while (getline(fout, lines))
    {
        if (i >= 10000)
        {
            cout << "Maximum Resident Limit Reached!" << endl;
            break;
        }

        a[i].id = stoi(lines.substr(12));

        getline(fout, lines);
        a[i].Name = lines.substr(6);

        getline(fout, lines);
        a[i].WingCharacter = lines[16];

        getline(fout, lines);
        a[i].FlatNo = stoi(lines.substr(12));

        getline(fout, lines);
        a[i].MobileNo = stoll(lines.substr(15));

        getline(fout, lines);
        a[i].Password = lines.substr(10);

        getline(fout, lines);
        if (a[i].id == id)
        {
            found = true;
            break;
        }
        i++;
    }

    fout.close();
    if (!found)
    {
        cout << "Resident Not Found " << endl;
        Enter_To_Continue();
        return;
    }

    long long int New_Mobile;
    string new_password, renew_password, new_name;
    switch (choice)
    {
    case 1:
    mobile:
        cout << " Enter Your New Mobile Number: ";
        if (!(cin >> New_Mobile))
        {
            cout << "Invalid input! Please enter digits only.\n";

            cin.clear();
            cin.ignore(10000, '\n');
            goto mobile;
        }

        if (!checkMobileNUmberValidation(New_Mobile))
        {
            cout << "Invalid Mobile Number! Please Enter 10 Digit Number." << endl;
            goto mobile;
        }
        else
        {
            UpdateResidentMobileNumber(a[i].MobileNo, New_Mobile);
            a[i].MobileNo = New_Mobile;
        }
        break;

    case 2:
    password:
        cout << "Enter New Password: ";
        cin >> new_password;

        if (!checkPasswordValidation(new_password))
        {
            cout << " Password must contain at least 8 characters, including an uppercase letter, a lowercase letter, a number, and a special character (!, @, #, $) ... " << endl
                 << "Reenter Your Password " << endl
                 << endl;

            goto password;
        }

        cout << "Renter Your New Password: ";
        cin >> renew_password;

        if (new_password == renew_password)
        {
            UpdateResidentPassword(a[i].Password, new_password);
            a[i].Password = new_password;
        }
        break;

    case 3:
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, new_name);

        updateResidentName(a[i].Name, new_name);
        a[i].Name = new_name;
        break;

    case 4:
        cout << "Nothing to be Updated" << endl;
        break;

    default:
        cout << "You have chhose Wrong Operations" << endl;
    }

    if (choice != 4)
    {
        // Enter_To_Continue();
        goto update_menu;
    }
}

void Admin::UpdateResidentMobileNumber(long long int Old_Mobile, long long int New_Mobile)
{
    string lines;
    fout.open(File1);
    fin.open(File3);

    if (!fout || !fin)
    {
        cout << "File Open Error!" << endl;
        return;
    }

    while (getline(fout, lines))
    {
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);

        long long int Temp_Mobile = stoll(lines.substr(15));

        if (Temp_Mobile == Old_Mobile)
            fin << "Mobile Number: " << New_Mobile << endl;
        else
            fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;
    }

    fout.close();
    fin.close();

    std::remove("Resident.txt");
    std::rename("TempResident.txt", "Resident.txt");

    cout << "Mobile Number Updated Successfully!" << endl;
    cin.ignore();
    Enter_To_Continue();
}

void Admin::UpdateResidentPassword(string Old_Password, string New_Password)
{
    string lines;
    fout.open(File1);
    fin.open(File3);

    if (!fout || !fin)
    {
        cout << "File Open Error!" << endl;
        return;
    }

    while (getline(fout, lines))
    {
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        string temp_Password = lines.substr(10);

        if (temp_Password == Old_Password)
            fin << "Password: " << New_Password << endl;
        else
            fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;
    }

    fout.close();
    fin.close();

    std::remove("Resident.txt");
    std::rename("TempResident.txt", "Resident.txt");

    cout << "Password Updated Successfully!" << endl;
    cin.ignore();
    Enter_To_Continue();
}

void Admin::updateResidentName(string old_name, string new_name)
{
    string lines;
    fout.open(File1);
    fin.open(File3);

    if (!fout || !fin)
    {
        cout << "File Open Error!" << endl;
        return;
    }

    while (getline(fout, lines))
    {
        fin << lines << endl;

        getline(fout, lines);
        string temp_name = lines.substr(6);

        if (temp_name == old_name)
            fin << "Name: " << new_name << endl;
        else
            fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;

        getline(fout, lines);
        fin << lines << endl;
    }

    fout.close();
    fin.close();

    std::remove("Resident.txt");
    std::rename("TempResident.txt", "Resident.txt");

    cout << "Name Updated Successfully!" << endl;
    cin.ignore();
    Enter_To_Continue();
}

void Admin::DeleteResident()
{
    int id, i = 0;
    bool found = false;
    string lines;

    cout << "Enter Resident Id: ";
    cin >> id;

    fout.open(File1);
    fin.open(File3);
    ofstream deletedFile(File8, ios::app);

    if (!fout || !fin || !deletedFile)
    {
        cout << "File Open Error!" << endl;

        fout.close();
        fin.close();
        deletedFile.close();

        return;
    }

    while (getline(fout, lines))
    {
        if (lines.find("ResidentID: ") != 0)
        {
            fin << lines << endl;
            continue;
        }

        int temp_id = stoi(lines.substr(12));

        string nameLine, wingLine, flatLine;
        string mobileLine, passwordLine, blankLine;

        getline(fout, nameLine);
        getline(fout, wingLine);
        getline(fout, flatLine);
        getline(fout, mobileLine);
        getline(fout, passwordLine);
        getline(fout, blankLine);

        if (temp_id == id)
        {
            found = true;

            deletedFile << lines << endl;
            deletedFile << nameLine << endl;
            deletedFile << wingLine << endl;
            deletedFile << flatLine << endl;
            deletedFile << mobileLine << endl;
            deletedFile << "Deleted Date: ";

            time_t now = time(0);
            tm *ltm = localtime(&now);

            deletedFile << ltm->tm_mday << "-"
                        << ltm->tm_mon + 1 << "-"
                        << ltm->tm_year + 1900 << endl;
            deletedFile << endl;
        }
        else
        {
            fin << lines << endl;
            fin << nameLine << endl;
            fin << wingLine << endl;
            fin << flatLine << endl;
            fin << mobileLine << endl;
            fin << passwordLine << endl;
            fin << blankLine << endl;
        }
    }

    fout.close();
    fin.close();
    deletedFile.close();

    std::remove("Resident.txt");
    std::rename("TempResident.txt", "Resident.txt");

    if (found)
        cout << "Resident Deleted Successfully!" << endl;
    else
        cout << "Resident Not Found" << endl;

    Enter_To_Continue();
}

void ViewMaintenanceMenu()
{
    int viewChoice;

    cout << "+========================================+" << endl;
    cout << "|        VIEW MAINTENANCE RECORDS        |" << endl;
    cout << "+========================================+" << endl;
    cout << "|  1. View All Maintenance Records       |" << endl;
    cout << "|  2. View Pending Maintenance Records   |" << endl;
    cout << "|  3. View Paid Maintenance Records      |" << endl;
    cout << "|  0. Back                               |" << endl;
    cout << "+----------------------------------------+" << endl;
    cout << "|  Enter Your Choice:                    |" << endl;
    cout << "+========================================+" << endl;
    cin >> viewChoice;

    switch (viewChoice)
    {
    case 1:
        clearScreen();
        a1.ViewMaintenance();
        break;

    case 2:
        clearScreen();
        a1.ViewPendingMaintenance();
        break;

    case 3:
        clearScreen();
        a1.ViewPaidMaintenance();
        break;

    case 0:
        break;

    default:
        cout << "Invalid Choice!" << endl;
        cin.ignore();
        Enter_To_Continue();
    }
}

void Admin::maintenaceMangment()
{
    int choice;

    do
    {
        clearScreen();

        cout << "\n";
        cout << "+==========================================+\n";
        cout << "|          MAINTENANCE MANAGEMENT          |\n";
        cout << "+==========================================+\n";
        cout << "|   1. Add Maintenance Record              |\n";
        cout << "|   2. View All Maintenance Records        |\n";
        cout << "|   3. Update Payment Status               |\n";
        cout << "|   4. Back to Admin Dashboard             |\n";
        cout << "+==========================================+\n";
        cout << "|  Enter Your Choice:                      |" << endl;
        cout << "+==========================================+" << endl;
        cin >> choice;

        switch (choice)
        {
        case 1:
            clearScreen();
            AddMaintenanceRecord();
            break;

        case 2:
            clearScreen();
            ViewMaintenanceMenu();
            break;

        case 3:
            clearScreen();
            UpdateMaintenanceStatus();
            break;

        case 4:
            clearScreen();
            cout << "\nReturning to Admin Dashboard...\n";
            break;

        default:
            cout << "\nInvalid choice! Please try again.\n";
            system("pause");
        }

    } while (choice != 4);
}

void Admin::AddMaintenanceRecord()
{
    int id;
    string lines;
    bool found = false;
    bool duplicate = false;
    char choice;

    cout << "+========================================+" << endl;
    cout << "|         ADD MAINTENANCE RECORD         |" << endl;
    cout << "+========================================+" << endl;

    cout << "Enter Resident ID: ";
    cin >> id;

    fout.open(File1);

    if (!fout)
    {
        cout << "Resident File Opening Error!" << endl;
        cin.ignore();
        Enter_To_Continue();
        return;
    }

    while (getline(fout, lines))
    {
        if (lines.find("ResidentID: ") == 0)
        {
            int temp_id = stoi(lines.substr(12));

            if (temp_id == id)
            {
                found = true;
                break;
            }
        }
    }

    fout.close();
    fout.clear();

    if (!found)
    {
        cout << "Resident ID Not Found!" << endl;
        cin.ignore();
        Enter_To_Continue();
        return;
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);

    int currentMonthNumber = ltm->tm_mon + 1;
    int currentYear = ltm->tm_year + 1900;

    string currentMonth = months[currentMonthNumber];

    fout.open(File4);

    if (fout)
    {
        while (getline(fout, lines))
        {
            if (lines.find("Resident ID: ") != 0)
                continue;

            int temp_id = stoi(lines.substr(13));

            string monthLine, yearLine;
            string amountLine, statusLine;

            if (!getline(fout, monthLine) ||
                !getline(fout, yearLine) ||
                !getline(fout, amountLine) ||
                !getline(fout, statusLine))
            {
                break;
            }

            if (temp_id == id &&
                monthLine == "Month: " + currentMonth &&
                yearLine == "Year: " + to_string(currentYear))
            {
                duplicate = true;
                break;
            }
        }

        fout.close();
        fout.clear();
    }

    if (duplicate)
    {
        cout << "\nMaintenance Record Already Exists for "
             << currentMonth << " " << currentYear << "!"
             << endl;

        cout << "Do you want to add next month's "
             << "maintenance record? (Y/N): ";

        cin >> choice;

        if (choice != 'Y' && choice != 'y')
        {
            cout << "Maintenance Record Not Added!" << endl;
            cin.ignore();
            Enter_To_Continue();
            return;
        }

        currentMonthNumber++;

        if (currentMonthNumber > 12)
        {
            currentMonthNumber = 1;
            currentYear++;
        }

        currentMonth = months[currentMonthNumber];

        duplicate = false;

        fout.open(File4);

        if (fout)
        {
            while (getline(fout, lines))
            {
                if (lines.find("Resident ID: ") != 0)
                    continue;

                int temp_id = stoi(lines.substr(13));

                string monthLine, yearLine;
                string amountLine, statusLine;

                if (!getline(fout, monthLine) ||
                    !getline(fout, yearLine) ||
                    !getline(fout, amountLine) ||
                    !getline(fout, statusLine))
                {
                    break;
                }

                if (temp_id == id &&
                    monthLine == "Month: " + currentMonth &&
                    yearLine == "Year: " + to_string(currentYear))
                {
                    duplicate = true;
                    break;
                }
            }

            fout.close();
            fout.clear();
        }

        if (duplicate)
        {
            cout << "Maintenance Record Already Exists for "
                 << currentMonth << " " << currentYear << "!"
                 << endl;

            cin.ignore();
            Enter_To_Continue();
            return;
        }
    }

    fin.open(File4, ios::app);

    if (!fin)
    {
        cout << "Maintenance File Opening Error!" << endl;
        cin.ignore();
        Enter_To_Continue();
        return;
    }

    fin << "Resident ID: " << id << endl
        << "Month: " << currentMonth << endl
        << "Year: " << currentYear << endl
        << "Amount: 1500" << endl
        << "Status: Pending" << endl
        << endl;

    fin.close();

    cout << "\nMaintenance Record Added Successfully!"
         << endl;
    cout << "Resident ID : " << id << endl;
    cout << "Month       : " << currentMonth << endl;
    cout << "Year        : " << currentYear << endl;
    cout << "Amount      : 1500" << endl;
    cout << "Status      : Pending" << endl;

    cin.ignore();
    Enter_To_Continue();
}

void Admin::ViewMaintenance()
{
    string lines;
    int i = 0;

    fout.open(File4);

    if (!fout)
    {
        cout << "Maintenance File Not Found!" << endl;
        cin.ignore();
        Enter_To_Continue();
        return;
    }

    cout << "+========================================+" << endl;
    cout << "|       ALL MAINTENANCE RECORDS          |" << endl;
    cout << "+========================================+" << endl;

    while (getline(fout, lines))
    {
        if (lines.find("Resident ID: ") != 0)
            continue;

        if (i >= 10000)
        {
            cout << "Maximum Record Limit Reached!" << endl;
            break;
        }

        cout << lines << endl;

        for (int j = 0; j < 4; j++)
        {
            if (getline(fout, lines))
                cout << lines << endl;
            else
                break;
        }

        cout << endl;
        i++;
    }

    if (i == 0)
        cout << "No Maintenance Records Found!" << endl;

    fout.close();
    fout.clear();

    cin.ignore();
    Enter_To_Continue();
}

void Admin::ViewPendingMaintenance()
{
    string lines;
    int i = 0;

    fout.open(File4);

    if (!fout)
    {
        cout << "Maintenance File Not Found!" << endl;
        cin.ignore();
        Enter_To_Continue();
        return;
    }

    cout << "+========================================+" << endl;
    cout << "|     PENDING MAINTENANCE RECORDS        |" << endl;
    cout << "+========================================+" << endl;

    while (getline(fout, lines))
    {
        if (lines.find("Resident ID: ") != 0)
            continue;

        string residentLine = lines;
        string monthLine, yearLine;
        string amountLine, statusLine;

        if (!getline(fout, monthLine) ||
            !getline(fout, yearLine) ||
            !getline(fout, amountLine) ||
            !getline(fout, statusLine))
            break;

        if (statusLine == "Status: Pending" ||
            statusLine == "Status:Pending")
        {
            cout << residentLine << endl;
            cout << monthLine << endl;
            cout << yearLine << endl;
            cout << amountLine << endl;
            cout << statusLine << endl
                 << endl;

            i++;
        }
    }

    if (i == 0)
        cout << "No Pending Maintenance Records Found!" << endl;

    fout.close();
    fout.clear();

    cin.ignore();
    Enter_To_Continue();
}

void Admin::ViewPaidMaintenance()
{
    string lines;
    int i = 0;

    fout.open(File4);

    if (!fout)
    {
        cout << "Maintenance File Not Found!" << endl;
        cin.ignore();
        Enter_To_Continue();
        return;
    }

    cout << "+========================================+" << endl;
    cout << "|       PAID MAINTENANCE RECORDS         |" << endl;
    cout << "+========================================+" << endl;

    while (getline(fout, lines))
    {
        if (lines.find("Resident ID: ") != 0)
            continue;

        string residentLine = lines;
        string monthLine, yearLine;
        string amountLine, statusLine;

        if (!getline(fout, monthLine) ||
            !getline(fout, yearLine) ||
            !getline(fout, amountLine) ||
            !getline(fout, statusLine))
            break;

        if (statusLine == "Status: Paid" ||
            statusLine == "Status:Paid")
        {
            cout << residentLine << endl;
            cout << monthLine << endl;
            cout << yearLine << endl;
            cout << amountLine << endl;
            cout << statusLine << endl
                 << endl;

            i++;
        }
    }

    if (i == 0)
        cout << "No Paid Maintenance Records Found!" << endl;

    fout.close();
    fout.clear();

    cin.ignore();
    Enter_To_Continue();
}

void Admin::UpdateMaintenanceStatus()
{
    int id;
    string lines;
    bool found = false;
    bool duplicate = false;

    fin.open(File5);
    fout.open(File4);

    if (!fin || !fout)
    {
        cout << "File Opening Error!" << endl;
        return;
    }

    cout << "Enter Resident ID: ";
    cin >> id;

    while (getline(fout, lines))
    {
        if (lines.empty())
        {
            continue;
        }

        // Sirf Resident ID wali line process karo
        if (lines.find("Resident ID: ") != 0)
        {
            continue;
        }

        int temp_id = stoi(lines.substr(12));

        string monthLine, yearLine, amountLine, statusLine;

        getline(fout, monthLine);
        getline(fout, yearLine);
        getline(fout, amountLine);
        getline(fout, statusLine);

        if (id == temp_id)
        {
            found = true;
            int choice;

            cout << "+========================================+" << endl;
            cout << "|        CURRENT MAINTENANCE DETAILS    |" << endl;
            cout << "+========================================+" << endl;

            cout << lines << endl;
            cout << monthLine << endl;
            cout << yearLine << endl;
            cout << amountLine << endl;
            cout << statusLine << endl;

            cout << "+----------------------------------------+" << endl;
            cout << "|  1. Paid                               |" << endl;
            cout << "|  2. Pending                            |" << endl;
            cout << "+----------------------------------------+" << endl;
            cout << "|  Enter Your Choice:                    |" << endl;
            cin >> choice;
            cout << "+========================================+" << endl;

            if (choice == 1)
            {
                statusLine = "Status: Paid";
            }
            else if (choice == 2)
            {
                statusLine = "Status: Pending";
            }
            else
            {
                cout << "Invalid Choice! Status Not Updated."
                     << endl;
            }
        }
        fin << lines << endl;
        fin << monthLine << endl;
        fin << yearLine << endl;
        fin << amountLine << endl;
        fin << statusLine << endl;
        fin << endl;
    }
    fin.close();
    fout.close();

    if (found)
    {
        remove("Maintence.txt");
        rename("TempMaintence.txt", "Maintence.txt");

        cout << "Maintenance Status Updated Successfully!" << endl;
    }
    else
    {
        remove("TempMaintence.txt");
        cout << "Resident ID Not Found!" << endl;
    }

    cin.ignore();
    Enter_To_Continue();
}

void Admin::ComplaintManagement()
{
    int choice;

    do
    {
        cout << "+========================================+" << endl;
        cout << "|          COMPLAINT MANAGEMENT          |" << endl;
        cout << "+========================================+" << endl;
        cout << "|  1. View All Complaints                |" << endl;
        cout << "|  2. View Pending Complaints            |" << endl;
        cout << "|  3. Update Complaint Status            |" << endl;
        cout << "|  4. Back                               |" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "|  Enter Your Choice:                    |" << endl;
        cout << "+========================================+" << endl;
        cin >> choice;

        switch (choice)
        {
        case 1:
            ViewAllComplaints();
            break;

        case 2:
            ViewPendingComplaints();
            break;

        case 3:
            UpdateComplaintStatus();
            break;

        case 4:
            cout << "Returning to Admin Dashboard..." << endl;
            break;

        default:
            cout << "Invalid Choice!" << endl;
        }

    } while (choice != 4);
}

void Admin::ViewAllComplaints()
{
    fout.open(File6);

    if (!fout.is_open())
    {
        cout << "\nNo complaint records found!" << endl;
        return;
    }

    string line;
    bool found = false;

    cout << "+========================================+" << endl;
    cout << "|             ALL COMPLAINTS             |" << endl;
    cout << "+========================================+" << endl;

    while (getline(fout, line))
    {
        if (line.find("Complaint ID: ") == 0)
            found = true;

        if (found)
        {
            cout << line << endl;

            if (line.empty())
            {
                found = false;
                cout << "------------------------------" << endl;
            }
        }
    }

    fout.close();
}

void Admin::ViewPendingComplaints()
{
    fout.open(File6);

    if (!fout.is_open())
    {
        cout << "\nNo complaint records found!" << endl;
        return;
    }

    string line;
    string record = "";
    bool found = false;

    while (getline(fout, line))
    {
        if (line.find("Complaint ID: ") == 0)
            record = "";

        record += line + "\n";

        if (line.empty())
        {
            if (record.find("Status: Pending") != string::npos)
            {
                cout << "\n"
                     << record;
                cout << "+================================+" << endl;
                found = true;
            }

            record = "";
        }
    }

    // Check the last record if it has no trailing blank line.
    if (!record.empty() &&
        record.find("Status: Pending") != string::npos)
    {
        cout << "\n"
             << record;
        cout << "+================================+" << endl;
        found = true;
    }

    if (!found)
        cout << "\nNo pending complaints found!" << endl;

    fout.close();
}

void Admin::UpdateComplaintStatus()
{
    int SearchID, choice;
    string line;
    string Status;
    bool found = false;
    bool updated = false;

    cout << "+========================================+" << endl;
    cout << "|          UPDATE COMPLAINT STATUS       |" << endl;
    cout << "+========================================+" << endl;
    cout << "  Enter Complaint ID: ";
    cin >> SearchID;

    cout << "+----------------------------------------+" << endl;
    cout << "|  1. Pending                            |" << endl;
    cout << "|  2. In Progress                        |" << endl;
    cout << "|  3. Resolved                            |" << endl;
    cout << "+----------------------------------------+" << endl;
    cout << "|  Enter Your Choice:                    |" << endl;
    cout << "+========================================+" << endl;
    cin >> choice;

    switch (choice)
    {
    case 1:
        Status = "Pending";
        break;

    case 2:
        Status = "In Progress";
        break;

    case 3:
        Status = "Resolved";
        break;

    default:
        cout << "Invalid Status Choice!" << endl;
        return;
    }

    fout.open(File6);

    if (!fout.is_open())
    {
        cout << "\nUnable to open Complaint.txt!" << endl;
        return;
    }

    fin.open(File7);

    if (!fin.is_open())
    {
        fout.close();
        cout << "\nUnable to create temporary complaint file!" << endl;
        return;
    }

    while (getline(fout, line))
    {
        if (line.find("Complaint ID: ") == 0)
        {
            int CurrentID = stoi(line.substr(14));

            found = (CurrentID == SearchID);
        }

        if (found && line.find("Status: ") == 0)
        {
            fin << "Status: " << Status << endl;
            found = false;
            updated = true;
        }
        else
        {
            fin << line << endl;
        }
    }

    fout.close();
    fin.close();

    if (!found)
    {
        ifstream check(File7);
        bool idExists = false;

        while (getline(check, line))
        {
            if (line == "Complaint ID: " + to_string(SearchID))
            {
                idExists = true;
                break;
            }
        }

        check.close();

        if (!updated)
        {
            remove("TempComplaint.txt");
            cout << "\nComplaint ID not found!" << endl;
            return;
        }
    }

    if (remove("Complaint.txt") != 0)
    {
        cout << "\nUnable to replace Complaint.txt!" << endl;
        return;
    }

    if (rename("TempComplaint.txt", "Complaint.txt") != 0)
    {
        cout << "\nUnable to rename temporary file!" << endl;
        return;
    }

    cout << "\nComplaint status updated successfully!" << endl;
}

void search(int choice)
{
    string lines;
    int i = 0;
    bool found = false;

    int id, FlatNo;
    long long int MobileNo;

    fout.open(File1);

    if (!fout)
    {
        fin.open(File1);
        fin.close();
    }

    if (choice == 1)
    {
        cout << endl
             << "Enter Resident I'd: ";
        cin >> id;
    }
    else if (choice == 2)
    {
    mobile:
        cout << endl
             << "Enter Mobile Number: ";
        if (!(cin >> MobileNo))
        {
            cout << "Invalid input! Please enter digits only.\n";

            cin.clear();
            cin.ignore(10000, '\n');
            goto mobile;
        }

        if (!checkMobileNUmberValidation(MobileNo))
        {
            cout << "Invalid Mobile Number! Please Enter 10 Digit Number." << endl;
            goto mobile;
        }
    }
    else if (choice == 3)
    {
        cout << "Enter Your Flat No: ";
        cin >> FlatNo;
    }

    while (getline(fout, lines))
    {
        if (i >= 10000)
        {
            cout << "Maximum Resident Limit Reached!" << endl;
            break;
        }

        a[i].id = stoi(lines.substr(12));

        getline(fout, lines);
        a[i].Name = lines.substr(6);

        getline(fout, lines);
        a[i].WingCharacter = lines[16];

        getline(fout, lines);
        a[i].FlatNo = stoi(lines.substr(12));

        getline(fout, lines);
        a[i].MobileNo = stoll(lines.substr(15));

        getline(fout, lines);
        a[i].Password = lines.substr(10);

        getline(fout, lines);

        if (choice == 1)
        {
            if (id == a[i].id)
            {
                found = true;

                cout << "+========================================+" << endl;
                cout << "|             RESIDENT DETAILS           |" << endl;
                cout << "+========================================+" << endl;

                cout << " Resident ID     : " << a[i].id << endl
                     << " Name            : " << a[i].Name << endl
                     << " Wing Character  : " << a[i].WingCharacter << endl
                     << " Flat Number     : " << a[i].FlatNo << endl
                     << " Mobile No       : " << a[i].MobileNo << endl;

                cout << "+========================================+" << endl;

                break;
            }
        }
        else if (choice == 2)
        {
            if (MobileNo == a[i].MobileNo)
            {
                found = true;

                cout << "+========================================+" << endl;
                cout << "|             RESIDENT DETAILS           |" << endl;
                cout << "+========================================+" << endl;

                cout << " Resident ID     : " << a[i].id << endl
                     << " Name            : " << a[i].Name << endl
                     << " Wing Character  : " << a[i].WingCharacter << endl
                     << " Flat Number     : " << a[i].FlatNo << endl
                     << " Mobile No       : " << a[i].MobileNo << endl;

                cout << "+========================================+" << endl;

                break;
            }
        }
        else if (choice == 3)
        {
            if (FlatNo == a[i].FlatNo)
            {
                found = true;

                cout << "+========================================+" << endl;
                cout << "|             RESIDENT DETAILS           |" << endl;
                cout << "+========================================+" << endl;

                cout << " Resident ID     : " << a[i].id << endl
                     << " Name            : " << a[i].Name << endl
                     << " Wing Character  : " << a[i].WingCharacter << endl
                     << " Flat Number     : " << a[i].FlatNo << endl
                     << " Mobile No       : " << a[i].MobileNo << endl;

                cout << "+========================================+" << endl;
            }
        }
        i++;
    }

    if (!found)
        cout << "Resident Not Found ... " << endl;

    fout.close();
}

int main()
{
    LoadData();

    clearScreen();

    loginPage();

    return 0;
}

void Enter_To_Continue()
{
    cout << endl
         << "Press Enter to Continue...";
    cin.get();
}

void LoadData()
{
    string lines;
    int i = 0;
    fout.open(File1);
    int maxResidentID = 99;

    if (!fout)
    {
        fin.open(File1);
        fin.close();
    }

    while (getline(fout, lines))
    {
        if (i >= 10000)
        {
            cout << "Maximum Resident Limit Reached!" << endl;
            break;
        }

        r[i].id = stoi(lines.substr(12));

        if (r[i].id > maxResidentID)
        {
            maxResidentID = r[i].id;
        }

        getline(fout, lines);
        r[i].Name = lines.substr(6);

        getline(fout, lines);
        r[i].WingCharacter = lines[16];

        getline(fout, lines);
        r[i].FlatNo = stoi(lines.substr(12));

        getline(fout, lines);
        r[i].MobileNo = stoll(lines.substr(15));

        getline(fout, lines);
        r[i].Password = lines.substr(10);

        getline(fout, lines);

        i++;
    }

    Resident::ResidentID = maxResidentID + 1;

    fout.close();
}

bool CheckLogin(long long int mobile_No, string password, int &Logged_In_Number)
{
    fout.open(File1);
    string lines;
    int i = 0;

    while (getline(fout, lines))
    {
        getline(fout, lines);
        getline(fout, lines);
        getline(fout, lines);

        getline(fout, lines);
        long long int Stored_Mobile_No = stoll(lines.substr(15));

        getline(fout, lines);
        string Stored_Password = lines.substr(10);
        getline(fout, lines);

        if (mobile_No == Stored_Mobile_No && password == Stored_Password)
        {
            fout.close();
            Logged_In_Number = i;
            return true;
        }

        i++;
    }
    fout.close();
    return false;
}

void loginPage()
{
    int ch;

    do
    {
        clearScreen();

        cout << "+========================================+" << endl;
        cout << "|                  LOGIN                 |" << endl;
        cout << "+========================================+" << endl;
        cout << "|  1. Resident Login                     |" << endl;
        cout << "|  2. Admin Login                        |" << endl;
        cout << "|  0. Exit                               |" << endl;
        cout << "+----------------------------------------+" << endl;
        cout << "|  Enter Your Choice:                    |" << endl;
        cout << "+========================================+" << endl;
        cin >> ch;

        if (ch == 1)
        {
            clearScreen();
            ResidentLogin();
        }
        else if (ch == 2)
        {
            clearScreen();
            AdminLogin();
        }
        else if (ch == 0)
        {
            clearScreen();
            cout << "Byy";
            exit(0);
        }
        else
        {
            clearScreen();
            cout << "You are Choose Wrong Choice ..." << endl;
        }
    } while (ch != 0);
}

void ResidentLogin()
{
    char ac_Choice, Choose_Registration;
    int Logged_In_Number = -1;

    cout << "+========================================+" << endl;
    cout << "|             RESIDENT LOGIN             |" << endl;
    cout << "+========================================+" << endl;

    cout << " Do you have an account? (Y/N): ";
    cin >> ac_Choice;

    if (ac_Choice == 'y' || ac_Choice == 'Y')
    {
        clearScreen();
        cout << line << "Resident Login Page" << ModuleLines << endl;

        long long int MobileNumber;
        int len, i = 0;
        string Password;

    Mobile:
        cout << " Enter Your Mobile Number : ";
        cin >> MobileNumber;

        if (MobileNumber < 1000000000 || MobileNumber > 9999999999)
        {
            cout << "Invalid Mobile Number! Please Enter 10 Digit Number." << endl;
            cin.ignore();
            goto Mobile;
        }

    pass:
        cout << "Enter Your Password: ";
        cin.ignore();
        getline(cin, Password);

        LoadData();

        if (CheckLogin(MobileNumber, Password, Logged_In_Number))
        {
            clearScreen();

            cout << "+========================================+" << endl;
            cout << "|          LOGIN SUCCESSFUL              |" << endl;
            cout << "+========================================+" << endl;
            cout << "|                                        |" << endl;
            cout << "|  Welcome, "
                 << r[Logged_In_Number].GetName() << "!" << endl;
            cout << "|                                        |" << endl;
            cout << "+========================================+" << endl;

            Enter_To_Continue();

            r1.ResidentDashobard(Logged_In_Number);
        }
        else
        {
            cout << endl
                 << "Account Not Found ..." << endl
                 << endl;

            Enter_To_Continue();

            clearScreen();

            cout << line << "Would you like to register an account? (y/N): " << line << endl;
            cin >> Choose_Registration;

            if (Choose_Registration == 'y' || Choose_Registration == 'Y')
            {
                clearScreen();
                Resident_Registration_Page();
            }

            else if (Choose_Registration == 'n' || Choose_Registration == 'N')
            {
                exit(0);
            }
            else
            {
                cout << "You Are chhose Wrong Operations ... ";
            }
        }
    }
    else if (ac_Choice == 'n' || ac_Choice == 'N')
    {
        clearScreen();
        r1.CreateAccount();
    }
    else
    {
        cout << "You Are chhose Wrong Operations ... ";
    }
}

void Resident_Registration_Page()
{
    cout << "Resident Registration Page ... " << endl;
    r1.CreateAccount();
}

void AdminLogin()
{
    string AdminUsername, AdminPassword;

    string username = "Admin";
    string password = "Admin@123";

    cin.ignore();

login:
    cout << "Enter Admin Username: ";
    getline(cin, AdminUsername);

    cout << "Enter Admin Password: ";
    getline(cin, AdminPassword);

    if (AdminUsername == username && AdminPassword == password)
    {
        clearScreen();

        cout << "+========================================+" << endl;
        cout << "|          ADMIN LOGIN SUCCESSFUL        |" << endl;
        cout << "+========================================+" << endl;
        cout << "|             Welcome Admin!             |" << endl;
        cout << "+========================================+" << endl;

        a1.AdminDashboard();
    }
    else
    {
        cout << endl
             << "Invalid Username or Password!" << endl
             << "Please Try Again." << endl;

        goto login;
    }
}
