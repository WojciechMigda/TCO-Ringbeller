#include "cli.hpp"

#include "boost/ut.hpp"


using namespace boost::ut;

auto success = [](auto &&) -> int { return 0; };
auto failure = [](auto &&) -> int { return -1; };

suite CLI = []
{


"fail without arguments"_test = []
{
    auto const args = {""};

    auto rv = Cli::parse(args.begin(), args.begin(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"fail without a command"_test = []
{
    auto const args = {"-v"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"fail with unknown argument"_test = []
{
    auto const args = {"ehfiweh"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject at_ok without device"_test = []
{
    auto const args = {"at_ok"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject at_ok with unknown flow_control"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--flow-control", "xyz"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"accept at_ok with parity=none"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--parity", "none"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_parity.has_value()
                    && cli.maybe_parity->value() == boost::asio::serial_port_base::parity::none;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept at_ok with parity=odd"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--parity", "odd"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_parity.has_value()
                    && cli.maybe_parity->value() == boost::asio::serial_port_base::parity::odd;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept at_ok with parity=even"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--parity", "even"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_parity.has_value()
                    && cli.maybe_parity->value() == boost::asio::serial_port_base::parity::even;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject at_ok with unknown parity"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--parity", "xyz"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"accept at_ok with stop-bits=one"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--stop-bits", "one"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_stop_bits.has_value()
                    && cli.maybe_stop_bits->value() == boost::asio::serial_port_base::stop_bits::one;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept at_ok with stop-bits=onepointfive"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--stop-bits", "onepointfive"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_stop_bits.has_value()
                    && cli.maybe_stop_bits->value() == boost::asio::serial_port_base::stop_bits::onepointfive;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept at_ok with stop-bits=two"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--stop-bits", "two"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_stop_bits.has_value()
                    && cli.maybe_stop_bits->value() == boost::asio::serial_port_base::stop_bits::two;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject at_ok with unknown stop-bits"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--stop-bits", "acab"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"accept at_ok with baud-rate=112000"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--baud-rate", "112000"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_baud_rate.has_value()
                    && cli.maybe_baud_rate->value() == 112000;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject at_ok with non-integer baud-rate"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--baud-rate", "ftp"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"accept at_ok with character-size=7"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--character-size", "7"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok
                    && cli.maybe_character_size.has_value()
                    && cli.maybe_character_size->value() == 7;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject at_ok with non-integer character-size"_test = []
{
    auto const args = {"at_ok", "--device", "/dev/ttyUSB1312", "--character-size", "ftp"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"accept at_ok with modem configuration"_test = []
{
    auto const args = {
        "at_ok",
        "--device", "/dev/ttyUSB1312",
        "--trace",
        "--baud-rate", "9600",
        "--flow-control", "hw",
        "--parity", "odd",
        "--stop-bits", "two",
        "--character-size", "8"
    };

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_ok;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.trace;
                ok = ok
                    && cli.maybe_baud_rate.has_value()
                    && cli.maybe_baud_rate->value() == 9600;
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::hardware;
                ok = ok
                    && cli.maybe_parity.has_value()
                    && cli.maybe_parity->value() == boost::asio::serial_port_base::parity::odd;
                ok = ok
                    && cli.maybe_stop_bits.has_value()
                    && cli.maybe_stop_bits->value() == boost::asio::serial_port_base::stop_bits::two;
                ok = ok
                    && cli.maybe_character_size.has_value()
                    && cli.maybe_character_size->value() == 8;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject ati without device"_test = []
{
    auto const args = {"ati"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
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

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept ati with modem configuration"_test = []
{
    auto const args = {
        "ati",
        "--baud-rate", "9600",
        "--flow-control", "hw",
        "--trace",
        "--parity", "odd",
        "--stop-bits", "two",
        "--character-size", "8",
        "--device", "/dev/ttyUSB1312"
    };

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_ati;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.trace;
                ok = ok
                    && cli.maybe_baud_rate.has_value()
                    && cli.maybe_baud_rate->value() == 9600;
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::hardware;
                ok = ok
                    && cli.maybe_parity.has_value()
                    && cli.maybe_parity->value() == boost::asio::serial_port_base::parity::odd;
                ok = ok
                    && cli.maybe_stop_bits.has_value()
                    && cli.maybe_stop_bits->value() == boost::asio::serial_port_base::stop_bits::two;
                ok = ok
                    && cli.maybe_character_size.has_value()
                    && cli.maybe_character_size->value() == 8;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept at+cops with device"_test = []
{
    auto const args = {"at+cops", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_cops;
                ok = ok && cli.device == "/dev/ttyUSB1312";

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject at+cops without device"_test = []
{
    auto const args = {"at+cops"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"accept at+cops with device with debug"_test = []
{
    auto const args = {"at+cops", "--device", "/dev/ttyUSB1312", "--debug"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_cops;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.debug;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept at+cops with modem configuration"_test = []
{
    auto const args = {
        "at+cops",
        "--flow-control", "sw",
        "--baud-rate", "9600",
        "--device", "/dev/ttyUSB1312",
        "--parity", "even",
        "--character-size", "8",
        "-v",
        "--stop-bits", "one"
    };

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_at_cops;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.verbose;
                ok = ok
                    && cli.maybe_baud_rate.has_value()
                    && cli.maybe_baud_rate->value() == 9600;
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::software;
                ok = ok
                    && cli.maybe_parity.has_value()
                    && cli.maybe_parity->value() == boost::asio::serial_port_base::parity::even;
                ok = ok
                    && cli.maybe_stop_bits.has_value()
                    && cli.maybe_stop_bits->value() == boost::asio::serial_port_base::stop_bits::one;
                ok = ok
                    && cli.maybe_character_size.has_value()
                    && cli.maybe_character_size->value() == 8;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept rcv-sms with device"_test = []
{
    auto const args = {"rcv-sms", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_rcv_sms;
                ok = ok && cli.device == "/dev/ttyUSB1312";

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject rcv-sms without device"_test = []
{
    auto const args = {"rcv-sms"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"accept rcv-sms with device with trace"_test = []
{
    auto const args = {"rcv-sms", "--trace", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_rcv_sms;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.trace;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept rcv-sms with modem configuration"_test = []
{
    auto const args = {
        "rcv-sms",
        "--baud-rate", "19200",
        "--flow-control", "none",
        "--device", "/dev/ttyUSB1312",
        "--verbose",
        "--parity", "even",
        "--character-size", "8",
        "--stop-bits", "onepointfive"
    };

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_rcv_sms;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.verbose;
                ok = ok
                    && cli.maybe_baud_rate.has_value()
                    && cli.maybe_baud_rate->value() == 19200;
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::none;
                ok = ok
                    && cli.maybe_parity.has_value()
                    && cli.maybe_parity->value() == boost::asio::serial_port_base::parity::even;
                ok = ok
                    && cli.maybe_stop_bits.has_value()
                    && cli.maybe_stop_bits->value() == boost::asio::serial_port_base::stop_bits::onepointfive;
                ok = ok
                    && cli.maybe_character_size.has_value()
                    && cli.maybe_character_size->value() == 8;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept send-sms with device"_test = []
{
    auto const args = {"send-sms", "+12124109000", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_send_sms;
                ok = ok && cli.da == "+12124109000";
                ok = ok && cli.device == "/dev/ttyUSB1312";

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject send-sms without destination address"_test = []
{
    auto const args = {"send-sms", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"reject send-sms without device"_test = []
{
    auto const args = {"send-sms", "+12124109000"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"accept send-sms with sms text"_test = []
{
    auto const args = {"send-sms", "+12124109000", "--device", "/dev/ttyUSB1312", "--text", "Lock him up"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_send_sms;
                ok = ok && cli.da == "+12124109000";
                ok = ok && cli.sms_text == "Lock him up";
                ok = ok && cli.device == "/dev/ttyUSB1312";

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept send-sms with verbose"_test = []
{
    auto const args = {"send-sms", "+12124109000", "--device", "/dev/ttyUSB1312", "--verbose", "--text", "Lock him up"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_send_sms;
                ok = ok && cli.da == "+12124109000";
                ok = ok && cli.sms_text == "Lock him up";
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.verbose;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept send-sms with modem configuration"_test = []
{
    auto const args = {
        "send-sms",
        "+16467073826",
        "--baud-rate", "19200",
        "--flow-control", "none",
        "--device", "/dev/ttyUSB1312",
        "--debug",
        "--parity", "even",
        "--character-size", "8",
        "--stop-bits", "onepointfive",
        "--text", "Lock him up"
    };

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_send_sms;
                ok = ok && cli.da == "+16467073826";
                ok = ok && cli.sms_text == "Lock him up";
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.debug;
                ok = ok
                    && cli.maybe_baud_rate.has_value()
                    && cli.maybe_baud_rate->value() == 19200;
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::none;
                ok = ok
                    && cli.maybe_parity.has_value()
                    && cli.maybe_parity->value() == boost::asio::serial_port_base::parity::even;
                ok = ok
                    && cli.maybe_stop_bits.has_value()
                    && cli.maybe_stop_bits->value() == boost::asio::serial_port_base::stop_bits::onepointfive;
                ok = ok
                    && cli.maybe_character_size.has_value()
                    && cli.maybe_character_size->value() == 8;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept rcv-call with device"_test = []
{
    auto const args = {"rcv-call", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_rcv_call;
                ok = ok && cli.device == "/dev/ttyUSB1312";

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject rcv-call without device"_test = []
{
    auto const args = {"rcv-call"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"accept rcv-call with device with debug"_test = []
{
    auto const args = {"rcv-call", "--device", "/dev/ttyUSB1312", "--debug"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_rcv_call;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.debug;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept rcv-call with modem configuration"_test = []
{
    auto const args = {
        "rcv-call",
        "--character-size", "8",
        "--baud-rate", "19200",
        "--flow-control", "none",
        "--trace",
        "--device", "/dev/ttyUSB1312",
        "--parity", "even",
        "--stop-bits", "two"
    };

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_rcv_call;
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.trace;
                ok = ok
                    && cli.maybe_baud_rate.has_value()
                    && cli.maybe_baud_rate->value() == 19200;
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::none;
                ok = ok
                    && cli.maybe_parity.has_value()
                    && cli.maybe_parity->value() == boost::asio::serial_port_base::parity::even;
                ok = ok
                    && cli.maybe_stop_bits.has_value()
                    && cli.maybe_stop_bits->value() == boost::asio::serial_port_base::stop_bits::two;
                ok = ok
                    && cli.maybe_character_size.has_value()
                    && cli.maybe_character_size->value() == 8;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept make-call with device"_test = []
{
    auto const args = {"make-call", "+19732225287", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_make_call;
                ok = ok && cli.da == "+19732225287";
                ok = ok && cli.device == "/dev/ttyUSB1312";

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"reject make-call without destination address"_test = []
{
    auto const args = {"make-call", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"reject make-call without device"_test = []
{
    auto const args = {"make-call", "+19732225287"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1_i == rv);
};


"accept make-call with verbose"_test = []
{
    auto const args = {"make-call", "+17039302271", "-v", "--device", "/dev/ttyUSB1312"};

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_make_call;
                ok = ok && cli.da == "+17039302271";
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.verbose;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


"accept make-call with modem configuration"_test = []
{
    auto const args = {
        "make-call",
        "+17036234678",
        "--trace",
        "--baud-rate", "19200",
        "--flow-control", "sw",
        "--parity", "even",
        "--character-size", "6",
        "--stop-bits", "one",
        "--device", "/dev/ttyUSB1312"
    };

    auto rv = Cli::parse(args.begin(), args.end(), "app")
        .rightMap([](Cli && cli)
            {
                bool ok = true;

                ok = ok && cli.do_make_call;
                ok = ok && cli.da == "+17036234678";
                ok = ok && cli.device == "/dev/ttyUSB1312";
                ok = ok && cli.trace;
                ok = ok
                    && cli.maybe_baud_rate.has_value()
                    && cli.maybe_baud_rate->value() == 19200;
                ok = ok
                    && cli.maybe_flow_control.has_value()
                    && cli.maybe_flow_control->value() == boost::asio::serial_port_base::flow_control::software;
                ok = ok
                    && cli.maybe_parity.has_value()
                    && cli.maybe_parity->value() == boost::asio::serial_port_base::parity::even;
                ok = ok
                    && cli.maybe_stop_bits.has_value()
                    && cli.maybe_stop_bits->value() == boost::asio::serial_port_base::stop_bits::one;
                ok = ok
                    && cli.maybe_character_size.has_value()
                    && cli.maybe_character_size->value() == 6;

                return ok ? 0 : 1;
            })
        .leftMap(failure)
        .join();

    expect(0_i == rv);
};


}; // suite CLI


int main()
{
    return 0;
}
