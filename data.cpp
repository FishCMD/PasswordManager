#include<data.h>
#include<QDebug>

#define t(str) QString::fromLocal8Bit(str)

QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC, QAESEncryption::ISO);
QByteArray AESEncrypt(QString content, QString key) {
	QByteArray hashKey=QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha256);
	QByteArray hashIV = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
	return encryption.encode(content.toUtf8(), hashKey,hashIV);
}

QString AESDecrypt(QByteArray content, QString key) {
	QByteArray hashKey = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha256);
	QByteArray hashIV = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);
	return QString::fromUtf8(encryption.removePadding(encryption.decode(content, hashKey, hashIV)));
}

QString QModel2QString(QStandardItemModel* model) {
	QString result;
	for (int y = 0; y < model->rowCount(); y++) {
		result.append(model->item(y, 0)->text());
		result.append("\t");
		result.append(model->item(y, 1)->text());
		result.append("\t");
		result.append(model->item(y, 2)->data(Qt::UserRole).toString());

		result.append("\n");
	}
	result.chop(1);
	return result;
}

void QString2QModel(QString content,QStandardItemModel *model) {
	model->clear();
	model->setColumnCount(3);
	model->setHorizontalHeaderLabels({ t("平台"),t("用户名"),t("密码") });
	if (content.isEmpty())return;
	foreach(QString column, content.split("\n")) {
		QStringList items = column.split("\t");
		QStandardItem* password = new QStandardItem(QString(items.at(2)).fill('*'));
		password->setData(items.at(2), Qt::UserRole);
		model->appendRow({new QStandardItem(items.at(0)),new QStandardItem(items.at(1)), password});
	}
}


bool saveData(QString path,QStandardItemModel* model,QString key) {
	QFile file(path);
	if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))return false;
	file.write(AESEncrypt(QModel2QString(model).append("EOF"), key));
	return true;
}

bool readData(QString path, QStandardItemModel* model,QString key) {
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly))return false;
	QString table = AESDecrypt(file.readAll(), key);
	if (!table.endsWith("EOF"))return false;
	QString2QModel(table.chopped(3), model);
	return true;
}