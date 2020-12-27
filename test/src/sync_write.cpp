#include "write.hpp"

#include "messages/at.hpp"
#include "messages/ati.hpp"

#include "boost/ut.hpp"

#include <boost/asio/buffers_iterator.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/streambuf.hpp>

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
struct MockSyncWriteStream
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


using namespace boost::ut;


suite sync_write = []
{


"write works with basic command, no command, no arguments"_test = []
{
    MockSyncWriteStream s;
    auto const cmd = Ringbeller::make_at();

    auto const wb = Ringbeller::write(s, cmd);

    expect(4_u == wb);
    expect("AT\r\n" == s.payload);
};


"write works with basic command, with command, no arguments"_test = []
{
    MockSyncWriteStream s;
    auto const cmd = Ringbeller::make_ati();

    auto const wb = Ringbeller::write(s, cmd);

    expect(5_u == wb);
    expect("ATI\r\n" == s.payload);
};


};
