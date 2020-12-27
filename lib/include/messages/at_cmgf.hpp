#pragma once

#ifndef LIB_INCLUDE_MESSAGES_AT_CMGF_HPP_
#define LIB_INCLUDE_MESSAGES_AT_CMGF_HPP_

#include "create.hpp"
#include "message.hpp"

#include <string>


namespace Ringbeller
{


/*
 * Prepare "AT+CMGF" command
 */
template<typename Body = string_body>
write_request<Body> make_at_cmgf_write(std::string const & args)
{
    return make_write<Body>("CMGF", args);
}


/*
 * Prepare "AT+CMGF" command
 */
template<typename Body = string_body, typename Char>
write_request<Body> make_at_cmgf_write(Char const * args)
{
    return make_write<Body>("CMGF", args);
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_AT_CMGF_HPP_ */
