/*
 * xmltv.cpp
 *
 * Copyright (C) 2019 Mauro Carvalho Chehab <mchehab+samsung@kernel.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "log.h"

#include <KLocalizedString>
#include <QFile>
#include <QLocale>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QXmlStreamReader>

#include "dvbchannel.h"
#include "dvbepg.h"
#include "dvbmanager.h"
#include "iso-codes.h"
#include "xmltv.h"

#include <QEventLoop>

XmlTv::XmlTv(DvbManager *manager_) : manager(manager_), r(NULL)
{
	channelModel = manager->getChannelModel();
	epgModel = manager->getEpgModel();

	connect(&watcher, &QFileSystemWatcher::fileChanged,
		this, &XmlTv::load);

//	exec();
};

void XmlTv::addFile(QString file)
{
	if (file == "")
		return;

	load(file);
};

void XmlTv::clear()
{
	if (watcher.files().empty())
		return;
	channelMap.clear();
	watcher.removePaths(watcher.files());
};


// This function is very close to the one at dvbepg.cpp
DvbEpgLangEntry *XmlTv::getLangEntry(DvbEpgEntry &epgEntry,
				     QString &code)
{
	DvbEpgLangEntry *langEntry;

	if (!epgEntry.langEntry.contains(code)) {
		DvbEpgLangEntry e;
		epgEntry.langEntry.insert(code, e);
		if (!manager->languageCodes.contains(code)) {
			manager->languageCodes[code] = true;
			emit epgModel->languageAdded(code);
		}
	}
	langEntry = &epgEntry.langEntry[code];

	return langEntry;
}

bool XmlTv::parseChannel(void)
{
	const QString emptyString("");
	QStringRef empty(&emptyString);

	const QXmlStreamAttributes attrs = r->attributes();
	QStringRef channelName = attrs.value("id");
	QList<QString>list;

	while (r->readNextStartElement()) {
		QStringRef name;
		name = r->name();
		if (name == "display-name") {
			QString display = r->readElementText();
			list.append(display);
		}
	}
	channelMap.insert(channelName.toString(), list);
	return true;
}

void XmlTv::parseKeyValues(QHash<QString, QString> &keyValues)
{
	QXmlStreamAttributes attrs;

	while (r->readNextStartElement()) {
		QString key, value;

		attrs = r->attributes();
		key = r->name().toString();
		value = attrs.value("lang").toString();

		keyValues.insert(key, value);
	}
}

QString XmlTv::getValue(QHash<QString, QString> &keyValues, QString key)
{
	QHash<QString, QString>::ConstIterator it;

	it = keyValues.constFind(key);
	if (it == keyValues.constEnd())
		return QString("");

	return *it;
}

bool XmlTv::parseProgram(void)
{
	const QString emptyString("");
	QStringRef empty(&emptyString);

	QXmlStreamAttributes attrs = r->attributes();
	QStringRef channelName = attrs.value("channel");
	QHash<QString, QList<QString>>::ConstIterator it;

	it = channelMap.constFind(channelName.toString());
	if (it == channelMap.constEnd()) {
		qCWarning(logDvb,
			  "Error parsing program: channel %s not found",
			  qPrintable(channelName.toString()));
		return false;
	}

	QList<QString>list = it.value();
	QList<QString>::iterator name;
	bool has_channel = false;

	for (name = list.begin(); name != list.end(); name++) {
		if (channelModel->hasChannelByName(*name)) {
			has_channel = true;
			break;
		}
	}

	if (!has_channel) {
#if 1 // This can be too noisy to keep enabled
		static QString lastNotFound("");
		if (channelName.toString() != lastNotFound) {
			qCWarning(logDvb,
				"Error: channel %s not found at transponders",
				qPrintable(channelName.toString()));
			lastNotFound = channelName.toString();
		}
#endif
		return true; // Not a parsing error
	}

	DvbSharedChannel channel = channelModel->findChannelByName(*name);
	DvbEpgEntry epgEntry;
	DvbEpgLangEntry *langEntry;
	QString start = attrs.value("start").toString();
	QString stop = attrs.value("stop").toString();

	/* Place "-", ":" and spaces to date formats for Qt::ISODate parser */
	start.replace(QRegularExpression("^(\\d...)(\\d)"), "\\1-\\2");
	start.replace(QRegularExpression("^(\\d...-\\d.)(\\d)"), "\\1-\\2");
	start.replace(QRegularExpression("^(\\d...-\\d.-\\d.)(\\d)"), "\\1 \\2");
	start.replace(QRegularExpression("^(\\d...-\\d.-\\d. \\d.)(\\d)"), "\\1:\\2");
	start.replace(QRegularExpression("^(\\d...-\\d.-\\d. \\d.:\\d.)(\\d)"), "\\1:\\2");

	stop.replace(QRegularExpression("^(\\d...)(\\d)"), "\\1-\\2");
	stop.replace(QRegularExpression("^(\\d...-\\d.)(\\d)"), "\\1-\\2");
	stop.replace(QRegularExpression("^(\\d...-\\d.-\\d.)(\\d)"), "\\1 \\2");
	stop.replace(QRegularExpression("^(\\d...-\\d.-\\d. \\d.)(\\d)"), "\\1:\\2");
	stop.replace(QRegularExpression("^(\\d...-\\d.-\\d. \\d.:\\d.)(\\d)"), "\\1:\\2");

	/* Convert formats to QDateTime */
	epgEntry.begin = QDateTime::fromString(start, Qt::ISODate);
	QDateTime end = QDateTime::fromString(stop, Qt::ISODate);
	epgEntry.duration = QTime(0, 0, 0).addSecs(epgEntry.begin.secsTo(end));

	epgEntry.begin.setTimeSpec(Qt::UTC);
	epgEntry.channel = channel;

	QString starRating;
	while (r->readNextStartElement()) {
		QStringRef element;
		QString lang;
		element = r->name();
		if (element == "title") {
			attrs = r->attributes();
			lang = IsoCodes::code2Convert(attrs.value("lang").toString());
			langEntry = getLangEntry(epgEntry, lang);
			if (langEntry->title != "")
				langEntry->title += " ";
			langEntry->title += r->readElementText();
		} else if (element == "sub-title") {
			attrs = r->attributes();
			lang = IsoCodes::code2Convert(attrs.value("lang").toString());
			langEntry = getLangEntry(epgEntry, lang);
			if (langEntry->subheading != "")
				langEntry->subheading += " ";
			langEntry->subheading += r->readElementText();
		} else if (element == "desc") {
			attrs = r->attributes();
			lang = IsoCodes::code2Convert(attrs.value("lang").toString());
			langEntry = getLangEntry(epgEntry, lang);
			if (langEntry->details != "")
				langEntry->details += " ";
			langEntry->details += r->readElementText();
		} else if (element == "rating") {
			QHash<QString, QString> keyValues;

			attrs = r->attributes();
			parseKeyValues(keyValues);

			QString system = attrs.value("system").toString();

			QString value = getValue(keyValues, "value");

			if (system == "" || value == "")
				break;

			if (epgEntry.parental != "")
				epgEntry.parental += ", ";

			epgEntry.parental += "System: " + system + ", rating: " + value;
		} else if (element == "star-rating") {
			QHash<QString, QString> keyValues;

			attrs = r->attributes();
			parseKeyValues(keyValues);

			QString value = getValue(keyValues, "value");

			if (value != "") {
				if (langEntry->details != "")
					langEntry->details += " ";

				starRating += value;
			}
		}
		/* FIXME: add support for other fields:
		 * credits, date, country, episode-num, video
		 * rating, star-rating
		 */
	}

	if (starRating != "") {
			if (langEntry->details != "")
				langEntry->details += "\n\n";
			langEntry->details += "Star rating: " + starRating;
		}

	epgModel->addEntry(epgEntry);

	/*
	 * It is not uncommon to have the same xmltv channel
	 * associated with multiple DVB channels. It happens,
	 * for example, when there is a SD, HD, 4K video
	 * streams associated with the same programs.
	 * So, add entries also for the other channels.
	 */
	for (; name != list.end(); name++) {
		if (channelModel->hasChannelByName(*name)) {
			channel = channelModel->findChannelByName(*name);
			epgEntry.channel = channel;
			epgModel->addEntry(epgEntry);
		}
	}
	return true;
}

bool XmlTv::load(QString file)
{
	bool parseError = false;

	watcher.removePath(file);
	if (file.isEmpty()) {
		qCInfo(logDvb, "File to load not specified");
		return false;
	}

	QFile f(file);
	if (!f.open(QIODevice::ReadOnly)) {
		qCWarning(logDvb,
				"Error opening %s: %s. Will stop monitoring it",
				qPrintable(file),
				qPrintable(f.errorString()));
		return false;
	}

	qCInfo(logDvb, "Reading XMLTV file from %s", qPrintable(file));

	r = new QXmlStreamReader(&f);
	while (!r->atEnd()) {
		const QXmlStreamReader::TokenType t = r->readNext();
		QStringRef name;
		switch (t) {
		case QXmlStreamReader::StartElement:
			name = r->name();
			if (name == "tv") {
				break;
			} else if (name == "channel") {
				if (!parseChannel())
					parseError = true;
			} else if (name == "programme") {
				if (!parseProgram())
					parseError = true;
			} else {
#if 1 // This can be too noisy to keep enabled
				static QString lastNotFound("");
				if (name.toString() != lastNotFound) {
					qCWarning(logDvb,
						"Ignoring tag '%s'",
						qPrintable(name.toString()));
					lastNotFound = name.toString();
				}
#endif
			}
			break;
		default:
			break;
		}
		if (parseError)
			break;
	}

	if (r->error()) {
		qCWarning(logDvb, "XMLTV: error: %s",
			  qPrintable(r->errorString()));
	}

	if (parseError) {
		qCWarning(logDvb, "XMLTV: parsing error");
	}

	f.close();
	watcher.addPath(file);
	return parseError;
}
