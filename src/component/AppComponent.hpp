
#ifndef AppComponent_hpp
#define AppComponent_hpp
// cust
#include "component/SwaggerComponent.hpp"
#include "component/DatabaseComponent.hpp"
#include "handler/ErrorHandler.hpp"
// ssl
#include "oatpp-libressl/server/ConnectionProvider.hpp"
#include "oatpp-libressl/client/ConnectionProvider.hpp"
#include "oatpp-libressl/Config.hpp"
// lib
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent
{
public:

    /**
     *  Swagger component
     */
    SwaggerComponent swaggerComponent;

    /**
     * Database component
     */
    DatabaseComponent databaseComponent;

    /**
     * Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                           apiObjectMapper)([] {
        auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
        return objectMapper;
    }());

    /**
     *  Create ConnectionProvider component which listens on the port
     */
    OATPP_CREATE_COMPONENT(
            std::shared_ptr<oatpp::network::ServerConnectionProvider>,
            serverConnectionProvider)([] {
        /* for http
        return oatpp::network::tcp::server::ConnectionProvider::createShared(
                {"0.0.0.0", 8000, oatpp::network::Address::IP_4});
        */

        /* for https */
        OATPP_LOGD("oatpp::libressl::Config", "pem='%s'", CERT_PEM_PATH);
        OATPP_LOGD("oatpp::libressl::Config", "crt='%s'", CERT_CRT_PATH);
        auto config = oatpp::libressl::Config::createDefaultServerConfigShared(
                CERT_CRT_PATH, CERT_PEM_PATH /* private key */);

        /**
         * if you see such error:
         * oatpp::libressl::server::ConnectionProvider:Error on call to 'tls_configure'. ssl context failure
         * It might be because you have several ssl libraries installed on your machine.
         * Try to make sure you are using libtls, libssl, and libcrypto from the same package
         */

        return oatpp::libressl::server::ConnectionProvider::createShared(
                config,
                {"0.0.0.0",
                 8009,
                 oatpp::network::Address::IP_4});
    }());

    /**
     *  Create Router component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                           httpRouter)([] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    /**
     *  Create ConnectionHandler component which uses Router component to route requests
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                           serverConnectionHandler)([] {

        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                        router); // get Router component
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
                        objectMapper); // get ObjectMapper component

        auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(
                router);
        connectionHandler->setErrorHandler(
                std::make_shared<ErrorHandler>(objectMapper));
        return connectionHandler;

    }());

};

#endif /* AppComponent_hpp */
