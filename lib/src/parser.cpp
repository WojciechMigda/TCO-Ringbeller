#include "parser.hpp"
#include "message.hpp"

#include "spdlog/spdlog.h"

#include <boost/spirit/home/x3.hpp>

#include <string>
#include <optional>
#include <variant>


// debug URL: http://coliru.stacked-crooked.com/a/9f5adbc76755f892


namespace Ringbeller
{


namespace detail
{


using StringResponse = Ringbeller::response<Ringbeller::string_body>;
using StringFinalResult = Ringbeller::final_result<Ringbeller::string_body>;


auto grammar_spec = [](auto & response_setter)
{
    namespace x3 = boost::spirit::x3;
    using x3::lit;
    using x3::char_;

    StringResponse res;

    auto rc_setter = [&](auto rc){ StringFinalResult fr(rc); res = fr; response_setter(res); };

    auto set_ok = [&](auto &){ rc_setter(Ringbeller::result_code::ok); };
    auto set_connect = [&](auto &){ rc_setter(Ringbeller::result_code::connect); };
    auto set_ring = [&](auto &){ rc_setter(Ringbeller::result_code::ring); };
    auto set_no_carrier = [&](auto &){ rc_setter(Ringbeller::result_code::no_carrier); };
    auto set_error = [&](auto &){ rc_setter(Ringbeller::result_code::error); };
    auto set_no_dialtone = [&](auto &){ rc_setter(Ringbeller::result_code::no_dialtone); };
    auto set_busy = [&](auto &){ rc_setter(Ringbeller::result_code::busy); };
    auto set_no_answer = [&](auto &){ rc_setter(Ringbeller::result_code::no_answer); };
    auto set_cme_error = [&](auto &){ rc_setter(Ringbeller::result_code::cme_error); };
    auto set_cms_error = [&](auto &){ rc_setter(Ringbeller::result_code::cms_error); };
    auto set_command_not_support = [&](auto &){ rc_setter(Ringbeller::result_code::command_not_support); };
    auto set_too_many_parameters = [&](auto &){ rc_setter(Ringbeller::result_code::too_many_parameters); };

    auto rc_text_setter = [&](auto & ctx)
    {
        auto val = _attr(ctx);
        StringFinalResult & final_result = std::get<StringFinalResult>(res);
        final_result.text = std::string(val.begin(), val.end());
        response_setter(res);
    };

    auto prompt_setter = [&](auto &){ res = Ringbeller::prompt(); response_setter(res); };

    /*
     * cheat sheet:
     * + : >=1 times
     * - : 0 or 1 times
     * * : 0 or n times
     * More: https://www.boost.org/doc/libs/1_74_0/libs/spirit/doc/html/spirit/qi/reference/operator.html
     */

    static auto const CR = lit('\r');
    static auto const CRLF = lit("\r\n"); // This is actually a configurable
                                          // parameter of the device.
                                          // It gets funky if it's changed.
                                          // Hardcoded for now.

    static auto const text = x3::raw[+(char_ - CR)];

    // these are also a matter of device configuration, smh
    auto const OK = lit("OK")[set_ok];
    auto const CONNECT = lit("CONNECT")[set_connect] >> -text[rc_text_setter];
    auto const RING = lit("RING")[set_ring];
    auto const NO_CARRIER = lit("NO CARRIER")[set_no_carrier];
    auto const ERROR = lit("ERROR")[set_error];
    auto const NO_DIALTONE = lit("NO DIALTONE")[set_no_dialtone];
    auto const BUSY = lit("BUSY")[set_busy];
    auto const NO_ANSWER = lit("NO ANSWER")[set_no_answer];

    auto const CME_ERROR = lit("+CME ERROR: ")[set_cme_error] >> text[rc_text_setter];
    auto const CMS_ERROR = lit("+CMS ERROR: ")[set_cms_error] >> text[rc_text_setter];
    auto const COMMAND_NOT_SUPPORT = lit("COMMAND NOT SUPPORT")[set_command_not_support];
    auto const TOO_MANY_PARAMETERS = lit("TOO MANY PARAMETERS")[set_too_many_parameters];

    auto const PROMPT = CRLF >> x3::raw["> "][prompt_setter];
//
//    auto const RESPONSE_LINE = text[body_appender] >> CRLF;
    auto const RESPONSE_COMMAND = (lit('+') | lit('^') | lit('*') | "$$") >> text >> ':' >> text;

    auto FINAL_RESULT_CODE =
        CRLF >>
        (
            OK | CONNECT | RING | NO_CARRIER | ERROR | NO_DIALTONE |
            BUSY | NO_ANSWER |
            CME_ERROR | CMS_ERROR |
            COMMAND_NOT_SUPPORT | TOO_MANY_PARAMETERS
        ) >> CRLF;

    auto response =
        FINAL_RESULT_CODE
        | PROMPT
        | RESPONSE_COMMAND
//        | (CRLF >> +RESPONSE_LINE >> -FINAL_RESULT_CODE)
//        | PROMPT
        ;

    return response;
};



std::pair<asio_buf_iterator, bool>
match_condition(asio_buf_iterator begin, asio_buf_iterator end)
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;
    using x3::phrase_parse;
    using ascii::blank;

    auto nop = [](auto &&){};

    spdlog::debug("match_condition sz:{} range:[{}]", std::distance(begin, end), std::string(begin, end));

    bool ok = phrase_parse(
        begin,
        end,
        grammar_spec(nop),
        blank
    );

    return std::make_pair(begin, ok);
}


std::pair<asio_buf_iterator, bool>
parse_response(
    asio_buf_iterator begin,
    asio_buf_iterator end,
    StringResponse & response
)
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;
    using x3::phrase_parse;
    using ascii::blank;

    auto response_setter = [&](StringResponse & resp){ response = resp; };

    bool ok = phrase_parse(
        begin,
        end,
        grammar_spec(response_setter),
        blank
    );

    return std::make_pair(begin, ok);
}


}  // namespace detail


}  // namespace Ringbeller
