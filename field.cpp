#include "field.h"


// Возвращает нужное поле в соответствии с идентификатором
Field* Field::make_field(Field_ID id, int size)
{
    if (id == Matr_field_ID)
        return new Matr_field(size);
//    if (id == Set_field_ID)
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
