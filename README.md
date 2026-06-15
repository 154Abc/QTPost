# Qt LIMS Client

A Qt-based client for communicating with the LIMS (Laboratory Information Management System) API.

## Features

- **Login API**: Authenticate with LIMS and obtain ApiKey
- **Device Checklist API**: Query device checklist information
- **Standard Measurement API**: Retrieve standard measurement equipment specifications
- **File Upload API**: Upload test result files to LIMS
- **JSON Serialization**: Automatic conversion between Qt objects and JSON
- **Asynchronous Networking**: Non-blocking HTTP requests with signal/slot callbacks
- **Error Handling**: Comprehensive error reporting and status codes

## Architecture

### Components

1. **LimsApiClient** - Main API client class
   - Handles all HTTP POST requests
   - Emits signals for success/failure events
   - Manages network connectivity

2. **Data Models** - Qt structures for LIMS data
   - `LoginResponse` - Login response with ApiKey
   - `DeviceChecklistResponse` - Device checklist query results
   - `StandardMeasurementResponse` - Standard measurement data
   - `FileUploadResponse` - Upload confirmation
   - Supporting data structures for nested JSON objects

3. **JSON Parsing** - Automatic serialization
   - `fromJson()` - Convert JSON to Qt objects
   - `toJson()` - Convert Qt objects to JSON

## API Endpoints

### 1. Login API
```
POST /mms/api/v1/Login
```

**Request:**
```json
{
  "AccountID": "admin",
  "Password": "123333"
}
```

**Response:**
```json
{
  "code": 200,
  "message": "成功",
  "success": true,
  "ApiKey": "9c022223c4-1939-4d4f-b0ef-2332323"
}
```

### 2. Device Checklist API
```
POST /mms/api/v1/toCheckList
```

**Request:**
```json
{
  "apiKey": "9c022223c4-1939-4d4f-b0ef-2332323",
  "conditionType": "0",
  "autoSysUnique": "autosys0001",
  "deviceCode": ["device001", "device002"]
}
```

### 3. Standard Measurement API
```
POST /mms/api/v1/getStandardMeasurement
```

**Request:**
```json
{
  "apiKey": "9c022223c4-1939-4d4f-b0ef-2332323",
  "param": {
    "devicecode": "device001,device002",
    "conditiontype": "0"
  }
}
```

### 4. File Upload API
```
POST /mms/api/v1/uploadExtNew
```

**Request:**
```json
{
  "apiKey": "9c022223c4-1939-4d4f-b0ef-2332323",
  "conditionType": "0",
  "autoSysUnique": "autosys0001",
  "param": {
    "key": "12345678",
    "certfile": "UEsDBAoAAAAAAA..."
  }
}
```

## Usage Example

```cpp
#include "lims_api_client.h"
#include <QCoreApplication>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Create client
    LimsApiClient client("http://tj.jlxt.cn:9081");

    // Connect signals
    QObject::connect(&client, &LimsApiClient::loginSuccess,
        [](const LoginResponse &response) {
            qDebug() << "Login successful, ApiKey:" << response.apiKey;
        });

    QObject::connect(&client, &LimsApiClient::loginFailed,
        [](const QString &error) {
            qDebug() << "Login failed:" << error;
        });

    // Send login request
    client.login("admin", "123333");

    return app.exec();
}
```

## Compilation

### Requirements
- Qt 6.x or Qt 5.15+
- CMake 3.21+
- C++17 compiler

### Build
```bash
mkdir build
cd build
cmake ..
make
```

### Run Example
```bash
./lims_example
```

## Error Handling

All API methods emit both success and failure signals:

```cpp
// Success signals
connect(&client, &LimsApiClient::loginSuccess, this, &MyClass::onLoginSuccess);
connect(&client, &LimsApiClient::deviceChecklistReady, this, &MyClass::onChecklistReady);
connect(&client, &LimsApiClient::standardMeasurementReady, this, &MyClass::onMeasurementReady);
connect(&client, &LimsApiClient::fileUploadSuccess, this, &MyClass::onUploadSuccess);

// Failure signals
connect(&client, &LimsApiClient::loginFailed, this, &MyClass::onLoginFailed);
connect(&client, &LimsApiClient::deviceChecklistFailed, this, &MyClass::onChecklistFailed);
connect(&client, &LimsApiClient::standardMeasurementFailed, this, &MyClass::onMeasurementFailed);
connect(&client, &LimsApiClient::fileUploadFailed, this, &MyClass::onUploadFailed);

// Check last error
qDebug() << client.getLastError();
qDebug() << client.getLastErrorCode();
```

## Status Codes

- **200** - Success
- **400** - Parameter error
- **401** - Authentication failed (invalid ApiKey)
- **Network errors** - Connection failures

## License

MIT License
