#include"payment.h"
class Credit :public Payment
{
public:
    string checkCardValidity(string cardID, string pw, double totalAmount);
    void checkPayment(double totalAmount, double paid);
};


