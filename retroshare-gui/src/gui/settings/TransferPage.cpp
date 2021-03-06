/****************************************************************
 *  RetroShare is distributed under the following license:
 *
 *  Copyright (C) 2006 - 2010 RetroShare Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/

#include "TransferPage.h"

#include "rshare.h"

#include <iostream>

#include <retroshare/rsiface.h>
#include <retroshare/rsfiles.h>
#include <retroshare/rspeers.h>

TransferPage::TransferPage(QWidget * parent, Qt::WindowFlags flags)
    : ConfigPage(parent, flags)
{
    /* Invoke the Qt Designer generated object setup routine */
    ui.setupUi(this);

    ui._queueSize_SB->setValue(rsFiles->getQueueSize()) ;

    switch(rsFiles->defaultChunkStrategy())
    {
    case FileChunksInfo::CHUNK_STRATEGY_STREAMING: ui._defaultStrategy_CB->setCurrentIndex(0) ; break ;
    case FileChunksInfo::CHUNK_STRATEGY_PROGRESSIVE: ui._defaultStrategy_CB->setCurrentIndex(1) ; break ;
    case FileChunksInfo::CHUNK_STRATEGY_RANDOM: ui._defaultStrategy_CB->setCurrentIndex(2) ; break ;
    }

    switch(rsFiles->defaultEncryptionPolicy())
    {
    case RS_FILE_CTRL_ENCRYPTION_POLICY_PERMISSIVE: ui._e2e_encryption_CB->setCurrentIndex(0) ; break ;
    case RS_FILE_CTRL_ENCRYPTION_POLICY_STRICT    : ui._e2e_encryption_CB->setCurrentIndex(1) ; break ;
    }

    ui._diskSpaceLimit_SB->setValue(rsFiles->freeDiskSpaceLimit()) ;

    QObject::connect(ui._queueSize_SB,SIGNAL(valueChanged(int)),this,SLOT(updateQueueSize(int))) ;
    QObject::connect(ui._defaultStrategy_CB,SIGNAL(activated(int)),this,SLOT(updateDefaultStrategy(int))) ;
    QObject::connect(ui._e2e_encryption_CB,SIGNAL(activated(int)),this,SLOT(updateEncryptionPolicy(int))) ;
    QObject::connect(ui._diskSpaceLimit_SB,SIGNAL(valueChanged(int)),this,SLOT(updateDiskSizeLimit(int))) ;
    QObject::connect(ui._max_tr_up_per_sec_SB, SIGNAL( valueChanged( int ) ), this, SLOT( updateMaxTRUpRate(int) ) );

    ui._max_tr_up_per_sec_SB->setValue(rsTurtle->getMaxTRForwardRate()) ;
}
void TransferPage::updateMaxTRUpRate(int b)
{
    rsTurtle->setMaxTRForwardRate(b) ;
}

void TransferPage::updateEncryptionPolicy(int b)
{
    switch(b)
    {
    case 1: rsFiles->setDefaultEncryptionPolicy(RS_FILE_CTRL_ENCRYPTION_POLICY_STRICT) ;
        break ;
    default:
    case 0: rsFiles->setDefaultEncryptionPolicy(RS_FILE_CTRL_ENCRYPTION_POLICY_PERMISSIVE) ;
        break ;
    }
}

void TransferPage::updateDefaultStrategy(int i)
{
	switch(i)
	{
		case 0: rsFiles->setDefaultChunkStrategy(FileChunksInfo::CHUNK_STRATEGY_STREAMING) ;
				  break ;

		case 2: rsFiles->setDefaultChunkStrategy(FileChunksInfo::CHUNK_STRATEGY_RANDOM) ;
				  break ;

		case 1: rsFiles->setDefaultChunkStrategy(FileChunksInfo::CHUNK_STRATEGY_PROGRESSIVE) ;
				  break ;
		default: ;
	}
}

void TransferPage::updateDiskSizeLimit(int s)
{
	rsFiles->setFreeDiskSpaceLimit(s) ;
}

void TransferPage::updateQueueSize(int s)
{
	rsFiles->setQueueSize(s) ;
}
