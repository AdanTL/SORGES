#include <QtCore/QCoreApplication>
#include "sslechoclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SslEchoClient client(QUrl(QStringLiteral("wss://localhost")));

    Q_UNUSED(client);

    return a.exec();
}
