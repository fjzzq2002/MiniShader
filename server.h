#ifndef SERVER_H
#define SERVER_H
#include "vecmath.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <string>
#define ASIO_STANDALONE
//add more defines if necessary
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
struct Server
{
	server m_endpoint;
	typedef void (*func)(Vector3f, Vector3f, Vector3f, float, bool);
	func f;
	std::string init_msg;
	Server(func f,std::string init_msg=""):f(f),init_msg(init_msg) {}
	void echo_handler(websocketpp::connection_hdl hdl, server::message_ptr msg) {
		std::string s = msg->get_payload() + ",", ut;
		if (s == "?," && init_msg != "")
		{
			m_endpoint.send(hdl, init_msg, websocketpp::frame::opcode::TEXT);
			return;
		}
		bool ke = 0;
		if (s[0] == 'M') s.erase(s.begin()), ke = 1;
		std::vector<float> v;
		for (int i = 0; i < (int)s.size(); ++i)
		{
			if (s[i] == ',')
				v.push_back(atof(ut.c_str())),ut="";
			else
				ut.push_back(s[i]);
		}
		if (v.size() != 10) return;
		Vector3f o(v[0], v[1], v[2]);
		Vector3f d(v[3], v[4], v[5]);
		Vector3f u(v[6], v[7], v[8]);
		float dis = v[9];
		f(o, d, u, dis, ke);
		m_endpoint.send(hdl,"shaded", websocketpp::frame::opcode::TEXT);
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