#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QColor>
#include <QColorDialog>
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
    setting_vlout->addStretch(10);

    // Горизонтальная главная компоновка
    QHBoxLayout* main_hlout    = new QHBoxLayout;
    main_hlout->addWidget(game);
    main_hlout->addLayout(setting_vlout);
    main_hlout->setStretchFactor(setting_vlout, 2);

    this->setLayout(main_hlout);        // Выставляем компоновку для создаваемого виджета


    // Сигнально-слотовые соединения
    connect(start, SIGNAL(clicked()), game, SLOT(start_game()));
    connect(stop,  SIGNAL(clicked()), game, SLOT(stop_game()));
    connect(clear, SIGNAL(clicked()), game, SLOT(clear()));

    connect(gen_interval, SIGNAL(valueChanged(int)), game, SLOT(set_interval(int)));
    connect(cells_num,    SIGNAL(valueChanged(int)), game, SLOT(set_cell_number(int)));

    // Если состояние клеток на поле менялось, изменять размер поля нельзя
    connect(game, SIGNAL(environment_changed(bool)), cells_num, SLOT(setDisabled(bool)));
    // Когда поле очищено, можно изменять размер
    connect(game, SIGNAL(game_ends(bool)), cells_num, SLOT(setEnabled(bool)));

    // Сохранение
//    QObject::connect(, SIGNAL(), , SLOT());
    // Загрузка
//    QObject::connect(, SIGNAL(), , SLOT());

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


Standard_game::Standard_game(QWidget* parent) : Main_window(new Game_of_life(), parent) {}
