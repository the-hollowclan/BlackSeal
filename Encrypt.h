#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

class Encryptor : public QObject
{
    Q_OBJECT
public:
    explicit Encryptor(QObject *parent = nullptr);

    // Publicly callable from QML
    Q_INVOKABLE bool encryptFile(const QString &filePath, const QByteArray &key);
    Q_INVOKABLE void encryptFromFile(const QString &pathsFile, const QString &key);

private:
    QByteArray generateKey(const QString &keyString);
    QByteArray aesXorEncrypt(const QByteArray &data, const QByteArray &key);
    void encryptDirectory(const QString &dirPath, const QByteArray &key);

signals:
    void encryptionFinished(bool success);
};
