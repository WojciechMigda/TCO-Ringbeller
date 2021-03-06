#include "cli.hpp"
#include "formatters.hpp"

#include "neither/either.hpp"
#include "clipp.hpp"
#include "fmt/format.h"

#include <boost/asio/serial_port.hpp>

#include <string>
#include <sstream>
#include <optional>


/*
 * Convert option type into string representation.
 */
template<typename T>
std::string to_string(std::optional<T> const & v, std::string const & none = "(empty)")
{
    return v.has_value() ? fmt::format("{}", v.value()) : none;
}



Cli::Cli()
: sms_text("Feel the bern!")
{
}


static
void flow_control_setter(Cli & cli, std::string const s)
{
    using flow_control = boost::asio::serial_port::flow_control;

    if (s == "none")
    {
        cli.maybe_flow_control = flow_control(flow_control::none);
    }
    else if (s == "hw")
    {
        cli.maybe_flow_control = flow_control(flow_control::hardware);
    }
    else if (s == "sw")
    {
        cli.maybe_flow_control = flow_control(flow_control::software);
    }
}


static
void parity_setter(Cli & cli, std::string const s)
{
    using parity = boost::asio::serial_port::parity;

    if (s == "none")
    {
        cli.maybe_parity = parity(parity::none);
    }
    else if (s == "odd")
    {
        cli.maybe_parity = parity(parity::odd);
    }
    else if (s == "even")
    {
        cli.maybe_parity = parity(parity::even);
    }
}


static
void stop_bits_setter(Cli & cli, std::string const s)
{
    using stop_bits = boost::asio::serial_port::stop_bits;

    if (s == "one")
    {
        cli.maybe_stop_bits = stop_bits(stop_bits::one);
    }
    else if (s == "onepointfive")
    {
        cli.maybe_stop_bits = stop_bits(stop_bits::onepointfive);
    }
    else if (s == "two")
    {
        cli.maybe_stop_bits = stop_bits(stop_bits::two);
    }
}


neither::Either<std::string, Cli>
Cli::parse(char const * const * begin, char const * const * end, char const * argv0)
{
    using rv_type = neither::Either<std::string, Cli>;

    std::vector<std::string> error_info;

    Cli cli;
    bool has_cmd = false;
    auto baud_rate_setter = [&cli](std::string const & s){ cli.maybe_baud_rate = boost::asio::serial_port::baud_rate(std::stoi(s)); };
    auto flow_control_setter = [&cli](std::string const & s){ ::flow_control_setter(cli, s); };
    auto parity_setter = [&cli](std::string const & s){ ::parity_setter(cli, s); };
    auto stop_bits_setter = [&cli](std::string const & s){ ::stop_bits_setter(cli, s); };
    auto character_size_setter = [&cli](std::string const & s){ cli.maybe_character_size = boost::asio::serial_port::character_size(std::stoi(s)); };

    auto at_ok = (
        clipp::command("at_ok").set(cli.do_at_ok, true).set(has_cmd, true).doc("Execute AT/OK scenario (synchronous API)")
    );

    auto ati = (
        clipp::command("ati").set(cli.do_ati, true).set(has_cmd, true).doc("Execute ATI scenario (synchronous API)")
    );

    auto at_cops = (
        clipp::command("at+cops").set(cli.do_at_cops, true).set(has_cmd, true).doc("Execute AT+COPS=? scenario (asynchronous API)")
    );

    auto receive_sms = (
        clipp::command("rcv-sms").set(cli.do_rcv_sms, true).set(has_cmd, true).doc("Wait and receive SMS")
    );

    auto send_sms = (
        clipp::command("send-sms").set(cli.do_send_sms, true).set(has_cmd, true).doc("Send SMS")
            & clipp::value("Destination address", cli.da)
                .if_missing([&]
                {
                    if (cli.do_send_sms)
                    {
                        error_info.emplace_back("Destination address is missing.");
                    }
                }),
        clipp::option("--text").doc(fmt::format("Set SMS text, default=\"{}\"", cli.sms_text)) & clipp::value("SMS text to send", cli.sms_text)
    );

    auto make_call = (
        clipp::command("make-call").set(cli.do_make_call, true).set(has_cmd, true).doc("Make voice call") & clipp::value("Destination address", cli.da)
    );

    auto receive_call = (
        clipp::command("rcv-call").set(cli.do_rcv_call, true).set(has_cmd, true).doc("Wait and receive voice call")
    );

    auto common = (
        clipp::required("--device").doc("Modem device path") & clipp::value("Path to the modem device", cli.device)
            .if_missing([&]
            {
                if (has_cmd)
                {
                    error_info.emplace_back("Modem device path (--device) is missing.");
                }
            }),

        // modem params
        clipp::option("--baud-rate").doc("Set new baud rate on device, default=" + to_string(cli.maybe_baud_rate))
            & clipp::integer("New baud rate to set").call(baud_rate_setter),
        clipp::option("--flow-control").doc("Set new flow control on device, default=" + to_string(cli.maybe_flow_control))
            & (clipp::required("none").call(flow_control_setter)
                | clipp::required("sw").call(flow_control_setter)
                | clipp::required("hw").call(flow_control_setter)),
        clipp::option("--parity").doc("Set new parity on device, default=" + to_string(cli.maybe_parity))
            & (clipp::required("none").call(parity_setter)
                | clipp::required("odd").call(parity_setter)
                | clipp::required("even").call(parity_setter)),
        clipp::option("--stop-bits").doc("Set new stop bits on device, default=" + to_string(cli.maybe_stop_bits))
            & (clipp::required("one").call(stop_bits_setter)
                | clipp::required("onepointfive").call(stop_bits_setter)
                | clipp::required("two").call(stop_bits_setter)),
        clipp::option("--character-size").doc("Set new character size on device, default=" + to_string(cli.maybe_character_size))
            & clipp::one_of(
                clipp::required("5").call(character_size_setter),
                clipp::required("6").call(character_size_setter),
                clipp::required("7").call(character_size_setter),
                clipp::required("8").call(character_size_setter)),


        clipp::option("--verbose", "-v").set(cli.verbose, true).doc("Enable verbose output, default=" + fmt::format("{}", cli.verbose)),
        clipp::option("--debug").set(cli.debug, true).doc("Enable debug output, default=" + fmt::format("{}", cli.debug)),
        clipp::option("--trace").set(cli.trace, true).doc("Enable trace output, default=" + fmt::format("{}", cli.trace))
    );

    auto schema = (clipp::one_of(at_ok, ati, at_cops, receive_sms, send_sms, make_call, receive_call), common);

    if (clipp::parse(begin, end, schema))
    {
        return rv_type::rightOf(cli);
    }
    else
    {
        std::stringstream ss;
        ss << clipp::make_man_page(schema, argv0);

        std::string rv;

        if (not error_info.empty())
        {
            rv += "ERROR\n";

            for (auto const & ei : error_info)
            {
                rv += fmt::format("        {}\n", ei);
            }

            rv += '\n';
        }
        rv += ss.str();

        return rv_type::leftOf(rv);
    }
}
