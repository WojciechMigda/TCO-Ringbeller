#include "write.hpp"

#include "messages/at.hpp"
#include "messages/ati.hpp"
#include "messages/atd.hpp"
#include "messages/at_cmgf.hpp"

#include "boost/ut.hpp"

#include <boost/asio/buffers_iterator.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/error.hpp>

#include <string>
#include <iterator>
#include <algorithm>


/*
 * Helper function to string-ify asio buffers
 */
template<class ConstBufferSequence>
std::string
buffers_to_string(ConstBufferSequence const & buffers)
{
    auto bufs = buffers.data();

    std::string result(boost::asio::buffers_begin(bufs), boost::asio::buffers_end(bufs));

    return result;
}


/*
 * Mock SyncWriteStream
 */
struct GoodSyncWriteStream
{
    std::string payload;

    template<typename ConstBufferSequence>
    std::size_t write_some(ConstBufferSequence const & buffers, boost::system::error_code & ec)
    {
        std::string chunk;

        std::copy(
            boost::asio::buffers_begin(buffers), boost::asio::buffers_end(buffers),
            std::back_inserter(chunk));

        payload += chunk;

        return chunk.size();
    }

    template<typename ConstBufferSequence>
    std::size_t write_some(ConstBufferSequence const & buffers)
    {
        boost::system::error_code ec;

        return write_some(buffers, ec);
    }
};


/*
 * Mock SyncWriteStream
 */
struct FailingSyncWriteStream
{
    template<typename ConstBufferSequence>
    std::size_t write_some(ConstBufferSequence const & buffers, boost::system::error_code & ec)
    {
        ec = boost::asio::error::broken_pipe;

        return 0;
    }
};


using namespace boost::ut;


suite sync_write = []
{


"write works with basic command, no command, no arguments"_test = []
{
    GoodSyncWriteStream s;
    auto const cmd = Ringbeller::make_at();

    auto const wb = Ringbeller::write(s, cmd);

    expect(4_u == wb);
    expect("AT\r\n" == s.payload);
};


"write works with basic command, with command, no arguments"_test = []
{
    GoodSyncWriteStream s;
    auto const cmd = Ringbeller::make_ati();

    auto const wb = Ringbeller::write(s, cmd);

    expect(5_u == wb);
    expect("ATI\r\n" == s.payload);
};


"write works with basic command, with command & arguments"_test = []
{
    GoodSyncWriteStream s;
    auto const cmd = Ringbeller::make_atd("112;");

    auto const wb = Ringbeller::write(s, cmd);

    expect(9_u == wb);
    expect("ATD112;\r\n" == s.payload);
};


"write works with 'write' command"_test = []
{
    GoodSyncWriteStream s;
    auto const cmd = Ringbeller::make_at_cmgf_write("1");

    auto const wb = Ringbeller::write(s, cmd);

    expect(11_u == wb);
    expect("AT+CMGF=1\r\n" == s.payload);
};


"write propagates error code"_test = []
{
    FailingSyncWriteStream s;
    auto const cmd = Ringbeller::make_at();
    boost::system::error_code ec;

    auto const wb = Ringbeller::write(s, cmd, ec);

    expect(0_u == wb);
    expect(boost::asio::error::broken_pipe == ec.value());
};


"write throws on error"_test = []
{
    FailingSyncWriteStream s;
    auto const cmd = Ringbeller::make_at();

    expect(throws([&]{ auto const wb = Ringbeller::write(s, cmd); (void)wb; }));
};


};
