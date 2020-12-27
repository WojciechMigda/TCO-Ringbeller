#pragma once

#ifndef LIB_INCLUDE_PARSER_HPP_
#define LIB_INCLUDE_PARSER_HPP_

#include "message.hpp"

#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/buffer.hpp>

#include <utility>
#include <string>
#include <vector>


namespace Ringbeller
{


namespace detail
{


using asio_buf_iterator = boost::asio::buffers_iterator<boost::asio::const_buffers_1>;


std::pair<asio_buf_iterator, bool>
match_condition(asio_buf_iterator begin, asio_buf_iterator end);

std::pair<asio_buf_iterator, bool>
parse_response(
    asio_buf_iterator begin,
    asio_buf_iterator end,
    Ringbeller::response<Ringbeller::string_body> & response);


}  // namespace detail


}  // namespace Ringbeller


#endif /* LIB_INCLUDE_PARSER_HPP_ */
