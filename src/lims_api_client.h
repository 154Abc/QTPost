#ifndef LIMS_API_CLIENT_H
#define LIMS_API_CLIENT_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include "lims_models.h"

class LimsApiClient : public QObject {
    Q_OBJECT

public:
    explicit LimsApiClient(const QString &baseUrl = "http://tj.jlxt.cn:9081", QObject *parent = nullptr);
    ~LimsApiClient();

    // Login API
    void login(const QString &accountId, const QString &password);

    // Device Checklist API
    void queryDeviceChecklist(const QString &apiKey,
                             const QString &conditionType,
                             const QString &autoSysUnique,
                             const QStringList &deviceCodes);

    // Standard Measurement API
    void getStandardMeasurement(const QString &apiKey,
                               const QStringList &deviceCodes,
                               const QString &conditionType);

    // File Upload API - with file path
    void uploadTestResults(const QString &apiKey,
                          const QString &conditionType,
                          const QString &autoSysUnique,
                          const QJsonArray &paramArray);

    // File Upload API - with Base64 encoded files
    void uploadTestResultsWithBase64(const QString &apiKey,
                                    const QString &conditionType,
                                    const QString &autoSysUnique,
                                    const QStringList &filePaths,
                                    const QStringList &keys);

    // Utility: Encode file to Base64
    static QString fileToBase64(const QString &filePath);

    // Getters
    QString getLastError() const { return m_lastError; }
    int getLastErrorCode() const { return m_lastErrorCode; }

signals:
    void loginSuccess(const LoginResponse &response);
    void loginFailed(const QString &error);

    void deviceChecklistReady(const DeviceChecklistResponse &response);
    void deviceChecklistFailed(const QString &error);

    void standardMeasurementReady(const StandardMeasurementResponse &response);
    void standardMeasurementFailed(const QString &error);

    void fileUploadSuccess(const FileUploadResponse &response);
    void fileUploadFailed(const QString &error);

private slots:
    void onLoginFinished();
    void onDeviceChecklistFinished();
    void onStandardMeasurementFinished();
    void onFileUploadFinished();
    void onNetworkError(QNetworkReply::NetworkError error);

private:
    QString m_baseUrl;
    QNetworkAccessManager *m_networkManager;
    QString m_lastError;
    int m_lastErrorCode;
    QNetworkReply *m_currentReply;

    QNetworkRequest createRequest(const QString &endpoint);
    void handleNetworkError(QNetworkReply *reply);
};

#endif // LIMS_API_CLIENT_H
