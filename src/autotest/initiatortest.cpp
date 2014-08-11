/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "initiatortest.h"
#include "../initiator.h"
#include "gmock/gmock.h"

#include <QTest>
#include <QDebug>

class MockInitiator : public AbstractInitator {
public:
    MOCK_METHOD0(test, bool());
};

void InitiatorTest::test()
{
    MockInitiator init;
//    EXPECT_CALL(init, test()).Times(1);
    using ::testing::Return;
    EXPECT_CALL(init, test())
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    qDebug() << init.test();
    qDebug() << init.test();
}

#define QTEST_GMOCK_MAIN(TestObject) \
int main(int argc, char *argv[]) \
{ \
    QCoreApplication app(argc, argv); \
    app.setAttribute(Qt::AA_Use96Dpi, true); \
    ::testing::GTEST_FLAG(throw_on_failure) = true; \
    ::testing::InitGoogleMock(&argc, argv); \
    TestObject tc; \
    return QTest::qExec(&tc, argc, argv); \
}

QTEST_GMOCK_MAIN(InitiatorTest)
