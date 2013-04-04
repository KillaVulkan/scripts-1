/*
 * Copyright (C) 2008 - 2013 nR7GaminG <http://nR7GaminG.eu>
 */

#include "GossipDef.h"
#include "Player.h"
#include "precompiled.h"
#include "ScriptMgr.h"
#include "SpellMgr.h"
#include "Common.h"

bool GossipHello_vip_teleporter(Player *pPlayer, Creature *_Creature)
{
	pPlayer->SetTaxiCheater(true);
	// pPlayer->ADD_GOSSIP_ITEM(5, "VIP Teleport",	             GOSSIP_SENDER_MAIN, 0);
	pPlayer->ADD_GOSSIP_ITEM(5, "VIP Account",	                 GOSSIP_SENDER_MAIN, 0);
	pPlayer->ADD_GOSSIP_ITEM(5, "Normale Account",	             GOSSIP_SENDER_MAIN, 1);
	// pPlayer->ADD_GOSSIP_ITEM(5, "Normaler Teleporter",	     GOSSIP_SENDER_MAIN, 2);
	pPlayer->ADD_GOSSIP_ITEM(5, "Build",                         GOSSIP_SENDER_MAIN, 100);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,            _Creature->GetObjectGuid());

	return true;	   
}

void SendDefaultMenu_vip_teleporter(Player *pPlayer, Creature *_Creature, uint32 action)
{
	switch(action)
    {
	   case 0: // VIP Nutzer! Menü 2
		   if (pPlayer->GetSession()->IsPremium() && (pPlayer->getAttackers().empty()))// Premium status abfragen?
		   {
                pPlayer->ADD_GOSSIP_ITEM(5, "Teleport",                GOSSIP_SENDER_MAIN, 2);
                // pPlayer->ADD_GOSSIP_ITEM(3, "more",                 GOSSIP_SENDER_MAIN, xxxx);
                pPlayer->ADD_GOSSIP_ITEM(3, "Waffen F\303\244higkeit auf Max",                 GOSSIP_SENDER_MAIN, 6);
 			    pPlayer->ADD_GOSSIP_ITEM(0, "Sch\303\266nen Tag noch", GOSSIP_SENDER_MAIN, 101);

			    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,       _Creature->GetObjectGuid());
		   }	   
		   else
		   {
			   pPlayer->GetSession()->SendNotification("Tut uns Leid, du bist kein V.I.P");

			   pPlayer->CLOSE_GOSSIP_MENU();
		   }
		   break;

	   case 1: // Normale Nutzer Menü 2
		   if (pPlayer->getAttackers().empty())                        // im Kampf?
		    {
                pPlayer->ADD_GOSSIP_ITEM(5, "Teleport",                GOSSIP_SENDER_MAIN, 4);
 			    pPlayer->ADD_GOSSIP_ITEM(0, "Sch\303\266nen Tag noch", GOSSIP_SENDER_MAIN, 101);
			    
                pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,       _Creature->GetObjectGuid());
            }	   
		   else
		   {
			   pPlayer->CLOSE_GOSSIP_MENU();
		   }
		   break;

	   case 2: // VIP Nutzer! Menü 3
		   if (pPlayer->GetSession()->IsPremium() && (pPlayer->getAttackers().empty()))// Premium status abfragen?
		   {
                pPlayer->ADD_GOSSIP_ITEM(5, "Hauptst\303\244dte",        GOSSIP_SENDER_MAIN, 3);
                pPlayer->ADD_GOSSIP_ITEM(5, "Instanzen",                 GOSSIP_SENDER_MAIN, 5);
                pPlayer->ADD_GOSSIP_ITEM(5, "Welt",                 GOSSIP_SENDER_MAIN, 102);
 			    pPlayer->ADD_GOSSIP_ITEM(0, "Zur\303\274ck",   GOSSIP_SENDER_MAIN, 0);

			    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,         _Creature->GetObjectGuid());
		   }	   
		   else
		   {
			   pPlayer->GetSession()->SendNotification("Tut uns Leid, du bist kein V.I.P");
			   pPlayer->CLOSE_GOSSIP_MENU();
		   }
		   break;

	   case 3:
		   if (pPlayer->GetTeam() == ALLIANCE)
		   {
			    // pPlayer->ADD_GOSSIP_ITEM(5, "Gurubashi Arena",          GOSSIP_SENDER_MAIN, 10);
			    pPlayer->ADD_GOSSIP_ITEM(5, "Sturmwind",		           GOSSIP_SENDER_MAIN, 11);
			    pPlayer->ADD_GOSSIP_ITEM(5, "Darnassus",                   GOSSIP_SENDER_MAIN, 12);
		        pPlayer->ADD_GOSSIP_ITEM(5, "Eisenschmiede",               GOSSIP_SENDER_MAIN, 13);
		   	    pPlayer->ADD_GOSSIP_ITEM(5, "Exodar",                      GOSSIP_SENDER_MAIN, 14);
	    	    pPlayer->ADD_GOSSIP_ITEM(5, "Shattrath",                   GOSSIP_SENDER_MAIN, 15);
	    	    pPlayer->ADD_GOSSIP_ITEM(5, "Quel'Danas",                  GOSSIP_SENDER_MAIN, 20);
			    pPlayer->ADD_GOSSIP_ITEM(0, "Teleport Men\303\274",        GOSSIP_SENDER_MAIN,  2);

			   pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,          _Creature->GetObjectGuid());
		   }
		   else
		   if (pPlayer->GetTeam() == HORDE)
		   {
			   // pPlayer->ADD_GOSSIP_ITEM(5, "Gurubashi Arena",           GOSSIP_SENDER_MAIN, 10);
			    pPlayer->ADD_GOSSIP_ITEM(5, "Orgrimmar",                   GOSSIP_SENDER_MAIN, 16);
		        pPlayer->ADD_GOSSIP_ITEM(5, "Donnerfels",                  GOSSIP_SENDER_MAIN, 17);
			    pPlayer->ADD_GOSSIP_ITEM(5, "Unterstadt",                  GOSSIP_SENDER_MAIN, 18);
			    pPlayer->ADD_GOSSIP_ITEM(5, "Silbermond",                  GOSSIP_SENDER_MAIN, 19);
			    pPlayer->ADD_GOSSIP_ITEM(5, "Shattrath",                   GOSSIP_SENDER_MAIN, 15);
	    	    pPlayer->ADD_GOSSIP_ITEM(5, "Quel'Danas",                  GOSSIP_SENDER_MAIN, 20);
			    pPlayer->ADD_GOSSIP_ITEM(0, "Teleport Men\303\274",        GOSSIP_SENDER_MAIN,  2);

			   pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,            _Creature->GetObjectGuid());
		   }
			  break;

          // Normal Teleporter!
	   case 4:
		   if (pPlayer->GetTeam() == ALLIANCE)
		   {
			   pPlayer->ADD_GOSSIP_ITEM(5, "Sturmwind",		               GOSSIP_SENDER_MAIN, 11);
 			   pPlayer->ADD_GOSSIP_ITEM(0, "Sch\303\266nen Tag noch",      GOSSIP_SENDER_MAIN, 101);

			   pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,           _Creature->GetObjectGuid());

		   }
		   else
		   if (pPlayer->GetTeam() == HORDE)
		   {
			   pPlayer->ADD_GOSSIP_ITEM(5, "Orgrimmar",                   GOSSIP_SENDER_MAIN, 16);
 			   pPlayer->ADD_GOSSIP_ITEM(0, "Sch\303\266nen Tag noch",     GOSSIP_SENDER_MAIN, 101);

			   pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,          _Creature->GetObjectGuid());
		   }
			  break;
	   
           case 5:                                                         // INSTANZEN
		   {

			    pPlayer->ADD_GOSSIP_ITEM(5, "Der Schwarzer Tempel",             GOSSIP_SENDER_MAIN, 21);
		        pPlayer->ADD_GOSSIP_ITEM(5, "Festung der St\303\274rme",               GOSSIP_SENDER_MAIN, 22);
		   	    // pPlayer->ADD_GOSSIP_ITEM(5, "Gruuls Unterschlupf",              GOSSIP_SENDER_MAIN, 23);
	    	    // pPlayer->ADD_GOSSIP_ITEM(5, "H\303\266hle des Schlangenschreins",      GOSSIP_SENDER_MAIN, 24);
	    	    pPlayer->ADD_GOSSIP_ITEM(5, "Hyjal",                            GOSSIP_SENDER_MAIN, 25);
	    	    // pPlayer->ADD_GOSSIP_ITEM(5, "Karazahn",                         GOSSIP_SENDER_MAIN, 26);
	    	    // pPlayer->ADD_GOSSIP_ITEM(5, "Magtheridons Kammer",              GOSSIP_SENDER_MAIN, 27);
	    	    // pPlayer->ADD_GOSSIP_ITEM(5, "Sonnenbrunnenplateau",             OSSIP_SENDER_MAIN, 28);
	    	    pPlayer->ADD_GOSSIP_ITEM(5, "Zul'Aman",                         GOSSIP_SENDER_MAIN, 29);
			    // pPlayer->ADD_GOSSIP_ITEM(0, "Teleport Men\303\274",             GOSSIP_SENDER_MAIN, 0);
                pPlayer->ADD_GOSSIP_ITEM(0, "Teleport Men\303\274",        GOSSIP_SENDER_MAIN,  2);

			    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,          _Creature->GetObjectGuid());
		   }
			    break;

			case 6: // Max Skills
				pPlayer->CLOSE_GOSSIP_MENU();
				pPlayer->UpdateSkillsToMaxSkillsForLevel();

				break;
/*
#############################################
#############################################
################ HAUPTSTÄDTE ################
#############################################
#############################################
*/
        //Gurubashi Arena!
        case  10: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(0,   -13240.937500f,    204.797928f,     30.869392f, 1.102942f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
         //Sturmwind
        case  11: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(0,    -8881.000000f,    575.000000f,     93.000000f, 0.000000f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Darnassus
        case  12: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(1,     9951.890000f,   2280.550000f,   1341.390000f, 0.000000f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Eisenschmiede
        case  13: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(0,    -4981.000000f,   -881.000000f,    501.660000f, 0.000000f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Exodar
        case  14: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(530,  -4014.000000f, -11895.790000f,     -2.000000f, 0.000000f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Shattrath
        case  15: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(530,  -1887.510010f,   5359.379883f,    -12.427300f, 0.000000f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Orgrimmar
        case  16: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(1,     1629.777344f,  -4373.210449f,     31.246769f, 0.000000f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Donnerfels
        case  17: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(1,    -1196.858398f,     30.147331f,    176.949188f, 0.000000f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;

        // Unterstadt
        case  18: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(0,     1844.765625f,    237.655991f,     62.275150f, 0.000000f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;

		// Silbermod
        case  19: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(530,   9400.486328f,  -7278.376953f,     14.206780f, 0.000000f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;

		//Quel'Danas
        case  20: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(530,   12942.400391f,  -6930.770020f,     4.213060f, 2.437920f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;


/*
#############################################
#############################################
################# INSTANZEN #################
#############################################
#############################################
*/
        // Der Schwarzer Tempel
		case  21: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(530,   -3640.432861f,  314.126221f,     35.256004f, 2.897555f);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //FDS: Das Auge
		case  22: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(530,   3088.391357f,  1386.200073f,     184.957825f, 4.546514f);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Gruuls Unterschlupf
		// case  23: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(MAP,   XXXXXf,  YYYYf,     ZZZZZf, OOOOOf);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Höhle des Schlangenschreins
		// case  24: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(MAP,   XXXXXf,  YYYYf,     ZZZZZf, OOOOOf);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Hyjal
		case  25: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(1,   -8168.119629f,  -4177.029785f,     -165.076645f, 4.125618f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Karazahn
		// case  26: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(MAP,   XXXXXf,  YYYYf,     ZZZZZf, OOOOOf);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Magtheridons Kammer
		// case  27: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(MAP,   XXXXXf,  YYYYf,     ZZZZZf, OOOOOf);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Sonnenbrunnenplateau
		// case  28: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(MAP,   XXXXXf,  YYYYf,     ZZZZZf, OOOOOf);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        //Zul'Aman
		case  29: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(530,   6851.297852f,  -7954.612793f,     170.040161f, 4.668588f); break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;

/*
#############################################
#############################################
#################### WELT ###################
#############################################
#############################################
*/
        //XXXXXX
		// case  30: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(MAP,   XXXXXf,  YYYYf,     ZZZZZf, OOOOOf);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;

        //XXXXXX
		// case  31: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(MAP,   XXXXXf,  YYYYf,     ZZZZZf, OOOOOf);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;

        //XXXXXX
		// case  32: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(MAP,   XXXXXf,  YYYYf,     ZZZZZf, OOOOOf);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;

        //XXXXXX
		// case  33: pPlayer->CLOSE_GOSSIP_MENU(); pPlayer->TeleportTo(MAP,   XXXXXf,  YYYYf,     ZZZZZf, OOOOOf);  break; // _Creature->MonsterSay(4444405, pPlayer->GetObjectGuid()); break;
		
        case 100:
	        pPlayer->ADD_GOSSIP_ITEM(6, "Version",                   GOSSIP_SENDER_MAIN, 110);
	        pPlayer->ADD_GOSSIP_ITEM(6, "Entwickler",                GOSSIP_SENDER_MAIN, 111);
	        pPlayer->ADD_GOSSIP_ITEM(0, "Sch\303\266nen Tag noch",   GOSSIP_SENDER_MAIN, 101);

	        pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,        _Creature->GetObjectGuid());
		break;

		case 101:
			pPlayer->GetSession()->SendNotification("Viel Spa\303\237 noch auf Sturmschlag");
			pPlayer->CLOSE_GOSSIP_MENU();
		break;

		case 102:
			pPlayer->GetSession()->SendNotification("Dies befindet sich noch in Entwicklung");
			pPlayer->CLOSE_GOSSIP_MENU();
		break;

		case 110:
			pPlayer->CLOSE_GOSSIP_MENU();
		    pPlayer->GetSession()->SendNotification("Version 2.0.2");
		  //_Creature->MonsterSay(4444406, pPlayer->GetObjectGuid());
	    break;

		case 111:
			pPlayer->CLOSE_GOSSIP_MENU();
		    pPlayer->GetSession()->SendNotification("by KillaVulkan");
	    break;
    }
}

bool GossipSelect_vip_teleporter(Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action)
{
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_vip_teleporter(pPlayer, _Creature, action);

    return true;
}

void AddSC_vip_teleporter()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="vip_teleporter";
    newscript->pGossipHello =  &GossipHello_vip_teleporter;
    newscript->pGossipSelect = &GossipSelect_vip_teleporter;
    newscript->RegisterSelf();
}