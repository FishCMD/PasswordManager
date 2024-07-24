#pragma once

#include <QtWidgets>
#include <QClipboard>
#include <QFileDialog>
#include <QString>
#include <QStandardItemModel>
#include <QtWidgets/QMainWindow>
#include "ui_PasswordManager.h"
#include <QInputDialog>

class PasswordManager : public QMainWindow
{
    Q_OBJECT

public:
    PasswordManager(QWidget* parent = nullptr);
    ~PasswordManager();
    void closeEvent(QCloseEvent* event);

private:
    enum STATE {
        none,
        newFile,
        openFile,
        unlocked
    } state;

private slots:
    void on_select_file();
    void on_new_file();
    void on_save_file();
    void on_save_as_file();
    void on_show_password();
    void on_hide_password();
    void on_confirm_password();
    void add_data(QString platform, QString username, QString password);
    void on_push_new();
    void on_remove_row();
    void on_menu(QAction* action);
    void on_set_password();
    void update_state(enum STATE new_state);
    void clear_data();
    void on_view(QModelIndex index);
    void on_edit(QModelIndex index);
    void on_copy_username();
    void on_copy_password();

private:
    Ui::PasswordManagerClass ui;
    QClipboard* clipboard=QApplication::clipboard();
    QStandardItemModel *model;
    QString path;
    QString key;
    QStandardItem *item;
    bool isSaved;
    bool ask_save_file();
};
