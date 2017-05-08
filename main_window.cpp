#include "main_window.h"
#include "game_widget.h"

#define STDGAME

Main_window::Main_window(Game_widget* game_widget, QWidget* parent) :
    game(game_widget), QWidget(parent)
{

}

Main_window::~Main_window()
{
    delete game;
}

//----------------------------------------------------------------------

Standard_game::Standard_game(QWidget* parent) :
    Main_window(
//        #ifdef STDGAME
        new Game_of_life(),
//        #endif
        parent) {}
