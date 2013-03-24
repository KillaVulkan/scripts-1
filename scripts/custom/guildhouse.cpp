#include "ScriptMgr.h"
#include "GossipDef.h"
#include "precompiled.h"
#include "Player.h"
#include "Common.h"

//extern DatabaseMysql SD2Database;

extern DatabaseType WorldDatabase;

#define MSG_GOSSIP_TELE          "Teleport zum Gildenhaus"
#define MSG_GOSSIP_BUY           "Gildenhaus kaufen (25000 gold)"
#define MSG_GOSSIP_SELL          "Gildenhaus verkaufen (15000 gold)"
#define MSG_GOSSIP_NEXTPAGE      "Next -->"
#define MSG_ALREADYHAVEGH        "Deine Gilde besitzt bereits ein Gildenhaus (%s)."
#define MSG_NOTENOUGHMONEY       "Nicht genug Gold."
#define MSG_CONGRATULATIONS      "Du hast ein Gildenhaus erworben."
#define MSG_SOLD                 "Du hast dein Gildenhaus verkauft hier ist dein Gold"

#define CODE_SELL                "SELL"
#define MSG_CODEBOX_SELL         "Enter \"" CODE_SELL "\" M\303\266 du dein Gildenhaus verkaufen?"

#define OFFSET_GH_ID_TO_ACTION   1500
#define OFFSET_SHOWBUY_FROM      10000

#define ACTION_TELE              1001
#define ACTION_SHOW_BUYLIST      1002  //deprecated. Use (OFFSET_SHOWBUY_FROM + 0) instead
#define ACTION_SELL_GUILDHOUSE   1003

#define ICON_GOSSIP_BALOON       0
#define ICON_GOSSIP_WING         2
#define ICON_GOSSIP_BOOK         3
#define ICON_GOSSIP_WHEEL1       4
#define ICON_GOSSIP_WHEEL2       5
#define ICON_GOSSIP_GOLD         6
#define ICON_GOSSIP_BALOONDOTS   7
#define ICON_GOSSIP_TABARD       8
#define ICON_GOSSIP_XSWORDS      9

#define COST_GH_BUY              -250000000  //1000 g.
#define COST_GH_SELL             150000000   //500 g.

#define GOSSIP_COUNT_MAX         10

bool isPlayerGuildLeader(Player *pPlayer)
{
	return (pPlayer->GetRank() == 0) && (pPlayer->GetGuildId() != 0);
}

bool getGuildHouseCoords(uint32 guildId, float &x, float &y, float &z, uint32 &map)
{
	if (guildId == 0)
	{
		//if pPlayer has no guild
		return false;
	}

	QueryResult *result;
	result = WorldDatabase.PQuery("SELECT `x`, `y`, `z`, `map` FROM `guildhouses` WHERE `guildId` = %u", guildId);
	if(result)
	{
		Field *fields = result->Fetch();
		x = fields[0].GetFloat();
		y = fields[1].GetFloat();
		z = fields[2].GetFloat();
		map = fields[3].GetUInt32();
		return true;
	}
	return false;
}

void teleportPlayerToGuildHouse(Player *pPlayer, Creature *_Creature)
{
	if (pPlayer->GetGuildId() == 0)
	{
		//if pPlayer has no guild
		pPlayer->GetSession()->SendNotification("Du bist in keiner Gilde erstelle eine oder trete einer vorhandenen Gilde bei.");
		return;
	}

	if (!pPlayer->getAttackers().empty())
	{
		//if pPlayer in combat
		pPlayer->GetSession()->SendNotification("Du befindest dich im Kampf!");
		return;
	}

	float x, y, z;
	uint32 map;

	if (getGuildHouseCoords(pPlayer->GetGuildId(), x, y, z, map))
	{
		//teleport pPlayer to the specified location
		pPlayer->TeleportTo(map, x, y, z, 0.0f);
	}
	else
		pPlayer->GetSession()->SendNotification("Deine Gilde hat kein Gildenhaus!");

}

bool showBuyList(Player *pPlayer, Creature *_Creature, uint32 showFromId = 0)
{
	//show not occupied guildhouses

	QueryResult *result;
	result = WorldDatabase.PQuery("SELECT `id`, `comment` FROM `guildhouses` WHERE `guildId` = 0 AND `id` > %u ORDER BY `id` ASC LIMIT %u",
		showFromId, GOSSIP_COUNT_MAX);

	if (result)
	{
		uint32 guildhouseId = 0;
		std::string comment = "";
		do
		{

			Field *fields = result->Fetch();

			guildhouseId = fields[0].GetInt32();
			comment = fields[1].GetString();
			
			//send comment as a gossip item
			//transmit guildhouseId in Action variable
			pPlayer->ADD_GOSSIP_ITEM(ICON_GOSSIP_TABARD, comment, GOSSIP_SENDER_MAIN, guildhouseId + OFFSET_GH_ID_TO_ACTION);

		} while (result->NextRow());

		if (result->GetRowCount() == GOSSIP_COUNT_MAX)
		{
			//assume that we have additional page
			//add link to next GOSSIP_COUNT_MAX items
			pPlayer->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOONDOTS, MSG_GOSSIP_NEXTPAGE, GOSSIP_SENDER_MAIN, guildhouseId + OFFSET_SHOWBUY_FROM);
		}

		
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetObjectGuid());

		return true;
	} else
	{
		if (showFromId = 0)
		{
			//all guildhouses are occupied
			pPlayer->GetSession()->SendNotification("Es steht kein freihes Gildenhaus mehr zum Verkauf");
			pPlayer->CLOSE_GOSSIP_MENU();
		} else
		{
			//this condition occurs when COUNT(guildhouses) % GOSSIP_COUNT_MAX == 0
			//just show GHs from beginning
			showBuyList(pPlayer, _Creature, 0);
		}
	}

	return false;
}

bool isPlayerHasGuildhouse(Player *pPlayer, Creature *_Creature, bool say = false)
{

	QueryResult *result;

	result = WorldDatabase.PQuery("SELECT `comment` FROM `guildhouses` WHERE `guildId` = %u",
		pPlayer->GetGuildId());

	if (result)
	{
		if (say)
		{
			//whisper to pPlayer "already have etc..."
			Field *fields = result->Fetch();
			char msg[100];
			sprintf(msg, MSG_ALREADYHAVEGH, fields[0].GetString());
			_Creature->MonsterSay(msg, pPlayer->GetObjectGuid());
		}

		return true;
	}
	return false;

}

void buyGuildhouse(Player *pPlayer, Creature *_Creature, uint32 guildhouseId)
{
	if (pPlayer->GetMoney() < 250000000)
	{
		//show how much money pPlayer need to buy GH (in gold)
		char msg[100];
		sprintf(msg, MSG_NOTENOUGHMONEY, COST_GH_BUY / -250000000);
		pPlayer->ModifyMoney(-250000000);
		_Creature->MonsterYell(msg, pPlayer->GetObjectGuid());
		return;
	}

	if (isPlayerHasGuildhouse(pPlayer, _Creature, true))
	{
		//pPlayer already have GH
		return;
	}

	QueryResult *result;

	//check if somebody already occupied this GH
	result = WorldDatabase.PQuery("SELECT `id` FROM `guildhouses` WHERE `id` = %u AND `guildId` <> 0",
		guildhouseId);

	if (result)
	{
		pPlayer->GetSession()->SendNotification("Dieses Gildenhaus wird bereits verwendet.");
		return;
	}

	//update DB
	result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = %u WHERE `id` = %u",
		pPlayer->GetGuildId(), guildhouseId);
	pPlayer->ModifyMoney(-250000000);

	if (result)

	_Creature->MonsterSay("Du hast ein Gildenhaus erworben.", pPlayer->GetObjectGuid());

	
}

void sellGuildhouse(Player *pPlayer, Creature *_Creature)
{
	if (isPlayerHasGuildhouse(pPlayer, _Creature))
	{
		QueryResult *result;

		result = WorldDatabase.PQuery("UPDATE `guildhouses` SET `guildId` = 0 WHERE `guildId` = %u",
		pPlayer->GetGuildId());
		
		if (result)
			
		pPlayer->ModifyMoney(150000000);

		//display message e.g. "here your money etc."
		char msg[100];
		sprintf(msg, MSG_SOLD, COST_GH_SELL / 150000000);
		pPlayer->ModifyMoney(150000000);
		_Creature->MonsterYell(msg, pPlayer->GetObjectGuid());
	}
}

bool GossipHello_guildmaster(Player *pPlayer, Creature *_Creature)
{
	pPlayer->ADD_GOSSIP_ITEM(ICON_GOSSIP_BALOON, MSG_GOSSIP_TELE, 
		GOSSIP_SENDER_MAIN, ACTION_TELE);

	if (isPlayerGuildLeader(pPlayer))
	{
		//show additional menu for guild leader
		pPlayer->ADD_GOSSIP_ITEM(ICON_GOSSIP_GOLD, MSG_GOSSIP_BUY,
			GOSSIP_SENDER_MAIN, ACTION_SHOW_BUYLIST);
		if (isPlayerHasGuildhouse(pPlayer, _Creature))
		{
			//and additional for guildhouse owner
			pPlayer->PlayerTalkClass->GetGossipMenu().AddMenuItem(ICON_GOSSIP_GOLD, MSG_GOSSIP_SELL, GOSSIP_SENDER_MAIN, ACTION_SELL_GUILDHOUSE, MSG_CODEBOX_SELL, 0, true);
		}
	}
	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetObjectGuid());
	return true;
}


bool GossipSelect_guildmaster (Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action)
{
	if (sender != GOSSIP_SENDER_MAIN)
		return false;

	switch (action)
	{
		case ACTION_TELE:
			//teleport pPlayer to GH
			pPlayer->CLOSE_GOSSIP_MENU();
			teleportPlayerToGuildHouse(pPlayer, _Creature);
			break;
		case ACTION_SHOW_BUYLIST:
			//show list of GHs which currently not occupied
			showBuyList(pPlayer, _Creature);
			break;
		default:
			if (action > OFFSET_SHOWBUY_FROM)
			{
				showBuyList(pPlayer, _Creature, action - OFFSET_SHOWBUY_FROM);
			} else if (action > OFFSET_GH_ID_TO_ACTION)
			{
				//pPlayer clicked on buy list
				pPlayer->CLOSE_GOSSIP_MENU();
				//get guildhouseId from action
				//guildhouseId = action - OFFSET_GH_ID_TO_ACTION
				buyGuildhouse(pPlayer, _Creature, action - OFFSET_GH_ID_TO_ACTION);
			}
			break;
	}
	
	return true;
}

bool GossipSelectWithCode_guildmaster (Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action, const char* sCode)
{
    if(sender == GOSSIP_SENDER_MAIN)
    {
        if(action == ACTION_SELL_GUILDHOUSE)
        {
			int i = -1;
			try
			{
				//compare code
				if (strlen(sCode) + 1 == sizeof CODE_SELL)
					i = strcmp(CODE_SELL, sCode);

			} catch(char *str) {error_db_log(str);}

			if (i == 0)
			{
				//right code
				sellGuildhouse(pPlayer, _Creature);
			}
			pPlayer->CLOSE_GOSSIP_MENU();
			return true;
		}
	}
	return false;
}


void AddSC_guildmaster()
{
	Script *newscript;
	newscript = new Script;
	newscript->Name = "guildmaster";
	newscript->pGossipHello = &GossipHello_guildmaster;
	newscript->pGossipSelect = &GossipSelect_guildmaster;
	newscript->pGossipSelectWithCode =  &GossipSelectWithCode_guildmaster;
	newscript->RegisterSelf();
}
