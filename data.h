#pragma once
#include<qaesencryption.h>
#include<QCryptographicHash>
#include<QStandardItemModel>
#include<QFile>

QByteArray AESEncrypt(QString content, QString key);
QString AESDecrypt(QByteArray content, QString key);
QString QModel2QString(QStandardItemModel *model);
void QString2QModel(QString content,QStandardItemModel *model);
bool saveData(QString path, QStandardItemModel *model, QString key);
bool readData(QString path, QStandardItemModel *model, QString key);