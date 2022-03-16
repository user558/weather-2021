#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "./httplib.h"
#include <json.hpp>


nlohmann::json get_data() {
	httplib::Client cli("http://api.openweathermap.org");
	auto res = 
		cli.Get("/data/2.5/weather?q=Paris&units=metric&lang=ru&appid=5505529d41453eb92ab4fe485cecc1ad");
	if (res) {
		if (res->status != 200) {
			std::cout << "Status code: " << res->status << std::endl;
		}
	}
	else {
		auto err = res.error();
		std::cout << "Error code: " << err << std::endl;
	}
    
	nlohmann::json j = nlohmann::json::parse(res->body);
	nlohmann::json result; 
	result["name"] = j["name"];
	result["weather"] = j["weather"][0]["description"];
	double t = j["main"]["temp"];
	result["temp"] = (int)(fabs(t) + 0.5) * (t < 0 ? -1 : 1);
	result["icon"] = j["weather"][0]["icon"]; 
	std::cout << result.dump(4) << std::endl;
	return result;
}


void gen_response_normal(httplib::Request const& req, httplib::Response& cli_res) {
	nlohmann::json result = get_data();
	std::string template_description = "{hourly[i].weather[0].description}";
	std::string template_icon = "{hourly[i].weather[0].icon}";
	std::string template_temperature = "{hourly[i].temp}";
	std::string content;
	std::ifstream fin("file:///Users/elizavetamalyseva/Desktop/papka/project/weather_cpp_py/src/res/template.html");
	if(!fin.is_open()) {
		std::cout << "Error opening file" << std::endl;
	}
	std::getline(fin, content, '\0');
	content.replace(content.find(template_description), template_description.length(), result["weather"]);
	content.replace(content.find(template_icon), template_icon.length(), result["icon"]);
	content.replace(content.find(template_temperature), template_temperature.length(), 
		std::to_string(result["temp"].get<int>()));
	content.replace(content.find(template_temperature), template_temperature.length(), 
		std::to_string(result["temp"].get<int>()));
	cli_res.set_content(content, "text/html; charset=UTF-8");
}


void gen_response_raw(httplib::Request const& req, httplib::Response& cli_res) {
	nlohmann::json result = get_data();
	cli_res.set_content(result.dump(), "text/plain; charset=UTF-8");
}


int main() 
{
	httplib::Server svr;
	svr.Get("/", gen_response_normal);
	svr.Get("/raw", gen_response_raw);
	std::cout << "Start server... OK\n";
	svr.listen("localhost", 3000);
}
