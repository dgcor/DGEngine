#include "PlayerAI.h"
#include "Player.h"

void PlayerAI::ProcessMonsters(Player& Monst)
{
//	int i, mi, mx, my, _menemy;
//	BOOL raflag;
//	MonsterStruct* Monst;
//
//	DeleteMonsterList();
//
//	assert((DWORD)nummonsters <= MAXMONSTERS);
//	for (i = 0; i < nummonsters; i++) {
//		mi = monstactive[i];
//		Monst = &monster[mi];
//		raflag = FALSE;
//		if (gbMaxPlayers > 1) {
//			SetRndSeed(Monst->_mAISeed);
//			Monst->_mAISeed = GetRndSeed();
//		}
//		if (!(monster[mi]._mFlags & MFLAG_NOHEAL) && Monst->_mhitpoints < Monst->_mmaxhp && Monst->_mhitpoints >> 6 > 0) {
//			if (Monst->mLevel > 1) {
//				Monst->_mhitpoints += Monst->mLevel >> 1;
//			}
//			else {
//				Monst->_mhitpoints += Monst->mLevel;
//			}
//		}
//		mx = Monst->_mx;
//		my = Monst->_my;
//#ifndef SPAWN
//		if (dFlags[mx][my] & BFLAG_VISIBLE && Monst->_msquelch == 0) {
//			if (Monst->MType->mtype == MT_CLEAVER) {
//				PlaySFX(USFX_CLEAVER);
//			}
//		}
//#endif
//		if (Monst->_mFlags & MFLAG_TARGETS_MONSTER) {
//			_menemy = Monst->_menemy;
//			if ((DWORD)_menemy >= MAXMONSTERS) {
//				app_fatal("Illegal enemy monster %d for monster \"%s\"", _menemy, Monst->mName);
//			}
//			Monst->_lastx = monster[Monst->_menemy]._mfutx;
//			Monst->_menemyx = Monst->_lastx;
//			Monst->_lasty = monster[Monst->_menemy]._mfuty;
//			Monst->_menemyy = Monst->_lasty;
//		}
//		else {
//			_menemy = Monst->_menemy;
//			if ((DWORD)_menemy >= MAX_PLRS) {
//				app_fatal("Illegal enemy player %d for monster \"%s\"", _menemy, Monst->mName);
//			}
//			Monst->_menemyx = plr[Monst->_menemy]._px;
//			Monst->_menemyy = plr[Monst->_menemy]._py;
//			if (dFlags[mx][my] & BFLAG_VISIBLE) {
//				Monst->_msquelch = 255;
//				Monst->_lastx = plr[Monst->_menemy]._px;
//				Monst->_lasty = plr[Monst->_menemy]._py;
//			}
//			else if (Monst->_msquelch != 0 && Monst->_mAi != MT_DIABLO) { /// BUGFIX: change '_mAi' to 'MType->mtype'
//				Monst->_msquelch--;
//			}
//		}
//		do {
//			if (!(Monst->_mFlags & MFLAG_SEARCH)) {
//				AiProc[Monst->_mAi](mi);
//			}
//			else if (!MAI_Path(mi)) {
//				AiProc[Monst->_mAi](mi);
//			}
//			switch (Monst->_mmode) {
//			case MM_STAND:
//				raflag = M_DoStand(mi);
//				break;
//			case MM_WALK:
//				raflag = M_DoWalk(mi);
//				break;
//			case MM_WALK2:
//				raflag = M_DoWalk2(mi);
//				break;
//			case MM_WALK3:
//				raflag = M_DoWalk3(mi);
//				break;
//			case MM_ATTACK:
//				raflag = M_DoAttack(mi);
//				break;
//			case MM_GOTHIT:
//				raflag = M_DoGotHit(mi);
//				break;
//			case MM_DEATH:
//				raflag = M_DoDeath(mi);
//				break;
//			case MM_SATTACK:
//				raflag = M_DoSAttack(mi);
//				break;
//			case MM_FADEIN:
//				raflag = M_DoFadein(mi);
//				break;
//			case MM_FADEOUT:
//				raflag = M_DoFadeout(mi);
//				break;
//			case MM_RATTACK:
//				raflag = M_DoRAttack(mi);
//				break;
//			case MM_SPSTAND:
//				raflag = M_DoSpStand(mi);
//				break;
//			case MM_RSPATTACK:
//				raflag = M_DoRSpAttack(mi);
//				break;
//			case MM_DELAY:
//				raflag = M_DoDelay(mi);
//				break;
//			case MM_CHARGE:
//				raflag = FALSE;
//				break;
//			case MM_STONE:
//				raflag = M_DoStone(mi);
//				break;
//			case MM_HEAL:
//				raflag = M_DoHeal(mi);
//				break;
//			case MM_TALK:
//				raflag = M_DoTalk(mi);
//				break;
//			}
//			if (raflag) {
//				GroupUnity(mi);
//			}
//		} while (raflag);
//		if (Monst->_mmode != MM_STONE) {
//			Monst->_mAnimCnt++;
//			if (!(Monst->_mFlags & MFLAG_ALLOW_SPECIAL) && Monst->_mAnimCnt >= Monst->_mAnimDelay) {
//				Monst->_mAnimCnt = 0;
//				if (Monst->_mFlags & MFLAG_LOCK_ANIMATION) {
//					Monst->_mAnimFrame--;
//					if (Monst->_mAnimFrame == 0) {
//						Monst->_mAnimFrame = Monst->_mAnimLen;
//					}
//				}
//				else {
//					Monst->_mAnimFrame++;
//					if (Monst->_mAnimFrame > Monst->_mAnimLen) {
//						Monst->_mAnimFrame = 1;
//					}
//				}
//			}
//		}
//	}
//
//	DeleteMonsterList();
}
