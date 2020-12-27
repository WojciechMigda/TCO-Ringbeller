#pragma once

#ifndef LIB_INCLUDE_MESSAGES_ATD_HPP_
#define LIB_INCLUDE_MESSAGES_ATD_HPP_

#include "create.hpp"
#include "message.hpp"


namespace Ringbeller
{


/*
 * Prepare "ATD" command
 */
template<typename Body = string_body>
basic_request<Body> make_atd(std::string const & args)
{
    return make_basic<Body>("D", args);
}


/*
 * Prepare "ATD" command
 */
template<typename Body = string_body, typename Char>
basic_request<Body> make_atd(Char const * args)
{
    return make_basic<Body>("D", args);
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_ATD_HPP_ */
