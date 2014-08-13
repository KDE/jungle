/*
 * Copyright (C) 2014 Alejandro Fiestas <afiestas@kde.org>
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

#include "shared.h"

#include "../src/interfaces/IConfig.h"
#include "../src/junglefirstrun.h"

#include "gmock/gmock.h"

#include <QTest>
#include <QDebug>

#include <KConfig>
class MockConfig : public IConfig
{
public:
    virtual bool isFirstRun() const {return true;}

    MOCK_METHOD1(setFirstRun, void(bool isFirstRun));
};

class JungleFirstRunTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void start();
};

void JungleFirstRunTest::start()
{
    MockConfig config;

    JungleFirstRun sut(&config);

    EXPECT_CALL(config, setFirstRun(false)).Times(1);

    sut.start();
}

QTEST_GMOCK_MAIN(JungleFirstRunTest)

#include "junglefirstruntest.moc"