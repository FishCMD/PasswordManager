#pragma execution_character_set("utf-8")

#include "PasswordManager.h"
#include "data.h"
#include<QDebug>

#define t(str) QString::fromUtf8(str)

PasswordManager::PasswordManager(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.password_entry->setEchoMode(QLineEdit::Password);
    isSaved = true;
    
    item = new QStandardItem();

    model=new QStandardItemModel();
    ui.tableView->setModel(model);

    clear_data();

    ui.statusBar->addPermanentWidget(new QLabel(t("PasswordManager 0.0.1")));

    update_state(none);
}
void PasswordManager::update_state(enum STATE new_state) {
    state = new_state;
    switch (state) {
        case none:{
            ui.actionSave->setEnabled(false);
            ui.actionSaveAs->setEnabled(false);

            ui.save_file->setEnabled(false);
            ui.password_entry->setEnabled(false);
            ui.view_button->setEnabled(false);
            ui.confirm_password->setEnabled(false);
            ui.reset_password->setEnabled(false);
            ui.tableView->setEnabled(false);
            ui.copy_username->setEnabled(false);
            ui.copy_password->setEnabled(false);
            ui.push_button->setEnabled(false);
            ui.delete_button->setEnabled(false);

            ui.statusBar->showMessage(t("打开或创建一个文件"));
            break;
        }
        case newFile: {
            ui.actionSave->setEnabled(false);
            ui.actionSaveAs->setEnabled(false);

            ui.save_file->setEnabled(false);
            ui.password_entry->setEnabled(false);
            ui.view_button->setEnabled(false);
            ui.confirm_password->setEnabled(false);
            ui.reset_password->setEnabled(true);
            ui.tableView->setEnabled(false);
            ui.copy_username->setEnabled(false);
            ui.copy_password->setEnabled(false);
            ui.push_button->setEnabled(false);
            ui.delete_button->setEnabled(false);

            ui.statusBar->showMessage(t("创建密码本 请设置密码"));
            break;
        }
        case openFile: {
            ui.actionSave->setEnabled(false);
            ui.actionSaveAs->setEnabled(false);

            ui.save_file->setEnabled(false);
            ui.password_entry->setEnabled(true);
            ui.view_button->setEnabled(true);
            ui.confirm_password->setEnabled(true);
            ui.reset_password->setEnabled(false);
            ui.tableView->setEnabled(false);
            ui.copy_username->setEnabled(false);
            ui.copy_password->setEnabled(false);
            ui.push_button->setEnabled(false);
            ui.delete_button->setEnabled(false);

            ui.statusBar->showMessage(t("打开密码本 请输入密码"));
            break;
        }
        case unlocked: {
            ui.actionSave->setEnabled(true);
            ui.actionSaveAs->setEnabled(true);

            ui.save_file->setEnabled(true);
            ui.password_entry->setEnabled(false);
            ui.view_button->setEnabled(false);
            ui.confirm_password->setEnabled(false);
            ui.reset_password->setEnabled(true);
            ui.tableView->setEnabled(true);
            ui.copy_username->setEnabled(true);
            ui.copy_password->setEnabled(true);
            ui.push_button->setEnabled(true);
            ui.delete_button->setEnabled(true);

            ui.statusBar->showMessage(t("双击以编辑 单击显示密码"));
            break;
        }
    }
}

void PasswordManager::clear_data() {
    QString2QModel(QString(), model);
    ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui.tableView->setColumnWidth(0, 150);
    ui.password_entry->clear();
    key = QString();
    item = new QStandardItem();
}

void PasswordManager::on_set_password() {
    bool confirm1 = false;
    bool confirm2 = false;
    QString first_key=QInputDialog::getText(this, t("设置密码"), t("输入新密码："), QLineEdit::Password, "", &confirm1);
    if (!confirm1)return;
    if (first_key.isEmpty()) {
        if (QMessageBox::critical(this, t("设置密码"), t("密码不能为空！"), QMessageBox::Retry, QMessageBox::Cancel) == QMessageBox::Retry) on_set_password();
        return;
    }
    QString second_key= QInputDialog::getText(this, t("设置密码"), t("确认密码："), QLineEdit::Password, "", &confirm2);
    if (!confirm2)return;
    if (first_key == second_key) {
        key = first_key;
        ui.password_entry->setText(key);
        update_state(unlocked);
        isSaved = false;
        ui.statusBar->showMessage(t("成功设置密码"), 3000);
    }
    else {
        if (QMessageBox::critical(this, t("设置密码"), t("两次输入密码不一致"), QMessageBox::Retry, QMessageBox::Cancel) == QMessageBox::Retry) on_set_password();
        return;
    }
}

void PasswordManager::on_select_file() {
    if (!ask_save_file())return;
    path = QFileDialog::getOpenFileName(this, t("打开密码本文件"), "", t("密码本文件(*.password);;所有文件(*.*)"));
    if (path.isEmpty()) return;
    ui.file_path->setText(path);
    clear_data();
    update_state(openFile);
}

void PasswordManager::on_new_file() {
    if (!ask_save_file())return;
    path = QFileDialog::getSaveFileName(this, t("打开密码本文件"), "", t("密码本文件(*.password)"));
    if (path.isEmpty()) return;
    ui.file_path->setText(path);
    clear_data();
    update_state(newFile);
    on_set_password();
}

void PasswordManager::on_save_file(){
    item->setData(item->text(), Qt::UserRole);
    item = new QStandardItem();
    if (saveData(path, model, key)) {
        ui.statusBar->showMessage(t("保存成功"), 3000);
        isSaved = true;
    }
    else {
        ui.statusBar->showMessage(t("保存失败"), 3000);
    }
}

void PasswordManager::on_save_as_file() {
    QString _path = QFileDialog::getSaveFileName(this, t("打开密码本文件"), "", t("密码本文件(*.password)"));
    if (_path.isEmpty()) return;
    path = _path;
    ui.file_path->setText(path);
    on_save_file();
}

void PasswordManager::on_show_password() {
    ui.password_entry->setEchoMode(QLineEdit::Normal);
}

void PasswordManager::on_hide_password() {
    ui.password_entry->setEchoMode(QLineEdit::Password);
}

void PasswordManager::on_confirm_password() {
    key = ui.password_entry->text();
    if (key.isEmpty()) {
        QMessageBox::critical(this, t("输入密码"), t("请输入密码！"), QMessageBox::Ok);
        return;
    }
    if (readData(path, model, key))update_state(unlocked);
    else ui.statusBar->showMessage(t("密码错误 请重试"));
}

void PasswordManager::add_data(QString platform,QString username,QString password){
    model->appendRow({ new QStandardItem(platform),new QStandardItem(username),new QStandardItem(password) });
}

void PasswordManager::on_push_new() {
    PasswordManager::add_data("", "", "");
    isSaved = false;
}

void PasswordManager::on_remove_row() {
    QModelIndexList item = ui.tableView->selectionModel()->selectedIndexes();
    if (item.length() == 1) model->removeRow(item.at(0).row());
    else ui.statusBar->showMessage(t("请选中项目"), 2000);
    isSaved = false;
}

void PasswordManager::on_view(QModelIndex index) {
    item->setData(item->text(), Qt::UserRole);
    item = new QStandardItem();
    if (index.column() != 2)return;
    QStandardItem *_item = model->item(index.row(), index.column());
    if (_item->text() == _item->data(Qt::UserRole))_item->setText(_item->data(Qt::UserRole).toString().fill('*'));
    else _item->setText(_item->data(Qt::UserRole).toString());
}

void PasswordManager::on_edit(QModelIndex index) {
    isSaved = false;
    if (index.column() != 2)return;
    item = model->item(index.row(), index.column());
    item->setText(item->data(Qt::UserRole).toString());
}

void PasswordManager::on_menu(QAction* action) {
    if (action->objectName() == "actionNew")PasswordManager::on_new_file();
    if (action->objectName() == "actionOpen")PasswordManager::on_select_file();
    if (action->objectName() == "actionSave")PasswordManager::on_save_file();
    if (action->objectName() == "actionSaveAs")PasswordManager::on_save_as_file();
}

void PasswordManager::on_copy_username(){
    QModelIndexList item = ui.tableView->selectionModel()->selectedIndexes();
    if (item.length() == 1) {
        clipboard->setText(model->item(item.at(0).row(), 1)->text());
        ui.statusBar->showMessage(t("复制用户名成功"),2000);
    }
    else ui.statusBar->showMessage(t("请选中项目"), 2000);
}

void PasswordManager::on_copy_password() {
    QModelIndexList item = ui.tableView->selectionModel()->selectedIndexes();
    if (item.length() == 1) {
        clipboard->setText(model->item(item.at(0).row(), 2)->data(Qt::UserRole).toString());
        ui.statusBar->showMessage(t("复制密码成功"), 2000);
    }
    else ui.statusBar->showMessage(t("请选中项目"), 2000);
}

bool PasswordManager::ask_save_file() {
    if (isSaved)return true;
    switch (QMessageBox::question(this, t("保存文件"), t("文件未保存，是否保存？"), QMessageBox::Save|QMessageBox::No|QMessageBox::Cancel,QMessageBox::Save)) {
    case QMessageBox::Save:on_save_file();isSaved=true; return true;
    case QMessageBox::No:isSaved = true; return true;
    default:return false;
    }
}

void PasswordManager::closeEvent(QCloseEvent* event) {
    if (!ask_save_file())event->ignore();
}

PasswordManager::~PasswordManager()
{}
