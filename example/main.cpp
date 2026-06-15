#include <QCoreApplication>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include "../src/lims_api_client.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Create LIMS API Client
    LimsApiClient limsClient("http://tj.jlxt.cn:9081");

    // Example 1: Login
    qDebug() << "\n=== Example 1: Login ===";
    QEventLoop loginLoop;
    QTimer loginTimeout;
    loginTimeout.setSingleShot(true);
    loginTimeout.start(10000); // 10 second timeout

    QString apiKey;
    bool loginSuccess = false;

    // Connect signals
    QObject::connect(&limsClient, &LimsApiClient::loginSuccess,
                    [&](const LoginResponse &response) {
        qDebug() << "Login Success!";
        qDebug() << "Code:" << response.code;
        qDebug() << "Message:" << response.message;
        qDebug() << "ApiKey:" << response.apiKey;
        apiKey = response.apiKey;
        loginSuccess = true;
        loginLoop.quit();
    });

    QObject::connect(&limsClient, &LimsApiClient::loginFailed,
                    [&](const QString &error) {
        qDebug() << "Login Failed:" << error;
        loginLoop.quit();
    });

    QObject::connect(&loginTimeout, &QTimer::timeout,
                    [&]() {
        qDebug() << "Login timeout";
        loginLoop.quit();
    });

    // Send login request
    limsClient.login("admin", "123333");
    loginLoop.exec();

    if (!loginSuccess) {
        qDebug() << "Login failed, exiting...";
        return 1;
    }

    // Example 2: Query Device Checklist
    if (!apiKey.isEmpty()) {
        qDebug() << "\n=== Example 2: Query Device Checklist ===";
        QEventLoop checklistLoop;
        QTimer checklistTimeout;
        checklistTimeout.setSingleShot(true);
        checklistTimeout.start(10000);

        QObject::connect(&limsClient, &LimsApiClient::deviceChecklistReady,
                        [&](const DeviceChecklistResponse &response) {
            qDebug() << "Device Checklist Ready!";
            qDebug() << "Code:" << response.code;
            qDebug() << "Message:" << response.message;
            qDebug() << "Number of records:" << response.data.size();

            for (int i = 0; i < response.data.size(); ++i) {
                const auto &item = response.data[i];
                qDebug() << "\nRecord" << (i + 1) << ":";
                qDebug() << "  Device Code:" << item.devicecode;
                qDebug() << "  Equipment Name:" << item.equipName;
                qDebug() << "  Temperature:" << item.temData;
                qDebug() << "  Humidity:" << item.humData;
                qDebug() << "  Device List Count:" << item.devicelist.size();

                for (int j = 0; j < item.devicelist.size(); ++j) {
                    const auto &device = item.devicelist[j];
                    qDebug() << "    Device" << (j + 1) << ":" << device.equipName;
                    qDebug() << "      Control Code:" << device.controlCode;
                    qDebug() << "      Measure Range:" << device.measureRange;
                    qDebug() << "      Accuracy Level:" << device.accuracyLevel;
                }
            }
            checklistLoop.quit();
        });

        QObject::connect(&limsClient, &LimsApiClient::deviceChecklistFailed,
                        [&](const QString &error) {
            qDebug() << "Device Checklist Failed:" << error;
            checklistLoop.quit();
        });

        QObject::connect(&checklistTimeout, &QTimer::timeout,
                        [&]() {
            qDebug() << "Device checklist timeout";
            checklistLoop.quit();
        });

        QStringList deviceCodes;
        deviceCodes << "device001" << "device002";
        limsClient.queryDeviceChecklist(apiKey, "0", "autosys0001", deviceCodes);
        checklistLoop.exec();
    }

    // Example 3: Get Standard Measurement
    if (!apiKey.isEmpty()) {
        qDebug() << "\n=== Example 3: Get Standard Measurement ===";
        QEventLoop measurementLoop;
        QTimer measurementTimeout;
        measurementTimeout.setSingleShot(true);
        measurementTimeout.start(10000);

        QObject::connect(&limsClient, &LimsApiClient::standardMeasurementReady,
                        [&](const StandardMeasurementResponse &response) {
            qDebug() << "Standard Measurement Ready!";
            qDebug() << "Code:" << response.code;
            qDebug() << "Message:" << response.message;
            qDebug() << "Number of records:" << response.data.size();

            for (int i = 0; i < response.data.size(); ++i) {
                const auto &item = response.data[i];
                qDebug() << "\nRecord" << (i + 1) << ":";
                qDebug() << "  Device Code:" << item.devicecode;
                qDebug() << "  Equipment Name:" << item.equipName;
                qDebug() << "  Cert No:" << item.certNo;
                qDebug() << "  Test Valid Date:" << item.testValidDate;
                qDebug() << "  Specs Count:" << item.specsList.size();

                for (int j = 0; j < item.specsList.size(); ++j) {
                    const auto &spec = item.specsList[j];
                    qDebug() << "    Spec" << (j + 1) << ":";
                    qDebug() << "      Measure Range:" << spec.measureRange;
                    qDebug() << "      Accuracy Level:" << spec.accuracyLevel;
                }
            }
            measurementLoop.quit();
        });

        QObject::connect(&limsClient, &LimsApiClient::standardMeasurementFailed,
                        [&](const QString &error) {
            qDebug() << "Standard Measurement Failed:" << error;
            measurementLoop.quit();
        });

        QObject::connect(&measurementTimeout, &QTimer::timeout,
                        [&]() {
            qDebug() << "Standard measurement timeout";
            measurementLoop.quit();
        });

        QStringList deviceCodes;
        deviceCodes << "device001" << "device002";
        limsClient.getStandardMeasurement(apiKey, deviceCodes, "0");
        measurementLoop.exec();
    }

    qDebug() << "\n=== Example completed ===";
    return 0;
}
