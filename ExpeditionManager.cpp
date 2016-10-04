#include "ExpeditionManager.h"
#include "kansavedata.h"
#include "ControlManager.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>

//#include <QDebug>

#define SPSHIP_AKIGUMO	4767
#define SPSHIP_YUGUMO	5012
#define SPSHIP_MAKIGUMO	1703
#define SPSHIP_NAGANAMI	4774
#define SPSHIP_FUMITSUKI	34
#define SPSHIP_NAGATSUKI	157
#define SPSHIP_KIKUZUKI	3142
#define SPSHIP_MIKAZUKI	3175

ExpeditionManager::ExpeditionManager()
{
	_expeditionDefineFileName = QApplication::applicationDirPath() + "/action/" + "expedition.txt";
}

void ExpeditionManager::BuildByPreset(ExpeditionPreset preset)
{
	Clear();
	switch (preset)
	{
		////////////////////////////////////////////////////////////////////////
	case ExpeditionPreset::General:
	{
		ExpeditionSchedule* pschedule = NULL;
		SingleExpedition exp;
		// first team toukyu1 -> padding 30 -> 3last
		{
			pschedule = &(_schedules[0]);
			BuildSingleTokyu1(pschedule, 22, 29);
			BuildSingle30(pschedule, 22, 30);
			BuildSingle3last(pschedule, -1, -1);
		}
		// second team 30->2last
		{
			pschedule = &(_schedules[1]);
			BuildSingle30(pschedule, 19, 0);			
			BuildSingle2last(pschedule, -1, -1);
		}
		// third team toukyu2 -> padding40 -> 5last
		{
			pschedule = &(_schedules[2]);
			BuildSingleTokyu2(pschedule, 22, 29);
			BuildSingle40(pschedule, 22, 30);
			BuildSingle5last(pschedule, -1, -1);
		}
	}
	break;
	case ExpeditionPreset::Fuel:
	{
		ExpeditionSchedule* pschedule = NULL;
		SingleExpedition exp;
		// first team 20 -> tokyuu1 ->padding 20 -> 3last
		{
			pschedule = &(_schedules[0]);

			BuildSingle20(pschedule, 19, 0);
			BuildSingleTokyu1(pschedule, 22, 29);
			BuildSingle20(pschedule, 22, 30);
			BuildSingle3last(pschedule, -1, -1);
		}
		// second team 2:20 -> padding30 ->2last
		{
			pschedule = &(_schedules[1]);
			BuildSingleNezumiYusou(pschedule, 18, 59);
			BuildSingle30(pschedule, 19, 0);
			BuildSingle2last(pschedule, -1, -1);

		}
		// third team toukyu2 ->padding 40 -> 5last
		{
			pschedule = &(_schedules[2]);
			BuildSingleTokyu2(pschedule, 22, 29);
			BuildSingle40(pschedule, 22, 30);
			BuildSingle5last(pschedule, -1, -1);
		}
	}
	break;
	////////////////////////////////////////////////////////////////////////
	case ExpeditionPreset::RepairAndBauxite:
	{
		ExpeditionSchedule* pschedule = NULL;
		SingleExpedition exp;
		// first team toukyu1 -> padding 30 -> 3last
		{
			pschedule = &(_schedules[0]);
			BuildSingleTokyu1(pschedule, 22, 29);
			BuildSingle30(pschedule, 22, 30);
			BuildSingle3last(pschedule, -1, -1);
		}
		// second team 30->2last
		{
			pschedule = &(_schedules[1]);
			BuildSingle30(pschedule, 19, 0);
			BuildSingle2last(pschedule, -1, -1);

		}
		// third team 40 -> toukyu2 -> padding 40 -> 5last
		{
			pschedule = &(_schedules[2]);
			BuildSingle40(pschedule, 19, 0);
			BuildSingleTokyu2(pschedule, 22, 29);
			BuildSingle40(pschedule, 22, 30);
			BuildSingle5last(pschedule, -1, -1);
		}
	}
	break;

	////////////////////////////////////////////////////////////////////////
	case ExpeditionPreset::Bauxite:
	{
		ExpeditionSchedule* pschedule = NULL;
		SingleExpedition exp;
		// first team toukyu1 -> padding 30 -> 3last
		{
			pschedule = &(_schedules[0]);
			BuildSingleTokyu1(pschedule, 22, 29);
			BuildSingle30(pschedule, 22, 30);
			BuildSingle3last(pschedule, -1, -1);
		}
		// second team 40->2last
		{
			pschedule = &(_schedules[1]);
			BuildSingle40(pschedule, 19, 0);
			BuildSingle2last(pschedule, -1, -1);

		}
		// third team bauxite->padding30->bauxite->padding40->5last
		{
			pschedule = &(_schedules[2]);
			BuildSingleBauxiteYusou(pschedule, 16, 29);
			BuildSingle30(pschedule, 16, 30);
			BuildSingleBauxiteYusou(pschedule, 22, 29);
			BuildSingle40(pschedule, 22, 30);
			BuildSingle5last(pschedule, -1, -1);
		}
	}
	break;

	default:
		break;
	}
}

void ExpeditionManager::BuildByPreset(const QString& preset)
{
	if (!ParsePresetBySettingFileAndRebuild())
	{
		_currentPreset = preset;
		if (!preset.compare("Bauxite", Qt::CaseInsensitive))
		{
			BuildByPreset(ExpeditionPreset::Bauxite);
			return;
		}
		else if (!preset.compare("RepairAndBauxite", Qt::CaseInsensitive))
		{
			BuildByPreset(ExpeditionPreset::RepairAndBauxite);
			return;
		}
		else if (!preset.compare("Fuel", Qt::CaseInsensitive))
		{
			BuildByPreset(ExpeditionPreset::Fuel);
			return;
		}
		BuildByPreset(ExpeditionPreset::General);
	}
}

bool ExpeditionManager::ParsePresetBySettingFileAndRebuild()
{
	QFile * file = new QFile(_expeditionDefineFileName);
	if (file)
	{
		if (file->open(QIODevice::ReadOnly | QIODevice::Text))
		{
			bool error = false;

			QTextStream instream(file);

			QString currentPreset;
			QMap<QString, QList<ExpeditionSchedule> > presets;

			QString enterSection;
			int currentTeam = 0;

			while (!instream.atEnd())
			{
				QString line = instream.readLine().trimmed();
				if (line.isEmpty())
				{
					continue;
				}

				if (line.startsWith('@'))
				{
					currentPreset = line.remove(0, 1);
				}
				else if (line.startsWith('#'))
				{
					if (enterSection.isEmpty())
					{
						enterSection = line.remove(0, 1);
						currentTeam = 0;
					}
					else
					{
						enterSection = "";
						currentTeam = 0;
					}
				}
				else
				{
					if (enterSection.isEmpty())
					{
						error = true;
						break;
					}

					//
					QRegExp rx("[\\s:]");
					QStringList strlist = line.split(rx, QString::SkipEmptyParts);

					if (strlist.size() != 3)
					{
						error = true;
						break;
					}

					bool bOk = false;
					int hour = strlist[1].toInt(&bOk);
					if (!bOk)
					{
						error = true;
						break;
					}
					int minute = strlist[2].toInt(&bOk);
					if (!bOk)
					{
						error = true;
						break;
					}

					if (presets[enterSection].empty())
					{
						presets[enterSection].push_back(ExpeditionSchedule());
						presets[enterSection].push_back(ExpeditionSchedule());
						presets[enterSection].push_back(ExpeditionSchedule());
					}
					if (currentTeam > 2)
					{
						error = true;
						break;
					}
					buildSingleByPresetLine(&(presets[enterSection][currentTeam]), strlist[0], hour, minute);

					if (hour < 0 || minute < 0)
					{
						currentTeam++;
					}
				}
			}

			if (error)
			{
				delete file;
				return error;
			}

			if (!error)
			{
				if (currentPreset.isEmpty() || presets[currentPreset].count() != 3)
				{
					error = true;
				}

			}
			if (error)
			{
				delete file;
				return false;
			}

			// replace presets
			_currentPreset = currentPreset;
			_presetSchedules.clear();
			_presetSchedules = presets;
			_schedules.clear();
			_schedules = _presetSchedules[_currentPreset];
			delete file;
			return true;
		}
		delete file;
	}
	return false;
}

void ExpeditionManager::Clear()
{
	_schedules.clear();
	_schedules.append(ExpeditionSchedule());
	_schedules.append(ExpeditionSchedule());
	_schedules.append(ExpeditionSchedule());
	// keep three
}

ExpeditionSchedule* ExpeditionManager::getSchedule(int team)
{
	int index = team - 1;
	if (index >= 0 && index < _schedules.size())
	{
		return &_schedules[index];
	}
	return NULL;
}

SingleExpedition* ExpeditionManager::getShouldNextSchedule(int team, qint64 ct, qint64 bt)
{
	auto pSchedule = getSchedule(team);
	if (!pSchedule)
	{
		dateChange();
		return NULL;
	}

	// time shift
	ct -= _timeShiftMin * 60 * 1000;
	bt -= _timeShiftMin * 60 * 1000;

	QDateTime currentTime = QDateTime::fromMSecsSinceEpoch(ct);
	QDateTime backTime = QDateTime::fromMSecsSinceEpoch(bt);
	QDateTime zeroToday = QDateTime::currentDateTime();
	zeroToday.setTime(QTime(0, 0));

	if (backTime > zeroToday.addDays(1))
	{
		dateChange();
		return NULL;
	}

	QMap<QTime, SingleExpedition>::iterator it = pSchedule->expeditions.begin();
	while (it != pSchedule->expeditions.end()) {
		bool bOverNight = false;

		QTime endTime = it.key();
		if (endTime > QTime(23, 59, 59))
		{
			bOverNight = true;
		}

		QDateTime schEndTime = zeroToday;
		schEndTime.setTime(it.key());

		QTime costTime = it.value().costTime;

		QDateTime estNextBackTime = backTime;
		estNextBackTime = estNextBackTime.addMSecs(costTime.msecsSinceStartOfDay());

//		qDebug() << "estNext" << estNextBackTime.toString("dd:hh:mm:ss.zzz");
//		qDebug() << "schEndTime" << schEndTime.toString("dd:hh:mm:ss.zzz");

		if (estNextBackTime < schEndTime || bOverNight)
		{
			return &(it.value());
		}

		++it;
	}

	dateChange();
	return NULL;
}

void ExpeditionManager::BuildSingle20(ExpeditionSchedule* pschedule, int toHour, int toMin)
{
	SingleExpedition exp;

	exp.Clear();
	exp.addShip((int)ShipType::KuChiKu, false, 0);
	exp.addShip((int)ShipType::KuChiKu, false, 0);
	exp.addShip((int)ShipType::KuChiKu, false, 0);
	exp.addSpecialExcludes(SPSHIP_AKIGUMO);
	exp.addSpecialExcludes(SPSHIP_YUGUMO);
	exp.addSpecialExcludes(SPSHIP_MAKIGUMO);
	exp.addSpecialExcludes(SPSHIP_NAGANAMI);
	exp.addSpecialExcludes(SPSHIP_FUMITSUKI);
	exp.addSpecialExcludes(SPSHIP_NAGATSUKI);
	exp.addSpecialExcludes(SPSHIP_KIKUZUKI);
	exp.addSpecialExcludes(SPSHIP_MIKAZUKI);
	exp.setInfo(QTime(0, 19), 0, 2);

	if (toHour < 0)
	{
		pschedule->addExpedition(QTime(23, 59, 59, 900), exp);
	}
	else
	{
		pschedule->addExpedition(QTime(toHour, toMin), exp);
	}
}

void ExpeditionManager::BuildSingle30(ExpeditionSchedule* pschedule, int toHour, int toMin)
{
	SingleExpedition exp;
	exp.Clear();
	exp.addShip(-SPSHIP_FUMITSUKI, false, 0);
	exp.addShip(-SPSHIP_NAGATSUKI, false, 0);
	exp.addShip(-SPSHIP_KIKUZUKI, false, 0);
	exp.addShip(-SPSHIP_MIKAZUKI, false, 0);
	exp.setInfo(QTime(0, 29), 0, 1);

	if (toHour < 0)
	{
		pschedule->addExpedition(QTime(23, 59, 59, 900), exp);
	}
	else
	{
		pschedule->addExpedition(QTime(toHour, toMin), exp);
	}
}

void ExpeditionManager::BuildSingle40(ExpeditionSchedule* pschedule, int toHour, int toMin)
{
	SingleExpedition exp;

	exp.Clear();
	exp.addShip((int)ShipType::KuChiKu, false, 0);
	exp.addShip((int)ShipType::KuChiKu, false, 0);
	exp.addShip((int)ShipType::KuChiKu, false, 0);
	exp.addShip((int)ShipType::KuChiKu, false, 0);
	exp.addSpecialExcludes(SPSHIP_AKIGUMO);
	exp.addSpecialExcludes(SPSHIP_YUGUMO);
	exp.addSpecialExcludes(SPSHIP_MAKIGUMO);
	exp.addSpecialExcludes(SPSHIP_NAGANAMI);
	exp.addSpecialExcludes(SPSHIP_FUMITSUKI);
	exp.addSpecialExcludes(SPSHIP_NAGATSUKI);
	exp.addSpecialExcludes(SPSHIP_KIKUZUKI);
	exp.addSpecialExcludes(SPSHIP_MIKAZUKI);
	exp.setInfo(QTime(0, 39), 0, 5);

	if (toHour < 0)
	{
		pschedule->addExpedition(QTime(23, 59, 59, 900), exp);
	}
	else
	{
		pschedule->addExpedition(QTime(toHour, toMin), exp);
	}
}

void ExpeditionManager::BuildSingleTokyu1(ExpeditionSchedule* pschedule, int toHour, int toMin)
{
	SingleExpedition exp;
	exp.Clear();
	exp.addShip((int)ShipType::KeiJun, true, 0);
	exp.addShip(-SPSHIP_AKIGUMO, false, 2);
	exp.addShip(-SPSHIP_YUGUMO, false, 2);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addSpecialExcludes(SPSHIP_MAKIGUMO);
	exp.addSpecialExcludes(SPSHIP_NAGANAMI);
	exp.addSpecialExcludes(SPSHIP_FUMITSUKI);
	exp.addSpecialExcludes(SPSHIP_NAGATSUKI);
	exp.addSpecialExcludes(SPSHIP_KIKUZUKI);
	exp.addSpecialExcludes(SPSHIP_MIKAZUKI);
	exp.setInfo(QTime(2, 44), 4, 4);

	if (toHour < 0)
	{
		pschedule->addExpedition(QTime(23, 59, 59, 900), exp);
	}
	else
	{
		pschedule->addExpedition(QTime(toHour, toMin), exp);
	}
}

void ExpeditionManager::BuildSingleTokyu2(ExpeditionSchedule* pschedule, int toHour, int toMin)
{
	SingleExpedition exp;

	exp.Clear();
	exp.addShip(-SPSHIP_MAKIGUMO, false, 3);
	exp.addShip(-SPSHIP_NAGANAMI, false, 3);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addSpecialExcludes(SPSHIP_AKIGUMO);
	exp.addSpecialExcludes(SPSHIP_YUGUMO);
	exp.addSpecialExcludes(SPSHIP_FUMITSUKI);
	exp.addSpecialExcludes(SPSHIP_NAGATSUKI);
	exp.addSpecialExcludes(SPSHIP_KIKUZUKI);
	exp.addSpecialExcludes(SPSHIP_MIKAZUKI);
	exp.setInfo(QTime(2, 54), 4, 5);

	if (toHour < 0)
	{
		pschedule->addExpedition(QTime(23, 59, 59, 900), exp);
	}
	else
	{
		pschedule->addExpedition(QTime(toHour, toMin), exp);
	}
}

void ExpeditionManager::BuildSingle2last(ExpeditionSchedule* pschedule, int toHour, int toMin)
{
	SingleExpedition exp;

	exp.Clear();
	exp.addShip((int)ShipType::KeiJun, true, 0);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::YouRiKu, true, 0);
	exp.addSpecialExcludes(SPSHIP_AKIGUMO);
	exp.addSpecialExcludes(SPSHIP_YUGUMO);
	exp.addSpecialExcludes(SPSHIP_MAKIGUMO);
	exp.addSpecialExcludes(SPSHIP_NAGANAMI);
	exp.addSpecialExcludes(SPSHIP_FUMITSUKI);
	exp.addSpecialExcludes(SPSHIP_NAGATSUKI);
	exp.addSpecialExcludes(SPSHIP_KIKUZUKI);
	exp.addSpecialExcludes(SPSHIP_MIKAZUKI);
	exp.setInfo(QTime(14, 59), 1, 7);

	if (toHour < 0)
	{
		pschedule->addExpedition(QTime(23, 59, 59, 900), exp);
	}
	else
	{
		pschedule->addExpedition(QTime(toHour, toMin), exp);
	}

}

void ExpeditionManager::BuildSingle3last(ExpeditionSchedule* pschedule, int toHour, int toMin)
{
	SingleExpedition exp;
	exp.Clear();
	exp.addShip((int)ShipType::KeiJun, true, 0);
	exp.addShip(-SPSHIP_AKIGUMO, false, 2);
	exp.addShip(-SPSHIP_YUGUMO, false, 2);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::YouRiKu, true, 0);
	exp.addSpecialExcludes(SPSHIP_AKIGUMO);
	exp.addSpecialExcludes(SPSHIP_YUGUMO);
	exp.addSpecialExcludes(SPSHIP_MAKIGUMO);
	exp.addSpecialExcludes(SPSHIP_NAGANAMI);
	exp.addSpecialExcludes(SPSHIP_FUMITSUKI);
	exp.addSpecialExcludes(SPSHIP_NAGATSUKI);
	exp.addSpecialExcludes(SPSHIP_KIKUZUKI);
	exp.addSpecialExcludes(SPSHIP_MIKAZUKI);
	exp.setInfo(QTime(8, 19), 2, 7);

	if (toHour < 0)
	{
		pschedule->addExpedition(QTime(23, 59, 59, 900), exp);
	}
	else
	{
		pschedule->addExpedition(QTime(toHour, toMin), exp);
	}

}

void ExpeditionManager::BuildSingle5last(ExpeditionSchedule* pschedule, int toHour, int toMin)
{
	SingleExpedition exp;

	exp.Clear();
	exp.addShip((int)ShipType::KeiJun, true, 0);
	exp.addShip(-SPSHIP_MAKIGUMO, false, 3);
	exp.addShip(-SPSHIP_NAGANAMI, false, 3);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::SuiBou, true, 0);
	exp.addShip((int)ShipType::SuiBou, true, 0);
	exp.addSpecialExcludes(SPSHIP_AKIGUMO);
	exp.addSpecialExcludes(SPSHIP_YUGUMO);
	exp.addSpecialExcludes(SPSHIP_MAKIGUMO);
	exp.addSpecialExcludes(SPSHIP_NAGANAMI);
	exp.addSpecialExcludes(SPSHIP_FUMITSUKI);
	exp.addSpecialExcludes(SPSHIP_NAGATSUKI);
	exp.addSpecialExcludes(SPSHIP_KIKUZUKI);
	exp.addSpecialExcludes(SPSHIP_MIKAZUKI);
	exp.setInfo(QTime(6, 49), 4, 7);

	if (toHour < 0)
	{
		pschedule->addExpedition(QTime(23, 59, 59, 900), exp);
	}
	else
	{
		pschedule->addExpedition(QTime(toHour, toMin), exp);
	}
}

void ExpeditionManager::BuildSingleNezumiYusou(ExpeditionSchedule* pschedule, int toHour, int toMin)
{
	SingleExpedition exp;

	exp.Clear();
	exp.addShip((int)ShipType::KeiJun, true, 0);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::YouRiKu, true, 0);
	exp.addSpecialExcludes(SPSHIP_AKIGUMO);
	exp.addSpecialExcludes(SPSHIP_YUGUMO);
	exp.addSpecialExcludes(SPSHIP_MAKIGUMO);
	exp.addSpecialExcludes(SPSHIP_NAGANAMI);
	exp.addSpecialExcludes(SPSHIP_FUMITSUKI);
	exp.addSpecialExcludes(SPSHIP_NAGATSUKI);
	exp.addSpecialExcludes(SPSHIP_KIKUZUKI);
	exp.addSpecialExcludes(SPSHIP_MIKAZUKI);
	exp.setInfo(QTime(2, 19), 2, 4);

	if (toHour < 0)
	{
		pschedule->addExpedition(QTime(23, 59, 59, 900), exp);
	}
	else
	{
		pschedule->addExpedition(QTime(toHour, toMin), exp);
	}
}

void ExpeditionManager::BuildSingleBauxiteYusou(ExpeditionSchedule* pschedule, int toHour, int toMin)
{
	SingleExpedition exp;

	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::KuChiKu, true, 1);
	exp.addShip((int)ShipType::YouRiKu, true, 0);
	exp.addSpecialExcludes(SPSHIP_AKIGUMO);
	exp.addSpecialExcludes(SPSHIP_YUGUMO);
	exp.addSpecialExcludes(SPSHIP_MAKIGUMO);
	exp.addSpecialExcludes(SPSHIP_NAGANAMI);
	exp.addSpecialExcludes(SPSHIP_FUMITSUKI);
	exp.addSpecialExcludes(SPSHIP_NAGATSUKI);
	exp.addSpecialExcludes(SPSHIP_KIKUZUKI);
	exp.addSpecialExcludes(SPSHIP_MIKAZUKI);
	exp.setInfo(QTime(4, 59), 1, 2);

	if (toHour < 0)
	{
		pschedule->addExpedition(QTime(23, 59, 59, 900), exp);
	}
	else
	{
		pschedule->addExpedition(QTime(toHour, toMin), exp);
	}
}

void ExpeditionManager::setTimeShiftMin(int min)
{
	_timeShiftMin = min;
	ControlManager* cm = &ControlManager::getInstance();
	if (cm->isExpeditionMode() && !cm->isActiveRunning())
	{
		cm->Terminate();
		if (cm->BuildNext_Expedition())
		{
			cm->StartJob();
		}
	}
}

void ExpeditionManager::buildSingleByPresetLine(ExpeditionSchedule* pschedule, const QString& presetName, int hour, int minute)
{
	if (!presetName.compare("TK1", Qt::CaseInsensitive))
	{
		BuildSingleTokyu1(pschedule, hour, minute);
	}
	else if (!presetName.compare("TK2", Qt::CaseInsensitive))
	{
		BuildSingleTokyu2(pschedule, hour, minute);
	}

	else if (!presetName.compare("20", Qt::CaseInsensitive))
	{
		BuildSingle20(pschedule, hour, minute);
	}
	else if (!presetName.compare("30", Qt::CaseInsensitive))
	{
		BuildSingle30(pschedule, hour, minute);
	}
	else if (!presetName.compare("40", Qt::CaseInsensitive))
	{
		BuildSingle40(pschedule, hour, minute);
	}

	else if (!presetName.compare("2L", Qt::CaseInsensitive))
	{
		BuildSingle2last(pschedule, hour, minute);
	}
	else if (!presetName.compare("3L", Qt::CaseInsensitive))
	{
		BuildSingle3last(pschedule, hour, minute);
	}
	else if (!presetName.compare("5L", Qt::CaseInsensitive))
	{
		BuildSingle5last(pschedule, hour, minute);
	}

	else if (!presetName.compare("NY", Qt::CaseInsensitive))
	{
		BuildSingleNezumiYusou(pschedule, hour, minute);
	}
	else if (!presetName.compare("BY", Qt::CaseInsensitive))
	{
		BuildSingleBauxiteYusou(pschedule, hour, minute);
	}
}

void ExpeditionManager::dateChange()
{
	// date change:
	QString exchangeName;
	if (_currentPreset.startsWith("Pre_"))
	{
		exchangeName = _currentPreset.replace("Pre_", "Post_");
	}
	else if (_currentPreset.startsWith("Post_"))
	{
		_currentPreset = "General";
	}

	if (!exchangeName.isEmpty() &&
		_presetSchedules.contains(exchangeName))
	{
		_currentPreset = exchangeName;
		_schedules.clear();
		_schedules = _presetSchedules[_currentPreset];
		// write to expedition txt


		QFile * file = new QFile(_expeditionDefineFileName);
		if (file)
		{
			QString outstring;
			if (file->open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QTextStream instream(file);
				
				while (!instream.atEnd())
				{
					QString line = instream.readLine().trimmed();

					if (line.startsWith('@'))
					{
						outstring += "@" + _currentPreset + "\n";
					}
					else
					{
						outstring += line + "\n";
					}
				}

				file->close();
			}
			delete file;

			if (!outstring.isEmpty())
			{
				file = new QFile(_expeditionDefineFileName);
				if (file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
				{
					file->seek(0);
					file->write(outstring.toLocal8Bit());
					file->close();
				}
				delete file;
			}
		}
	}
}

void SingleExpedition::Clear()
{
	shipTypes.clear();
	kiraState.clear();
	drumCount.clear();
	specialExcludes.clear();
	costTime = QTime();
}

void SingleExpedition::addShip(int type, bool kira, int drum)
{
	shipTypes.append(type);
	kiraState.append(kira);
	drumCount.append(drum);
}

void SingleExpedition::setInfo(const QTime& cost, int page, int index)
{
	costTime = cost;
	destPage = page;
	destIndex = index;
}

void SingleExpedition::addSpecialExcludes(int shipid)
{
	specialExcludes.append(shipid);
}

QString SingleExpedition::checkMatches(int shipid, int index, int team, int& toShipid, 
	const QList<int>& shipList, const QList<int>& excludeShips)
{
	// return ErrorMessage if error
	// return "" : toShipid == -1 no change
	ControlManager* cm = &ControlManager::getInstance();
	toShipid = -1;

	QString errorIndexString = QString("_T%1_%2").arg(team).arg(index);
	int shiptype = shipTypes[index];

	if (shiptype < 0)
	{
		int id = -shiptype;
		// current one is this ship
		if (shipid == id)
		{
			return "";
		}
		// warning no check on kira
		if (!cm->isShipInDock(id)
			&& !cm->isShipInOtherTeam(id, team)
			&& !cm->isShipDamaged(id)
			&& cm->isShipCharged(id)
			&& cm->hasSlotitem(id, SlotitemType::YuSou, drumCount[index]))
		{
			toShipid = id;
			return "";
		}
		else
		{
			return QString("CurrentShipError")+errorIndexString;
		}
	}
	else
	{
		int shouldChangeCondBorder = 52;
		// if current ship is ok
		if (cm->isShipType(shipid, (ShipType)shiptype)
			&& cm->hasSlotitem(shipid, SlotitemType::YuSou, drumCount[index])
			&& checkDaihatsu(shipid, shiptype))
		{
			if (!kiraState[index])
			{
				// already no kira
				if (cm->getShipCondVal(shipid) <= shouldChangeCondBorder)
				{
					return "";
				}
				else
				{
					bool bChanged = false;
					int minCondVal = std::numeric_limits<int>::max();
					for (int id : shipList)
					{
						// TODO:no check on treated same ship!!
						int condVal = cm->getShipCondVal(id);
						if (cm->isShipType(id, (ShipType)shiptype)
							&& !cm->isShipInDock(id)
							&& !cm->isShipInOtherTeam(id, -1)	// every team
							&& !cm->isShipDamaged(id)
							&& cm->isShipCharged(id)
							&& cm->hasSlotitem(id, SlotitemType::YuSou, drumCount[index])
							&& !cm->hasSlotitem(id, SlotitemType::Sonar)
							&& !cm->hasSlotitem(id, SlotitemType::BaKuRai))
						{
							if (excludeShips.contains(id) || specialExcludes.contains(id))
							{
								continue;
							}
							else  if (!checkDaihatsu(id, shiptype))
							{
								continue;
							}
							else
							{
								if (condVal < minCondVal)
								{
									toShipid = id;
									minCondVal = condVal;
									bChanged = true;
								}
							}
						}
					}
					if (bChanged)
					{
						return "";
					}
					else
					{
						return QString("NoOtherShip") + errorIndexString;
					}
				}
			}
			// need kira
			else
			{
				if (cm->getShipCondVal(shipid) > shouldChangeCondBorder)
				{
					return "";
				}
				else
				{
					// continue to below, no return
				}
			}
		}

		// need change
		bool bChanged = false;
		// no need kira (change for the min Cond)
		if (!kiraState[index])
		{
			int minCondVal = std::numeric_limits<int>::max();
			for (int id : shipList)
			{
				// TODO:no check on treated same ship!!
				int condVal = cm->getShipCondVal(id);
				if (cm->isShipType(id, (ShipType)(shipTypes[index]))
					&& !cm->isShipInDock(id)
					&& !cm->isShipInOtherTeam(id, -1)	// every team
					&& !cm->isShipDamaged(id)
					&& cm->isShipCharged(id)
					&& cm->hasSlotitem(id, SlotitemType::YuSou, drumCount[index])
					&& !cm->hasSlotitem(id, SlotitemType::Sonar)
					&& !cm->hasSlotitem(id, SlotitemType::BaKuRai))
				{
					if (excludeShips.contains(id) || specialExcludes.contains(id))
					{
						continue;
					}
					else  if (!checkDaihatsu(id, shiptype))
					{
						continue;
					}
					else
					{
						if (condVal < minCondVal)
						{
							toShipid = id;
							minCondVal = condVal;
							bChanged = true;
						}
					}
				}
			}
			if (bChanged)
			{
				return "";
			}
			else
			{
				return QString("NoAvailableShip") + errorIndexString;
			}
		}
		// need a kira ship
		else
		{
			for (int id : shipList)
			{
				if (cm->isShipType(id, (ShipType)(shipTypes[index]))
					&& (KanDataCalc::GetCondState(cm->getShipCondVal(id)) == CondState::Kira)
					&& !cm->isShipInDock(id)
					&& !cm->isShipInOtherTeam(id, -1)	// every team
					&& !cm->isShipDamaged(id)
					&& cm->isShipCharged(id)
					&& cm->hasSlotitem(id, SlotitemType::YuSou, drumCount[index])
					&& !cm->hasSlotitem(id, SlotitemType::Sonar)
					&& !cm->hasSlotitem(id, SlotitemType::BaKuRai))
				{
					if (excludeShips.contains(id) || specialExcludes.contains(id))
					{
						continue;
					}
					else  if (!checkDaihatsu(id, shiptype))
					{
						continue;
					}
					else
					{
						toShipid = id;
						bChanged = true;
						break;
					}
				}
			}
			if (bChanged)
			{
				return "";
			}
			else
			{
				return QString("NoAvailableShip") + errorIndexString;
			}
		}
	}

	return QString("Fatal") + errorIndexString;
}

bool SingleExpedition::checkDaihatsu(int shipid, int shiptype)
{
	if (shiptype == (int)ShipType::SuiBou
		|| shiptype == (int)ShipType::YouRiKu)
	{
		if (ControlManager::getInstance().hasSlotitem(shipid, SlotitemType::JouRiKuTei, 3))
		{
			return true;
		}
		return false;
	}
	return true;
}

void ExpeditionSchedule::Clear()
{
	expeditions.clear();
}

void ExpeditionSchedule::addExpedition(const QTime& endTime, const SingleExpedition& exp)
{
	expeditions.insert(endTime, exp);
	// check valid time?
}
