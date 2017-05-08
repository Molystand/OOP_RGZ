#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QColor>
#include <QWidget>
#include "field.h"


class Game_widget : public QWidget      /* Реализация */
{
    Q_OBJECT
public:
    explicit Game_widget(QWidget *parent = 0) : QWidget(parent) {}

    virtual ~Game_widget() {}

signals:
    // Отправляется при постановке клетки на поле, чтобы заблокировать изменение размера поля
    /*virtual*/ void environment_changed(bool ok);
    // Отправляется, когда игра окончена, чтобы разблокировать изменение размера поля
    /*virtual*/ void game_ends(bool ok);

public slots:
    virtual void start_game() = 0;
    virtual void stop_game()  = 0;
    virtual void clear()      = 0;                              // Очистить поле

    virtual int  interval()             = 0;
    virtual void set_interval(int msec) = 0;

    virtual QColor main_color()                        = 0;     // Возвращает цвет клетки
    virtual void   set_main_color(const QColor& color) = 0;     // Установка размера клетки

    virtual int  cell_number()             = 0;                 // Возвращает размер поля
    virtual void set_cell_number(int size) = 0;                 // Установка размера поля

    //QString dump();     // Для сохранения
    //void set_dump();    // Для загрузки

protected:
    virtual void paintEvent(QPaintEvent* event)      = 0;       // Отрисовка окна
    virtual void mousePressEvent(QMouseEvent *event) = 0;       // Нажатие кнопки мыши
    virtual void mouseMoveEvent(QMouseEvent *event)  = 0;       // Перемещение мыши при зажатой кнопке мыши

private slots:
    /*
    void paint_grid(QPainter& p);       // Отрисовка сетки
    void paint_universe(QPainter& p);   // Отрисовка клеток
    void new_generation();              // Обновляет поколение в universe
    */

private:
    /*
    QColor main_color_;             // Цвет клетки
    QTimer* timer;                  // Время между поколениями

    int universe_size;              // Размер поля
    Matrix<bool>* universe;         // Текущее поколение
    Matrix<bool>* next_universe;    // Следующее поколение

    bool is_alive(int k, int r);    // Жива ли клетка с координатами k,r

    void reset_universe();          // Сброс поля в исходное состояние
    */
};


//-------------------------------------------------------------------------------------------


class Game_of_life :/* public QWidget, */public Game_widget     /* Конкретная реализация */
{
    Q_OBJECT
public:
    explicit Game_of_life(QWidget *parent = 0);
    ~Game_of_life();

signals:
//    // Отправляется при постановке клетки на поле, чтобы заблокировать изменение размера поля
//    void environment_changed(bool ok);
//    // Отправляется, когда игра окончена, чтобы разблокировать изменение размера поля
//    void game_ends(bool ok);

public slots:
    void start_game();
    void stop_game();
    void clear();       // Очистить поле

    int  interval();
    void set_interval(int msec);

    QColor main_color();                            // Возвращает цвет клетки
    void   set_main_color(const QColor& color);     // Установка размера клетки

    int  cell_number();                             // Возвращает размер поля
    void set_cell_number(int size);                 // Установка размера поля

    //QString dump();     // Для сохранения
    //void set_dump();    // Для загрузки

protected:
    void paintEvent(QPaintEvent* event);            // Отрисовка окна
    void mousePressEvent(QMouseEvent *event);       // Нажатие кнопки мыши
    void mouseMoveEvent(QMouseEvent *event);        // Перемещение мыши при зажатой кнопке мыши

private slots:
    void paint_grid(QPainter& p);                   // Отрисовка сетки
    void paint_universe(QPainter& p);               // Отрисовка клеток
    void new_generation();                          // Обновляет поколение в universe

private:
    QColor  main_color_;            // Цвет клетки
    QTimer* timer;                  // Время между поколениями

    int universe_size;              // Размер поля
    Field* universe;                // Текущее поколение
    Field* next_universe;           // Следующее поколение

    bool is_alive(int k, int r);    // Будет ли жива клетка с координатами k,r в следующем поколении

    void reset_universe();          // Сброс поля в исходное состояние
};


#endif // GAME_WIDGET_H
