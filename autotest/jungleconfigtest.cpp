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

#include "../src/interfaces/IInitiator.h"
#include "../src/jungleapp.h"
#include "../src/jungleconfig.h"

#include <QTest>
#include <QDebug>

#include <KConfig>
#include <KSharedConfig>

class JungleConfigTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testIsFirstRun();
};

void JungleConfigTest::testIsFirstRun()
{
    JungleConfig *config = new JungleConfig();
    QCOMPARE(config->isFirstRun(), true);
}

QTEST_MAIN(JungleConfigTest)

#include "jungleconfigtest.moc"