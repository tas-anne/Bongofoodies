#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#pragma once


//This is the latest we got
using namespace std;



/*Payment class and its inheritance, Order*/

class Payment
{
public:
    virtual void checkPayment(double totalAmount, double paid)=0;
};

