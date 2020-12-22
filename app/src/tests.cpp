#include "cli.hpp"

#include "boost/ut.hpp"

#include <array>

using namespace boost::ut;

auto success = [](auto &&){ return 0; };
auto failure = [](auto &&){ return -1; };

suite CLI = []
{


"fail without arguments"_test = []
{
    std::array<char const *, 1> args({""});

    auto rv = Cli::parse(args.cbegin(), args.cbegin(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1 == rv);
};


"fail with unknown argument"_test = []
{
    std::array<char const *, 1> args({"ehfiweh"});

    auto rv = Cli::parse(args.cbegin(), args.cend(), "app")
        .rightMap(success)
        .leftMap(failure)
        .join();

    expect(-1 == rv);
};


"accept at_ok with device"_test = []
{
    std::array<char const *, 3> args({"at_ok", "--device", "/dev/ttyUSB1312"});

    auto rv = Cli::parse(args.cbegin(), args.cend(), "app")
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
    std::array<char const *, 4> args({"at_ok", "--device", "/dev/ttyUSB1312", "--verbose"});

    auto rv = Cli::parse(args.cbegin(), args.cend(), "app")
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
    std::array<char const *, 4> args({"at_ok", "--verbose", "--device", "/dev/ttyUSB1312"});

    auto rv = Cli::parse(args.cbegin(), args.cend(), "app")
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
    std::array<char const *, 4> args({"at_ok", "--device", "/dev/ttyUSB1312", "--debug"});

    auto rv = Cli::parse(args.cbegin(), args.cend(), "app")
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
    std::array<char const *, 4> args({"at_ok", "--device", "/dev/ttyUSB1312", "--trace"});

    auto rv = Cli::parse(args.cbegin(), args.cend(), "app")
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


"accept ati with device"_test = []
{
    std::array<char const *, 3> args({"ati", "--device", "/dev/ttyUSB1312"});

    auto rv = Cli::parse(args.cbegin(), args.cend(), "app")
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
    std::array<char const *, 4> args({"ati", "--device", "/dev/ttyUSB1312", "--verbose"});

    auto rv = Cli::parse(args.cbegin(), args.cend(), "app")
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
