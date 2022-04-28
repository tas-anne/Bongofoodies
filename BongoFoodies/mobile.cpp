#include"mobile.h"
string Mobile::checkValidity(string mobileNo, string pin, double totalAmount)
{
    /*The card info will contain id no., password, balance(Charge)*/
    //cout << "hiii, I'm in" << endl;
    ifstream check("mobileBankingCheck.txt");
    string ID;
    string Pass;
    string line;
    double Balance;

    while (!check.eof())
    {
        //cout << "I'm in 1" << endl;
        getline(check, ID);
        if (mobileNo == ID)
        {
            getline(check, Pass);
            if (Pass == pin)
            {
                getline(check, line);
                Balance = stod(line);
                checkPayment(totalAmount, Balance);
                if (Balance < totalAmount)
                {
                    cout << "Choose a different payment method? (y/n)...";
                    char ch;
                    cin >> ch;
                    if (ch == 'y' || ch == 'Y')
                        return "another method";
                    else
                        return "unsuccessful";
                }
                return "success";
            }
            else
            {
                cout << "Incorrect Password!!" << endl << endl;
                cout << "Enter Mobile No:";
                cin >> mobileNo;
                cout << "Enter password:";
                cin >> pin;
                return checkValidity(mobileNo, pin, totalAmount);
            }
        }
        getline(check, line);
        getline(check, line);
    }
    cout << "Incorrect Card Number!!" << endl << endl;
    cout << "Enter Mobile No:";
    cin >> mobileNo;
    cout << "Enter password:";
    cin >> pin;
    return checkValidity(mobileNo, pin, totalAmount);
    return checkValidity(mobileNo, pin, totalAmount);
}


void Mobile::checkPayment(double totalAmount, double balance)
{

    /* An object must be instantiated from credit class to get its price using the getPrice function to use the price in this current function
    like so :
    Credit my_payment;
    price = my_payment.getPrice();
    */

    if (totalAmount <= balance)
        cout << "Successful payment! Thanks for choosing BongoFoodies order service!!!" << endl;
    else
        cout << "Insufficient Balance!!" << endl;

}

