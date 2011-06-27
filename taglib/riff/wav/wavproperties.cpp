/***************************************************************************
    copyright            : (C) 2008 by Scott Wheeler
    email                : wheeler@kde.org
 ***************************************************************************/

/***************************************************************************
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License version   *
 *   2.1 as published by the Free Software Foundation.                     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful, but   *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA         *
 *   02110-1301  USA                                                       *
 *                                                                         *
 *   Alternatively, this file is available under the Mozilla Public        *
 *   License Version 1.1.  You may obtain a copy of the License at         *
 *   http://www.mozilla.org/MPL/                                           *
 ***************************************************************************/

#include "wavproperties.h"

#include <tstring.h>
#include <tdebug.h>
#include <cmath>
#include <math.h>

using namespace TagLib;

class RIFF::WAV::Properties::PropertiesPrivate
{
public:
  PropertiesPrivate(uint streamLength = 0) :
    format(0),
    length(0),
    bitrate(0),
    sampleRate(0),
    channels(0),
    sampleWidth(0),
    sampleFrames(0),
    streamLength(streamLength)
  {

  }

  short format;
  int length;
  int bitrate;
  int sampleRate;
  int channels;
  int sampleWidth;
  uint sampleFrames;
  uint streamLength;
};

////////////////////////////////////////////////////////////////////////////////
// public members
////////////////////////////////////////////////////////////////////////////////

RIFF::WAV::Properties::Properties(const ByteVector &data, ReadStyle style) : AudioProperties(style)
{
  d = new PropertiesPrivate();
  read(data);
}

RIFF::WAV::Properties::Properties(const ByteVector &data, uint streamLength, ReadStyle style) : AudioProperties(style)
{
  d = new PropertiesPrivate(streamLength);
  read(data);
}

RIFF::WAV::Properties::~Properties()
{
  delete d;
}

int RIFF::WAV::Properties::length() const
{
  return d->length;
}

int RIFF::WAV::Properties::bitrate() const
{
  return d->bitrate;
}

int RIFF::WAV::Properties::sampleRate() const
{
  return d->sampleRate;
}

int RIFF::WAV::Properties::channels() const
{
  return d->channels;
}

int RIFF::WAV::Properties::sampleWidth() const
{
  return d->sampleWidth;
}

uint RIFF::WAV::Properties::sampleFrames() const
{
  return d->sampleFrames;
}

////////////////////////////////////////////////////////////////////////////////
// private members
////////////////////////////////////////////////////////////////////////////////

void RIFF::WAV::Properties::read(const ByteVector &data)
{
  d->format = data.mid(0, 2).toShort(false);
  d->channels = data.mid(2, 2).toShort(false);
  d->sampleRate = data.mid(4, 4).toUInt(false);
  d->sampleWidth = data.mid(14, 2).toShort(false);

  uint byteRate = data.mid(8, 4).toUInt(false);
  d->bitrate = byteRate * 8 / 1000;

  d->length = byteRate > 0 ? d->streamLength / byteRate : 0;
  d->sampleFrames = d->streamLength / (d->channels * (d->sampleWidth / 8));
}
