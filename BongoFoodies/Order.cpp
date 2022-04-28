#include "Order.h"
#include "cash.h"
#include "credit.h"
#include "mobile.h"
int Order::order_id = 0;

Order::Order()
{
    //ctor
}

Order::~Order()
{
    //dtor
}

double Order::calculate_amount(SAConnection& conn)
{
    double p=0;
    SACommand select(&conn);
    try
    {

        select.setCommandText(_TSA("select sum(amount) from ord where orderID = :1"));
        select << (unsigned short)order_id;
        select.Execute();

        while (select.FetchNext())
        {
            p = select.Field(_TSA("sum(amount)")).asDouble();
        }
    }
    catch (SAException& e)
    {
        (void)e;
        //conn.Rollback();
        cout << e.ErrText().GetMultiByteChars();
        cout << "\nProblem calculating!\n";
    }
    return p;
}

void Order::order_details(SAConnection& conn, int userid, int i, double p, string rt)
{
    Sleep(100);
    system("CLS");
    cout << "\n\n\n\t\t\========== WELCOME TO BONGOFOODIES' FOOD ORDERING SYSTEM ==========" << endl;
    cout << "\n\n\t\t\t  =====START YOUR ORDER=====" << endl;
    cout << "\n" << endl;

    SACommand Count(&conn);
    Count.setCommandText(_TSA("SELECT orderID FROM ord"));
    Count.Execute();
    while (Count.FetchNext())
    {
        order_id = Count.Field(_TSA("orderID")).asUShort();
    }
    order_id++;
    //	time_t now = time(0);
    //	struct tm* date = localtime(&now);

    food_id = i;
    customer_id = userid;
    cout << "Enter quantity of your order: ";
    cin >> quantity;
    amount = p * quantity;
    status = "In progress";

    SACommand insert(&conn);
    try
    {
        insert.setCommandText(_TSA("insert into ord(OrderID, foodID, customerID, quantity, amount, status) VALUES (:1, :2, :3, :4, :5, :6)"));
        insert << (unsigned short)order_id << (unsigned short)food_id << (unsigned short)customer_id << (unsigned short)quantity << (double)amount << status.c_str();
        insert.Execute();
        conn.Commit();
    }
    catch (SAException& e)
    {
        (void)e;
        //conn.Rollback();
        cout << e.ErrText().GetMultiByteChars();
        cout << "\nProblem placing order!\n";
    }
    cout << "Recipe ordered" << endl;
    cout << "\n\tTHANK YOU FOR YOUR ORDER :) GENERATING BILL ";
    for (int a = 1; a < 8; a++) // Change 'a<?' to how many * you want
    {
        Sleep(500);
        cout << "...";
    }
    cout << "\n\t\t\t\t\t\t\t";

    system("pause");
    show_order(conn, rt);
}

void Order::show_order(SAConnection& conn, string rt)
{
    Sleep(100);
    system("CLS");

    cout << "\n\n\t\t\t  =====DISPLAYING ORDER INFO=====" << endl;

    cout << "Order ID: " << order_id << endl;
    cout << "Recipe ID: " << food_id << endl;
    cout << "Recipe title: " << rt << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Total amount (in taka): " << calculate_amount(conn) << endl;
    cout << "\nDo you want to confirm your order? (y/n): ";
    char ch;
    cin >> ch;
    if (ch == 'y')
    {
        cout << "==================";
        cout << "\n\n\n" << endl;
        cout << "Payment method:\n\t\t1.Cash on delivery\n\t\t2.Credit card\n\t\t3.mobile banking?\n\t\t>>> ";
        int choice;
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            Cash c;
            double paid, totalAmount;
            totalAmount = calculate_amount(conn);
            cout << totalAmount << endl;
            cout << "pay "<< totalAmount <<" Taka : ";
            cin >> paid;
            c.checkPayment(totalAmount, paid);
            SACommand statusChange(&conn);
            statusChange.setCommandText("UPDATE ORD SET status='Done!' WHERE ORDERID=:1");
            statusChange<< (unsigned short)order_id;
            statusChange.Execute();
            conn.Commit();
            break;
        }
        case 2:
        {
            Credit cr;
            string cardID;
            string pass;
            cout << "Enter Card ID:";
            cin >> cardID;
            cout << "Enter password:";
            cin >> pass;
            int totalAmount = calculate_amount(conn);
            string valid = cr.checkCardValidity(cardID, pass, totalAmount);
            if (valid=="another method")
            {
                show_order(conn, rt);
            }

            break;
        }
        case 3:
        {
            Mobile mo;
            string mobileno;
            string pinNo;
            cout << "Enter phone number:";
            cin >> mobileno;
            cout << "Enter pin:";
            cin >> pinNo;
            double totalAmount = calculate_amount(conn);
            string valid = mo.checkValidity(mobileno, pinNo, totalAmount);
            if (valid == "another method")
            {
                show_order(conn, rt);
            }

            break;
        }
        default:
            break;
        }
        system("pause");
    }

}