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
bool checkPasswordValidation(string);
void loginPage();
void LoadData();

void ResidentLogin();
void Resident_Registration_Page();
void ResidentDashobard(int);

void AdminLogin();

class Resident
{
    static int ResidentID;
    static int CountResident;

    int id, FlatNo;
    long long int MobileNo;

    string Name, Password, rePassword;
    char WingNumber;

public:
    void CreateAccount();

    friend void LoadData();
    friend bool CheckLogin(long long int, string, int&);

    void ViewProfile(int);
    void UpdateProfile(int);
} r[100], r1;

int Resident::ResidentID = 100;

/* Validate password strength requirements: minimum 8 characters,
must include uppercase, lowercase, number, and a special character. */

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

// This function collects resident details and stores the account in the resident file.
void Resident::CreateAccount()
{
    cout << "Enter Your Name: ";
    cin.ignore();
    getline(cin, Name);

    cout << "Enter Your Wing Number: ";
    cin >> WingNumber;

    cout << "Enter Your Flat Number: ";
    cin >> FlatNo;

mobile:
    // Ask for a valid 10-digit mobile number.
    cout << "Enter Your Mobile Number: ";
    cin >> MobileNo;

    if (MobileNo < 1000000000 || MobileNo > 9999999999)
    {
        cout << "Invalid Mobile Number! Please Enter 10 Digit Number." << endl;
        goto mobile;
    }

    cin.ignore();

pass:
    // Password must follow the defined validation rules.
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
        // Open the resident file in append mode to save the new account.
        ofstream fin;

        fin.open(File1, ios::app);

        if (!fin)
        {
            cout << "File is Not Found \n";
            return;
        }
        else
        {
            // Write resident information to file in a readable format.
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
    }
    else
    {
        cout << "Password Do not match ... " << endl
             << endl;
        goto pass;
    }
}

// View All Resident.
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

// Update the Resident Mobile Number and Password
void Resident :: UpdateProfile(int Logged_In_Number)
{
    cout << line << " What do you have to update? " << line << endl;
}

// Entry point of the application. It loads saved resident data and then shows login page.
int main()
{
    LoadData();

    clearScreen();

    loginPage();

    return 0;
}

// After pressing enter then work other.
void Enter_To_Continue()
{
    cout << endl
         << "Press Enter to Continue...";
    cin.get();
}

// Load all resident information from the text file into memory for login and dashboard use.
void LoadData()
{
    string lines;
    int i = 0;
    fout.open(File1);
    int count_resident = 0;

    if (!fout)
    {
        // If the file does not exist, create it so future data can be saved.
        fin.open(File1);
        fin.close();
    }

    while (getline(fout, lines))
    {
        if (i >= 100)
        {
            cout << "Maximum Resident Limit Reached!" << endl;
            break;
        }

        r[i].id = stoi(lines.substr(12));

        // Each resident record is stored in blocks separated by blank lines.
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

    // Set the next available resident ID after reading all saved residents.
    Resident::ResidentID = count_resident + 100;

    fout.close();
}

// Check whether a resident's mobile number and password match a saved account.
bool CheckLogin(long long mobile_No, string password, int &Logged_In_Number)
{
    fout.open(File1);
    string lines;
    int i = 0;

    while (getline(fout, lines))
    {
        // Skip through the resident record fields to reach the mobile and password values.
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

// Main menu for selecting resident login, admin login, or exit.
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

// Handles the resident login flow, including account checking and registration option.
void ResidentLogin()
{
    char ac_Choice, Choose_Registration;
    int Logged_In_Number = -1;

    // Ask the resident whether they already have an account.
    cout << line << "Do you have an account? ( y / N)" << line << endl;
    cin >> ac_Choice;

    if (ac_Choice == 'y' || ac_Choice == 'Y')
    {
        // Resident already has an account, so show the login form.
        clearScreen();
        cout << line << "Resident Login Page" << ModuleLines << endl;

        long long int MobileNumber;
        int len, i = 0;
        string Password;

    Mobile:
        // The mobile number must be exactly 10 digits.
        cout << " Enter Your Mobile Number : ";
        cin >> MobileNumber;

        if (MobileNumber < 1000000000 || MobileNumber > 9999999999)
        {
            cout << "Invalid Mobile Number! Please Enter 10 Digit Number." << endl;
            cin.ignore();
            goto Mobile;
        }

    pass:
        // Ask for password and validate its strength.
        cout << "Enter Your Password: ";
        cin.ignore();
        getline(cin, Password);

        // Load resident data from file before authentication.
        LoadData();

        if (CheckLogin(MobileNumber, Password, Logged_In_Number))
        {
            // If the credentials match, open the resident dashboard.
            cout << "Login Succsffully ..." << endl;
            ResidentDashobard(Logged_In_Number);
        }
        else
        {
            // If login fails, ask whether the resident wants to register a new account.
            cout << endl
                 << "Account Not Found ..." << endl
                 << endl;

            Enter_To_Continue();

            clearScreen();

            cout << line << "Would you like to register an account? (y/N): " << line << endl;
            cin >> Choose_Registration;

            if (Choose_Registration == 'y' || Choose_Registration == 'Y')
            {
                // Open the registration page for a new user.
                clearScreen();
                Resident_Registration_Page();
            }

            else if (Choose_Registration == 'n' || Choose_Registration == 'N')
            {
                // The user chooses to exit instead of registering.
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
        // New resident: create an account first and then open the dashboard.
        clearScreen();
        r1.CreateAccount();
    }
    else
    {
        // Invalid option entered for account selection.
        cout << "You Are chhose Wrong Operations ... ";
    }
}

// Resident registration page that directs the user to the account creation form.
void Resident_Registration_Page()
{
    cout << "Resident Registration Page ... " << endl;
    r1.CreateAccount();
}

// Resident dashboard after successful login.
void ResidentDashobard(int Logged_In_Number)
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

// Admin login section with fixed username and password authentication.
void AdminLogin()
{
    string AdminUsername, AdminPassword;

    string username = "Admin";
    string password = "Admin@123";

    cin.ignore();

login:
    // Prompt the admin to enter their credentials.
    cout << "Enter Admin Username: ";
    getline(cin, AdminUsername);

    cout << "Enter Admin Password: ";
    getline(cin, AdminPassword);

    if (AdminUsername == username && AdminPassword == password)
    {
        // Admin credentials are correct; show success message.
        clearScreen();

        cout << line << "Admin Login Successful" << line << endl;

        cout << "Welcome Admin!" << endl;
    }
    else
    {
        // If credentials are incorrect, allow retry.
        cout << endl
             << "Invalid Username or Password!" << endl
             << "Please Try Again." << endl;

        goto login;
    }
}
