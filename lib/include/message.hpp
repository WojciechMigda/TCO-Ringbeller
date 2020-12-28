#pragma once

#ifndef LIB_INCLUDE_MESSAGE_HPP_
#define LIB_INCLUDE_MESSAGE_HPP_

#include <string>
#include <vector>
#include <variant>


namespace Ringbeller
{


struct string_body
{
    using value_type = std::string;
};


template<
    bool isSet, bool isXTest, bool isXRead, bool isXWrite, bool isXExec,
    typename Body
>
struct request
{
    typename Body::value_type command;
    typename Body::value_type args;
};


template<typename Body>
using basic_request = request<false, false, false, false, false, Body>;

template<typename Body>
using set_request = request<true, false, false, false, false, Body>;

template<typename Body>
using test_request = request<false, true, false, false, false, Body>;

template<typename Body>
using read_request = request<false, false, true, false, false, Body>;

template<typename Body>
using write_request = request<false, false, false, true, false, Body>;

template<typename Body>
using exec_request = request<false, false, false, false, true, Body>;


struct prompt
{
};


enum class result_code
{
    none,
    ok,
    connect,
    ring,
    no_carrier,
    error,
    no_dialtone,
    busy,
    no_answer,
    cme_error,
    cms_error,
    command_not_support,
    too_many_parameters
};


template<typename Text>
struct final_result
{
    using text_type = typename Text::value_type;

    final_result(enum result_code rc)
    : rc(rc)
    {}

    enum result_code rc;
    text_type text;
};


template<typename Text>
struct response_command
{
    using text_type = typename Text::value_type;

    text_type command;
    text_type args;
};


template<typename Text>
struct response_text
{
    using text_type = typename Text::value_type;

    text_type text;
};


//template<typename Text>
//struct response
//{
//    using value_type = std::variant<
//        prompt,
//        final_result<Text>,
//        response_command<Text>,
//        response_text<Text>
//    >;
//    value_type
//};
template<typename Text>
using response = std::variant<
    prompt,
    final_result<Text>,
    response_command<Text>,
    response_text<Text>
>;


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_MESSAGE_HPP_ */
