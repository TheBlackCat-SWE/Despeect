/**
  * @author Riccardo Damiani
  * @version 1.0 27/05/18
  */

#include <QApplication>
#include "DSMainWindow.hpp"
#include <iostream>


int main(int argc, char **argv) {
        QApplication a(argc, argv);
        DSMainWindow main_win;
        main_win.setWindowState(Qt::WindowMaximized);
        main_win.show();
        //set locale to C so that speect produce the right audio
        std::setlocale(LC_ALL, "C");
        a.exec();
}

/*
 * Stack or Heap? In general, a QObject without a parent should be created on the stack
 * or defined as an subobject of another class. !!A QObject with a parent should not be
 * on the stack because then it might get deleted twice accidentally!!. All QObjects
 * created on the heap should either have a parent, or be managed somehow by another object.
*/
