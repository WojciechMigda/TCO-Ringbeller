#include "cli.hpp"

#include "boost/ut.hpp"


using namespace boost::ut;

auto success = [](auto &&){ return 0; };
auto failure = [](auto &&){ return -1; };

suite CLI = []
{


"fail without arguments"_test = []
{
    auto const args = {""};

    auto rv = Cli::parse(args.begin(), args.begin(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1 == rv);
};


"fail without a command"_test = []
{
    auto const args = {"-v"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1 == rv);
};


"fail with unknown argument"_test = []
{
    auto const args = {"ehfiweh"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1 == rv);
};


"accept at_ok with device"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"accept at_ok with --verbose at end"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--verbose"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.verbose;

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"accept at_ok with --verbose after command"_test = []
{
    auto const args = {"at_ok", "--verbose", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.verbose;

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"accept at_ok with -v after command"_test = []
{
    auto const args = {"at_ok", "-v", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.verbose;

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"accept at_ok with --debug at end"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--debug"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.debug;

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"accept at_ok with --trace at end"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--trace"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.trace;

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"accept at_ok with flow_control=sw"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--flow-control", "sw"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::software;

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"accept at_ok with flow_control=hw"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--flow-control", "hw"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::hardware;

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"accept at_ok with flow_control=none"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--flow-control", "none"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::none;

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"reject at_ok with unknown flow_control"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--flow-control", "xyz"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1 == rv);
};


"accept at_ok with modem configuration"_test = []
{
    auto const args = {
        "at_ok",
        "--device", "/dev/ttyUSB1312",
        "--trace",
        "--baud-rate", "9600",
        "--flow-control", "hw"
    };

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.trace;
                ok = ok && cli.maybe_baud_rate.has_value() && cli.maybe_baud_rate->value() == 9600;
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::hardware;

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"accept ati with device"_test = []
{
    auto const args = {"ati", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_ati;
                ok = ok && cli.device == "/dev/ttyUSB1312";

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


"accept ati with --verbose at end"_test = []
{
    auto const args = {"ati", "--device", "/dev/ttyUSB1312", "--verbose"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_ati;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.verbose;

                return ok ? 0 : -1;
            })
        .leftMap(failure)
        .join();

    expect(0 == rv);
};


}; // suite CLI


int main()
{
    return 0;
}
