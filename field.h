#ifndef FIELD_H
#define FIELD_H

#include "matrix.h"


enum Field_ID { Matr_field_ID, Set_field_ID };

class Field
{
public:
    /* Фабричный метод */
    static Field* make_field(Field_ID id, int size);

    virtual bool alive(int i, int j) = 0;                   // Жива ли клетка с кординатами i,j
    virtual void set_alive(int i, int j, bool alive) = 0;   // Установить для клетки с координатами i,j состояние alive
};


//--------------------------------------------------------------------------------------------------


class Matr_field : public Field
{
public:
    bool alive(int i, int j);
    void set_alive(int i, int j, bool alive);

    explicit Matr_field(int size);
    ~Matr_field();
private:
    Matrix<bool>* field;    // Поле
};


#endif // FIELD_H
