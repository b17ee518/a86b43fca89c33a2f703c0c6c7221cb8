#include "ActionCheckAndClickDefine.h"
#include "ControlManager.h"

ActionCheckAndClickDefine::ActionCheckAndClickDefine()
{
}


ActionCheckAndClickDefine::~ActionCheckAndClickDefine()
{
}

bool ActionCheckAndClickDefine::CheckColor(CheckColorNameDefine name)
{
	if (checkColorMap.contains(name))
	{
		const auto& v = checkColorMap[name];
		int c = v.count();
		auto& cm = ControlManager::getInstance();

		cm.moveMouseTo(0, 0);

		if (c < 5)
		{
			return false;
		}
		else if (c < 10)
		{
			return cm.checkColors(v[0], v[1], v[2], v[3], v[4]);
		}
		else if (c < 15)
		{
			return cm.checkColors(v[0], v[1], v[2], v[3], v[4],
				v[5], v[6], v[7], v[8], v[9]);
		}
		else
		{
			return cm.checkColors(v[0], v[1], v[2], v[3], v[4],
				v[5], v[6], v[7], v[8], v[9],
				v[10], v[11], v[12], v[13], v[14]);
		}

		return false;
	}

	return false;
}

void ActionCheckAndClickDefine::MoveAndClick(MoveMouseNameDefine name, int xMul/*=0*/, int yMul/*=0*/)
{
	if (mouseClickMap.contains(name))
	{
		const auto& v = mouseClickMap[name];
		int c = v.count();

		if (c < 2)
		{
			return;
		}
		float x = v[0];
		float y = v[1];
		float offsetX = 3;
		float offsetY = 3;
		float addX = 0;
		float addY = 0;
		if (c > 2)
		{
			offsetX = v[2];
		}
		if (c > 3)
		{
			offsetY = v[3];
		}
		if (c > 4)
		{
			addX = xMul * v[4];
		}
		if (c > 5)
		{
			addY = yMul * v[5];
		}

		ControlManager::getInstance().moveMouseToAndClick(x + addX, y + addY, offsetX, offsetY);
	}
}

QMap < ActionCheckAndClickDefine::CheckColorNameDefine, QList<int>> ActionCheckAndClickDefine::checkColorMap = {
	{
		CheckColorNameDefine::HomePort,
		QList<int> {
			340, 379, 251, 155, 32,
				467, 552, 187, 160, 33
		}
	},
	{
		CheckColorNameDefine::HenseiFirstView,
		QList<int>{
			49, 208, 236, 205, 36,
				81, 525, 202, 193, 187,
				134, 304, 253, 245, 241
		}
	},
	{
		CheckColorNameDefine::HenseiChangeMemberSelectionSortOK,
		QList<int>{
			49, 208, 236, 205, 36,
				1188, 192, 188, 211, 208,
				1174, 198, 56, 139, 136,
		}
	},
	{
		CheckColorNameDefine::HenseiChangeMemberSelectionFilterOK,
		QList<int>{
			49, 208, 236, 205, 36,
				1134, 163, 193, 193, 193,
		}
	},
	{
		CheckColorNameDefine::HenseiChangeMemberSelectionLeftMost,
		QList<int>{
			49, 208, 236, 205, 36,
				762, 674, 62, 196, 197
		}
	},
	{
		CheckColorNameDefine::HenseiChangeMemberSelectionLeftMostMac,
		QList<int>{
			49, 208, 236, 205, 36,
				762, 675, 30, 189, 192
		}
	},
	{
		CheckColorNameDefine::HenseiChangeMemberSelectionLeftMostMac2,
		QList<int>{
			49, 208, 236, 205, 36,
				762, 675, 87, 202, 201
		}
	},
	{
		CheckColorNameDefine::HenseiChangeMemberChangeButton,
		QList<int>{
			49, 208, 236, 205, 36,
				1026, 653, 31, 154, 154
		}
	},
	{
		CheckColorNameDefine::ChargePanel,
		QList<int>{
			1155, 179, 190, 127, 5,
				1073, 118, 31, 120, 122
		}
	},
	{
		CheckColorNameDefine::ChargeChangedToSecondTeam,
		QList<int>{
            278, 177, 59, 177, 175
		}
	},
	{
		CheckColorNameDefine::ChargeNeedChargeButton,
		QList<int>{
			1037, 649, 89, 197, 197,
				1076, 662, 212, 214, 203
		}
	},
	{
		CheckColorNameDefine::ChargeActionDone,
		QList<int>{
			1155, 179, 190, 127, 5,
				991, 528, 167, 165, 154
		}
	},
	{
		CheckColorNameDefine::KousyouPanel,
		QList<int>{
			81, 525, 214, 158, 80,
				447, 376, 141, 74, 52
		}
	},
	{
		CheckColorNameDefine::DestroyPanelReady,
		QList<int>{
			357, 116, 31, 154, 156,
		}
	},
	{
		CheckColorNameDefine::DestroySortDone,
		QList<int>{
			903, 160, 223, 233, 222,
				357, 116, 31, 154, 156
		}
	},
	{
		CheckColorNameDefine::DestroyFirstPage,
		QList<int>{
			440, 676, 29, 189, 192,
				497, 687, 84, 72, 60
		}
	},
	{
		CheckColorNameDefine::DestroyButtonReady,
		QList<int>{
			1078, 633, 63, 187, 187,
				972, 639, 255, 248, 230
		}
	},
	{
		CheckColorNameDefine::DestroyPanelEmptySelected,
		QList<int>{
			1067, 641, 242, 231, 221
		}
	},
	{
		CheckColorNameDefine::NyukyoPanel,
		QList<int>{
			81, 525, 214, 158, 80,
				38, 451, 226, 126, 34,
				1113, 681, 205, 188, 174
		}
	},
	{
		CheckColorNameDefine::NyukyoSelectionReady,
		QList<int>{
			509, 162, 165, 158, 150,
		}
	},
	{
		CheckColorNameDefine::NyukyoSortDone,
		QList<int>{
			509, 162, 165, 158, 150,
				1183, 162, 223, 233, 222
		}
	},
	{
		CheckColorNameDefine::NyukyoFirstPage,
		QList<int>{
			744, 679, 29, 189, 192,
				802, 689, 84, 72, 60
		}
	},
	{
		CheckColorNameDefine::NyukyoButtonReady,
		QList<int>{
			1024, 641, 137, 214, 213,
				949, 665, 228, 231, 229
		}
	},
	{
		CheckColorNameDefine::NyukyoFastToggled,
		QList<int>{
			1060, 430, 48, 133, 134
		}
	},
	{
		CheckColorNameDefine::NyukyoFinalConfirmPage,
		QList<int>{
			806, 602, 35, 158, 159,
				114, 519, 173, 167, 163
		}
	},
	{
		CheckColorNameDefine::SortiePanel,
		QList<int>{
			697, 328, 102, 172, 77,
				1015, 201, 198, 210, 204

		}
	},
	{
		CheckColorNameDefine::SortieMapArea1,
		QList<int>{
			828, 285, 6, 55, 70,
				830, 298, 43, 98, 163
		}
	},
	{
		CheckColorNameDefine::SortieMapArea2,
		QList<int>{
			348, 671, 234, 125, 74,
				538, 268, 85, 111, 87
		}
	},
	{
		CheckColorNameDefine::SortieMapArea3,
		QList<int>{
			443, 680, 231, 97, 16,
				320, 238, 27, 59, 37
		}
	},
	{
		CheckColorNameDefine::SortieMapArea4,
		QList<int>{
			540, 674, 231, 97, 16,
		}
	},
	{
		CheckColorNameDefine::SortieMapArea5,
		QList<int>{
			636, 677, 231, 97, 16,
				460, 336, 40, 72, 48
		}
	},
	{
		CheckColorNameDefine::SortieMapArea6,
		QList<int>{
			929, 221, 37, 93, 163,
				387, 241, 18, 69, 31
		}
	},
	{
		CheckColorNameDefine::SortieMapArea7,
		QList<int>{
			826, 676, 231, 97, 16,
				320, 311, 11, 23, 62
		}
	},
	{
		CheckColorNameDefine::SortieMapExArea,
		QList<int>{
		// untested
		182, 415, 226, 43, 42,
			219, 411, 255, 255, 255
		}
	},
	{
	CheckColorNameDefine::SortieBlackBoard,
	QList<int>{
		//676, 124, 173, 149, 109,
			//513, 570, 42, 68, 23
		//676, 124, 164, 141, 104,
			//513, 570, 44, 69, 24
		669, 110, 195, 167, 123,
			738, 562, 255, 122, 192
		}
	},
	{
		CheckColorNameDefine::SortieBlackBoardDone1,
		QList<int>{
			873, 87, 44, 158, 158,
				534, 574, 53, 69, 80
		}
	},
	{
		CheckColorNameDefine::SortieBlackBoardDone2,
		QList<int>{
		// untested?
			643, 121, 197, 170, 125,
				528, 575, 57, 77, 90
	}
	},
	{
		CheckColorNameDefine::SortieOKButton,
		QList<int>{
			1101, 656, 61, 140, 136,
				943, 121, 29, 187, 190
		}
	},
	{
		CheckColorNameDefine::SortieGOButton,
		QList<int>{
			1016, 188, 255, 249, 240,
				947, 680, 153, 43, 41
		}
	},
	{
		CheckColorNameDefine::SortieGOActiveButton,
		QList<int>{
			1016, 188, 255, 249, 240,
				861, 657, 246, 208, 61
		}
	},
	{
		CheckColorNameDefine::SortieGOWithAirebaseButton,
		QList<int>{
			1018, 689, 69, 108, 74,
				822, 691, 81, 189, 84,
	}
	},
	{
		CheckColorNameDefine::SortieTeam1,
		QList<int>{
            554, 170, 57, 175, 174
		}
	},
	{
		CheckColorNameDefine::SortieTeam2,
		QList<int>{
            600, 171, 96, 186, 182
		}
	},
	{
		CheckColorNameDefine::SortieTeam3,
		QList<int>{
            645, 171, 97, 186, 182
		}
	},
	{
		CheckColorNameDefine::SortieTeam4,
		QList<int>{
            689, 171, 55, 174, 173
		}
	},
	{
		CheckColorNameDefine::SortieCommonFormationWithKeikai6A,
		QList<int>{
			613, 267, 0, 123, 125,
				1065, 383, 90, 255, 131,
				1059, 507, 0, 100, 103
		}
	},
	{
		CheckColorNameDefine::SortieCommonFormationWithKeikai6B,
		QList<int>{
		// TODO
				// keikai
				// 1065, 383, 90, 255, 131,
				// 1059, 507, 0, 100, 103
		}
	},
	{
		CheckColorNameDefine::SortieCommonFormationWithKeikai5,
		QList<int>{
			770, 143, 94, 255, 134,
				1065, 383, 90, 255, 131,
				1059, 507, 0, 100, 103
		}
	},
	{
		CheckColorNameDefine::SortieCommonFormation5,
		QList<int>{
			613, 267, 0, 123, 125,
				644, 277, 34, 113, 120,
				597, 266, 202, 191, 67
		}
	},
	{
		CheckColorNameDefine::SortieCommonFormationCombined,
		QList<int>{
			690, 258, 0, 91, 95,
				768, 215, 95, 157, 84,
				904, 241, 117, 90, 13
		}
	},
	{
		CheckColorNameDefine::SortieCommonAirBaseSelection,
		QList<int>{
			209, 86, 40, 126, 41,
				331, 173, 123, 93, 72,
				348, 473, 182, 175, 165
		}
	},
	{
		CheckColorNameDefine::SortieCommonAirBaseSelectionR,
		QList<int>{
			1040, 87, 36, 124, 39,
				1162, 173, 123, 93, 72,
				1183, 381, 190, 182, 173
		}
	},
	{
		CheckColorNameDefine::SortieCommonAirBaseSelectDone,
		QList<int>{
			232, 86, 172, 245, 167,
				331, 173, 123, 93, 72,
		}
	},
	{
		CheckColorNameDefine::SortieCommonAirBaseSelectDoneR,
		QList<int>{
			1068, 88, 172, 245, 167,
				1162, 173, 123, 93, 72,
		}
	},
	{
		CheckColorNameDefine::SortieCommonNightOrNot,
		QList<int>{
			740, 291, 254, 255, 255,
				744, 292, 17, 56, 74,
				787, 426, 121, 254, 251
		}
	},
	{
		CheckColorNameDefine::SortieCommonLeaveOrNot,
		QList<int>{
			710, 419, 180, 198, 205,
				706, 416, 41, 94, 110,
				484, 408, 127, 251, 250
		}
	},
	{
		CheckColorNameDefine::LDAssign1,
		QList<int>{
			78, 129, 190, 183, 173,
				73, 86, 74, 154, 79
		}
	},
	{
		CheckColorNameDefine::LDAssign1R,
		QList<int>{
			908, 132, 192, 183, 175,
				904, 86, 74, 154, 79
		}
	},
	{
		CheckColorNameDefine::LDAssign2,
		QList<int>{
			169, 143, 193, 186, 176,
				74, 93, 87, 148, 83
		}
	},
	{
		CheckColorNameDefine::LDAssign2R,
		QList<int>{
			1001, 142, 191, 184, 175,
				905, 93, 87, 148, 83
		}
	},
	{
		CheckColorNameDefine::LDAssign3,
		QList<int>{
			270, 140, 194, 187, 177,
				72, 79, 72, 135, 69
		}
	},
	{
		CheckColorNameDefine::LDAssign3R,
		QList<int>{
			1101, 140, 194, 187, 177,
				903, 78, 74, 123, 68
		}
	},
    {
        CheckColorNameDefine::LDAssignOnly1,
        QList<int>{
            78, 129, 190, 183, 173,
                73, 86, 75, 145, 75
        }
    },
    {
        CheckColorNameDefine::LDAssignOnly1R,
        QList<int>{
            908, 132, 192, 183, 175,
                904, 86, 76, 145, 75
        }
    },
	{
		CheckColorNameDefine::LDCharge,
		QList<int>{
			430, 589, 232, 247, 250,
				392, 581, 212, 209, 204
		}
	},
	{
		CheckColorNameDefine::LDChargeNormal,
		QList<int>{
			732, 172, 74, 91, 80,
				659, 183, 195, 62, 33
		}
	},
	{
		CheckColorNameDefine::LDChargeTeam1,
		QList<int>{
			911, 180, 183, 176, 167,
				871, 281, 183, 176, 167
		}
	},
	{
		CheckColorNameDefine::LDChargeTeam2,
		QList<int>{
			1012, 184, 174, 167, 157,
				871, 281, 183, 176, 167
		}
	},
	{
		CheckColorNameDefine::LDChargeTeam3,
		QList<int>{
			1111, 175, 184, 177, 167,
				871, 281, 183, 176, 167
		}
	},
    {
        CheckColorNameDefine::LDChargeTeamOnly1,
        QList<int>{
            911, 180, 101, 84, 64,
                871, 281, 183, 176, 167
        }
    },
	{
		CheckColorNameDefine::LDDoCharge,
		QList<int>{
			1164, 341, 108, 176, 55,
				871, 281, 183, 176, 167
		}
	},
	{
		CheckColorNameDefine::ExpeditionPanel,
        QList<int>{
            763, 173, 219, 112, 27,
                175, 218, 108, 104, 95
		}
	},
	{
		CheckColorNameDefine::ExpeditionSelectItemDone,
		QList<int>{
            957, 666, 103, 169, 166
		}
	},
	{
		CheckColorNameDefine::ExpeditionTeamSelect,
		QList<int>{
			1020, 656, 79, 146, 186,
				1014, 185, 255, 255, 251
		}
	},
	{
		CheckColorNameDefine::ExpeditionTeamSelectActive,
		QList<int>{
			1021, 656, 241, 186, 40,
				1014, 185, 255, 255, 251
		}
	},
	{
		CheckColorNameDefine::ExpeditionTeamSelectActive,
		QList<int>{
			1015, 647, 94, 94, 94,
				1014, 185, 255, 255, 251
		}
	},
	{
		CheckColorNameDefine::MissionSkipCompleteDone,
		QList<int>{
			632, 596, 35, 158, 158,
				626, 598, 255, 252, 246
		}
	},
	{
		CheckColorNameDefine::MissionNonConnecting,
		QList<int>{
			1045, 707, 242, 232, 221
		}
	},
	{
		CheckColorNameDefine::MissionPanel,
		QList<int>{
			323, 118, 184, 160, 85,
				39, 197, 211, 232, 238,
				72, 714, 182, 182, 181
		}
	},
	{
		CheckColorNameDefine::DevelopReadyPanel,
		QList<int>{
			1002, 637, 72, 193, 194,
				395, 509, 178, 122, 51
		}
	},
};

QMap < ActionCheckAndClickDefine::MoveMouseNameDefine, QList<int>> ActionCheckAndClickDefine::mouseClickMap = {
	{
		MoveMouseNameDefine::HenseiButton,
		QList<int> {
			295, 195, 30, 30
		}
	},
	{
		MoveMouseNameDefine::ChargeButton,
		QList<int> {
			150, 343
		}
	},
	{
		MoveMouseNameDefine::NyukyoButton,
		QList<int> {
			187, 548, 30, 30
		}
	},
	{
		MoveMouseNameDefine::KousyouButton,
		QList<int> {
			407, 546, 30, 30
		}
	},
	{
		MoveMouseNameDefine::SortieButton,
		QList<int> {
			293, 389, 54, 47
		}
	},
	{
		MoveMouseNameDefine::PortButtonInPanel,
		QList<int> {
			108, 377, 15, 30
		}
	},
	{
		MoveMouseNameDefine::HenseiFleetTeam,
		QList<int> {
			197, 179, 3, 3
				, 45, 0
		}
	},
	{
		MoveMouseNameDefine::HenseiTeamMemberChangeButton,
		QList<int> {
			615, 323, 25, 5
				, 516, 168
		}
	},
	{
		MoveMouseNameDefine::HenseiRemoveAllButton,
		QList<int> {
			630, 178, 30, 4
		}
	},
	{
		MoveMouseNameDefine::HenseiSortButton,
		QList<int> {
			1167, 199, 8, 3
		}
	},
	{
		MoveMouseNameDefine::HenseiFilterButton,
		QList<int> {
			1133, 163
		}
	},
	{
		MoveMouseNameDefine::HenseiLeftMostButton,
		QList<int> {
			660, 678
		}
	},
	{
		MoveMouseNameDefine::HenseiRightMostButton,
		QList<int> {
			1078, 678
		}
	},
	{
		MoveMouseNameDefine::HenseiRightButton,
		QList<int> {
			1027, 679
		}
	},
	{
		MoveMouseNameDefine::HenseiSecondButton,
		QList<int> {
			814, 678
		}
	},
	{
		MoveMouseNameDefine::HenseiThirdButton,
		QList<int> {
			866, 678
		}
	},
	{
		MoveMouseNameDefine::HenseiFourthButton,
		QList<int> {
			920, 678
		}
	},
	{
		MoveMouseNameDefine::HenseiVerticalMemberSelection,
		QList<int> {
			691, 244, 10, 3,
				0, 43
		}
	},
	{
		MoveMouseNameDefine::HenseiChangeMemberChangeButton,
		QList<int> {
			1040, 669, 30, 10
		}
	},
	{
		MoveMouseNameDefine::ChargeFleetTeam,
		QList<int> {
			222, 179, 3, 3
				, 45, 0
		}
	},
	{
		MoveMouseNameDefine::ChargeFirstMember,
		QList<int> {
			563, 248, 20, 5
		}
	},
	{
		MoveMouseNameDefine::ChargeAllMember,
		QList<int> {
			180, 181, 5, 5
		}
	},
	{
		MoveMouseNameDefine::ChargeDoChargeButton,
		QList<int> {
			1056, 659, 40, 10
		}
	},
	{
		MoveMouseNameDefine::KousyouDestroyButton,
		QList<int> {
			354, 386, 60, 20
		}
	},
	{
		MoveMouseNameDefine::DestroySortButton,
		QList<int> {
			880, 166, 15, 5
		}
	},
	{
		MoveMouseNameDefine::DestroyLeftMost,
		QList<int> {
			339, 678
		}
	},
	{
		MoveMouseNameDefine::DestroyRightMost,
		QList<int> {
			759, 678
		}
	},
	{
		MoveMouseNameDefine::DestroyRightButton,
		QList<int> {
			703, 678
		}
	},
	{
		MoveMouseNameDefine::DestroySecondButton,
		QList<int> {
			493, 678
		}
	},
	{
		MoveMouseNameDefine::DestroyThirdButton,
		QList<int> {
			546, 678
		}
	},
	{
		MoveMouseNameDefine::DestroyFourthButton,
		QList<int> {
			601, 678
		}
	},
	{
		MoveMouseNameDefine::DestroyVerticalMember,
		QList<int> {
			450, 204, 3, 3,
				0, 47
		}
	},
	{
		MoveMouseNameDefine::DestroyDoDestroyButton,
		QList<int> {
			1056, 659, 40, 10
		}
	},
	{
		MoveMouseNameDefine::NyukyoVerticalSlot,
		QList<int> {
			375, 243, 63, 18,
				0, 123
		}
	},
	{
		MoveMouseNameDefine::NyukyoSortButton,
		QList<int> {
			1158, 166, 15, 5
		}
	},
	{
		MoveMouseNameDefine::NyukyoLeftMost,
		QList<int> {
			645, 678
		}
	},
	{
		MoveMouseNameDefine::NyukyoRightMost,
		QList<int> {
			1063, 678
		}
	},
	{
		MoveMouseNameDefine::NyukyoRightButton,
		QList<int> {
			1008, 678
		}
	},
	{
		MoveMouseNameDefine::NyukyoSecondButton,
		QList<int> {
			797, 678
		}
	},
	{
		MoveMouseNameDefine::NyukyoThirdButton,
		QList<int> {
			850, 678
		}
	},
	{
		MoveMouseNameDefine::NyukyoFourthButton,
		QList<int> {
			904, 678
		}
	},
	{
		MoveMouseNameDefine::NyukyoVerticalMember,
		QList<int> {
			781, 210, 3, 3,
				0, 46
		}
	},
	{
		MoveMouseNameDefine::NyukyoDoNyukyoButton,
		QList<int> {
			1023, 659, 40, 10
		}
	},
	{
		MoveMouseNameDefine::NyukyoToggleFastButton,
		QList<int> {
			1101, 433, 40, 10
		}
	},
	{
		MoveMouseNameDefine::NyukyoFinalOKButton,
		QList<int> {
			753, 602, 36, 12
		}
	},
	{
		MoveMouseNameDefine::SortieSortieButton,
		QList<int> {
			354, 328, 87, 71
		}
	},
	{
		MoveMouseNameDefine::SortieSortieButton,
		QList<int> {
			354, 328, 87, 71
		}
	},
	{
		MoveMouseNameDefine::SortieAreaHorizontalButton,
		QList<int> {
			223, 670, 21, 13,
				95, 0
		}
	},
	{
		MoveMouseNameDefine::SortieAreaEventButton,
		QList<int> {
			1112, 672, 11, 10,
		}
	},
	{
		MoveMouseNameDefine::SortieBlackBoardButton,
		QList<int> {
			547, 379, 204, 132,
		}
	},
	{
		MoveMouseNameDefine::SortieOKButton,
		QList<int> {
			1032, 666, 48, 18
		}
	},
	{
		MoveMouseNameDefine::SortieTeamHorizontalButton,
		QList<int> {
			543, 170, 3, 3,
				45, 0
		}
	},
	{
		MoveMouseNameDefine::SortieFinalButton,
		QList<int> {
			946, 651, 81, 9
		}
	},
	{
		MoveMouseNameDefine::SortieSimpleAdvance,
		QList<int> {
			434, 359, 50, 27
		}
	},
	{
        MoveMouseNameDefine::SortieSimpleAdvanceMegami,
		QList<int> {
            434, 579, 50, 40
		}
	},
	{
		MoveMouseNameDefine::SortieCommonLeftButton,
		QList<int> {
			444, 360, 37, 30
		}
	},
	{
		MoveMouseNameDefine::SortieCommonRightButton,
		QList<int> {
			768, 360, 37, 30
		}
	},
	{
		MoveMouseNameDefine::SortieCommonAirBaseOKButton,
		QList<int> {
			158, 90, 50, 10
		}
	},
	{
		MoveMouseNameDefine::SortieCommonAirBaseROKButton,
		QList<int> {
			998, 90, 50, 10
		}
	},
	{
		MoveMouseNameDefine::SortieCommonElseButton,
		QList<int> {
			1093, 624, 22, 22
		}
	},
	{
		MoveMouseNameDefine::LDCharge,
		QList<int> {
			350, 592, 100, 20
		}
	},
	{
		MoveMouseNameDefine::LDChargeNormal,
		QList<int> {
			727, 180, 30, 8
		}
	},
	{
		MoveMouseNameDefine::LDChargeTeam1,
		QList<int> {
			930, 185, 20, 10
		}
	},
	{
		MoveMouseNameDefine::LDChargeTeam2,
		QList<int> {
			1027, 182, 20, 10
		}
	},
	{
		MoveMouseNameDefine::LDChargeTeam3,
		QList<int> {
			1131, 189, 20, 10
		}
	},
	{
		MoveMouseNameDefine::LDDoCharge,
		QList<int> {
			1162, 327, 10, 10
		}
	},
	{
		MoveMouseNameDefine::LDAllChargeDone,
		QList<int> {
			368, 431, 100, 100
		}
	},
	{
		MoveMouseNameDefine::ExpeditionButton,
		QList<int> {
			1012, 328, 87, 71
		}
	},
	{
		MoveMouseNameDefine::ExpeditionAreaHorizontal,
		QList<int> {
            205, 682, 10, 5,
                60, 0
		}
	},
	{
		MoveMouseNameDefine::ExpeditionVertical,
		QList<int> {
            606, 260, 151, 5,
                0, 45
		}
	},
	{
		MoveMouseNameDefine::ExpeditionOKButton,
		QList<int> {
			1032, 666, 48, 18
		}
	},
	{
		MoveMouseNameDefine::ExpeditionTeamHorizontal,
		QList<int> {
			543, 170, 3, 3,
				45, 0
		}
	},
	{
		MoveMouseNameDefine::ExpeditionFinalOKButton,
		QList<int> {
			913, 669, 81, 18
		}
	},
	{
		MoveMouseNameDefine::MissionButton,
		QList<int> {
			841, 77, 18, 6
		}
	},
	{
		MoveMouseNameDefine::MissionSkipOoyodoButton,
		QList<int> {
			1051, 693
		}
	},
	{
		MoveMouseNameDefine::MissionNextPage,
		QList<int> {
			926, 693
		}
	},
	{
		MoveMouseNameDefine::MissionFirstPage,
		QList<int> {
			394, 693
		}
	},
	{
		MoveMouseNameDefine::MissionVertical,
		QList<int> {
			852, 213, 169, 25,
				0, 102
		}
	},
	{
		MoveMouseNameDefine::MissionCloseButton,
		QList<int> {
			600, 600, 25, 5
		}
	},
	{
		MoveMouseNameDefine::MissionReturnButton,
		QList<int> {
			92, 687, 45, 15
		}
	},
	{
		MoveMouseNameDefine::ToDevelopButton,
		QList<int> {
			336, 510, 45, 15
		}
	},
	{
		MoveMouseNameDefine::DoDevelopButton,
		QList<int> {
			1078, 661, 45, 15
		}
	},
	{
		MoveMouseNameDefine::DevelopSkip,
		QList<int> {
			1107, 601
		}
	},
};
