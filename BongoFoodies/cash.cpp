#include"cash.h"
#include<iostream>
using namespace std;
   void Cash::checkPayment(double totalAmount, double paid)
    {
       if (paid == totalAmount)
       {
           cout << "Payment successful! Thanks for choosing BongoFoodies Ordering Service!\n\n";
       }
       else if (paid < totalAmount)
       {
           double p = paid;
           double tA = totalAmount;
           while (p < tA)
           {
               tA -= p;
               cout << "pay " << tA << " Taka more : ";
               cin >> p;
           }
           if (p == tA)
           {
               cout << "Payment successful! Thanks for choosing BongoFoodies Ordering Service!" << endl << endl;
           }
           else
           {
               cout << "Your bill was " << totalAmount << " Taka. You recieved change of " << p - tA << " Taka. Thanks for choosing BongoFoodies Ordering Service!" << endl << endl;
           }
       }
       else
       {
           cout << "You paid " << paid << " Taka. Your bill was " << totalAmount << " Taka. You recieved change of " << paid - totalAmount << " Taka. Thanks for choosing BongoFoodies Ordering Service!" << endl << endl;
       }
    }


