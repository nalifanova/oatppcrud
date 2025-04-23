# CrudOat based on TLS-LibreSSL Example

Example project of how-to use oatpp modules.
- Serve via HTTPS
- Make calls via HTTPS.
- Crud examples with sqlite.

See more:

- [Oat++ Website](https://oatpp.io/)
- [Oat++ Github Repository](https://github.com/oatpp/oatpp)
- [Get Started](https://oatpp.io/docs/start)

## Overview

This project is using 
[oatpp](https://github.com/oatpp/oatpp), 
[oatpp-libressl](https://github.com/oatpp/oatpp-libressl),
[oatpp-sqlite](https://github.com/oatpp/oatpp-sqlite), 
[oatpp-swagger](https://github.com/oatpp/oatpp-swagger) modules of version `1.3.0`.

### Project layout

```
|- CMakeLists.txt                        // projects CMakeLists.txt
|- src/
|    |
|    |- component/
|       |- AppComponent.hpp              // Service config
|       |- ...
|    |- controller/                      // Folder containing Controller where all endpoints are declared
|    |- db/                              // Database queries are declared here
|    |- dto/                             // DTOs are declared here
|    |- handler/                         // handlers are declared here
|    |- service/                         // crud services are located here
|    
|- sql/                                  // *.sql files for migrations
|- test/                                 // test folder
|- cert/                                 // folder with test certificates
|- vendors/
|  |- libressl-4.0.0                     // for linux/macos
|  |- libressl_v4.0.0_windows_x64        // for win
|  |- oatpp-1.3.0
|  |- oatpp-libressl-1.3.0
|  |- oatpp-sqlite-1.3.0
|  |- oatpp-swagger-1.3.0
|- main.cpp 
```

---

### Build and Run

#### Using CMake
**Requires**

- LibreSSL installed. 

```shell
## For Mac/Linux

cd libressl-4.0.0
mkdir BUILD && cd BUILD

cmake .. \
  -DCMAKE_INSTALL_PREFIX=$(pwd)/../../libressl-install \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON

make -j && make install

## For Win << todo
```

#### Build and run


```
$ mkdir build && cd build
$ cmake ..
$ make 
$ ./OatCrud  # - run application.
```

result:
```
curl -X GET "https://localhost:8009/hello" --insecure
{"user-agent":"curl\/8.7.1","message":"Hello!","server":"oatpp\/1.3.0"}
```


---

### Configure AppComponent

Configure server secure connection provider

```c++

/**
 *  Create ConnectionProvider component which listens on the port
 */
OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                       serverConnectionProvider)([] 
{
    OATPP_LOGD("oatpp::libressl::Config", "pem='%s'", CERT_PEM_PATH);
    OATPP_LOGD("oatpp::libressl::Config", "crt='%s'", CERT_CRT_PATH);
    auto config = oatpp::libressl::Config::createDefaultServerConfigShared(
    CERT_CRT_PATH, CERT_PEM_PATH /* private key */);

    return oatpp::libressl::server::ConnectionProvider::createShared(
            config, {"0.0.0.0", 8009, oatpp::network::Address::IP_4}
    );
}());
```


### Endpoints
---
"Hello" hello endpoint with json response
```c++
ENDPOINT("GET", "/hello", hello,
         REQUEST(std::shared_ptr<IncomingRequest>, request))
    {
        auto dto = HelloDto::createShared();
        dto->message = "Hello!";
        dto->server = Header::Value::SERVER;
        dto->userAgent = request->getHeader(Header::USER_AGENT);
        return createDtoResponse(Status::CODE_200, dto);
    };
```

result:
```
$ curl -X GET "https://localhost:8443/" --insecure
{"user-agent": "curl\/7.54.0", "message": "Hello Async!", "server": "oatpp\/0.19.1"}
```
