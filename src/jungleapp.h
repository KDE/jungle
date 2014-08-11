/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Àlex Fiestas <afiestas@kde.org>
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

#ifndef JUNGLEAPP_H
#define JUNGLEAPP_H

#include "abstractapp.h"

class KConfig;
class AbstractInitiator;
class JungleApp : public AbstractApp
{
public:
    JungleApp(KConfig *config, AbstractInitiator *initiator);
    virtual ~JungleApp();

    virtual void init();

    KConfig *m_config;
    AbstractInitiator *m_initiator;
};

#endif // JUNGLEAPP_H
