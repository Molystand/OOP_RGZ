#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>

class Game_widget;


class Main_window : public QWidget      /* Абстракция */
{
    Q_OBJECT
public:
    explicit Main_window(Game_widget* game_widget, QWidget* parent = 0);
    virtual ~Main_window();

signals:

public slots:
    void select_cell_color();

protected:
    Game_widget* game;          // Игровое поле

    QSpinBox*    cells_num;     // Размер поля
    QSpinBox*    gen_interval;  // Интервал между поколениями
    QPushButton* color_button;  // Цвет клетки
};


//-----------------------------------------------------------------------------


class Standard_game : public Main_window    /* Конкретная абстракция */
{
    Q_OBJECT
public:
    explicit Standard_game(QWidget* parent = 0);
};


#endif // MAIN_WINDOW_H
