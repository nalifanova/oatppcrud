#include <iostream>
#include "config.h"
// cust
#include "component/AppComponent.hpp"
#include "controller/UserController.hpp"
#include "controller/StaticController.hpp"
// lib
#include "oatpp-libressl/Callbacks.hpp"
#include "oatpp-swagger/Controller.hpp"
#include "oatpp/network/Server.hpp"
// std
#include <iostream>
#include <csignal>

void run() {
    /* set lockingCallback for libressl */
    oatpp::libressl::Callbacks::setDefaultCallbacks();

    /* ignore SIGPIPE */
#if !(defined(WIN32) || defined(_WIN32))
    std::signal(SIGPIPE, SIG_IGN);
#endif
    /* end of ssl settings */

    AppComponent components; // Create scope Environment components

    /* Get router component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    oatpp::web::server::api::Endpoints docEndpoints;

    docEndpoints.append(router->addController(UserController::createShared())->getEndpoints());

    router->addController(oatpp::swagger::Controller::createShared(docEndpoints));
    router->addController(StaticController::createShared());

    /* Get connection handler component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    /* Get connection provider component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    /* create server */
    oatpp::network::Server server(connectionProvider,
                                  connectionHandler);

    OATPP_LOGD("Server", "Running on port %s...", connectionProvider->getProperty("port").toString()->c_str());

    server.run();

    /* stop db connection pool */
    OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
    dbConnectionProvider->stop();

}


int main(int argc, const char *argv[])
{
    std::cout << "Hello, World!" << std::endl;
    if (argc < 2) {
        // report version
        std::cout << argv[0] << "\nVersion " << OatCrud_VERSION_MAJOR << "."
                  << OatCrud_VERSION_MINOR << std::endl;
    }

    oatpp::base::Environment::init();

    run();

    /* Print how many objects were created during app running, and what have left-probably leaked */
    /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
    std::cout << "\nEnvironment:\n";
    std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
    std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

    oatpp::base::Environment::destroy();

    return 0;
}
