#include <boost/asio/associated_executor.hpp>
#include <boost/asio/this_coro.hpp>
#include <fstream>
#include <iostream>
#include <qmtest/test-login.h>
#include <qqmusic/login.h>
#include <qqmusic/result.h>
#include <qqmusic/utils/async-executor.h>

namespace qqmusic::test {

static qqmusic::Task<bool> qq_qr_login();

static qqmusic::Task<bool> wx_qr_login();

qqmusic::Task<bool> test_qr_login(qqmusic::QRLoginType type) {
    if (type == QRLoginType::QQ) {
        auto res = co_await qq_qr_login();
        co_return res;
    } else {
        auto res = co_await wx_qr_login();
        co_return res;
    }
}

qqmusic::Task<bool> test_phone_login() {
    std::string phone_num;
    std::cout << "Input Your Phone Number: +86 ";
    std::cin >> phone_num;
    auto res = co_await send_authcode(phone_num);
    if (res.isErr()) {
        std::cout << "Cannot send auth code: " << res.unwrapErr().what() << std::endl;
        co_return true;
    }
    auto phone_login_result = res.unwrap();
    auto event = phone_login_result.event;
    if (event == PhoneLoginEvent::CAPTCHA) {
        std::cout << "Need captcha, please open this url in browser: " << phone_login_result.msg
                  << std::endl;
        co_return true;
    } else if (event == PhoneLoginEvent::FREQUENCY) {
        std::cout << "Operation too frequetly" << std::endl;
        co_return true;
    }
    std::cout << "Auth code is sent successfully" << std::endl;
    std::cout << "Please enter the auth code >> ";
    std::string auth_code;
    std::cin >> auth_code;

    auto login_result = co_await phone_authorize(phone_num, auth_code);
    if (login_result.isErr()) {
        if (login_result.unwrapErr().get_error_enum() == utils::Exception::LoginError) {
            std::cout << "Login Error: " << login_result.unwrapErr().what() << std::endl;
            co_return true;
        }
        auto err = login_result.unwrapErr();
        std::cout << "Error: kind = " << err.kind() << ", reason: " << err.what() << std::endl;
        co_return true;
    } else {
        std::cout << "Login success -- your musicid: " << login_result.unwrap().musicid
                  << std::endl;
        std::cout << "Full Credential: " << std::endl;
        std::cout << login_result.unwrap().to_string().unwrap() << std::endl;
        co_return false;
    }
}

static qqmusic::Task<bool> qq_qr_login() {
    auto qr_res = co_await get_qrcode(QRLoginType::QQ);
    if (qr_res.isErr()) {
        std::cout << "Cannot get QR Code" << std::endl;
        std::cout << qr_res.unwrapErr().what() << std::endl;
        co_return true;
    }

    auto qr = qr_res.unwrap();
    std::cout << "Mimie Type is: " << qr.mimie_type << std::endl;
    std::cout << "Identifier is: " << qr.identifier << std::endl;
    std::string file_name = qr.identifier + ".png";
    std::cout << "QRcode saved to file ./" << file_name << std::endl;

    try {
        std::fstream fs;
        fs.open(file_name, std::ios::binary | std::ios::out);
        fs.write((char*) qr.data.data(), (long) qr.data.size());
        fs.close();
    } catch (const std::exception& e) {
        std::cout << "Failed to save QR Code: " << e.what() << std::endl;
    }

    while (true) {
        auto check_res = co_await check_qq_qr(qr);
        if (check_res.isErr()) {
            auto err = check_res.unwrapErr();
            std::cout << "Check QR Code status error:" << std::endl;
            std::cout << "Type: " << err.kind() << std::endl;
            std::cout << "Reason: " << err.what() << std::endl;
            co_return true;
        }

        auto check = check_res.unwrap();
        std::cout << "Current status: " << check.status.name() << std::endl;
        if (check.status == QRCodeLoginEvent::Status::DONE) {
            std::cout << "Login success -- your musicid is: " << check.credential->musicid
                      << std::endl;
            std::cout << "Full Credential: " << std::endl;
            std::cout << check.credential->to_string().unwrap() << std::endl;
            co_return false;
        } else if (check.status == QRCodeLoginEvent::Status::TIMEOUT) {
            std::cout << "QRCode is expired, please get QR Code again" << std::endl;
            co_return true;
        } else if (check.status == QRCodeLoginEvent::Status::SCAN) {
            /*sleep 5s*/
            auto executor = boost::asio::get_associated_executor(boost::asio::this_coro::executor);
            boost::asio::steady_timer timer(executor.context(), std::chrono::seconds(5));
            co_await timer.async_wait(boost::asio::use_awaitable);
        } else {
            /*sleep 2s*/
            auto executor = boost::asio::get_associated_executor(boost::asio::this_coro::executor);
            boost::asio::steady_timer timer(executor.context(), std::chrono::seconds(2));
            co_await timer.async_wait(boost::asio::use_awaitable);
        }
    }
}

static qqmusic::Task<bool> wx_qr_login() {
    auto qr_res = co_await get_qrcode(QRLoginType::WX);
    if (qr_res.isErr()) {
        std::cout << "Cannot get QR Code" << std::endl;
        std::cout << qr_res.unwrapErr().what() << std::endl;
        co_return true;
    }

    auto qr = qr_res.unwrap();
    std::cout << "Mimie Type is: " << qr.mimie_type << std::endl;
    std::cout << "Identifier is: " << qr.identifier << std::endl;
    std::string file_name = qr.identifier + ".jpg";
    std::cout << "QRcode saved to file ./" << file_name << std::endl;

    try {
        std::fstream fs;
        fs.open(file_name, std::ios::binary | std::ios::out);
        fs.write((char*) qr.data.data(), (long) qr.data.size());
        fs.close();
    } catch (const std::exception& e) {
        std::cout << "Failed to save QR Code: " << e.what() << std::endl;
    }

    while (true) {
        auto check_res = co_await check_wx_qr(qr);
        if (check_res.isErr()) {
            auto err = check_res.unwrapErr();
            std::cout << "Check QR Code status error:" << std::endl;
            std::cout << "Type: " << err.kind() << std::endl;
            std::cout << "Reason: " << err.what() << std::endl;
            co_return true;
        }

        auto check = check_res.unwrap();
        if (check.status == QRCodeLoginEvent::Status::DONE) {
            std::cout << "Login success -- your musicid is: " << check.credential->musicid
                      << std::endl;
            std::cout << "Full Credential: " << std::endl;
            std::cout << check.credential->to_string().unwrap() << std::endl;
            co_return false;
        } else if (check.status == QRCodeLoginEvent::Status::TIMEOUT) {
            std::cout << "QRCode is expired, please get QR Code again" << std::endl;
            co_return true;
        } else if (check.status == QRCodeLoginEvent::Status::SCAN) {
            /*sleep 5s*/
            std::cout << "Waiting for scanning" << std::endl;
            auto executor = boost::asio::get_associated_executor(boost::asio::this_coro::executor);
            boost::asio::steady_timer timer(executor.context(), std::chrono::seconds(5));
            co_await timer.async_wait(boost::asio::use_awaitable);
        } else {
            /*sleep 2s*/
            auto executor = boost::asio::get_associated_executor(boost::asio::this_coro::executor);
            boost::asio::steady_timer timer(executor.context(), std::chrono::seconds(2));
            co_await timer.async_wait(boost::asio::use_awaitable);
        }
    }
    co_return false;
}

} // namespace qqmusic::test
