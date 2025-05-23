#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <string>
#include "Monomial.h"
#include "Polynomial.h"

using namespace std;

void Polynomial::push_back(Monomial& obj)
{
    if (head == nullptr && tail == nullptr)
    {
        head = tail = &obj;
    }
    else if (head == tail)
    {
        head->SetNext(obj);
        obj.SetPrev(*tail);
        tail = &obj;
    }
    else
    {
        tail->SetNext(obj);
        obj.SetPrev(*tail);
        tail = &obj;
    }
}
void Polynomial::obj_init(const char* value)
{
    int size_value = strlen(value), j = 0;
    char monomial[25]; //Используется для хранения монома

    for (int i = 0; i < size_value; ++i)
    {
        //Если был найден + || -, зн, мы полностью вытащили моном
        if (value[i] == '+' || value[i] == '-')
        {
            if (i == 0 && value[i] == '-')
            {
                monomial[i] = value[i];
                continue;
            }

            operations.push_back(value[i]);

            monomial[j] = '\0';
            temp = new Monomial(monomial);
            if (operations.size() > 1 && operations[operations.size() - 2] == '-') temp->SetCoefficient(-temp->GetCoefficient());
            push_back(*temp);
            j = 0;
        }
        else
        {
            monomial[j] = value[i];
            ++j;
            if (i + 1 == size_value)
            {
                monomial[j] = '\0';
                temp = new Monomial(monomial);
                if (operations.size() > 1 && operations[operations.size() - 1] == '-') temp->SetCoefficient(-temp->GetCoefficient());
                push_back(*temp);
            }
        }
    }
    temp = nullptr;
}

bool Polynomial::operator==(const Polynomial& obj)
{
    if (this->operations.size() != obj.operations.size() && this->operations != obj.operations) return false;
    else
    {
        Monomial* p1 = this->head, * p2 = obj.head;
        while (p1 != nullptr)
        {
            if (*p1 != *p2)
            {
                return false;
            }
            p1 = &p1->GetNext(); p2 = &p2->GetNext();
        }
    }

    return true;
}
bool Polynomial::operator!=(const Polynomial& obj)
{
    return !(*this == obj);
}
Polynomial& Polynomial::operator=(const Polynomial& obj)
{

    if (*this == obj)
    {
        return *this;
    }
    else
    {
        Monomial* p = this->head;
        this->clear_s(p);

        this->operations = obj.operations;
        this->head = obj.head;
        this->tail = obj.tail;
    }

    return *this;
}
Polynomial& Polynomial::operator+=(const Polynomial& obj)
{
    int max_size_polynomial, min_size_polynomial;
    const Polynomial* max_p, * min_p;

    if (this->operations.size() > obj.operations.size())
    {
        max_size_polynomial = this->operations.size(); min_size_polynomial = obj.operations.size();
        max_p = this; min_p = &obj;
    }
    else
    {
        max_size_polynomial = obj.operations.size(); min_size_polynomial = this->operations.size();
        max_p = &obj; min_p = this;
    }

    temp_p = new Polynomial;
    Monomial* p_max = max_p->head, * p_min = min_p->head, * temp_m;
    while (p_max != nullptr)
    {
        double res_coeff = p_max->GetCoefficient();

        while (p_min != nullptr)
        {
            if (p_max->GetVariables() == p_min->GetVariables() && p_max->GetPowers() == p_min->GetPowers())
            {
                res_coeff += p_min->GetCoefficient();
            }
            p_min = &p_min->GetNext();

        }
        temp_m = new Monomial(res_coeff, p_max->GetPowers(), p_max->GetVariables());
        temp_p->push_back(*temp_m);

        p_max = &p_max->GetNext();
        p_min = min_p->head;
    }

    temp_p->operations = max_p->operations;
    *this = *temp_p;
    temp_p = nullptr;
    return *this;
}
Polynomial& Polynomial::operator-=(const Polynomial& obj)
{
    int max_size_polynomial, min_size_polynomial;
    const Polynomial* max_p, * min_p;

    if (this->operations.size() > obj.operations.size())
    {
        max_size_polynomial = this->operations.size(); min_size_polynomial = obj.operations.size();
        max_p = this; min_p = &obj;
    }
    else
    {
        max_size_polynomial = obj.operations.size(); min_size_polynomial = this->operations.size();
        max_p = &obj; min_p = this;
    }

    temp_p = new Polynomial;
    Monomial* p_max = max_p->head, * p_min = min_p->head, * temp_m;
    while (p_max != nullptr)
    {
        double res_coeff = p_max->GetCoefficient();

        while (p_min != nullptr)
        {
            if (p_max->GetVariables() == p_min->GetVariables() && p_max->GetPowers() == p_min->GetPowers())
            {
                res_coeff -= p_min->GetCoefficient();
            }
            p_min = &p_min->GetNext();

        }
        temp_m = new Monomial(res_coeff, p_max->GetPowers(), p_max->GetVariables());
        temp_p->push_back(*temp_m);

        p_max = &p_max->GetNext();
        p_min = min_p->head;
    }

    temp_p->operations = max_p->operations;
    *this = *temp_p;
    temp_p = nullptr;
    return *this;
}
Polynomial& Polynomial::operator*=(const Polynomial& obj)
{
    int max_size_polynomial, min_size_polynomial;
    const Polynomial* max_p, * min_p;

    if (this->operations.size() > obj.operations.size())
    {
        max_size_polynomial = this->operations.size(); min_size_polynomial = obj.operations.size();
        max_p = this; min_p = &obj;
    }
    else
    {
        max_size_polynomial = obj.operations.size(); min_size_polynomial = this->operations.size();
        max_p = &obj; min_p = this;
    }

    temp_p = new Polynomial;
    Monomial* p_max = max_p->head, * p_min = min_p->head, * temp_m = nullptr;
    bool flag = false;
    while (p_max != nullptr)
    {
        double res_coeff = p_max->GetCoefficient();

        while (p_min != nullptr)
        {

            temp_m = new Monomial(*p_max * *p_min);
            temp_p->push_back(*temp_m);
            p_min = &p_min->GetNext();

        }

        if (flag)
        {
            temp_m = new Monomial(res_coeff, p_max->GetPowers(), p_max->GetVariables());
            temp_p->push_back(*temp_m);
        }

        p_max = &p_max->GetNext();
        p_min = min_p->head;
    }

    temp_p->operations = max_p->operations;
    *this = *temp_p;
    temp_p = nullptr;
    return *this;
}


ostream& operator<<(ostream& os, const Polynomial& obj)
{
    Monomial* p = obj.head;
    int counter = 0;
    while (p != nullptr)
    {
        if (counter > 0)
        {
            if (p->GetCoefficient() != 0 && p->GetCoefficient() > 0)
                os << " + " << *p;
            else if (p->GetCoefficient() < 0)
                os << " " << *p;
        }
        else
            os << *p;

        p = &p->GetNext();
        ++counter;
    }


    return os;
}
istream& operator>>(istream& is, Polynomial& obj)
{
    char value[100];
    is >> value;

    obj.obj_init(value);

    return is;
}