#ifndef CONTAINERS_H_INCLUDED
#define CONTAINERS_H_INCLUDED

template<typename Type>
class Iterator
{
protected:
    Iterator() {}

public:
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual Type GetCurrent() const = 0;
    virtual ~Iterator() {}
};

template<typename Type>
class Stack_Iterator : public Iterator<Type>
{
private:
    Type *St_Container;
    size_t Pol;
    size_t Size;

public:
    Stack_Iterator(Type *container, size_t size) : St_Container(container), Pol(0), Size(size) {}

    void First() override { Pol=0; }
    void Next() override { Pol++; }
    bool IsDone() const override { return (Pol >= Size); }
    Type GetCurrent() const override { return St_Container[Pol]; }
};

const size_t Max_Size = 100;

template <typename Type>
class Stack_Class
{
private:
    Type Predmet[Max_Size];
    size_t Verh;

public:


    void AddPredmet (Type newPredmet) { Predmet[Verh++] = newPredmet; } //добавление предметов в стек
    Type Pop() { return Predmet[--Verh]; }
    size_t SizeStack() const { return Verh; }
    Type GetPredmet(size_t id) const { return Predmet[id]; }
    Stack_Class() : Verh(0) {} //конструктор

    class Iterator<Type> *GetIterator()
    {
        return new Stack_Iterator<Type>(Predmet, Verh);
    };

};

template<typename Type>
class Array_Iterator : public Iterator<Type>
{
private:
    const Type *Ar_Container;
    size_t Pol;
    size_t Size;

public:
    Array_Iterator(Type *container, size_t size) : Ar_Container(container), Pol(0), Size(size) {}

    void First() override { Pol=0; }
    void Next() override { Pol++; }
    bool IsDone() const override { return (Pol >= Size); }
    Type GetCurrent() const override { return Ar_Container[Pol]; }
};

//массив
template <typename Type>
class Array_Class
{
private:
    Type Predmet[Max_Size];
    size_t Array_Size;

public:
    void AddPredmet_2(Type newPredmet) { Predmet[Array_Size++] = newPredmet; }
    size_t Size() const { return Array_Size; }
    Type GetPredmet_2(size_t id) const { return Predmet[id]; }
    Type operator[](size_t id) const { return Predmet[id]; }

    class Iterator<Type> *GetIterator()
    {
        return new Array_Iterator<Type>(Predmet, Array_Size);
    };

    Array_Class() : Array_Size(0) {}
};

//пустой декоратор
template<typename Type>
class Iterator_Decorator : public Iterator<Type>
{
protected:
    Iterator<Type> *It;

public:
    Iterator_Decorator(Iterator<Type> *it) : It(it) {}
    virtual ~Iterator_Decorator() { delete It; }
    virtual void First() { It->First(); }
    virtual void Next() { It->Next(); }
    virtual bool IsDone() const { return It->IsDone(); }
    virtual Type GetCurrent() const { return It->GetCurrent(); }
};

//адаптер
template<typename Container_Type, typename Predmet_Type>
class Iterator_Adapter : public Iterator<Predmet_Type>
{
protected:
    Container_Type *Container;
    typename Container_Type::const_iterator It;

public:
    Iterator_Adapter(Container_Type *container) : Container(container)
    {
        It = Container->begin();
    }

    virtual ~Iterator_Adapter() {}
    virtual void First() { It = Container->begin(); }
    virtual void Next() { It++; }
    virtual bool IsDone() const { return (It == Container->end()); }
    virtual Predmet_Type GetCurrent() const { return *It; }
};
#endif // PATTERN_H_INCLUDED

