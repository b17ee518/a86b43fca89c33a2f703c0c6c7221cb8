#include "kandatacalc.h"

int AdmiralExperienceTable[][3] =
{
	{ 1, 100, 0 },
	{ 2, 200, 100 },
	{ 3, 300, 300 },
	{ 4, 400, 600 },
	{ 5, 500, 1000 },
	{ 6, 600, 1500 },
	{ 7, 700, 2100 },
	{ 8, 800, 2800 },
	{ 9, 900, 3600 },
	{ 10, 1000, 4500 },
	{ 11, 1100, 5500 },
	{ 12, 1200, 6600 },
	{ 13, 1300, 7800 },
	{ 14, 1400, 9100 },
	{ 15, 1500, 10500 },
	{ 16, 1600, 12000 },
	{ 17, 1700, 13600 },
	{ 18, 1800, 15300 },
	{ 19, 1900, 17100 },
	{ 20, 2000, 19000 },
	{ 21, 2100, 21000 },
	{ 22, 2200, 23100 },
	{ 23, 2300, 25300 },
	{ 24, 2400, 27600 },
	{ 25, 2500, 30000 },
	{ 26, 2600, 32500 },
	{ 27, 2700, 35100 },
	{ 28, 2800, 37800 },
	{ 29, 2900, 40600 },
	{ 30, 3000, 43500 },
	{ 31, 3100, 46500 },
	{ 32, 3200, 49600 },
	{ 33, 3300, 52800 },
	{ 34, 3400, 56100 },
	{ 35, 3500, 59500 },
	{ 36, 3600, 63000 },
	{ 37, 3700, 66600 },
	{ 38, 3800, 70300 },
	{ 39, 3900, 74100 },
	{ 40, 4000, 78000 },
	{ 41, 4100, 82000 },
	{ 42, 4200, 86100 },
	{ 43, 4300, 90300 },
	{ 44, 4400, 94600 },
	{ 45, 4500, 99000 },
	{ 46, 4600, 103500 },
	{ 47, 4700, 108100 },
	{ 48, 4800, 112800 },
	{ 49, 4900, 117600 },
	{ 50, 5000, 122500 },
	{ 51, 5200, 127500 },
	{ 52, 5400, 132700 },
	{ 53, 5600, 138100 },
	{ 54, 5800, 143700 },
	{ 55, 6000, 149500 },
	{ 56, 6200, 155500 },
	{ 57, 6400, 161700 },
	{ 58, 6600, 168100 },
	{ 59, 6800, 174700 },
	{ 60, 7000, 181500 },
	{ 61, 7300, 188500 },
	{ 62, 7600, 195800 },
	{ 63, 7900, 203400 },
	{ 64, 8200, 211300 },
	{ 65, 8500, 219500 },
	{ 66, 8800, 228000 },
	{ 67, 9100, 236800 },
	{ 68, 9400, 245900 },
	{ 69, 9700, 255300 },
	{ 70, 10000, 265000 },
	{ 71, 10400, 275000 },
	{ 72, 10800, 285400 },
	{ 73, 11200, 296200 },
	{ 74, 11600, 307400 },
	{ 75, 12000, 319000 },
	{ 76, 12400, 331000 },
	{ 77, 12800, 343400 },
	{ 78, 13200, 356200 },
	{ 79, 13600, 369400 },
	{ 80, 14000, 383000 },
	{ 81, 14500, 397000 },
	{ 82, 15000, 411500 },
	{ 83, 15500, 426500 },
	{ 84, 16000, 442000 },
	{ 85, 16500, 458000 },
	{ 86, 17000, 474500 },
	{ 87, 17500, 491500 },
	{ 88, 18000, 509000 },
	{ 89, 18500, 527000 },
	{ 90, 19000, 545500 },
	{ 91, 20000, 564500 },
	{ 92, 22000, 584500 },
	{ 93, 25000, 606500 },
	{ 94, 30000, 631500 },
	{ 95, 40000, 661500 },
	{ 96, 60000, 701500 },
	{ 97, 90000, 761500 },
	{ 98, 148500, 851500 },
	{ 99, 300000, 1000000 },
	{ 100, 300000, 1300000 },
	{ 101, 300000, 1600000 },
	{ 102, 300000, 1900000 },
	{ 103, 400000, 2200000 },
	{ 104, 400000, 2600000 },
	{ 105, 500000, 3000000 },
	{ 106, 500000, 3500000 },
	{ 107, 600000, 4000000 },
	{ 108, 600000, 4600000 },
	{ 109, 700000, 5200000 },
	{ 110, 700000, 5900000 },
	{ 111, 800000, 6600000 },
	{ 112, 800000, 7400000 },
	{ 113, 900000, 8200000 },
	{ 114, 900000, 9100000 },
	{ 115, 1000000, 11000000 },
	{ 116, 1000000, 12000000 },
	{ 117, 1000000, 13000000 },
	{ 118, 1000000, 14000000 },
	{ 119, 1000000, 15000000 },
	{ 120, 0, 16000000 },
};

int KanDataCalc::GetAdmiralNextLevelExp(int curExp, int curLevel)
{
	if (curLevel < 120)
	{
		int nextexp = AdmiralExperienceTable[curLevel][2] - curExp;
		return nextexp;
	}
	return 0;
}

WoundState KanDataCalc::GetWoundState(int curHP, int maxHP)
{
	double val = (double)curHP / (double)maxHP;
	if (val <= 0.0)
	{
		return WoundState::Dead;
	}
	else if (val <= 0.25)
	{
		return WoundState::Big;
	}
	else if (val <= 0.5)
	{
		return WoundState::Middle;
	}
	else if (val <= 0.75)
	{
		return WoundState::Small;
	}
	else if (val < 1.0)
	{
		return WoundState::Little;
	}
	return WoundState::Full;
}

ChargeState KanDataCalc::GetChargeState(int curVal, int maxVal)
{
	double val = (double)curVal / (double)maxVal;
	if (val <= 0.0)
	{
		return ChargeState::Empty;
	}
	else if (val <= 0.25)
	{
		return ChargeState::Big;
	}
	else if (val <= 0.5)
	{
		return ChargeState::Middle;
	}
	else if (val < 1.0)
	{
		return ChargeState::Small;
	}
	return ChargeState::Full;

}

CondState KanDataCalc::GetCondState(int cond)
{
	if (cond < 20)
	{
		return CondState::Big;
	}
	else if (cond < 30)
	{
		return CondState::Middle;
	}
	else if (cond < 40)
	{
		return CondState::Small;
	}
	else if (cond < 50)
	{
		return CondState::Normal;
	}
	return CondState::Kira;
}

int KanDataCalc::CalcTeamTyku(QList<ShipWithSlotItemsFullData> shipFullDataList, int& minTyku, int& maxTyku, LandBaseState landbaseState /*= LandBaseState::None*/)
{
	minTyku = 0;
	maxTyku = 0;
	int basicTyku = 0;
	double reconBonus = 1.0;

	for (const auto& shipFullData : shipFullDataList)
	{
		const auto* pship = shipFullData.pship;
		const auto* pmstship = shipFullData.pmstship;
		if (pship == NULL || pmstship == NULL)
		{
			continue;
		}

		for (int i = 0; i < shipFullData.slotitemlist.count(); i++)
		{
			const auto* pslotitem = shipFullData.slotitemlist[i];
			const kcsapi_mst_slotitem* pmstslotitem = NULL;
			if (shipFullData.mstslotitemlist.count() > i)
			{
				pmstslotitem = shipFullData.mstslotitemlist[i];
			}

			if (pslotitem == NULL || pmstslotitem == NULL)
			{
				continue;
			}

			if (pship->api_onslot.count() <= i)
			{
				continue;
			}

			int onslot = pship->api_onslot[i];
			if (onslot < 1)
			{
				continue;
			}

			double tempTyku = 0.0;
			int tempAlv = pslotitem->api_alv;

			double levelFactor = pmstslotitem->api_baku	> 0 ? 0.25 : 0.2;

			switch ((SlotitemType)(pmstslotitem->api_type[2]))
			{
			case SlotitemType::KanSen:
			case SlotitemType::KanBaKu:
			case SlotitemType::KanKou:
			case SlotitemType::SuiJouSenTouKi:
			case SlotitemType::RiKuJouKouGeKiKi:
			case SlotitemType::FunSen:
			case SlotitemType::FunSenBaku:
			case SlotitemType::FunKou:
			{
				tempTyku += sqrt((double)onslot) * (pmstslotitem->api_tyku + pslotitem->api_level*levelFactor);
				tempTyku += GetAirCraftLevelBonus((SlotitemType)pmstslotitem->api_type[2], tempAlv);
				basicTyku += (int)(sqrt((double)onslot) * pmstslotitem->api_tyku);
				minTyku += (int)(tempTyku + sqrt((double)GetAirCraftExp(tempAlv) / 10.0));
				maxTyku += (int)(tempTyku + sqrt((double)(GetAirCraftExp(tempAlv + 1) - 1) / 10.0));
			}
			break;
			case SlotitemType::SuiBaKu:
			{
				// todo check?
				tempTyku += sqrt((double)onslot) * pmstslotitem->api_tyku;
				tempTyku += GetAirCraftLevelBonus((SlotitemType)pmstslotitem->api_type[2], tempAlv);
				basicTyku += (int)(sqrt((double)onslot) * pmstslotitem->api_tyku);
				minTyku += (int)(tempTyku + sqrt((double)GetAirCraftExp(tempAlv) / 10.0));
				maxTyku += (int)(tempTyku + sqrt((double)(GetAirCraftExp(tempAlv + 1) - 1) / 10.0));
			}
			break;
			case SlotitemType::KyoKuChiSenTouKi:
			{
				double landbaseBonus = 0.0;
				if (landbaseState == LandBaseState::ShutsuGeki)
				{
					landbaseBonus = 1.5 * pmstslotitem->api_houk;
				}
				else if (landbaseState == LandBaseState::BouKu)
				{
					landbaseBonus = pmstslotitem->api_houk + 2.0 * pmstslotitem->api_houm;
				}
				tempTyku += sqrt((double)onslot) * (pmstslotitem->api_tyku + landbaseBonus + pslotitem->api_level*levelFactor);
				tempTyku += GetAirCraftLevelBonus((SlotitemType)pmstslotitem->api_type[2], tempAlv);
				basicTyku += (int)(sqrt((double)onslot) * pmstslotitem->api_tyku);
				minTyku += (int)(tempTyku + sqrt((double)GetAirCraftExp(tempAlv) / 10.0));
				maxTyku += (int)(tempTyku + sqrt((double)(GetAirCraftExp(tempAlv + 1) - 1) / 10.0));
			}
			break;
			case SlotitemType::SuiTei:
			case SlotitemType::HiKouTei_L:
			{
				if (landbaseState == LandBaseState::BouKu)
				{
					if (pmstslotitem->api_saku >= 9)
					{
						if (reconBonus <= 1.16)
						{
							reconBonus = 1.16;
						}
					}
					else if (pmstslotitem->api_saku == 8)
					{
						if (reconBonus <= 1.13)
						{
							reconBonus = 1.13;
						}
					}
					else
					{
						if (reconBonus <= 1.1)
						{
							reconBonus = 1.1;
						}
					}
				}
				else if (landbaseState == LandBaseState::ShutsuGeki)
				{
					tempTyku += sqrt((double)onslot) * pmstslotitem->api_tyku;
					minTyku += (int)(tempTyku + sqrt((double)GetAirCraftExp(tempAlv) / 10.0));
					maxTyku += (int)(tempTyku + sqrt((double)(GetAirCraftExp(tempAlv + 1) - 1) / 10.0));
				}
			}
			break;
			case SlotitemType::KanJouTeiSaTsu:
			{
				if (landbaseState == LandBaseState::BouKu)
				{
					if (pmstslotitem->api_saku >= 9)
					{
						if (reconBonus < 1.3)
						{
							reconBonus = 1.3;
						}
					}
					else
					{
						if (reconBonus < 1.2)
						{
							reconBonus = 1.2;
						}
					}
				}
			}
			break;
			}
		}
	}

	minTyku = (int)(minTyku * reconBonus);
	maxTyku = (int)(maxTyku * reconBonus);

	return (int)(basicTyku*reconBonus);
}

double KanDataCalc::CalcTeamSakuTeki(QList<ShipWithSlotItemsFullData> shipFullDataList, int teitokuLevel, double mapModifier /*= 1.0*/, int maxSlotCount /*= 6*/)
{
	double totalSaku = 0.0;
	double shipSaku = 0.0;
	double equipSaku = 0.0;
	double teitokuSaku = 0.0;
	int emptySlot = maxSlotCount;

	for (const auto& shipFullData : shipFullDataList)
	{
		const auto* pship = shipFullData.pship;
		const auto* pmstship = shipFullData.pmstship;
		if (pship == NULL || pmstship == NULL)
		{
			continue;
		}

		emptySlot--;
		double shipPureSaku = pship->api_sakuteki[0];

		for (int i = 0; i < shipFullData.slotitemlist.count(); i++)
		{
			const auto* pslotitem = shipFullData.slotitemlist[i];
			const kcsapi_mst_slotitem* pmstslotitem = NULL;
			if (shipFullData.mstslotitemlist.count() > i)
			{
				pmstslotitem = shipFullData.mstslotitemlist[i];
			}

			if (pslotitem == NULL || pmstslotitem == NULL)
			{
				continue;
			}

			shipPureSaku -= pmstslotitem->api_saku;

			switch ((SlotitemType)(pmstslotitem->api_type[2]))
			{
			case SlotitemType::KanKou:
				equipSaku += pmstslotitem->api_saku * 0.8;
				break;
			case SlotitemType::KanJouTeiSaTsu:
				equipSaku += pmstslotitem->api_saku * 1.0;
				break;
			case SlotitemType::SuiTei:
				equipSaku += (pmstslotitem->api_saku + 1.2 * sqrt(pslotitem->api_level)) * 1.2;
				break;
			case SlotitemType::SuiBaKu:
				equipSaku += pmstslotitem->api_saku * 1.1;
				break;
			case SlotitemType::DenTan_S:
				equipSaku += (pmstslotitem->api_saku + 1.25 * sqrt(pslotitem->api_level)) * 0.6;
				break;
			case SlotitemType::DenTan_L:
				equipSaku += (pmstslotitem->api_saku + 1.4 * sqrt(pslotitem->api_level)) * 0.6;
				break;
			default:
				equipSaku += pmstslotitem->api_saku * 0.6;
			}
		}

		shipSaku += sqrt(shipPureSaku);
	}

	equipSaku *= mapModifier;
	teitokuSaku = (int)(ceil(teitokuLevel * 0.4 + 0.5));
	totalSaku = shipSaku + equipSaku - teitokuSaku + 2 * emptySlot;

	return totalSaku;
}

int KanDataCalc::GetAirCraftLevelBonus(SlotitemType slotitemtype, int alv)
{
	static QList<int> kansenbonuslist{ 0, 0, 2, 5, 9, 14, 14, 22, 22 };
	static QList<int> suibakubonuslist{ 0, 1, 1, 1, 1, 3, 3, 6, 6 };

	if (alv < 0)
	{
		return 0;
	}

	switch (slotitemtype)
	{
	case SlotitemType::KanSen:
	case SlotitemType::SuiJouSenTouKi:
	case SlotitemType::KyoKuChiSenTouKi:
	{
		if (alv < kansenbonuslist.count())
		{
			return kansenbonuslist[alv];
		}
	}
	break;

	case SlotitemType::SuiBaKu:
	{
		if (alv < suibakubonuslist.count())
		{
			return suibakubonuslist[alv];
		}
	}
	break;
	}

	return 0;
}

int KanDataCalc::GetAirCraftExp(int alv)
{
	static QList<int> expTable{ 0, 10, 25, 40, 55, 70, 85, 100, 121 };
	if (alv < 0)
	{
		return 0;
	}

	if (alv < expTable.count())
	{
		return expTable[alv];
	}

	return expTable[expTable.count() - 1];
}
