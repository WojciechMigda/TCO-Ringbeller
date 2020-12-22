//#include "do_at_ok.hpp"
//#include "do_ati.hpp"
//#include "do_at_cops.hpp"
//#include "do_receive_sms.hpp"
//#include "do_send_sms.hpp"
//#include "do_make_vcall.hpp"
//#include "do_receive_vcall.hpp"

#include "cli.hpp"
//#include "formatters.hpp"

//#include "clipp.hpp"
#include "spdlog/spdlog.h"
#include "fmt/format.h"
#include "serial_port_setup.hpp"

#include <boost/asio/serial_port.hpp>

//#include <iostream>
//#include <optional>
#include <string>


/*
 * Set `spdlog` log level according to `verbose`, `debug`, and `trace` flags.
 */
static void set_log_level(bool verbose, bool debug, bool trace)
{
    if (trace)
    {
        spdlog::set_level(spdlog::level::trace);
    }
    else if (debug)
    {
        spdlog::set_level(spdlog::level::debug);
    }
    else if (verbose)
    {
        spdlog::set_level(spdlog::level::info);
    }
    else
    {
        spdlog::set_level(spdlog::level::warn);
    }
}


int main(int argc, char **argv)
{
    auto rv = Cli::parse(argc, argv)
        .leftMap([](auto && usage)
            {
                fmt::print("{}", usage);

                return -1;
            })
        .rightMap([](auto && cli)
            {
                set_log_level(cli.verbose, cli.debug, cli.trace);
                return 0;
            })
        .join()
        ;

//
//    auto cli = (clipp::one_of(at_ok, ati, at_cops, receive_sms, send_sms, make_call, receive_call), common);
//
//    int rv = 0;
//
//    if (not clipp::parse(argc, argv, cli))
//    {
//        std::cout << clipp::make_man_page(cli, argv[0]);
//
//        return -1;
//    }
//    else
//    {
//        set_log_level(verbose, debug, trace);
//
//        auto const serial_port_param_set = serial_port_param_set_t{maybe_baud_rate, maybe_parity, maybe_stop_bits, maybe_flow_control, maybe_character_size};

#if 0
        if (do_at_ok)
        {
            fmt::print("Executing AT/OK scenario\n");

            rv = run_at_ok(device, serial_port_param_set);
        }
        else if (do_ati)
        {
            fmt::print("Executing ATI scenario\n");

            rv = run_ati(device, serial_port_param_set);
        }
        else if (do_at_cops)
        {
            fmt::print("Executing AT+COPS scenario\n");

            rv = run_at_cops(device, serial_port_param_set);
        }
        else if (do_rcv_sms)
        {
            fmt::print("Executing \"Receive SMS\" scenario\n");

            rv = run_receive_sms(device, serial_port_param_set);
        }
        else if (do_send_sms)
        {
            fmt::print("Executing \"Send SMS\" scenario\n");
            fmt::print("SMS target: {}\n", da);
            fmt::print("SMS text: {}\n", sms_text);

            rv = run_send_sms(device, serial_port_param_set, da, sms_text);
        }
        else if (do_rcv_call)
        {
            fmt::print("Executing \"Receive voice call\" scenario\n");

            rv = run_receive_vcall(device, serial_port_param_set);
        }
        else if (do_make_call)
        {
            fmt::print("Executing \"Make voice call\" scenario\n");
            fmt::print("Voice call target: {}\n", da);

            rv = run_make_vcall(device, serial_port_param_set, da);
        }
#endif
//    }

    fmt::print("DONE with status={}\n", rv);

    return rv;
}
