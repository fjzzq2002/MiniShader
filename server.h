#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <string.h>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"
#include "utils.h"
using json = nlohmann::json;
#define ASIO_STANDALONE
//add more defines if necessary
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>


typedef websocketpp::server<websocketpp::config::asio> server;
struct Server
{
	server m_endpoint;
	typedef void (*func)(bool);
	bool k;
	func f;
	PARAMLIST_TYPE v;
	Server(func f, PARAMLIST_TYPE v):f(f),v(v),k(false) {}
	void echo_handler(websocketpp::connection_hdl hdl, server::message_ptr msg) {
		std::cerr << (msg->get_payload()) << std::endl;
		auto j = json::parse(msg->get_payload());
		if (j["type"] == "query")
		{
			json u;
			for (auto t : v)
			{
				const std::pair<ParamType, void*>& p = t.second;
				switch (p.first)
				{
				case PT_INT:
					u["result"][t.first] = int(*((int*)p.second)); break;
				case PT_FLOAT:
					u["result"][t.first] = float(*((float*)p.second)); break;
				case PT_VECTOR3F:
					Vector3f v = Vector3f(*((Vector3f*)p.second));
					u["result"][t.first] = { v[0],v[1],v[2] }; break;
				}
			}
			u["type"] = "query";
			m_endpoint.send(hdl, u.dump().c_str(), websocketpp::frame::opcode::TEXT);
		}
		else if(j["type"] == "shade")
		{
			f(k); k = 1;
			json u;
			u["type"] = "shade";
			u["result"] = "succeed";
			m_endpoint.send(hdl, u.dump().c_str(), websocketpp::frame::opcode::TEXT);
		}
		else if (j["type"] == "edit_parameters")
		{
			k = 0;
			int edt = 0;
			if(j.count("param"))
			for (auto t : v)
			{
				if (!j["param"].count(t.first)) continue;
				++edt;
				const std::pair<ParamType, void*>& p = t.second;
				switch (p.first)
				{
				case PT_INT:
					*((int*)p.second) = (int)j["param"][t.first]; break;
				case PT_FLOAT:
					*((float*)p.second) = (float)j["param"][t.first]; break;
				case PT_VECTOR3F:
					Vector3f tv;
					for (int jj = 0; jj < 3; ++jj) tv[jj] = (float)j["param"][t.first][jj];
					*((Vector3f*)p.second) = tv; break;
				}
			}
			json tmp;
			tmp["type"] = "edit_parameters";
			tmp["result"] = edt;
			m_endpoint.send(hdl, tmp.dump(), websocketpp::frame::opcode::TEXT);
		}
		else
		{
			std::cerr << "unsupported request "<<j["type"]<<"\n";
		}
	}
	void run(int port=9002)
	{
		std::cerr << "starting listening on port " << port << std::endl;
//		m_endpoint.set_error_channels(websocketpp::log::elevel::all);
//		m_endpoint.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);
		m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
		m_endpoint.init_asio();
		m_endpoint.set_message_handler(std::bind(
			&Server::echo_handler, this,
			std::placeholders::_1, std::placeholders::_2
		));
		m_endpoint.listen(port);
		m_endpoint.start_accept();
		m_endpoint.run();
	}
};
#endif