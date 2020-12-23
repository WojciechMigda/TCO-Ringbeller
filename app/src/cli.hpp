#pragma once

#ifndef APP_SRC_CLI_HPP_
#define APP_SRC_CLI_HPP_

#include "neither/either.hpp"

#include <boost/asio/serial_port.hpp>

#include <string>
#include <optional>


struct Cli
{
    bool do_at_ok = false;
    bool do_ati = false;
    bool do_at_cops = false;
    bool do_rcv_sms = false;
    bool do_send_sms = false;
    bool do_make_call = false;
    bool do_rcv_call = false;

    std::string da;
    std::string sms_text;

    bool verbose = false;
    bool debug = false;
    bool trace = false;

    std::string device;

    std::optional<boost::asio::serial_port::baud_rate> maybe_baud_rate;
    std::optional<boost::asio::serial_port::flow_control> maybe_flow_control;
    std::optional<boost::asio::serial_port::parity> maybe_parity;
//    auto parity_setter = [&maybe_parity](std::string const & s)
//    {
//        using parity = boost::asio::serial_port::parity;
//
//        if (s == "none")
//        {
//            maybe_parity = parity(parity::none);
//        }
//        else if (s == "odd")
//        {
//            maybe_parity = parity(parity::odd);
//        }
//        else if (s == "even")
//        {
//            maybe_parity = parity(parity::even);
//        }
//    };

    std::optional<boost::asio::serial_port::stop_bits> maybe_stop_bits;
//    auto stop_bits_setter = [&maybe_stop_bits](std::string const & s)
//    {
//        using stop_bits = boost::asio::serial_port::stop_bits;
//
//        if (s == "one")
//        {
//            maybe_stop_bits = stop_bits(stop_bits::one);
//        }
//        else if (s == "onepointfive")
//        {
//            maybe_stop_bits = stop_bits(stop_bits::onepointfive);
//        }
//        else if (s == "two")
//        {
//            maybe_stop_bits = stop_bits(stop_bits::two);
//        }
//    };

    std::optional<boost::asio::serial_port::character_size> maybe_character_size;

    Cli();

    static
    neither::Either<std::string, Cli> parse(
        char const * const * begin,
        char const * const * end,
        char const * argv0);
};



#endif /* APP_SRC_CLI_HPP_ */
