#include "UrlLoader.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>

UrlLoader::UrlLoader(QObject *parent) : QObject(parent) {}

QString UrlLoader::loadUrl() const {
    QString filePath = QCoreApplication::applicationDirPath() + QDir::separator() + "url.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();
    return line.trimmed();
}
