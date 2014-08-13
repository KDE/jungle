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

#include "../src/interfaces/irun.h"
#include "../src/jungleinitiator.h"
#include "../src/jungleconfig.h"

#include "gmock/gmock.h"

#include <QTest>
#include <QDebug>

#include <KConfig>

class MockRun : public IRun
{
public:
    MOCK_METHOD0(start, void());
};

class JungleInitiatorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testInit();
};

void JungleInitiatorTest::testInit()
{
    MockRun firstRun, normalRun;

    JungleConfig *config = new JungleConfig();
    config->setFirstRun(true);

    JungleInitiator sut(&firstRun, &normalRun, config);

    EXPECT_CALL(firstRun, start()).Times(1);
    sut.init();

    config->setFirstRun(false);
    EXPECT_CALL(normalRun, start()).Times(0);
}

QTEST_GMOCK_MAIN(JungleInitiatorTest)

#include "jungleinitiatortest.moc"