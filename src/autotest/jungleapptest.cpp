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

#include "../abstractinitiator.h"
#include "../jungleapp.h"

#include "gmock/gmock.h"

#include <QTest>
#include <QDebug>

#include <KConfig>

class MockInitiator : public AbstractInitiator
{
public:
    MOCK_METHOD1(init, void(KConfig *));
};

class JungleAppTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTest();
};

void JungleAppTest::initTest()
{
    KConfig *config = new KConfig();
    MockInitiator initiator;
    EXPECT_CALL(initiator, init(config)).Times(1);

    JungleApp app(config, &initiator);

    app.init();
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

QTEST_GMOCK_MAIN(JungleAppTest)

#include "jungleapptest.moc"