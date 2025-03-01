/*----------------------------qmtest/test-login.h--------------------------------
 * login api test functions
 *-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_QMTEST_TEST_LOGIN_H
#define QQMUSIC_QMTEST_TEST_LOGIN_H

#include "qqmusic/result.h"
#include <qqmusic/login.h>

namespace qqmusic::test {

qqmusic::Task<bool> test_qr_login(qqmusic::QRLoginType type);

qqmusic::Task<bool> test_phone_login();

} // namespace qqmusic::test

#endif // !QQMUSIC_QMTEST_TEST_LOGIN_H
