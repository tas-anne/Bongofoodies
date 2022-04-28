#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "payment.h"

//This is the latest we got
using namespace std;


class Mobile :public Payment
{
private:
    string mobileNo,pin;
public:
    string checkValidity(string mobileNo, string pin, double totalAmount);
    void checkPayment(double totalAmount, double Balance);
};



