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

#include "../src/interfaces/IInitGui.h"
#include "../src/junglenormalrun.h"

#include "gmock/gmock.h"

#include <QTest>
#include <QDebug>

#include <KConfig>

class MockiInitGui : public IInitGui
{
public:
    MOCK_METHOD0(start, void());
};

class JungleNormalRunTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void start();
};

void JungleNormalRunTest::start()
{
    MockiInitGui iInitGui;

    JungleNormalRun sut(&iInitGui);

    EXPECT_CALL(iInitGui, start()).Times(1);

    sut.start();
}

QTEST_GMOCK_MAIN(JungleNormalRunTest)

#include "junglenormalruntest.moc"