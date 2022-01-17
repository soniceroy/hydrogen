/*
 * Hydrogen
 * Copyright(c) 2002-2008 by Alex >Comix< Cominu [comix@users.sourceforge.net]
 * Copyright(c) 2008-2021 The hydrogen development team [hydrogen-devel@lists.sourceforge.net]
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see https://www.gnu.org/licenses
 *
 */

#ifndef DISK_WRITER_DRIVER_H
#define DISK_WRITER_DRIVER_H

#include <sndfile.h>

#include <inttypes.h>

#include <core/IO/AudioOutput.h>
#include <core/Object.h>

namespace H2Core
{

	void* diskWriterDriver_thread( void *param );
///
/// Driver for export audio to disk
///
/** \ingroup docCore docAudioDriver */
class DiskWriterDriver : public Object<DiskWriterDriver>, public AudioOutput
{
	H2_OBJECT(DiskWriterDriver)
	public:

		unsigned				m_nSampleRate;
		QString					m_sFilename;
		unsigned				m_nBufferSize;
		int						m_nSampleDepth;
		audioProcessCallback	m_processCallback;
		float*					m_pOut_L;
		float*					m_pOut_R;

		DiskWriterDriver( audioProcessCallback processCallback, unsigned nSamplerate, int nSampleDepth );
		~DiskWriterDriver();

		virtual int init( unsigned nBufferSize ) override;

		virtual int connect() override;
		virtual void disconnect() override;

		void write();

		virtual unsigned getBufferSize() override {
			return m_nBufferSize;
		}

		virtual unsigned getSampleRate() override;
		
		virtual float* getOut_L() override {
			return m_pOut_L;
		}
		virtual float* getOut_R() override {
			return m_pOut_R;
		}
		
		void  setFileName( const QString& sFilename ){
			m_sFilename = sFilename;
		}

	private:


};

};

#endif
