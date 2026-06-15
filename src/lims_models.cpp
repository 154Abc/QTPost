#include "lims_models.h"
#include <QJsonArray>

// ==================== LoginResponse ====================
LoginResponse LoginResponse::fromJson(const QJsonObject &json) {
    LoginResponse response;
    response.code = json["code"].toInt();
    response.message = json["message"].toString();
    response.success = json["success"].toBool();
    response.apiKey = json["ApiKey"].toString();
    return response;
}

QJsonObject LoginResponse::toJson() const {
    QJsonObject json;
    json["code"] = code;
    json["message"] = message;
    json["success"] = success;
    json["ApiKey"] = apiKey;
    return json;
}

// ==================== Library ====================
Library Library::fromJson(const QJsonObject &json) {
    Library lib;
    lib.stdName = json["stdName"].toString();
    lib.stdNameEn = json["stdNameEn"].toString();
    return lib;
}

QJsonObject Library::toJson() const {
    QJsonObject json;
    json["stdName"] = stdName;
    json["stdNameEn"] = stdNameEn;
    return json;
}

// ==================== Device ====================
Device Device::fromJson(const QJsonObject &json) {
    Device device;
    device.equipName = json["equipName"].toString();
    device.controlCode = json["controlCode"].toString();
    device.certNo = json["certNo"].toString();
    device.traceInstName = json["traceInstName"].toString();
    device.testValidDate = json["testValidDate"].toString();
    device.measureRange = json["measureRange"].toString();
    device.accuracyLevel = json["accuracyLevel"].toString();
    
    QJsonArray libArray = json["libraryList"].toArray();
    for (const auto &libVal : libArray) {
        device.libraryList.append(Library::fromJson(libVal.toObject()));
    }
    return device;
}

QJsonObject Device::toJson() const {
    QJsonObject json;
    json["equipName"] = equipName;
    json["controlCode"] = controlCode;
    json["certNo"] = certNo;
    json["traceInstName"] = traceInstName;
    json["testValidDate"] = testValidDate;
    json["measureRange"] = measureRange;
    json["accuracyLevel"] = accuracyLevel;
    
    QJsonArray libArray;
    for (const auto &lib : libraryList) {
        libArray.append(lib.toJson());
    }
    json["libraryList"] = libArray;
    return json;
}

// ==================== DeviceChecklistData ====================
DeviceChecklistData DeviceChecklistData::fromJson(const QJsonObject &json) {
    DeviceChecklistData data;
    data.devicecode = json["devicecode"].toString();
    data.certCorpName = json["certCorpName"].toString();
    data.certCorpAddr = json["certCorpAddr"].toString();
    data.equipName = json["equipName"].toString();
    data.equipModel = json["equipModel"].toString();
    data.factoryNo = json["factoryNo"].toString();
    data.manufacturer = json["manufacturer"].toString();
    data.controlCode = json["controlCode"].toString();
    data.certBarCode = json["certBarCode"].toString();
    data.temData = json["temData"].toString();
    data.humData = json["humData"].toString();
    data.locus = json["locus"].toString();
    data.resultTypeName = json["resultTypeName"].toString();
    data.checkDate = json["checkDate"].toString();
    data.receivedDate = json["receivedDate"].toString();
    data.certid = json["certid"].toString();
    data.extProperties = json["extProperties"].toString();
    data.checkman = json["checkman"].toString();
    data.inspector = json["inspector"].toString();
    data.authorizedSignatory = json["authorizedSignatory"].toString();
    
    QJsonArray libArray = json["libraryList"].toArray();
    for (const auto &libVal : libArray) {
        data.libraryList.append(Library::fromJson(libVal.toObject()));
    }
    
    QJsonArray devArray = json["devicelist"].toArray();
    for (const auto &devVal : devArray) {
        data.devicelist.append(Device::fromJson(devVal.toObject()));
    }
    return data;
}

QJsonObject DeviceChecklistData::toJson() const {
    QJsonObject json;
    json["devicecode"] = devicecode;
    json["certCorpName"] = certCorpName;
    json["certCorpAddr"] = certCorpAddr;
    json["equipName"] = equipName;
    json["equipModel"] = equipModel;
    json["factoryNo"] = factoryNo;
    json["manufacturer"] = manufacturer;
    json["controlCode"] = controlCode;
    json["certBarCode"] = certBarCode;
    json["temData"] = temData;
    json["humData"] = humData;
    json["locus"] = locus;
    json["resultTypeName"] = resultTypeName;
    json["checkDate"] = checkDate;
    json["receivedDate"] = receivedDate;
    json["certid"] = certid;
    json["extProperties"] = extProperties;
    json["checkman"] = checkman;
    json["inspector"] = inspector;
    json["authorizedSignatory"] = authorizedSignatory;
    
    QJsonArray libArray;
    for (const auto &lib : libraryList) {
        libArray.append(lib.toJson());
    }
    json["libraryList"] = libArray;
    
    QJsonArray devArray;
    for (const auto &dev : devicelist) {
        devArray.append(dev.toJson());
    }
    json["devicelist"] = devArray;
    return json;
}

// ==================== DeviceChecklistResponse ====================
DeviceChecklistResponse DeviceChecklistResponse::fromJson(const QJsonObject &json) {
    DeviceChecklistResponse response;
    response.code = json["code"].toInt();
    response.message = json["message"].toString();
    response.success = json["success"].toBool();
    
    QJsonArray dataArray = json["data"].toArray();
    for (const auto &dataVal : dataArray) {
        response.data.append(DeviceChecklistData::fromJson(dataVal.toObject()));
    }
    return response;
}

QJsonObject DeviceChecklistResponse::toJson() const {
    QJsonObject json;
    json["code"] = code;
    json["message"] = message;
    json["success"] = success;
    
    QJsonArray dataArray;
    for (const auto &d : data) {
        dataArray.append(d.toJson());
    }
    json["data"] = dataArray;
    return json;
}

// ==================== SpecItem ====================
SpecItem SpecItem::fromJson(const QJsonObject &json) {
    SpecItem spec;
    spec.measureRange = json["measureRange"].toString();
    spec.accuracyLevel = json["accuracyLevel"].toString();
    return spec;
}

QJsonObject SpecItem::toJson() const {
    QJsonObject json;
    json["measureRange"] = measureRange;
    json["accuracyLevel"] = accuracyLevel;
    return json;
}

// ==================== StandardMeasurementData ====================
StandardMeasurementData StandardMeasurementData::fromJson(const QJsonObject &json) {
    StandardMeasurementData data;
    data.devicecode = json["devicecode"].toString();
    data.equipName = json["equipName"].toString();
    data.controlCode = json["controlCode"].toString();
    data.certNo = json["certNo"].toString();
    data.traceInstName = json["traceInstName"].toString();
    data.testValidDate = json["testValidDate"].toString();
    
    QJsonArray specsArray = json["specsList"].toArray();
    for (const auto &specVal : specsArray) {
        data.specsList.append(SpecItem::fromJson(specVal.toObject()));
    }
    return data;
}

QJsonObject StandardMeasurementData::toJson() const {
    QJsonObject json;
    json["devicecode"] = devicecode;
    json["equipName"] = equipName;
    json["controlCode"] = controlCode;
    json["certNo"] = certNo;
    json["traceInstName"] = traceInstName;
    json["testValidDate"] = testValidDate;
    
    QJsonArray specsArray;
    for (const auto &spec : specsList) {
        specsArray.append(spec.toJson());
    }
    json["specsList"] = specsArray;
    return json;
}

// ==================== StandardMeasurementResponse ====================
StandardMeasurementResponse StandardMeasurementResponse::fromJson(const QJsonObject &json) {
    StandardMeasurementResponse response;
    response.code = json["code"].toInt();
    response.message = json["message"].toString();
    response.success = json["success"].toBool();
    
    QJsonArray dataArray = json["data"].toArray();
    for (const auto &dataVal : dataArray) {
        response.data.append(StandardMeasurementData::fromJson(dataVal.toObject()));
    }
    return response;
}

QJsonObject StandardMeasurementResponse::toJson() const {
    QJsonObject json;
    json["code"] = code;
    json["message"] = message;
    json["success"] = success;
    
    QJsonArray dataArray;
    for (const auto &d : data) {
        dataArray.append(d.toJson());
    }
    json["data"] = dataArray;
    return json;
}

// ==================== FileUploadResponse ====================
FileUploadResponse FileUploadResponse::fromJson(const QJsonObject &json) {
    FileUploadResponse response;
    response.code = json["code"].toInt();
    response.message = json["message"].toString();
    response.success = json["success"].toBool();
    return response;
}

QJsonObject FileUploadResponse::toJson() const {
    QJsonObject json;
    json["code"] = code;
    json["message"] = message;
    json["success"] = success;
    return json;
}
