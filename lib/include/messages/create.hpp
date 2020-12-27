#pragma once

#ifndef LIB_INCLUDE_MESSAGES_CREATE_HPP_
#define LIB_INCLUDE_MESSAGES_CREATE_HPP_


#include "message.hpp"


namespace Ringbeller
{


/*
 * Prepare 'basic' AT command
 */
template<typename Body = string_body, typename String>
basic_request<Body> make_basic(String && command, String && args)
{
    basic_request<Body> rv;

    rv.command = command;
    rv.args = args;

    return rv;
}


/*
 * Prepare 'basic' AT command
 */
template<typename Body = string_body, typename Char>
basic_request<Body> make_basic(Char const * command, Char const * args)
{
    basic_request<Body> rv;

    rv.command = command;
    rv.args = args;

    return rv;
}


/*
 * Prepare 'write' AT command
 */
template<typename Body = string_body, typename String>
write_request<Body> make_write(String && command, String && args)
{
    write_request<Body> rv;

    rv.command = command;
    rv.args = args;

    return rv;
}


/*
 * Prepare 'write' AT command
 */
template<typename Body = string_body, typename Char>
write_request<Body> make_write(Char const * command, Char const * args)
{
    write_request<Body> rv;

    rv.command = command;
    rv.args = args;

    return rv;
}


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGES_CREATE_HPP_ */
