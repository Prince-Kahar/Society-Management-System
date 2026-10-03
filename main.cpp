#include <iostream>
#include <fstream>
#include <string.h>

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

bool checkPasswordValidation(string);
void loginPage();
void LoadData();
void ResidentLogin();
void Resident_Registration_Page();
void ResidentDashobard();
void AdminLogin();

class Resident
{
    static int ResidentID;
    static int CountResident;

    int FlatNo;
    long long int MobileNo;

    string Name, Password, rePassword;
    char WingNumber;

public:
    void CreateAccount();

    friend void LoadData();
    friend bool CheckLogin(long long mobile_No, string password);
} r[100], r1;

int Resident::ResidentID = 100;
// int Resident :: CountResident = 1;

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
    cout << "Enter Your Mobile Number: ";
    cin >> MobileNo;

    if (MobileNo < 1000000000 || MobileNo > 9999999999)
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

        ResidentID++;

        fin.close();

        cout << line << " Account Created Successfully " << line << endl;
    }
    else
    {
        cout << "Password Do not match ... " << endl
             << endl;
        goto pass;
    }
}

int main()
{
    LoadData();

    clearScreen();

    loginPage();

    return 0;
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
        if (i >= 100)
        {
            cout << "Maximum Resident Limit Reached!" << endl;
            break;
        }

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

    cout << endl
         << "Data Load Succeffully" << endl;
}

bool CheckLogin(long long mobile_No, string password)
{
    fout.open(File1);
    string lines;
    int i;

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
            return true;
        }
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

        if (!checkPasswordValidation(Password))
        {
            cout << " Password must contain at least 8 characters, including an uppercase letter, a lowercase letter, a number, and a special character (!, @, #, $) ..." << endl;
            goto pass;
        }

        // Password And Mobile Number Checking Left

        LoadData();

        if (CheckLogin(MobileNumber, Password))
        {
            cout << "Login Succsffully ..." << endl;
            ResidentDashobard();
        }
        else
        {
            cout << endl
                 << "Account Not Found ..." << endl
                 << endl;

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

        clearScreen();
        ResidentDashobard();
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

void ResidentDashobard()
{
    cout << "Resident Dashboard Page ..." << endl;
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
    }
    else
    {
        cout << endl
             << "Invalid Username or Password!" << endl
             << "Please Try Again." << endl;

        goto login;
    }
}
