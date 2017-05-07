#include <QtWidgets>
#include "game_widget.h"

int main(int argc, char *argv[])
{
    try     /* Прототип интерфейса */
    {
    QApplication a(argc, argv);
    QWidget w;
    Game_widget* game = new Game_of_life;

    // Старт/стоп/сброс
    QHBoxLayout* control_hlout = new QHBoxLayout;
    QPushButton* start         = new QPushButton("Старт");
    QPushButton* stop          = new QPushButton("Стоп");
    QPushButton* clear         = new QPushButton("Сброс");
    control_hlout->addWidget(start);
    control_hlout->addWidget(stop);
    control_hlout->addWidget(clear);

    // Сохранение/загрузка
    QHBoxLayout* file_hlout    = new QHBoxLayout;
    QPushButton* save          = new QPushButton("Сохранить");
    QPushButton* load          = new QPushButton("Загрузить");
    file_hlout->addWidget(save);
    file_hlout->addWidget(load);

    // Вертикальная компоновка настроек
    QVBoxLayout* setting_vlout = new QVBoxLayout;
    QLabel*      label1        = new QLabel("Размер поля");
    QSpinBox*    cells_num     = new QSpinBox;
    cells_num->setRange(5, 100);
    cells_num->setSuffix(" клеток");
    cells_num->setValue(game->cell_number());
    QLabel*      label2        = new QLabel("Интервал между поколениями");
    QSpinBox*    gen_interval  = new QSpinBox;
    gen_interval->setRange(50, 2000);
    gen_interval->setSuffix(" миллисекунд");
    gen_interval->setValue(game->interval());
    gen_interval->setSingleStep(10);
    QPushButton* color_button  = new QPushButton("Цвет клеток");
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


    // Сигнально-слотовые соединения
    QObject::connect(start, SIGNAL(clicked()), game, SLOT(start_game()));
    QObject::connect(stop,  SIGNAL(clicked()), game, SLOT(stop_game()));
    QObject::connect(clear, SIGNAL(clicked()), game, SLOT(clear()));

    QObject::connect(gen_interval, SIGNAL(valueChanged(int)), game, SLOT(set_interval(int)));
    QObject::connect(cells_num,    SIGNAL(valueChanged(int)), game, SLOT(set_cell_number(int)));

    // Если состояние клеток на поле менялось, изменять размер поля нельзя
    QObject::connect(game, SIGNAL(environment_changed(bool)), cells_num, SLOT(setDisabled(bool)));
    // Когда поле очищено, можно изменять размер
    QObject::connect(game, SIGNAL(game_ends(bool)), cells_num, SLOT(setEnabled(bool)));

    // Сохранение
//    QObject::connect(, SIGNAL(), , SLOT());
    // Загрузка
//    QObject::connect(, SIGNAL(), , SLOT());

    // Изменение цвета клетки
//    QObject::connect(color_button, SIGNAL(clicked()), this, SLOT());

    main_hlout->setStretchFactor(setting_vlout, 2);
    w.setLayout(main_hlout);
    w.show();

    return a.exec();
    }
    catch(Matrix<bool>::Index_error& e)
    {
        qDebug() << e.what();
    }
    catch(Matrix<bool>::Size_error& e)
    {
        qDebug() << e.what();
    }
}
