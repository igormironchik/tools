/*
	SPDX-FileCopyrightText: 2011  Andre Somers <andre@familiesomers.nl>
	SPDX-License-Identifier: BSD-3-Clause
*/
/*
Copyright (c) 2011, Andre Somers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Rathenau Instituut, Andre Somers nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANDRE SOMERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef DELAYEDEXECUTIONTIMER_H
#define DELAYEDEXECUTIONTIMER_H

#include <QObject>
class QTimer;

/**
  Class to delay execution an action in response to events that may come in bursts
  */
class DelayedExecutionTimer : public QObject
{
Q_OBJECT
public:
    DelayedExecutionTimer(int maximumDelay = 1000, int minimumDelay = 250, QObject* parent = 0);
    DelayedExecutionTimer(QObject* parent);

    /**
      The minimum delay is the time the class will wait after being triggered before
      emitting the triggered() signals.
      */
    void setMinimumDelay(int delay) {m_minimumDelay = delay;}
    int minimumDelay() const {return m_minimumDelay;}
    /**
      The maximum delay is the maximum time that will pass before a call to the trigger() slot
      leads to a triggered() signal.
      */
    void setMaximumDelay(int delay) {m_maximumDelay = delay;}
    int maximumDelay() const {return m_maximumDelay;}

    /**
      Sets a string to be attached to the end of the last string set using trigger(QString)
      in the triggered(QString) signal. This is useful for, for instance, appending wildcard
      characters to a filter string.
      */
    void setStringPostfix(const QString& postfix) {m_postfix = postfix;}
    /**
      Sets a string to be prepended to the beginning of the last string set using trigger(QString)
      in the triggered(QString) signal. This is useful for, for instance, appending wildcard
      characters to a filter string.
      */
    void setStringPrefix(const QString& prefix) {m_prefix = prefix;}

signals:
    void triggered();
    void triggered(QString);
    void triggered(int);

public slots:
    void trigger();
    void trigger(QString);
    void trigger(int);

private slots:
    void timeout();

private:
    int m_minimumDelay;
    int m_maximumDelay;

    QTimer* m_minimumTimer;
    QTimer* m_maximumTimer;

    QString m_lastString;
    int m_lastInt;

    QString m_prefix;
    QString m_postfix;
};

#endif // DELAYEDEXECUTIONTIMER_H
