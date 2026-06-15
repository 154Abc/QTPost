#include "lims_api_client.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QDebug>

LimsApiClient::LimsApiClient(const QString &baseUrl, QObject *parent)
    : QObject(parent), m_baseUrl(baseUrl), m_lastErrorCode(0), m_currentReply(nullptr) {
    m_networkManager = new QNetworkAccessManager(this);
}

LimsApiClient::~LimsApiClient() {
    if (m_currentReply) {
        m_currentReply->deleteLater();
    }
}

QNetworkRequest LimsApiClient::createRequest(const QString &endpoint) {
    QUrl url(m_baseUrl + endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Qt LIMS Client/1.0");
    return request;
}

void LimsApiClient::login(const QString &accountId, const QString &password) {
    QJsonObject requestBody;
    requestBody["AccountID"] = accountId;
    requestBody["Password"] = password;

    QJsonDocument doc(requestBody);
    QNetworkRequest request = createRequest("/mms/api/v1/Login");

    if (m_currentReply) {
        m_currentReply->deleteLater();
    }
    m_currentReply = m_networkManager->post(request, doc.toJson(QJsonDocument::Compact));
    connect(m_currentReply, &QNetworkReply::finished, this, &LimsApiClient::onLoginFinished);
    connect(m_currentReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &LimsApiClient::onNetworkError);
}

void LimsApiClient::queryDeviceChecklist(const QString &apiKey,
                                        const QString &conditionType,
                                        const QString &autoSysUnique,
                                        const QStringList &deviceCodes) {
    QJsonObject requestBody;
    requestBody["apiKey"] = apiKey;
    requestBody["conditionType"] = conditionType;
    requestBody["autoSysUnique"] = autoSysUnique;

    QJsonArray deviceArray;
    for (const auto &code : deviceCodes) {
        deviceArray.append(code);
    }
    requestBody["deviceCode"] = deviceArray.isEmpty() ? QJsonArray() : deviceArray;

    QJsonDocument doc(requestBody);
    QNetworkRequest request = createRequest("/mms/api/v1/toCheckList");

    if (m_currentReply) {
        m_currentReply->deleteLater();
    }
    m_currentReply = m_networkManager->post(request, doc.toJson(QJsonDocument::Compact));
    connect(m_currentReply, &QNetworkReply::finished, this, &LimsApiClient::onDeviceChecklistFinished);
    connect(m_currentReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &LimsApiClient::onNetworkError);
}

void LimsApiClient::getStandardMeasurement(const QString &apiKey,
                                          const QStringList &deviceCodes,
                                          const QString &conditionType) {
    QJsonObject param;
    param["devicecode"] = deviceCodes.join(",");
    param["conditiontype"] = conditionType;

    QJsonObject requestBody;
    requestBody["apiKey"] = apiKey;
    requestBody["param"] = param;

    QJsonDocument doc(requestBody);
    QNetworkRequest request = createRequest("/mms/api/v1/getStandardMeasurement");

    if (m_currentReply) {
        m_currentReply->deleteLater();
    }
    m_currentReply = m_networkManager->post(request, doc.toJson(QJsonDocument::Compact));
    connect(m_currentReply, &QNetworkReply::finished, this, &LimsApiClient::onStandardMeasurementFinished);
    connect(m_currentReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &LimsApiClient::onNetworkError);
}

void LimsApiClient::uploadTestResults(const QString &apiKey,
                                     const QString &conditionType,
                                     const QString &autoSysUnique,
                                     const QJsonObject &paramData) {
    QJsonObject requestBody;
    requestBody["apiKey"] = apiKey;
    requestBody["conditionType"] = conditionType;
    requestBody["autoSysUnique"] = autoSysUnique;
    requestBody["param"] = paramData;

    QJsonDocument doc(requestBody);
    QNetworkRequest request = createRequest("/mms/api/v1/uploadExtNew");

    if (m_currentReply) {
        m_currentReply->deleteLater();
    }
    m_currentReply = m_networkManager->post(request, doc.toJson(QJsonDocument::Compact));
    connect(m_currentReply, &QNetworkReply::finished, this, &LimsApiClient::onFileUploadFinished);
    connect(m_currentReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &LimsApiClient::onNetworkError);
}

void LimsApiClient::onLoginFinished() {
    if (!m_currentReply) return;

    if (m_currentReply->error() == QNetworkReply::NoError) {
        QByteArray responseData = m_currentReply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        LoginResponse response = LoginResponse::fromJson(jsonObj);
        m_lastErrorCode = response.code;

        if (response.success && response.code == 200) {
            emit loginSuccess(response);
        } else {
            m_lastError = response.message;
            emit loginFailed(response.message);
        }
    } else {
        handleNetworkError(m_currentReply);
        emit loginFailed(m_lastError);
    }

    m_currentReply->deleteLater();
    m_currentReply = nullptr;
}

void LimsApiClient::onDeviceChecklistFinished() {
    if (!m_currentReply) return;

    if (m_currentReply->error() == QNetworkReply::NoError) {
        QByteArray responseData = m_currentReply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        DeviceChecklistResponse response = DeviceChecklistResponse::fromJson(jsonObj);
        m_lastErrorCode = response.code;

        if (response.success && response.code == 200) {
            emit deviceChecklistReady(response);
        } else {
            m_lastError = response.message;
            emit deviceChecklistFailed(response.message);
        }
    } else {
        handleNetworkError(m_currentReply);
        emit deviceChecklistFailed(m_lastError);
    }

    m_currentReply->deleteLater();
    m_currentReply = nullptr;
}

void LimsApiClient::onStandardMeasurementFinished() {
    if (!m_currentReply) return;

    if (m_currentReply->error() == QNetworkReply::NoError) {
        QByteArray responseData = m_currentReply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        StandardMeasurementResponse response = StandardMeasurementResponse::fromJson(jsonObj);
        m_lastErrorCode = response.code;

        if (response.success && response.code == 200) {
            emit standardMeasurementReady(response);
        } else {
            m_lastError = response.message;
            emit standardMeasurementFailed(response.message);
        }
    } else {
        handleNetworkError(m_currentReply);
        emit standardMeasurementFailed(m_lastError);
    }

    m_currentReply->deleteLater();
    m_currentReply = nullptr;
}

void LimsApiClient::onFileUploadFinished() {
    if (!m_currentReply) return;

    if (m_currentReply->error() == QNetworkReply::NoError) {
        QByteArray responseData = m_currentReply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();

        FileUploadResponse response = FileUploadResponse::fromJson(jsonObj);
        m_lastErrorCode = response.code;

        if (response.success && response.code == 200) {
            emit fileUploadSuccess(response);
        } else {
            m_lastError = response.message;
            emit fileUploadFailed(response.message);
        }
    } else {
        handleNetworkError(m_currentReply);
        emit fileUploadFailed(m_lastError);
    }

    m_currentReply->deleteLater();
    m_currentReply = nullptr;
}

void LimsApiClient::onNetworkError(QNetworkReply::NetworkError error) {
    Q_UNUSED(error);
    if (m_currentReply) {
        handleNetworkError(m_currentReply);
    }
}

void LimsApiClient::handleNetworkError(QNetworkReply *reply) {
    m_lastError = reply->errorString();
    m_lastErrorCode = reply->error();
    qDebug() << "Network Error:" << m_lastError << "Code:" << m_lastErrorCode;
}
