#pragma once
#include <QObject>
#include <QString>

class UrlLoader : public QObject {
    Q_OBJECT
public:
    explicit UrlLoader(QObject *parent = nullptr);

    Q_INVOKABLE QString loadUrl() const;
};
