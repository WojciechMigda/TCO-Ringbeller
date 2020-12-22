#include "cli.hpp"

#include "neither/either.hpp"
#include "clipp.hpp"
#include "fmt/format.h"

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



Cli::Cli() : sms_text("Feel the bern!")
{
}


neither::Either<std::string, Cli> Cli::parse(int const argc, char * argv[])
{
    using rv_type = neither::Either<std::string, Cli>;

    Cli cli;

    auto at_ok = (
        clipp::command("at_ok").set(cli.do_at_ok, true).doc("Execute AT/OK scenario (synchronous API)")
    );

    auto ati = (
        clipp::command("ati").set(cli.do_ati, true).doc("Execute ATI scenario (synchronous API)")
    );

    auto common = (
        clipp::required("--device").doc("Modem device path") & clipp::value("Path to the modem device", cli.device),

        // modem params
//        clipp::option("--baud-rate").doc("Set new baud rate on device, default=" + to_string(maybe_baud_rate))
//            & clipp::integer("New baud rate to set").call(baud_rate_setter),
//        clipp::option("--flow-control").doc("Set new flow control on device, default=" + to_string(maybe_flow_control))
//            & (clipp::required("none").call(flow_control_setter)
//                | clipp::required("sw").call(flow_control_setter)
//                | clipp::required("hw").call(flow_control_setter)),
//        clipp::option("--parity").doc("Set new parity on device, default=" + to_string(maybe_parity))
//            & (clipp::required("none").call(parity_setter)
//                | clipp::required("odd").call(parity_setter)
//                | clipp::required("even").call(parity_setter)),
//        clipp::option("--stop-bits").doc("Set new stop bits on device, default=" + to_string(maybe_stop_bits))
//            & (clipp::required("one").call(stop_bits_setter)
//                | clipp::required("onepointfive").call(stop_bits_setter)
//                | clipp::required("two").call(stop_bits_setter)),
//        clipp::option("--character-size").doc("Set new character size on device, default=" + to_string(maybe_character_size))
//            & clipp::integer("New character size to set").call(character_size_setter),


        clipp::option("--verbose", "-v").set(cli.verbose, true).doc("Enable verbose output, default=" + fmt::format("{}", cli.verbose)),
        clipp::option("--debug").set(cli.debug, true).doc("Enable debug output, default=" + fmt::format("{}", cli.debug)),
        clipp::option("--trace").set(cli.trace, true).doc("Enable trace output, default=" + fmt::format("{}", cli.trace))
    );

    auto schema = (clipp::one_of(at_ok, ati/*, at_cops, receive_sms, send_sms, make_call, receive_call*/), common);

    if (clipp::parse(argc, argv, schema))
    {
        return rv_type::rightOf(cli);
    }
    else
    {
        std::stringstream ss;
        ss << clipp::make_man_page(schema, argv[0]);

        return rv_type::leftOf(ss.str());
    }
}
