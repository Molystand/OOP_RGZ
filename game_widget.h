#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QColor>
#include <QWidget>
#include "matrix.h"

//class Game_widget : public QWidget
//{
//    Q_OBJECT
//public:
//    explicit Game_widget(QWidget *parent = 0);
//    virtual ~Game_widget();

//signals:
//    // Отправляется при постановке клетки на поле, чтобы заблокировать изменение размера поля
//    void environment_changed(bool ok);
//    // Отправляется, когда игра окончена, чтобы разблокировать изменение размера поля
//    void game_ends(bool ok);

//public slots:
//    virtual void start_game();
//    virtual void stop_game();
//    virtual void clear();       // Очистить поле

//    virtual int interval();
//    virtual void set_interval(int msec);

//    virtual QColor main_color();                        // Возвращает цвет клетки
//    virtual void set_main_color(const QColor& color);   // Установка размера клетки

//    virtual int cell_number();                  // Возвращает размер поля
//    virtual void set_cell_number(int size);     // Установка размера поля

//    //QString dump();     // Для сохранения
//    //void set_dump();    // Для загрузки

//protected:
//    virtual void paintEvent(QPaintEvent* event);        // Отрисовка окна
//    virtual void mousePressEvent(QMouseEvent *event);   // Нажатие кнопки мыши
//    virtual void mouseMoveEvent(QMouseEvent *event);    // Перемещение мыши при зажатой кнопке мыши

//private slots:
//    /*
//    void paint_grid(QPainter& p);       // Отрисовка сетки
//    void paint_universe(QPainter& p);   // Отрисовка клеток
//    void new_generation();              // Обновляет поколение в universe
//    */

//private:
//    /*
//    QColor main_color_;             // Цвет клетки
//    QTimer* timer;                  // Время между поколениями

//    int universe_size;              // Размер поля
//    Matrix<bool>* universe;         // Текущее поколение
//    Matrix<bool>* next_universe;    // Следующее поколение

//    bool is_alive(int k, int r);    // Жива ли клетка с координатами k,r

//    void reset_universe();          // Сброс поля в исходное состояние
//    */
//};








class Game_widget : public QWidget/*public Game_widget*/
{
    Q_OBJECT
public:
    explicit Game_widget(QWidget *parent = 0);
    ~Game_widget();

signals:
    // Отправляется при постановке клетки на поле, чтобы заблокировать изменение размера поля
    void environment_changed(bool ok);
    // Отправляется, когда игра окончена, чтобы разблокировать изменение размера поля
    void game_ends(bool ok);

public slots:
    void start_game();
    void stop_game();
    void clear();       // Очистить поле

    int interval();
    void set_interval(int msec);

    QColor main_color();                        // Возвращает цвет клетки
    void set_main_color(const QColor& color);   // Установка размера клетки

    int cell_number();                  // Возвращает размер поля
    void set_cell_number(int size);     // Установка размера поля

    //QString dump();     // Для сохранения
    //void set_dump();    // Для загрузки

protected:
    void paintEvent(QPaintEvent* event);        // Отрисовка окна
    void mousePressEvent(QMouseEvent *event);   // Нажатие кнопки мыши
    void mouseMoveEvent(QMouseEvent *event);    // Перемещение мыши при зажатой кнопке мыши

private slots:
    void paint_grid(QPainter& p);       // Отрисовка сетки
    void paint_universe(QPainter& p);   // Отрисовка клеток
    void new_generation();              // Обновляет поколение в universe

private:
    QColor main_color_;             // Цвет клетки
    QTimer* timer;                  // Время между поколениями

    int universe_size;              // Размер поля
    Matrix<bool>* universe;         // Текущее поколение
    Matrix<bool>* next_universe;    // Следующее поколение

    bool is_alive(int k, int r);    // Жива ли клетка с координатами k,r

    void reset_universe();          // Сброс поля в исходное состояние
};

#endif // GAME_WIDGET_H
