#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <string>

using namespace std;


class Monomial
{
    //Эти поля яляются указателями на предыдущий и следующий элементы двусв. списка
    Monomial* next = nullptr;
    Monomial* prev = nullptr;

    double coefficient = 1;
    vector<int> powers; //Служит для хранения степеней
    vector<char> variables; //Служит для хранения переменных

    //Функция, которая используется для инициализации(заполнения объекта)
    void obj_init(const char* value);

public:
    Monomial() = default;

    //Конструктор вида(коеффициент, степени, переменные)
    Monomial(double coeff, const vector<int>& powers, const vector<char>& variables)
    {
        coefficient = coeff;
        this->powers = powers;
        this->variables = variables;
    }

    //Контруктор вида(char* “5*x^2y^3z”)
    Monomial(const char* value)
    {
        obj_init(value);
    }

    //Деструктор
    ~Monomial() {}

    //Реализация геттеров/сеттеров закрытых членов класса
    double GetCoefficient()
    {
        return coefficient;
    }
    void SetCoefficient(double value)
    {
        this->coefficient = value;
    }
    vector<int>& GetPowers()
    {
        return powers;
    }
    void SetPowers(vector<int>& v)
    {
        this->powers = v;
    }
    vector<char>& GetVariables()
    {
        return variables;
    }
    Monomial& GetNext() const
    {
        return *next;
    }
    void SetNext(Monomial& obj)
    {
        next = &obj;
    }
    Monomial& GetPrev() const
    {
        return *prev;
    }
    void SetPrev(Monomial& obj)
    {
        prev = &obj;
    }

    //Реалиация операторов
    void operator=(const Monomial& obj);

    Monomial operator*(const Monomial& obj);

    Monomial operator/(const Monomial& obj);

    Monomial& operator*=(const Monomial& obj);

    Monomial& operator/=(const Monomial& obj);

    bool operator==(const Monomial& obj);

    bool operator!=(const Monomial& obj);


    friend ostream& operator<<(ostream& os, const Monomial& obj);
    friend istream& operator>>(istream& is, Monomial& obj);


};
