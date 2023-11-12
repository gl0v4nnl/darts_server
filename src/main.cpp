#include "sdk.h"
//
#include <boost/asio/signal_set.hpp>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "http_server.h"
#include "engine/player.h"
#include "engine/round.h"
#include "engine/game.h"

namespace {
namespace net = boost::asio;
using namespace std::literals;
namespace sys = boost::system;
namespace http = boost::beast::http;

using StringRequest = http::request<http::string_body>;     // Запрос, тело которого представлено в виде строки
using StringResponse = http::response<http::string_body>;   // Ответ, тело которого представлено в виде строки

struct ContentType {
    ContentType() = delete;
    constexpr static std::string_view TEXT_HTML = "text/html"sv;
    // При необходимости внутрь ContentType можно добавить и другие типы контента
};

// Создаёт StringResponse с заданными параметрами
StringResponse MakeStringResponse(http::status status, std::string_view body, unsigned http_version,
                                  bool keep_alive,
                                  std::string_view content_type = ContentType::TEXT_HTML) {
    StringResponse response(status, http_version);
    response.set(http::field::content_type, content_type);
    response.body() = body;
    response.content_length(body.size());
    response.keep_alive(keep_alive);
    return response;
}

#include <fstream>
#include <sstream>

StringResponse HandleRequest(StringRequest&& req) {
    // Подставьте сюда код из синхронной версии HTTP-сервера
    const auto text_response = [&req](http::status status, std::string_view text) {
        return MakeStringResponse(status, text, req.version(), req.keep_alive());
    };

    std::string ret(req.target().begin() + 1, req.target().length() - 1);
    std::string resp;

    if (ret.length() == 0) {
        resp = "Hello"sv;
    } else {
        resp = "Hello, "sv;
        resp.insert(resp.length(), ret.c_str());
    }

    // Read the HTML file
    std::ifstream file("../data/main_page.html");
    if (!file.is_open()) {
        // Handle the error if the file cannot be opened
        // return an appropriate response
    }

    // Read the contents of the file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string htmlContent = buffer.str();

    // Replace the content of the Name field
    std::string name = resp; // Replace with the desired name
    size_t namePos = htmlContent.find("{{Name}}");
    if (namePos != std::string::npos) {
        htmlContent.replace(namePos, std::string("{{Name}}").length(), name);
    }

    // Create the response with the modified HTML content
    StringResponse response = MakeStringResponse(http::status::ok, htmlContent, req.version(), req.keep_alive());

    return response;
}
/*
StringResponse HandleRequest(StringRequest&& req) {
    // Подставьте сюда код из синхронной версии HTTP-сервера
    const auto text_response = [&req](http::status status, std::string_view text) {
        return MakeStringResponse(status, text, req.version(), req.keep_alive());
    };
    
    // Здесь можно обработать запрос и сформировать ответ
    std::string ret(req.target().begin() + 1, req.target().length() - 1);
    std::string response;

    if (ret.length() == 0) {
        response = "Hello"sv;
    } else {
        response = "Hello, "sv;
        response.insert(response.length(), ret.c_str());
    }

    std::cout << "[HandleRequest] " << ret.c_str() << std::endl;

    return text_response(http::status::ok, response.data());
}
*/
// Запускает функцию fn на n потоках, включая текущий
template <typename Fn>
void RunWorkers(unsigned n, const Fn& fn) {
    // std::cout << "[RunWorkers] " << n << " threads started" << std::endl;
    n = std::max(1u, n);
    std::vector<std::jthread> workers;
    workers.reserve(n - 1);
    // Запускаем n-1 рабочих потоков, выполняющих функцию fn
    while (--n) {
        workers.emplace_back(fn);
    }
    fn();
}

}  // namespace

int main() {
    const unsigned num_threads = std::thread::hardware_concurrency();

    net::io_context ioc(num_threads);

    // Подписываемся на сигналы и при их получении завершаем работу сервера
    net::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait([&ioc](const sys::error_code& ec, [[maybe_unused]] int signal_number) {
        if (!ec) {
            std::cout << "\nSignal "sv << signal_number << " received"sv << std::endl;
            ioc.stop();
        }
    });

    const auto address = net::ip::make_address("0.0.0.0");
    constexpr net::ip::port_type port = 8080;

    http_server::ServeHttp(ioc, {address, port}, [](auto&& req, auto&& sender) {
        // std::cout << "[main][ServeHttp]" << std::endl;
        sender(HandleRequest(std::forward<decltype(req)>(req)));
    });

    // Эта надпись сообщает тестам о том, что сервер запущен и готов обрабатывать запросы
    std::cout << "Server has started..."sv << std::endl;

    RunWorkers(num_threads, [&ioc] {
        ioc.run();
    });
}
