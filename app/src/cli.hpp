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
    std::optional<boost::asio::serial_port::stop_bits> maybe_stop_bits;
    std::optional<boost::asio::serial_port::character_size> maybe_character_size;

    Cli();

    static
    neither::Either<std::string, Cli> parse(
        char const * const * begin,
        char const * const * end,
        char const * argv0);
};



#endif /* APP_SRC_CLI_HPP_ */
