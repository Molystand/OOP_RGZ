#include <QApplication>
#include <QDebug>
#include "game_widget.h"
#include "main_window.h"

int main(int argc, char *argv[])
{
    try
    {
    QApplication a(argc, argv);

    Main_window* st_game = new Standard_game();
    st_game->show();

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
