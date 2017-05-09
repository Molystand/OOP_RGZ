#include "field.h"


// Возвращает нужное поле в соответствии с идентификатором
Field* Field::make_field(Field_ID id, int size)
{
    if (id == Matr_field_ID)
        return new Matr_field(size);
    if (id == Set_field_ID)
        return new Set_field();
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


//-----------------------------------------------------------------


Set_field::Set_field() : field(new QSet<std::pair<int, int>>) {}

Set_field::~Set_field()
{
    delete field;
}

//----------------------------------------------------------------

bool Set_field::alive(int i, int j)
{
    return field->contains(std::pair<int, int>(i, j));  // Есть ли координаты в множестве
}

void Set_field::set_alive(int i, int j, bool alive)
{
    if (alive)      // Если жива, добавляем
        *field += std::pair<int, int>(i, j);
    else            // Иначе убираем
        field->remove(std::pair<int, int>(i,j));

}

