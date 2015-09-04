//
//  Server.h
//  Test
//
//  Created by ryan bartley on 9/3/15.
//
//

#pragma once

#include "asio/asio.hpp"

#include "Message.h"

namespace osc {
	
using Listener = std::function<void( Message message, void* userData )>;

class Receiver {
public:
	using ErrorHandler = std::function<void( const std::string & )>;
	
	Receiver( uint16_t port, asio::io_service &io = ci::app::App::get()->io_service() );
	
	Receiver( const Receiver &other ) = delete;
	Receiver& operator=( const Receiver &other ) = delete;
	
	Receiver( Receiver &&other );
	Receiver& operator=( Receiver &&other );
	
	~Receiver();
	
	void		open();
	void		close();
	
	uint16_t	getPort() const { return mPort; }
	void		setPort( uint16_t port );
	
	void		setListener( const std::string &address, Listener listener, void* userData = nullptr );
	void		removeListener( const std::string &address );
	
	void		setErrorHandler( ErrorHandler errorHandler ) { mErrorHandler = errorHandler; }
	
	void		setBufferSize( size_t bufferSize );
	
	
	const asio::ip::udp::socket&	getSocket() { return mSocket; }
	asio::ip::udp::endpoint			getSocketEndpoint() { return mSocket.local_endpoint(); }
	
protected:
	
	void listen();
	void receiveHandler( const asio::error_code &error, std::size_t bytesTransferred );
	
	void dispatchMethods( uint8_t *data, uint32_t size );
	bool decodeData( uint8_t *data, uint32_t size, std::vector<Message> &messages, uint64_t timetag = 0 );
	bool decodeMessage( uint8_t *data, uint32_t size, std::vector<Message> &messages, uint64_t timetag = 0 );
	
	bool patternMatch( const std::string &lhs, const std::string &rhs );
	
	uint16_t	mPort;
	
	asio::ip::udp::socket		mSocket;
	std::vector<uint8_t>		mBuffer;
	
	struct ListenerInfo {
		Listener listener;
		void* userData;
	};

	std::vector<std::pair<std::string, ListenerInfo>> mListeners;
	ErrorHandler mErrorHandler;
};

}