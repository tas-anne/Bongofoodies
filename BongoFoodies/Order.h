#pragma once
#ifndef ORDER_H
#define ORDER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#define NOMINMAX
#include <windows.h>
#include <SQLAPI.h>
#include<limits>

using namespace std;

class Order
{
public:
    Order();
    virtual ~Order();

    static int order_id;
    double calculate_amount(SAConnection& conn);
    void order_details(SAConnection& conn, int userid, int i, double p, string rt);
    void show_order(SAConnection& conn, string rt);

private:
    int food_id;
    int customer_id;
    int quantity;
    double amount;
    string status;
};

#endif // ORDER_H