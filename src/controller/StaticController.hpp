#ifndef CRUD_STATICCONTROLLER_HPP
#define CRUD_STATICCONTROLLER_HPP
// cust
#include "dto/HelloDto.hpp"
// lib
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

class StaticController : public oatpp::web::server::api::ApiController
{
public:
    StaticController(const std::shared_ptr<ObjectMapper> &objectMapper)
            : oatpp::web::server::api::ApiController(objectMapper)
    {
    }

public:

    static std::shared_ptr<StaticController> createShared(
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                            objectMapper) // Inject objectMapper component here as default parameter
    )
    {
        return std::make_shared<StaticController>(objectMapper);
    }

    ENDPOINT("GET", "/", root)
    {
        const char *html =
                "<html lang='en'>"
                "  <head>"
                "    <meta charset=utf-8/>"
                "  </head>"
                "  <body>"
                "    <p>Hello CRUD example project!</p>"
                "    <a href='swagger/ui'>Checkout Swagger-UI page</a>"
                "  </body>"
                "</html>";
        auto response = createResponse(Status::CODE_200, html);
        response->putHeader(Header::CONTENT_TYPE, "text/html");
        return response;
    }

    ENDPOINT("GET", "/hello", hello,
         REQUEST(std::shared_ptr<IncomingRequest>, request))
    {
        auto dto = HelloDto::createShared();
        dto->message = "Hello!";
        dto->server = Header::Value::SERVER;
        dto->userAgent = request->getHeader(Header::USER_AGENT);
        return createDtoResponse(Status::CODE_200, dto);
    };

};

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif //CRUD_STATICCONTROLLER_HPP
