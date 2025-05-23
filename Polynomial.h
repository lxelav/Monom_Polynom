#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <string>
#include "Monomial.h"

using namespace std;

class Polynomial
{
    vector<char> operations; //Массив, в котором хранятся знаки мономов
    Monomial* head = nullptr, * tail = nullptr; //Указатели на начало и конец двусв. списка

    //Временные указатели для создания мономов и полиномов
    Monomial* temp;
    Polynomial* temp_p;

    //Функция, которая добавляет объект(моном) в конец двусв. списка(полином)
    void push_back(Monomial& obj);

    //Фукнция, которая чистит двусв. список(удаляет элементы)
    void clear_s(Monomial* p)
    {
        while (p != nullptr)
        {
            Monomial* p_next = &p->GetNext();
            delete p;

            p = p_next;
        }
    }

    //Функция, которая используется для инициализации(заполнения объекта)(ей передается целый полином)
    void obj_init(const char* value);

public:
    Polynomial() = default;

    //Конструктор, которому передается целый полином
    Polynomial(const char* value)
    {
        this->obj_init(value);
    }

    //Конструктор копирования
    Polynomial(const Polynomial& obj1)
    {
        //Тут он копирует массив операций из одного объекта в другой
        this->operations = obj1.operations;

        //Ниже идет копирование двусв. списка
        Monomial* p = obj1.head; //Указывает на начало дв. списка
        Monomial* monom; //Переменная для хранения монома
        while (p != nullptr)
        {
            monom = new Monomial(p->GetCoefficient(), p->GetPowers(), p->GetVariables());
            this->push_back(*monom);

            p = &p->GetNext();
        }


    }

    //Деструктор
    ~Polynomial()
    {
        Monomial* p = head;
        while (p != nullptr)
        {
            Monomial* p_next = &p->GetNext();
            delete p;

            p = p_next;
        }

        delete temp;
        delete temp_p;
    }

    //Реализация операций
    bool operator==(const Polynomial& obj);
    bool operator!=(const Polynomial& obj);
    Polynomial& operator=(const Polynomial& obj);
    Polynomial& operator+=(const Polynomial& obj);
    Polynomial& operator-=(const Polynomial& obj);
    Polynomial& operator*=(const Polynomial& obj);
    friend ostream& operator<<(ostream& os, const Polynomial& obj);
    friend istream& operator>>(istream& is, Polynomial& obj);

};