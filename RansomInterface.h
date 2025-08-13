#pragma once

#include <QObject>
#include <QString>

class RansomInterface : public QObject {
    Q_OBJECT
public:
    explicit RansomInterface(QObject *parent = nullptr);

    Q_INVOKABLE void validateKey(const QString &key);

signals:
    void unlock();
    void invalidKey();
};
