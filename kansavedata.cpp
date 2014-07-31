#include "kansavedata.h"

KanSaveData::KanSaveData()
{
    kcsapi_material tm;
    tm.api_value = 0;
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);

    shipcountoffset = 0;
    slotitemcountoffset = 0;
	lastdeckid = 0;

	createshipdata.clearValue();
}

void CreateShipSaveData::setValue(int fuel, int bull, int steel, int bauxite, int dev, int kdock)
{
	usefuel = fuel;
	usebull = bull;
	usesteel = steel;
	usebauxite = bauxite;
	usedevelopment = dev;
	kdockid = kdock;
	flag = 1;
}

void CreateShipSaveData::clearValue()
{
	flag = 0;
}
