#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QColor>
#include <QColorDialog>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include "main_window.h"
#include "game_widget.h"

Main_window::Main_window(Game_widget* game_widget, QWidget* parent) :
    game(game_widget), QWidget(parent)
{
    // Старт/стоп/сброс
    QHBoxLayout* control_hlout = new QHBoxLayout;
    QPushButton* start         = new QPushButton("Старт", this);
    QPushButton* stop          = new QPushButton("Стоп" , this);
    QPushButton* clear         = new QPushButton("Сброс", this);
    control_hlout->addWidget(start);
    control_hlout->addWidget(stop);
    control_hlout->addWidget(clear);

    // Сохранение/загрузка
    QHBoxLayout* file_hlout    = new QHBoxLayout;
    QPushButton* save          = new QPushButton("Сохранить", this);
    QPushButton* load          = new QPushButton("Загрузить", this);
    file_hlout->addWidget(save);
    file_hlout->addWidget(load);

    // Настройки
    QLabel* label1 = new QLabel("Размер поля", this);
    cells_num      = new QSpinBox;
    cells_num->setRange(5, 100);
    cells_num->setSuffix(" кл");
    cells_num->setValue(game->cell_number());
    QLabel* label2 = new QLabel("Интервал между поколениями", this);
    gen_interval   = new QSpinBox;
    gen_interval->setRange(50, 2000);
    gen_interval->setSuffix(" мсек");
    gen_interval->setValue(game->interval());
    gen_interval->setSingleStep(10);

    // Цвет клеток
    color_button = new QPushButton("Цвет клеток");
    QPixmap color_icon(16, 16);
    color_icon.fill(game->main_color());
    color_button->setIcon(QIcon(color_icon));

    // Вертикальная компоновка настроек
    QVBoxLayout* setting_vlout = new QVBoxLayout;
    setting_vlout->addLayout(control_hlout);
    setting_vlout->addWidget(label1);
    setting_vlout->addWidget(cells_num);
    setting_vlout->addWidget(label2);
    setting_vlout->addWidget(gen_interval);
    setting_vlout->addLayout(file_hlout);
    setting_vlout->addWidget(color_button);
    optional_vlout = new QVBoxLayout;       // Дополнительная компоновка
    setting_vlout->addLayout(optional_vlout);
    setting_vlout->addStretch(1);

    // Горизонтальная главная компоновка
    QHBoxLayout* main_hlout    = new QHBoxLayout;
    main_hlout->addWidget(game);
    main_hlout->addLayout(setting_vlout);
    main_hlout->setStretchFactor(game, 9);
    main_hlout->setStretchFactor(setting_vlout, 1);

    this->setLayout(main_hlout);        // Выставляем компоновку для создаваемого виджета


    // Сигнально-слотовые соединения

    //Старт/стоп/сброс
    connect(start, SIGNAL(clicked()), game, SLOT(start_game()));
    connect(stop,  SIGNAL(clicked()), game, SLOT(stop_game()));
    connect(clear, SIGNAL(clicked()), game, SLOT(clear()));

    // Интервал, размер клеток
    connect(gen_interval, SIGNAL(valueChanged(int)), game, SLOT(set_interval(int)));
    connect(cells_num,    SIGNAL(valueChanged(int)), game, SLOT(set_cell_number(int)));

    // Если состояние клеток на поле менялось, изменять размер поля нельзя
    connect(game, SIGNAL(environment_changed(bool)), cells_num, SLOT(setDisabled(bool)));
    // Когда поле очищено, можно изменять размер
    connect(game, SIGNAL(game_ends(bool)), cells_num, SLOT(setEnabled(bool)));

    // Сохранение/загрузка
    QObject::connect(save, SIGNAL(clicked(bool)), this, SLOT(save_game()));
    QObject::connect(load, SIGNAL(clicked(bool)), this, SLOT(load_game()));

    // Изменение цвета клетки
    connect(color_button, SIGNAL(clicked()), this, SLOT(select_cell_color()));
}

Main_window::~Main_window()
{
    delete game;
    delete cells_num;
    delete gen_interval;
    delete color_button;
}

//------------------------------------------------------------------------------

void Main_window::select_cell_color()
{
    // Выбираем цвет
    QColor color = QColorDialog::getColor(game->main_color(), this, "Выберите цвет клеток");
    if (!color.isValid())
        return;

    // Устанавливаем цвет на поле
    game->set_main_color(color);

    // Изменяем иконку кнопки
    QPixmap color_icon(16, 16);
    color_icon.fill(game->main_color());
    color_button->setIcon(QIcon(color_icon));
}


//------------------------------------------------------------------------------


Standard_game::Standard_game(QWidget* parent) : Main_window(new Game_of_life(), parent)
{
    /*
    QPushButton* but = new QPushButton("Рандом", this);
    optional_vlout->addWidget(but);
    */
}


// Сохранение
void Standard_game::save_game()
{
    // Останавливаем игру
    game->stop_game();

    // Выбираем и открываем файл для сохранения
    QString file_name = QFileDialog::getSaveFileName(this, "Сохрание текущей игры",
                                           QDir::homePath(), "Файл игры \"жизнь\" *.lif (*.lif)");
    if (file_name.length() < 1)
        return;
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    // Записываем сведения в файл
    QString str = QString::number(game->cell_number()) + "\n";
    file.write(str.toUtf8());
    file.write(game->dump().toUtf8());
    QColor color = game->main_color();
    QString buf = QString::number(color.red())   + " " +
                  QString::number(color.green()) + " " +
                  QString::number(color.blue())  + "\n";
    file.write(buf.toUtf8());
    buf.clear();
    buf = QString::number(game->interval()) + "\n";
    file.write(buf.toUtf8());

    // Закрываем файл после сохранения
    file.close();
}

// Загрузка
void Standard_game::load_game()
{
    // Останавливаем игру
    game->stop_game();

    // Выбираем и открываем файл для загрузки
    QString file_name = QFileDialog::getOpenFileName(this, "Открыть сохранённую игру",
                                                     QDir::homePath(), "Файл игры \"жизнь\" (*lif)");
    if (file_name.length() < 1)
        return;
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
        return;

    // Считываем сведения из файла
    QTextStream in(&file);
    int value;

    in >> value;
    cells_num->setValue(value);

    game->set_cell_number(value);
    QString dump = "";
    for (int i = 0; i != value; i++)
    {
        QString temp;
        in >> temp;
        dump.append(temp + "\n");
    }
    game->set_dump(dump);

    int r, g, b;    // Цвет
    in >> r >> g >> b;
    QColor color(r, g, b);
    game->set_main_color(color);
    // Меняем иконку
    QPixmap color_icon(16, 16);
    color_icon.fill(color);
    color_button->setIcon(QIcon(color_icon));

    int interval;
    in >> interval;
    gen_interval->setValue(interval);
    game->set_interval(interval);
}

