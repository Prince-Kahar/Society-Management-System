#include <fstream>
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
    out << " ----------------- ";
    return out;
}

ostream &ModuleLines(ostream &out)
{
    out << " ------------------------------------------ ";
    return out;
}

ifstream fout;
ofstream fin;

string File1 = "Resident.txt";
string File2 = "Admin.txt";

void Enter_To_Continue();
bool checkMobileNUmberValidation(long long int);
bool checkPasswordValidation(string);
void loginPage();
void LoadData();

void ResidentLogin();
bool CheckLogin(long long int, string, int &);
void Resident_Registration_Page();

void AdminLogin();
void search(int);

class Resident
{
protected:
    static int ResidentID;
    static int CountResident;

    int id, FlatNo;
    long long int MobileNo;

    string Name, Password, rePassword;
    char WingNumber;

public:
    void ResidentDashobard(int);
    void CreateAccount();

    friend void LoadData();
    friend bool CheckLogin(long long int, string, int &);

    void ViewProfile(int);
    void UpdateProfile(int);
    void UpdateMobileNumber(long long int, long long int);
} r[10000], r1;

int Resident::ResidentID = 100;

void Resident::ResidentDashobard(int Logged_In_Number)
{
    int choose;

Dashboard:
    clearScreen();
    cout << "Resident Dashboard Page ..." << endl;
    cout << line << " Resident Dashboard " << line << endl
         << " 1. View Profile " << endl
         << " 2. Update Profile " << endl
         << " 3. Change Password " << endl
         << " 4. View Maintenance Details " << endl
         << " 5. Submit Complaint " << endl
         << " 6. View Complaint Status " << endl
         << " 0. Logout " << endl
         << ModuleLines << endl;

    cout << "Enter Your Choice: ";
    cin >> choose;

    switch (choose)
    {
    case 1:
        clearScreen();
        cout << "View Profile Selected..." << endl;
        r1.ViewProfile(Logged_In_Number);
        break;

    case 2:
        clearScreen();
        cout << "Update Profile Selected..." << endl;
        r1.UpdateProfile(Logged_In_Number);
        break;

    case 3:
        clearScreen();
        cout << "Change Password Selected..." << endl;
        break;

    case 4:
        clearScreen();
        cout << "View Maintenance Details Selected..." << endl;
        break;

    case 5:
        clearScreen();
        cout << "Submit Complaint Selected..." << endl;
        break;

    case 6:
        clearScreen();
        cout << "View Complaint Status Selected..." << endl;
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

    cout << "Enter Your Wing Number: ";
    cin >> WingNumber;

    cout << "Enter Your Flat Number: ";
    cin >> FlatNo;

mobile:
    cout << "Enter Your Mobile Number: ";
    cin >> MobileNo;

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
                << "Wing Number: " << WingNumber << endl
                << "Flat Number: " << FlatNo << endl
                << "Mobile Number: " << MobileNo << endl
                << "Password: " << Password << endl
                << endl;
        }

        id = ResidentID;
        ResidentID++;

        fin.close();

        cout << line << " Account Created Successfully " << line << endl;

        Enter_To_Continue();

        if (CheckLogin(MobileNo, Password, Logged_In_Number))
        {
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
    cout << line << " RESIDENT PROFILE " << line << endl;

    cout << "Resident ID : " << r[Logged_In_Number].id << endl
         << "Name        : " << r[Logged_In_Number].Name << endl
         << "Wing Number : " << r[Logged_In_Number].WingNumber << endl
         << "Flat Number : " << r[Logged_In_Number].FlatNo << endl
         << "Mobile No   : " << r[Logged_In_Number].MobileNo << endl
         << ModuleLines << endl;

    Enter_To_Continue();
}

void Resident::UpdateProfile(int Logged_In_Number)
{
    int choice;
    long long int New_Mobile;
    cout << line << " What do you have to update? " << line << endl;
    cout << " 1. Mobile Number " << endl
         << " 2. Password " << endl
         << " 3. Nothing Update "
         << endl;

    cout << " Chhose Your Choice: ";
    cin >> choice;

    if (choice == 1)
    {
    mobile:
        cout << " Enter Your New Mobile Number: ";
        cin >> New_Mobile;

        if (!checkMobileNUmberValidation(New_Mobile))
        {
            cout << "Invalid Mobile Number! Please Enter 10 Digit Number." << endl;
            goto mobile;
        }
        else
        {
            UpdateMobileNumber(r[Logged_In_Number].MobileNo, New_Mobile);
        }
    }
}

void Resident::UpdateMobileNumber(long long int Old_Mobile, long long int New_Mobile)
{
    string lines;
    int i = 0;
    fout.open(File1);

    if (!fout)
    {
        fin.open(File1);
        fin.close();
    }

    while (getline(fout, lines))
    {
        getline(fout, lines);
        getline(fout, lines);
        getline(fout, lines);
        getline(fout, lines);
        r[i].MobileNo = stoll(lines.substr(15));

        if (r[i].MobileNo == Old_Mobile)
        {
            fin.seekp(15);
            r[i].MobileNo = New_Mobile;
            cout << "Mobile Number Updated Succcessfuly " << endl;

            break;
        }

        getline(fout, lines);
        r[i].Password = lines.substr(10);

        getline(fout, lines);

        i++;
    }

    fout.close();
}

bool checkMobileNUmberValidation(long long int MobileNumber)
{
    if (MobileNumber <= 99999999 || MobileNumber > 9999999999)
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

    // void AddResident();
    // void UpdateResident();
    // void DeleteResident();
    // void ViewMaintenance();
    // void ViewComplaints();
    // void ComplaintStatus();
} a[10000], a1;

void Admin::AdminDashboard()
{
    int choice;

    do
    {
        clearScreen();

        cout << "\n========================================\n";
        cout << "           ADMIN DASHBOARD\n";
        cout << "========================================\n";

        cout << "1. View All Residents" << endl;
        cout << "2. Search Resident" << endl;
        cout << "3. Add Resident" << endl;
        cout << "4. Update Resident" << endl;
        cout << "5. Delete Resident" << endl;
        cout << "6. View Maintenance" << endl;
        cout << "7. View Complaints" << endl;
        cout << "8. Complaint Status" << endl;
        cout << "0. Logout" << endl;

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            clearScreen();
            cout << line << " View All Residents " << line << endl;
            ViewAllResidents();
            break;

        case 2:
            cout << "\nSearch Resident";
            SearchResident();
            break;

        case 3:
            cout << "\nAdd Resident";
            // AddResident();
            break;

        case 4:
            cout << "\nUpdate Resident";
            // UpdateResident();
            break;

        case 5:
            cout << "\nDelete Resident";
            // DeleteResident();
            break;

        case 6:
            cout << "\nView Maintenance";
            // ViewMaintenance();
            break;

        case 7:
            cout << "\nView Complaints";
            // ViewComplaints();
            break;

        case 8:
            cout << "\nComplaint Status";
            // ComplaintStatus();
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

        cout << line << " SEARCH RESIDENT " << line << endl;

        cout << " 1. Search By Resident ID " << endl
             << " 2. Search By Mobile Number " << endl
             << " 3. Search By Flat Number " << endl
             << " 0. Back " << endl
             << ModuleLines << endl;

        cout << "Enter Your Choice: ";
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

void search(int choice)
{
    string lines;
    int i = 0, found = 0;

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
        cin >> MobileNo;

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
        a[i].WingNumber = lines[13];

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
                found = 1;

                cout << " Resident ID : " << a[i].id << endl
                     << " Name        : " << a[i].Name << endl
                     << " Wing Number : " << a[i].WingNumber << endl
                     << " Flat Number : " << a[i].FlatNo << endl
                     << " Mobile No   : " << a[i].MobileNo << endl
                     << " Password    : " << a[i].Password << endl
                     << ModuleLines << endl;
                break;
            }
        }
        else if (choice == 2)
        {
            if (MobileNo == a[i].MobileNo)
            {
                found = 1;

                cout << " Resident ID : " << a[i].id << endl
                     << " Name        : " << a[i].Name << endl
                     << " Wing Number : " << a[i].WingNumber << endl
                     << " Flat Number : " << a[i].FlatNo << endl
                     << " Mobile No   : " << a[i].MobileNo << endl
                     << " Password    : " << a[i].Password << endl
                     << ModuleLines << endl;
                break;
            }
        }
        else if (choice == 3)
        {
            if (FlatNo == a[i].FlatNo)
            {
                found = 1;

                cout << " Resident ID : " << a[i].id << endl
                     << " Name        : " << a[i].Name << endl
                     << " Wing Number : " << a[i].WingNumber << endl
                     << " Flat Number : " << a[i].FlatNo << endl
                     << " Mobile No   : " << a[i].MobileNo << endl
                     << " Password    : " << a[i].Password << endl
                     << ModuleLines << endl;
                break;
            }
        }
        i++;
    }

    if (found == 0)
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

        r[i].id = stoi(lines.substr(12));

        getline(fout, lines);
        r[i].Name = lines.substr(6);

        getline(fout, lines);
        r[i].WingNumber = lines[13];

        getline(fout, lines);
        r[i].FlatNo = stoi(lines.substr(12));

        getline(fout, lines);
        r[i].MobileNo = stoll(lines.substr(15));

        getline(fout, lines);
        r[i].Password = lines.substr(10);

        getline(fout, lines);

        count_resident++;
        i++;
    }

    Resident::ResidentID = count_resident + 100;

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
        cout << line << "Login " << line << endl
             << " 1. Resident Login " << endl
             << " 2. Admin Login " << endl
             << " 0. Exit " << endl
             << ModuleLines << endl;

        cout << "Enter Your Choice : ";
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

    cout << line << "Do you have an account? ( y / N)" << line << endl;
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
            cout << "Login Succsffully ..." << endl;
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

        cout << line << "Admin Login Successful" << line << endl;

        cout << "Welcome Admin!" << endl;

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
