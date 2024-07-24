#pragma execution_character_set("utf-8")

#include "PasswordManager.h"
#include <QtWidgets/QApplication>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PasswordManager w;
    w.show();
    return a.exec();
}
