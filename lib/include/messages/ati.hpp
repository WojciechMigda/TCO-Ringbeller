#pragma once

#ifndef LIB_INCLUDE_MESSAGES_ATI_HPP_
#define LIB_INCLUDE_MESSAGES_ATI_HPP_

#include "create.hpp"
#include "message.hpp"


namespace Ringbeller
{


/*
 * Prepare "ATI" command
 */
template<typename Body = string_body>
basic_request<Body> make_ati()
{
    return make_basic<Body>("I", "");
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_ATI_HPP_ */
