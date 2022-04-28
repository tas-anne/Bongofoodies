#pragma once
#include"payment.h"
class Cash : public Payment
{
private:
    
public:
    Cash()
    {}
    void checkPayment(double totalAmount, double paid);
};



