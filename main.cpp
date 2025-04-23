#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include "Containers.h"

using namespace std;

enum class StColor : int
{
    Blue,
    Black,
    Yellow
};

enum class StPrice : int
{
    Expensive,
    Cheap
};

class Stationery
{
private:
    StColor Color;
    StPrice AllPrice;

protected:
    bool StWritesGood;

public:
    Stationery(StColor color, StPrice price)
    : Color(color), AllPrice(price), StWritesGood(false)
    {
        StWritesGood = static_cast<bool>(rand()%2);
    }

    virtual ~Stationery() {} //деструктор

    StColor GetColor() const {return Color;}
    bool Write() const { return StWritesGood; }
    StPrice GetPrice() const {return AllPrice;}
    virtual void Use()
    {
        if (Write())
        {
            cout << "Using a good writing stationery; ";
        }
        else
        {
            cout << "Using a bad writing stationery; ";
        }
    }

};

class Pen : public Stationery
{
public:
    Pen();
    ~Pen() {}

    void Use() override;
};

Pen::Pen() : Stationery(StColor::Blue, StPrice::Expensive)
{

}

void Pen::Use()
{
    Stationery::Use();
    cout << "The pen writes" << endl;
}

class Pencil : public Stationery
{
public:
    Pencil();
    ~Pencil() {}

    void Use() override;
};

Pencil::Pencil() : Stationery(StColor::Black, StPrice::Cheap)
{

}

void Pencil::Use()
{
    Stationery::Use();
    cout << "Pencil draws" << endl;
}

class Marker : public Stationery
{
public:
    Marker();
    ~Marker() {}

    void Use() override;
};


Marker::Marker() : Stationery(StColor::Yellow, StPrice::Expensive)
{

}

void Marker::Use()
{
    Stationery::Use();
    cout << "Marker draws" << endl;
}

enum class StationeryType : int
{
    Pen = 1,
    Pencil = 2,
    Marker = 3,
    Undefined = 0
};

Stationery *Create(StationeryType type)
{
    Stationery *st_1 = nullptr;
    if (type == StationeryType::Pen)
    {
        st_1 = new Pen;
    }
    else if (type == StationeryType::Pencil)
    {
        st_1 = new Pencil;
    }
    else if (type == StationeryType::Marker)
    {
        st_1 = new Marker;
    }
    return st_1;
}

//декоратор дл€ отбора по цвету
class StColorDecorator : public Iterator_Decorator<class Stationery*>
{
private:
    StColor TargetColor;

public:
    StColorDecorator(Iterator<Stationery*> *it, StColor color)
    : Iterator_Decorator<Stationery*>(it), TargetColor(color) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetColor() != TargetColor)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        }while(!It->IsDone() && It->GetCurrent()->GetColor() != TargetColor);
    }
};

//декоратор дл€ отбора предметов по цене
class StPriceDecorator : public Iterator_Decorator<class Stationery*>
{
private:
    StPrice TargetPrice;

public:
    StPriceDecorator(Iterator<Stationery*> *it, StPrice price)
    : Iterator_Decorator<Stationery*>(it), TargetPrice(price) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetPrice() != TargetPrice)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        }while(!It->IsDone() && It->GetCurrent()->GetPrice() != TargetPrice);
    }
};

//декоратор дл€ отбора предметов, которые хорошо пишут
class StGoodDecorator : public Iterator_Decorator<class Stationery*>
{
private:
    bool TargetGood;

public:
    StGoodDecorator(Iterator<Stationery*> *it, bool isGood)
    : Iterator_Decorator<Stationery*>(it), TargetGood(isGood) {}

     void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->Write() != TargetGood)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();
        }while(!It->IsDone() && It->GetCurrent()->Write() != TargetGood);
    }
};

void UsesAll(Iterator<Stationery*> *iterator0)
{
    for(iterator0->First(); !iterator0->IsDone(); iterator0->Next())
    {
        Stationery *current_St = iterator0->GetCurrent();
        current_St->Use();
    }
}



int main()
{
    setlocale (LC_ALL, "Russian");

    size_t N = 30;

    Array_Class<Stationery*> st_array;
    for(size_t i=0; i<N; i++)
    {
        int number = rand()%3+1;
        StationeryType st_type = static_cast<StationeryType>(number);
        Stationery *st_1 = Create(st_type);
        st_array.AddPredmet_2(st_1);
    }

    wcout << "»спользуем все предметы с помощью итератора:" << endl << endl;
    Iterator<Stationery*> *all = st_array.GetIterator();
    UsesAll(all);
    delete all;
    cout << endl << endl;

    wcout << "»спользуем хорошо пишущую канцел€рию:" << endl << endl;
    Iterator<Stationery*> *good = new StGoodDecorator(st_array.GetIterator(), true);
    UsesAll(good);
    delete good;
    cout << endl << endl;

    wcout << "»спользуем только дорогую канцел€рию:" << endl << endl;
    Iterator<Stationery*> *expensive =
    new StPriceDecorator(st_array.GetIterator(), StPrice::Expensive);
    UsesAll(expensive);
    delete expensive;
    cout << endl << endl;

    wcout << "»спользуем хорошо пишущую дорогую канцел€рию синего цвета" << endl << endl;
    Iterator<Stationery*> *goodExBlue =
    new StGoodDecorator
    (new StPriceDecorator
    (new StColorDecorator(st_array.GetIterator(), StColor::Blue),
     StPrice::Expensive),
     true);
     UsesAll(goodExBlue);
     delete goodExBlue;
     cout << endl << endl;


     list<Stationery*> st_list;
     for(size_t i=0; i<N; i++)
     {
        int number = rand()%3+1;
        StationeryType st_type = static_cast<StationeryType>(number);
        Stationery *st_1 = Create(st_type);
        st_list.push_back(st_1);
     }

     wcout << "»спользуем хорошо пишущую дорогую канцел€рию синего цвета при помощи адаптера" << endl<<endl;
     Iterator<Stationery*> *adapted = new Iterator_Adapter<std::list<Stationery*>, Stationery*>(&st_list);
     Iterator<Stationery*> *adapterGoodExBlue =
     new StGoodDecorator
     (new StPriceDecorator
     (new StColorDecorator(adapted, StColor::Blue),
     StPrice::Expensive),
     true);
     UsesAll(adapterGoodExBlue);
     delete adapterGoodExBlue;



    return 0;
}
