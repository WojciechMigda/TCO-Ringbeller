#include "read.hpp"

#include "boost/ut.hpp"

#include <boost/asio/buffers_iterator.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/streambuf.hpp>

#include <string>
#include <iterator>
#include <algorithm>


struct SyncReadStream
{
    std::string payload;

    SyncReadStream(std::string && s) : payload(s)
    {}

    template<typename ConstBufferSequence>
    std::size_t read_some(ConstBufferSequence const & buffers, boost::system::error_code & ec)
    {
        std::copy(payload.cbegin(), payload.cend(), boost::asio::buffers_begin(buffers));

        return payload.size();
    }

    template<typename ConstBufferSequence>
    std::size_t read_some(ConstBufferSequence const & buffers)
    {
        boost::system::error_code ec;

        return read_some(buffers, ec);
    }
};


using namespace boost::ut;


suite sync_read = []
{


using StringFinalResult = Ringbeller::final_result<Ringbeller::string_body>;
using StringResponseCommand = Ringbeller::response_command<Ringbeller::string_body>;


"read works with OK response code"_test = []
{
    SyncReadStream s("\r\nOK\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(6_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::ok == fr.rc);
    expect(fr.text.empty());
};


"read works with OK response code followed by extra input"_test = []
{
    SyncReadStream s("\r\nOK\r\n\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(6_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::ok == fr.rc);
    expect(fr.text.empty());
};


"read works with CONNECT response code"_test = []
{
    SyncReadStream s("\r\nCONNECT\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(11_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::connect == fr.rc);
    expect(fr.text.empty());
};


"read works with CONNECT 7200000 response code"_test = []
{
    SyncReadStream s("\r\nCONNECT 7200000\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(19_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::connect == fr.rc);
    expect("7200000" == fr.text);
};


"read works with CONNECT 7200000 response code followed by extra input"_test = []
{
    SyncReadStream s("\r\nCONNECT 7200000\r\n\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(19_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::connect == fr.rc);
    expect("7200000" == fr.text);
};


"read works with RING response code"_test = []
{
    SyncReadStream s("\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(8_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::ring == fr.rc);
    expect(fr.text.empty());
};


"read works with RING response code followed by extra input"_test = []
{
    SyncReadStream s("\r\nRING\r\n\r\nOK\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(8_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::ring == fr.rc);
    expect(fr.text.empty());
};


"read works with NO CARRIER response code"_test = []
{
    SyncReadStream s("\r\nNO CARRIER\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(14_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::no_carrier == fr.rc);
    expect(fr.text.empty());
};


"read works with NO CARRIER response code followed by extra input"_test = []
{
    SyncReadStream s("\r\nNO CARRIER\r\n\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(14_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::no_carrier == fr.rc);
    expect(fr.text.empty());
};


"read works with ERROR response code"_test = []
{
    SyncReadStream s("\r\nERROR\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(9_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::error == fr.rc);
    expect(fr.text.empty());
};


"read works with ERROR response code followed by extra input"_test = []
{
    SyncReadStream s("\r\nERROR\r\n\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(9_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::error == fr.rc);
    expect(fr.text.empty());
};


"read works with NO DIALTONE response code"_test = []
{
    SyncReadStream s("\r\nNO DIALTONE\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(15_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::no_dialtone == fr.rc);
    expect(fr.text.empty());
};


"read works with NO DIALTONE response code followed by extra input"_test = []
{
    SyncReadStream s("\r\nNO DIALTONE\r\n\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(15_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::no_dialtone == fr.rc);
    expect(fr.text.empty());
};


"read works with BUSY response code"_test = []
{
    SyncReadStream s("\r\nBUSY\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(8_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::busy == fr.rc);
    expect(fr.text.empty());
};


"read works with BUSY response code followed by extra input"_test = []
{
    SyncReadStream s("\r\nBUSY\r\n\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(8_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::busy == fr.rc);
    expect(fr.text.empty());
};


"read works with NO ANSWER response code"_test = []
{
    SyncReadStream s("\r\nNO ANSWER\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(13_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::no_answer == fr.rc);
    expect(fr.text.empty());
};


"read works with NO ANSWER response code followed by extra input"_test = []
{
    SyncReadStream s("\r\nNO ANSWER\r\n\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(13_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::no_answer == fr.rc);
    expect(fr.text.empty());
};


"read works with +CME ERROR <numeric> response code"_test = []
{
    SyncReadStream s("\r\n+CME ERROR: 10\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(18_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::cme_error == fr.rc);
    expect("10" == fr.text);
};


"read works with +CME ERROR <verbose text> response code"_test = []
{
    SyncReadStream s("\r\n+CME ERROR: SIM not inserted\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(32_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::cme_error == fr.rc);
    expect("SIM not inserted" == fr.text);
};


"read works with +CMS ERROR <numeric> response code"_test = []
{
    SyncReadStream s("\r\n+CMS ERROR: 500\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(19_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::cms_error == fr.rc);
    expect("500" == fr.text);
};


"read works with +CMS ERROR <verbose text> response code"_test = []
{
    SyncReadStream s("\r\n+CMS ERROR: Operation not supported\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(39_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::cms_error == fr.rc);
    expect("Operation not supported" == fr.text);
};


"read works with +CMS ERROR <numeric> response code followed by extra input"_test = []
{
    SyncReadStream s("\r\n+CMS ERROR: 500\r\n\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(19_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::cms_error == fr.rc);
    expect("500" == fr.text);
};


"read works with COMMAND NOT SUPPORT response code"_test = []
{
    SyncReadStream s("\r\nCOMMAND NOT SUPPORT\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(23_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::command_not_support == fr.rc);
    expect(fr.text.empty());
};


"read works with COMMAND NOT SUPPORT response code followed by extra input"_test = []
{
    SyncReadStream s("\r\nCOMMAND NOT SUPPORT\r\n\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(23_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::command_not_support == fr.rc);
    expect(fr.text.empty());
};


"read works with TOO MANY PARAMETERS response code"_test = []
{
    SyncReadStream s("\r\nTOO MANY PARAMETERS\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(23_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::too_many_parameters == fr.rc);
    expect(fr.text.empty());
};


"read works with TOO MANY PARAMETERS response code followed by extra input"_test = []
{
    SyncReadStream s("\r\nTOO MANY PARAMETERS\r\n\r\nRING\r\n");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(23_u == rb);
    expect(std::holds_alternative<StringFinalResult>(resp));

    auto const fr = std::get<StringFinalResult>(resp);
    expect(Ringbeller::result_code::too_many_parameters == fr.rc);
    expect(fr.text.empty());
};


"read works with the prompt > "_test = []
{
    SyncReadStream s("\r\n> ");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(4_u == rb);
    expect(std::holds_alternative<Ringbeller::prompt>(resp));
};


"read works with +<command> URC"_test = []
{
    SyncReadStream s("\r\n+CREG: 1,\"D509\",\"80D413D\",7");
    boost::asio::streambuf buf;
    Ringbeller::response<Ringbeller::string_body> resp;
    boost::system::error_code ec;

    auto const rb = Ringbeller::read(s, buf, resp, ec);

    expect(4_u == rb);
    expect(std::holds_alternative<StringResponseCommand>(resp));
};


};
