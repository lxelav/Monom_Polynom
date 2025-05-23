#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <string>
#include "Monomial.h"

using namespace std;

void Monomial::obj_init(const char* value)
{

    int size_value = strlen(value), i = 0;

    //Тут проверяем первый символ у монома и исходя от ответа выбираем как действовать дальше
    if (value[0] == '-' && isdigit(value[1]))
    {
        /*Почему минус '0' ? Потому что value[1] имеет тип чар и
        чтобы вытащить из него число, нужно вычесть аскии код нуля.*/
        coefficient = value[1] - '0';
        coefficient = -coefficient;
        i += 2;
    }
    else if (isdigit(value[0]))
    {
        coefficient = value[0] - '0';
        ++i;
    }

    //В этом цикле мы смотрим каждый символ строки и их значения раскидываем в нужные массивы.
    for (; i < size_value; ++i)
    {
        if (isalpha(value[i]))
        {
            variables.push_back(value[i]);
            if (value[i + 1] != '^') powers.push_back(1);
        }

        else if (isdigit(value[i]))
        {
            powers.push_back(value[i] - '0');
        }
    }
}

void Monomial::operator=(const Monomial& obj)
{
    //Тут просто каждое поле одного объекта присваивается полю другого объекта.
    this->coefficient = obj.coefficient;
    this->variables = obj.variables;
    this->powers = obj.powers;
}
Monomial Monomial::operator*(const Monomial& obj)
{
    Monomial temp;
    Monomial max_el, min_el;

    temp.coefficient = this->coefficient * obj.coefficient;

    //Нахождение размеров каждого из мономов и последующее сравнение, чтобы найти наибольший.
    int size_this_variables = this->variables.size();
    int size_obj_variables = obj.variables.size();
    int max_size, min_size;

    //Нахождение наибольшего монома(по количеству переменных)
    if (size_this_variables > size_obj_variables)
    {
        max_size = size_this_variables; min_size = size_obj_variables;
        max_el = *this; min_el = obj;
    }
    else
    {
        max_size = size_obj_variables; min_size = size_this_variables;
        max_el = obj; min_el = *this;
    }

    for (int i = 0; i < max_size; ++i)
    {
        bool flag = true;
        for (int j = 0; j < min_size; ++j)
        {
            if (max_el.variables[i] == min_el.variables[j])
            {
                temp.variables.push_back(max_el.variables[i]);
                temp.powers.push_back(max_el.powers[i] + min_el.powers[j]);
                flag = false;
                break;
            }
        }
        if (flag)
        {
            temp.variables.push_back(max_el.variables[i]);
            temp.powers.push_back(max_el.powers[i]);
        }
    }

    for (int i = 0; i < min_size; ++i)
    {
        bool res = count(max_el.variables.begin(), max_el.variables.end(), min_el.variables[i]) > 0;
        if (!res)
        {
            this->variables.push_back(min_el.variables[i]);
            this->powers.push_back(min_el.powers[i]);
        }
    }

    return temp;
}
Monomial Monomial::operator/(const Monomial& obj)
{
    //Создаем временную переменную, в которую будем сохранять новый результирующий моном
    Monomial temp;

    //Находим коэфф.
    temp.coefficient = this->coefficient / obj.coefficient;

    int size_this_variables = this->variables.size();
    int size_obj_variables = obj.variables.size();

    for (int i = 0; i < size_this_variables; ++i)
    {
        bool flag = true;
        for (int j = 0; j < size_obj_variables; ++j)
        {
            if (this->variables[i] == obj.variables[j])
            {
                temp.variables.push_back(this->variables[i]);
                temp.powers.push_back(this->powers[i] - obj.powers[j]);
                flag = false;
                break;
            }
        }
        if (flag)
        {
            temp.variables.push_back(this->variables[i]);
            temp.powers.push_back(this->powers[i]);
        }
    }

    for (int i = 0; i < size_obj_variables; ++i)
    {
        bool res = count(this->variables.begin(), this->variables.end(), obj.variables[i]) > 0;
        if (!res)
        {
            temp.variables.push_back(obj.variables[i]);
            temp.powers.push_back(-obj.powers[i]);
        }
    }

    return temp;
}
Monomial& Monomial::operator*=(const Monomial& obj)
{
    Monomial max_el, min_el;

    this->coefficient = this->coefficient * obj.coefficient;

    int size_this_variables = this->variables.size();
    int size_obj_variables = obj.variables.size();
    int max_size, min_size;

    if (size_this_variables > size_obj_variables)
    {
        max_size = size_this_variables; min_size = size_obj_variables;
        max_el = *this; min_el = obj;
    }
    else
    {
        max_size = size_obj_variables; min_size = size_this_variables;
        max_el = obj; min_el = *this;
    }

    this->variables.clear();
    this->powers.clear();

    for (int i = 0; i < max_size; ++i)
    {
        bool flag = true;
        for (int j = 0; j < min_size; ++j)
        {
            if (max_el.variables[i] == min_el.variables[j])
            {
                this->variables.push_back(max_el.variables[i]);
                this->powers.push_back(max_el.powers[i] + min_el.powers[j]);
                flag = false;
                break;
            }
        }

        if (flag)
        {
            this->variables.push_back(max_el.variables[i]);
            this->powers.push_back(max_el.powers[i]);
        }
    }

    for (int i = 0; i < min_size; ++i)
    {
        bool res = count(max_el.variables.begin(), max_el.variables.end(), min_el.variables[i]) > 0;
        if (!res)
        {
            this->variables.push_back(min_el.variables[i]);
            this->powers.push_back(min_el.powers[i]);
        }
    }

    return *this;
}
Monomial& Monomial::operator/=(const Monomial& obj)
{
    Monomial this_el = *this;

    this->coefficient = this->coefficient / obj.coefficient;

    int size_this_variables = this->variables.size();
    int size_obj_variables = obj.variables.size();
    int max_size, min_size;

    this->variables.clear();
    this->powers.clear();

    for (int i = 0; i < size_this_variables; ++i)
    {
        bool flag = true;
        for (int j = 0; j < size_obj_variables; ++j)
        {
            if (this_el.variables[i] == obj.variables[j])
            {
                this->variables.push_back(this_el.variables[i]);
                this->powers.push_back(this_el.powers[i] - obj.powers[j]);
                flag = false;
                break;
            }
        }

        if (flag)
        {
            this->variables.push_back(this_el.variables[i]);
            this->powers.push_back(this_el.powers[i]);
        }
    }

    for (int i = 0; i < size_obj_variables; ++i)
    {
        bool res = count(this->variables.begin(), this->variables.end(), obj.variables[i]) > 0;
        if (!res)
        {
            this->variables.push_back(obj.variables[i]);
            this->powers.push_back(-obj.powers[i]);
        }
    }

    return *this;
}
bool Monomial::operator==(const Monomial& obj)
{
    return this->coefficient == obj.coefficient && this->powers == obj.powers && this->variables == obj.variables;
}
bool Monomial::operator!=(const Monomial& obj)
{
    return !(*this == obj);
}

ostream& operator<<(ostream& os, const Monomial& obj)
{
    if (!obj.variables.empty())
    {
        if (obj.coefficient != 1) os << obj.coefficient;
        for (int i = 0; i < obj.variables.size(); ++i)
        {
            if (obj.powers[i] != 0 && obj.powers[i] != 1)
                os << obj.variables[i] << "^" << obj.powers[i];
            else if (obj.powers[i] == 1)
                os << obj.variables[i];
        }

        return os;
    }
    else
        os << obj.coefficient;

}
istream& operator>>(istream& is, Monomial& obj)
{

    char value[100];
    is >> value;

    obj.obj_init(value);

    return is;
}