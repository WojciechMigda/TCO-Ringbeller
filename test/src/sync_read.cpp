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


};
