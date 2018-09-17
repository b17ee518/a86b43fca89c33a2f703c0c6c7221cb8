#pragma once

#include <QMap>
#include <QList>
#include "ControlManager.h"

class ActionCheckAndClickDefine
{
public:
	ActionCheckAndClickDefine();
	~ActionCheckAndClickDefine();

	enum class CheckColorNameDefine
	{
		None,

		HomePort,
		HenseiFirstView,

		HenseiChangeMemberSelectionSortOK,
		HenseiChangeMemberSelectionFilterOK,

        HenseiChangeMemberSelectionLeftMost,
        HenseiChangeMemberSelectionLeftMostMac,
        HenseiChangeMemberSelectionLeftMostMac2,

		HenseiChangeMemberChangeButton,

		ChargePanel,
		ChargeChangedToSecondTeam,
		ChargeNeedChargeButton,

		ChargeActionDone,

		KousyouPanel,

		DestroyPanelReady,
		DestroySortDone,

		DestroyFirstPage,
		DestroyButtonReady,

		DestroyPanelEmptySelected,

		NyukyoPanel,

		NyukyoSelectionReady,
		NyukyoSortDone,

		NyukyoFirstPage,
		NyukyoButtonReady,

		NyukyoFastToggled,
		NyukyoFinalConfirmPage,

		SortiePanel,
		SortieMapArea1,
		SortieMapArea2,
		SortieMapArea3,
		SortieMapArea4,
		SortieMapArea5,
		SortieMapArea6,
		SortieMapArea7,

		SortieMapExArea, // untested

		SortieBlackBoard,
		SortieBlackBoardDone1,
		SortieBlackBoardDone2,

		SortieOKButton,

		SortieGOButton,
		SortieGOActiveButton,
		SortieGOWithAirebaseButton,

		SortieTeam1,
		SortieTeam2,
		SortieTeam3,
		SortieTeam4,

		SortieCommonFormationWithKeikai6A,
		SortieCommonFormationWithKeikai6B,
		SortieCommonFormationWithKeikai5,
		SortieCommonFormation5,
		SortieCommonFormationCombined,

        SortieCommonAirBaseSelection,
        SortieCommonAirBaseSelectDone,
        SortieCommonAirBaseSelectionR,
        SortieCommonAirBaseSelectDoneR,

		SortieCommonNightOrNot,
		SortieCommonLeaveOrNot,

		ExpeditionPanel,

		ExpeditionSelectItemDone,
		ExpeditionTeamSelect,
		ExpeditionTeamSelectActive,
		ExpeditionTeamSelectInactive,

		MissionSkipCompleteDone,
		MissionNonConnecting,
		MissionPanel,

		DevelopReadyPanel,
	};

	enum class MoveMouseNameDefine
	{
		None,

		HenseiButton,
		ChargeButton,
		NyukyoButton,
		KousyouButton,
		SortieButton,

		PortButtonInPanel,

		HenseiFleetTeam,
		HenseiTeamMemberChangeButton,
		HenseiRemoveAllButton,

		HenseiSortButton,
		HenseiFilterButton,

		// single selection
		HenseiLeftMostButton,
		HenseiRightMostButton,
		HenseiRightButton,
		HenseiSecondButton,
		HenseiThirdButton,
		HenseiFourthButton,


		HenseiVerticalMemberSelection,

		HenseiChangeMemberChangeButton,

		ChargeFleetTeam,
		ChargeFirstMember,
		ChargeAllMember,

		ChargeDoChargeButton,

		KousyouDestroyButton,
		DestroySortButton,

		DestroyLeftMost,
		DestroyRightMost,
		DestroyRightButton,
		DestroySecondButton,
		DestroyThirdButton,
		DestroyFourthButton,

		DestroyVerticalMember,
		DestroyDoDestroyButton,

		NyukyoVerticalSlot,


		NyukyoSortButton,

		NyukyoLeftMost,
		NyukyoRightMost,
		NyukyoRightButton,
		NyukyoSecondButton,
		NyukyoThirdButton,
		NyukyoFourthButton,

		NyukyoVerticalMember,
		NyukyoDoNyukyoButton,

		NyukyoToggleFastButton,

		NyukyoFinalOKButton,

		SortieSortieButton,
		SortieAreaHorizontalButton,

        SortieAreaEventButton,
        SortieBlackBoardButton,

		SortieOKButton,
		SortieTeamHorizontalButton,
		SortieFinalButton,

		SortieSimpleAdvance,

		SortieCommonLeftButton,
		SortieCommonRightButton,

        SortieCommonAirBaseOKButton,
        SortieCommonAirBaseROKButton,

		SortieCommonElseButton,

		ExpeditionButton,

		ExpeditionAreaHorizontal,
		ExpeditionVertical,
		ExpeditionOKButton,

		ExpeditionTeamHorizontal,
		ExpeditionFinalOKButton,

		MissionButton,
		MissionSkipOoyodoButton,
		MissionNextPage,
		MissionFirstPage,
		MissionVertical,
		MissionCloseButton,
		MissionReturnButton,

		ToDevelopButton,
		DoDevelopButton,
		DevelopSkip,
	};

	static bool CheckColor(CheckColorNameDefine name);
	static void MoveAndClick(MoveMouseNameDefine name, int xMul = 0, int yMul = 0);

	static QMap<CheckColorNameDefine, QList<int>> checkColorMap;
	static QMap<MoveMouseNameDefine, QList<int>> mouseClickMap;

};

