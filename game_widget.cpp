#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>
#include <QRectF>
#include <QPainter>
#include <qmath.h>
#include "game_widget.h"

Game_widget::Game_widget(QWidget *parent) :
    QWidget(parent),
    timer(new QTimer(this)),
    universe_size(50)
{
    timer->setInterval(100);
    main_color_ = "#000";               // Установка цвета клеток
    universe      = new Matrix<bool>(universe_size, universe_size);
    next_universe = new Matrix<bool>(universe_size, universe_size);

    // Когда время таймера истекает, генерируется новое поколение
    connect(timer, SIGNAL(timeout()), this, SLOT(new_generation()));

    this->setMinimumSize(400, 400);     // Установка минимального размера окна виджета

    //this->setMouseTracking(true);
    //(*universe)(1, 1) = (*universe)(1, 3) = true;
}

Game_widget::~Game_widget()
{
    delete universe;
    delete next_universe;
}

//------------------------------------------------------

void Game_widget::start_game()
{
//    qDebug() << "start";
    timer->start();
}

void Game_widget::stop_game()
{
//    qDebug() << "stop";
    timer->stop();
}

// Очистить поле
void Game_widget::clear()
{
    // Останавливаем игру
    stop_game();

    // Делаем все клетки "мёртвыми"
    for (int i = 0; i < universe_size; i++)
        for (int j = 0; j < universe_size; j++)
            (*universe)(i, j) = false;

    // Посылаем сигнал, что игра закончена
    game_ends(true);

    // Перерисовываем
    update();
}

//------------------------------------------------------

// Отрисовка игры
void Game_widget::paintEvent(QPaintEvent* event)
{
    QPainter p(this);   // Рисовать будем на вызывающем виджете (виджете игры)
    paint_grid(p);      // Рисуем сетку
    paint_universe(p);  // Рисуем клетки
}

// Отрисовка сетки
void Game_widget::paint_grid(QPainter& p)
{
    QRect borders(0, 0, width() - 1, height() - 1);     // Граница
    QColor grid_color = main_color_;                     // Цвет сетки
    grid_color.setAlpha(10);
    p.setPen(grid_color);

    double cell_width = (double)width() / universe_size;        // Ширина клетки
    // Рисуем вертикальные линии
    for (double k = cell_width; k < width(); k += cell_width)
        p.drawLine(k, 0, k, height());

    double cell_height = (double)height() / universe_size;      // Высота сетки
    // Рисуем горизонтальные линии
    for (int k = cell_height; k < height(); k += cell_height)
        p.drawLine(0, k, width(), k);

    p.drawRect(borders);        // Рисуем границу сетки
}

// Отрисовка клеток
void Game_widget::paint_universe(QPainter& p)
{
    double cell_width = (double)width() / universe_size;    // Ширина клетки
    double cell_height = (double)height() / universe_size;  // Высота клетки

    for (int i = 0; i < universe_size; i++)
    {
        for (int j = 0; j < universe_size; j++)
        {
            if ((*universe)(i, j) == true)      // Если клетка жива
            {
                // Координаты левого верхнего угла клетки
                qreal left = (qreal)(cell_width  * (j + 1) - cell_width);
                qreal top  = (qreal)(cell_height * (i + 1) - cell_height);

                // Рисуем клетку
                QRectF r(left, top, (qreal)cell_width,(qreal)cell_height);
                p.fillRect(r, QBrush(main_color_));
            }
        }
    }
}

//------------------------------------------------------

// Нажатие кнопки мыши
void Game_widget::mousePressEvent(QMouseEvent *event)
{
    // Отправляем сигнал, что состояние клетки изменилось
    emit environment_changed(true);

    double cell_width  = (double)width()  / universe_size;  // Ширина клетки
    double cell_height = (double)height() / universe_size;  // Высота клетки

    // Координаты клетки на поле
    int i = floor(event->y() / cell_height);
    int j = floor(event->x() / cell_width);

    /*
    if (event->button() & Qt::LeftButton)   // Если нажата левая кнопка,
        (*universe_)(i, j) = true;           // то делаем клетку "живой"
    if (event->button() & Qt::RightButton)  // Если была нажата правая кнопка,
        (*universe_)(i, j) = false;          // то делаем клетку "мёртвой"
    */

    (*universe)(i, j) = !(*universe)(i, j);

    // Перерисовываем
    update();
}

// Перемещение мыши при зажатой кнопке мыши
void Game_widget::mouseMoveEvent(QMouseEvent *event)
{
    double cell_width  = (double)width()  / universe_size;  // Ширина клетки
    double cell_height = (double)height() / universe_size;  // Высота клетки

    // Координаты клетки на поле
    int i = floor(event->y() / cell_height);
    int j = floor(event->x() / cell_width);


    // Проверяем выход за границу поля
    if (i < 0 || i > universe_size - 1 || j < 0 || j > universe_size - 1)
        return;

    if (!(*universe)(i, j))
    {
        (*universe)(i, j) = true;
        update();
    }

    /*
    if (event->button() & Qt::LeftButton)   // Если нажата левая кнопка,
    {
        (*universe_)(i, j) = true;           // то делаем клетку "живой"
        update();                           // и перерисовываем
    }
    if (event->button() & Qt::RightButton)  // Если была нажата правая кнопка,
    {
        (*universe_)(i, j) = false;          // то делаем клетку "мёртвой"
        update();                           // и перерисовываем
    }
    */
}

//---------------------------------------------------------------

// Жива ли клетка
bool Game_widget::is_alive(int k, int r)
{
    int power = 0;  // Количество "живых" соседей клетки

    // На случай, если клетка граничная
    int temp_i;
    int temp_j;

    for (int i = k - 1; i <= k + 1; i++)
    {
        temp_i = i;
        if (i == -1)
            temp_i = universe_size - 1;
        else if (i == universe_size)
            temp_i = 0;

        for (int j = r - 1; j <= r + 1; j++)
        {
            temp_j = j;
            if (j == -1)
                temp_j = universe_size - 1;
            else if (j == universe_size)
                temp_j = 0;

            if ((*universe)(temp_i, temp_j) == true &&
                    !(temp_i == k && temp_j == r))           /* Если клетка "живая" и не является исходной*/
                power++;
        }
    }


    /*for (int i = 0; i < universe_size; i++)
    {
        for (int j = 0; j < universe_size; j++)
            qDebug() << (*universe)(i, j) << " ";
        qDebug() << "\n";
    }*/



    /*
    power += (*universe)((k+1), r);
    power += (*universe)((k-1 + universe_size) % universe_size, r);
    power += (*universe)(k, (r+1) % universe_size);
    power += (*universe)(k, (r-1 + universe_size) % universe_size);
    power += (*universe)((k+1) % universe_size, (r-1 + universe_size) % universe_size);
    power += (*universe)((k-1 + universe_size) % universe_size, (r+1) % universe_size);
    power += (*universe)((k-1 + universe_size) % universe_size, (r-1 + universe_size) % universe_size);
    power += (*universe)((k+1) % universe_size, (r+1) % universe_size);
    */

    // Клетка останется живой, если у неё 2 или 3 соседа, и оживёт, если у неё ровно 3 соседа
    if ((power == 3) ||
            (((*universe)(k, r) == true) && (power == 2)))
        return true;
    return false;
}

// Обновляет поколение в universe
void Game_widget::new_generation()
{
    int not_changed = 0;      // Количество клеток, не изменивших состояние

    for (int i = 0; i < universe_size; i++)
        for (int j = 0; j < universe_size; j++)
        {
            (*next_universe)(i, j) = is_alive(i, j);            // Обновляем клетку в новом поколении
            if ((*next_universe)(i, j) == (*universe)(i, j))    // Если клетка не изменила состояние,
                not_changed++;                                  // увеличиваем счётчик
        }

    // Игра окончена, если ни одна клетка не изменила состояния
    if (not_changed == universe_size * universe_size)
    {
        // Выдаём сообщение
        QMessageBox::information(this,
                                 "Игра окончена.",
                                 "Конец игры. Ни одна клетка не изменила состояния.",
                                 QMessageBox::Ok
                                 );
        // Закачиваем игру
        stop_game();
        // Посылаем сигнал, что игра окончена
        game_ends(true);

        return;
    }

    // Следующее поколение теперь текущее
    (*universe) = (*next_universe);
    // Перерисовываем
    update();
}

//------------------------------------------------------

// Возвращает интервал между поколениями
int Game_widget::interval()
{
    return timer->interval();
}

// Установить интервал
void Game_widget::set_interval(int msec)
{
    timer->setInterval(msec);
}

// Возвращает цвет клетки
QColor Game_widget::main_color()
{
    return main_color_;
}

// Установить цвет клетки
void Game_widget::set_main_color(const QColor &color)
{
    // Устанавливаем новый цвет
    main_color_ = color;
    // Перерисовываем
    update();
}

// Возвращает размер поля
int Game_widget::cell_number()
{
    return universe_size;
}

// Установка размера поля
void Game_widget::set_cell_number(int size)
{
    // Устанавливаем новый размер
    universe_size = size;
    reset_universe();

    // Перерисовываем
    update();
}

//---------------------------------------------------------------

// Сброс поля в исходное состояние
void Game_widget::reset_universe()
{
    delete universe;
    delete next_universe;
    universe      = new Matrix<bool>(universe_size, universe_size);
    next_universe = new Matrix<bool>(universe_size, universe_size);
}
