#pragma once

#ifndef LIB_INCLUDE_MESSAGES_AT_HPP_
#define LIB_INCLUDE_MESSAGES_AT_HPP_

#include "create.hpp"
#include "message.hpp"


namespace Ringbeller
{


/*
 * Prepare "AT" command
 */
template<typename Body = string_body>
basic_request<Body> make_at()
{
    return make_basic<Body>("", "");
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_AT_HPP_ */
