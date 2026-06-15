#ifndef LIMS_MODELS_H
#define LIMS_MODELS_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

// ==================== Login Response ====================
struct LoginResponse {
    int code;
    QString message;
    bool success;
    QString apiKey;

    static LoginResponse fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};

// ==================== Device Checklist Models ====================
struct Library {
    QString stdName;
    QString stdNameEn;

    static Library fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};

struct Device {
    QString equipName;
    QString controlCode;
    QString certNo;
    QString traceInstName;
    QString testValidDate;
    QString measureRange;
    QString accuracyLevel;
    QList<Library> libraryList;

    static Device fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};

struct DeviceChecklistData {
    QString devicecode;
    QString certCorpName;
    QString certCorpAddr;
    QString equipName;
    QString equipModel;
    QString factoryNo;
    QString manufacturer;
    QString controlCode;
    QString certBarCode;
    QString temData;
    QString humData;
    QString locus;
    QList<Library> libraryList;
    QList<Device> devicelist;
    QString resultTypeName;
    QString checkDate;
    QString receivedDate;
    QString certid;
    QString extProperties;
    QString checkman;
    QString inspector;
    QString authorizedSignatory;

    static DeviceChecklistData fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};

struct DeviceChecklistResponse {
    int code;
    QString message;
    bool success;
    QList<DeviceChecklistData> data;

    static DeviceChecklistResponse fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};

// ==================== Standard Measurement Models ====================
struct SpecItem {
    QString measureRange;
    QString accuracyLevel;

    static SpecItem fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};

struct StandardMeasurementData {
    QString devicecode;
    QString equipName;
    QString controlCode;
    QString certNo;
    QString traceInstName;
    QString testValidDate;
    QList<SpecItem> specsList;

    static StandardMeasurementData fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};

struct StandardMeasurementResponse {
    int code;
    QString message;
    bool success;
    QList<StandardMeasurementData> data;

    static StandardMeasurementResponse fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};

// ==================== File Upload Models ====================
struct FileUploadResponse {
    int code;
    QString message;
    bool success;

    static FileUploadResponse fromJson(const QJsonObject &json);
    QJsonObject toJson() const;
};

#endif // LIMS_MODELS_H
