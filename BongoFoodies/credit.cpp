#include"credit.h"
string Credit::checkCardValidity(string cardID, string pw, double totalAmount)
    {
        /*The card info will contain id no., password, balance(Charge)*/
        //cout << "hiii, I'm in" << endl;
        ifstream check("creditCheck.txt");
        string ID;
        string Pass;
        string line;
        double Balance;
        while (!check.eof())
        {
            //cout << "I'm in 1" << endl;
            getline(check, ID);
            
            if(cardID==ID)
            {
                getline(check, Pass);
                if (Pass == pw)
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
                    cout << "Enter Card ID:";
                    cin >> cardID;
                    cout << "Enter password:";
                    cin >> pw;
                    return checkCardValidity(cardID, pw, totalAmount);
                }
            }
            getline(check, line);
            getline(check, line);
        }
        cout << "No acount found with this number!" << endl << endl;
        cout << "Enter Card ID:";
        cin >> cardID;
        cout << "Enter password:";
        cin >> pw;
        return checkCardValidity(cardID, pw, totalAmount);
    }


    void Credit::checkPayment(double totalAmount, double balance)
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