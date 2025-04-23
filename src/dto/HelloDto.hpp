#ifndef OATCRUD_HELLODTO_HPP
#define OATCRUD_HELLODTO_HPP

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class HelloDto : public oatpp::DTO
{

    DTO_INIT(HelloDto, DTO)

    DTO_FIELD(String, userAgent, "user-agent");

    DTO_FIELD(String, message);

    DTO_FIELD(String, server);

};

#include OATPP_CODEGEN_END(DTO)

#endif // OATCRUD_HELLODTO_HPP
