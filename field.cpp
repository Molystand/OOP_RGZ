#include "field.h"


// Возвращает нужное поле в соответствии с идентификатором
Field* Field::make_field(int choise, int size)
{
    if (choise == 1)
        return new Matr_field(size);
//    if (choise == 2)
//        return new Set_field;
}


//-----------------------------------------------------------------


Matr_field::Matr_field(int size) : field(new Matrix<bool>(size, size)) {}

Matr_field::~Matr_field()
{
    delete field;
}

//-----------------------------------------------------------------

bool Matr_field::alive(int i, int j)
{
    return (*field)(i, j);
}

void Matr_field::set_alive(int i, int j, bool alive)
{
    (*field)(i, j) = alive;
}
