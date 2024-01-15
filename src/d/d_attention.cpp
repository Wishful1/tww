//
// Generated by dtk
// Translation Unit: d_attention.cpp
//

#include "d/d_attention.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/actor/d_a_player_main.h"
#include "d/d_s_play.h"
#include "m_Do/m_Do_ext.h"
#include "JSystem/JKernel/JKRSolidHeap.h"

s32 dAttention_c::loc_type_num = 3;
u32 dAttention_c::act_type_num = 5;

/* 8009D220-8009D268       .text __ct__11dAttParam_cFl */
dAttParam_c::dAttParam_c(s32) {
    /* Nonmatching */
    field_0x04 = 45.0f;
    field_0x08 = 30.0f;
    field_0x0c = 90.0f;
    field_0x00 = 1;
    field_0x18 = -0.9f;
    field_0x10 = 3000.0f;
    field_0x14 = 1000.0f;
}

/* 8009D268-8009D2B0       .text __dt__11dAttParam_cFv */
dAttParam_c::~dAttParam_c() {
}

/* 8009D2B0-8009D2E0       .text execute__19dAttDraw_CallBack_cFUsP16J3DTransformInfo */
bool dAttDraw_CallBack_c::execute(u16 timing, J3DTransformInfo* xform) {
    if (timing == 0) {
        xform->mTranslate.y *= g_regHIO.mChild[6].mFloatRegs[17] + 0.6f;
    }
    return true;
}

/* 8009D2E0-8009D654       .text __ct__12dAttention_cFP10fopAc_ac_cUl */
dAttention_c::dAttention_c(fopAc_ac_c* player, u32 playerNo) {
    /* Nonmatching */
    mpPlayer = (daPy_lk_c*)player;
    mPlayerNo = playerNo;
    initList(0xFFFFFFFF);
    mFlagMask = 0;
    field_0x01c = -1;
    field_0x019 = 0;
    mLockOnState = 0;
    field_0x01a = 0;
    field_0x01b = 0;
    mLockOnTargetBsPcID = fpcM_ERROR_PROCESS_ID_e;
    heap = mDoExt_createSolidHeapFromGameToCurrent(0x3600, 0);
    JUT_ASSERT(0xb9, heap != 0);

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Always", ALWAYS_BDL_YAZIRUSHI_01);
    JUT_ASSERT(0xbe, modelData != 0);

    int anmColNum = 0;
    for (u32 i = 0; i < 5; i++) {
        static u16 l_bpkIdx[] = {
            ALWAYS_BPK_YJ_IN, ALWAYS_BPK_YJ_OUT, ALWAYS_BPK_YJ_SCALE, ALWAYS_BPK_YJ_LOOP, ALWAYS_BPK_YJ_DELETE,
        };
        J3DAnmColor* anmCol = (J3DAnmColor*)dComIfG_getObjectRes("Always", l_bpkIdx[i]);
        JUT_ASSERT(0xcc, anmCol != 0);

        anmCol->searchUpdateMaterialID(modelData);
        if (anmCol->getUpdateMaterialNum() > anmColNum)
            anmColNum = anmCol->getUpdateMaterialNum();
    }

    for (s32 i = 0; i < (s32)ARRAY_SIZE(draw); i++) {
        draw[i].anm = new mDoExt_McaMorf(modelData, &mCallBack, NULL, (J3DAnmTransformKey*)dComIfG_getObjectRes("Always", ALWAYS_BCK_YJ_LOOP), J3DFrameCtrl::LOOP_REPEAT_e, 1.0f, 0, -1, 1, NULL, 0x80000, 0x1000003);
        JUT_ASSERT(0xe3, draw[i].anm != 0 && draw[i].anm->getModel() != 0);
        draw[i].mpAnmClr = NULL;
        draw[i].mpAnmMatClr = new J3DMatColorAnm[anmColNum];
    }

    mDoExt_restoreCurrentHeap();
    if (mDoExt_adjustSolidHeap(heap) >= 0)
        DCStoreRangeNoSync(heap->getStartAddr(), heap->getSize());

    field_0x028 = -1;
    mFlags = 0;
    mHint.init();
    mCatch.init();
    mLook[0].init();
    mLook[1].init();
}

/* 8009D654-8009D6EC       .text __dt__12dAttention_cFv */
dAttention_c::~dAttention_c() {
    if (heap != NULL) {
        mDoExt_destroySolidHeap(heap);
        heap = NULL;
    }
}

/* 8009D6EC-8009D728       .text GetActionList__12dAttention_cFl */
dAttList_c* dAttention_c::GetActionList(s32 idx) {
    if (mActionNum != 0)
        return &mActionList[(mActionOffs + idx) % mActionNum];
    else
        return NULL;
}

/* 8009D728-8009D764       .text GetLockonList__12dAttention_cFl */
dAttList_c* dAttention_c::GetLockonList(s32 idx) {
    if (mLockOnNum != 0)
        return &mLockOnList[(mLockOnOffs + idx) % mLockOnNum];
    else
        return NULL;
}

/* 8009D764-8009D858       .text getActionBtnB__12dAttention_cFv */
dAttList_c* dAttention_c::getActionBtnB() {
    /* Nonmatching */
    dAttList_c* list = GetLockonList(0);
    if (list != NULL && list->getActor() != NULL && list->mType == 1 && LockonTruth() != 0 && !(list->getActor()->mAttentionInfo.mFlags & fopAc_Attn_TALKFLAG_NOTALK_e))
        return list;

    if (mActionNum == 0)
        return NULL;

    for (s32 i = 0; i < mActionNum; i++) {
        if (mActionList[i].mType == 3) {
            if (!(mActionList[i].getActor()->mAttentionInfo.mFlags & fopAc_Attn_TALKFLAG_NOTALK_e))
                return &mActionList[i];
        } else {
            return &mActionList[i];
        }
    }

    return NULL;
}

/* 8009D858-8009D9A8       .text getActionBtnXYZ_local__12dAttention_cFi */
dAttList_c* dAttention_c::getActionBtnXYZ_local(int button) {
    /* Nonmatching */
    dAttList_c* list = GetLockonList(0);
    if (list != NULL && list->getActor() != NULL && list->mType == 1 && LockonTruth() != 0) {
        fopAc_ac_c* actor = list->getActor();
        if ((actor->mEvtInfo.mCondition & dEvtCnd_CANTALKITEM_e) == dEvtCnd_CANTALKITEM_e) {
            s16 rt;
            if (actor->mEvtInfo.mpCheckCB == NULL)
                rt = 1;
            else
                rt = actor->mEvtInfo.mpCheckCB(actor, button);

            if (rt)
                return list;
        }

        return NULL;
    } else {
        if (mActionNum == 0)
            return NULL;

        for (s32 i = 0; i < mActionNum; i++) {
            if (mActionList[i].mType == 3) {
                fopAc_ac_c* actor = mActionList[i].getActor();
                if ((actor->mEvtInfo.mCondition & dEvtCnd_CANTALKITEM_e) == dEvtCnd_CANTALKITEM_e) {
                    s16 rt;
                    if (actor->mEvtInfo.mpCheckCB == NULL)
                        rt = 1;
                    else
                        rt = actor->mEvtInfo.mpCheckCB(actor, button);

                    if (rt)
                        return &mActionList[i];
                }
            }
        }

        return NULL;
    }
}

/* 8009D9A8-8009D9FC       .text getActionBtnX__12dAttention_cFv */
dAttList_c* dAttention_c::getActionBtnX() {
    if (!dComIfGs_checkGetItemNum(dComIfGp_getSelectItem(0)))
        return NULL;

    return getActionBtnXYZ_local(0);
}

/* 8009D9FC-8009DA50       .text getActionBtnY__12dAttention_cFv */
dAttList_c* dAttention_c::getActionBtnY() {
    if (!dComIfGs_checkGetItemNum(dComIfGp_getSelectItem(1)))
        return NULL;

    return getActionBtnXYZ_local(1);
}

/* 8009DA50-8009DAA4       .text getActionBtnZ__12dAttention_cFv */
dAttList_c* dAttention_c::getActionBtnZ() {
    if (!dComIfGs_checkGetItemNum(dComIfGp_getSelectItem(2)))
        return NULL;

    return getActionBtnXYZ_local(2);
}

dAttention_c::LocTbl dAttention_c::loc_type_tbl[3] = {
    { 0, 1 },
    { 1, 2 },
    { 2, 4 },
};

/* 8009DAA4-8009DAF4       .text chkAttMask__12dAttention_cFUlUl */
u32 dAttention_c::chkAttMask(u32 type, u32 mask) {
    for (s32 i = 0; i < loc_type_num; i++) {
        if (type == loc_type_tbl[i].mType) {
            return mask & loc_type_tbl[i].mMask;
        }
    }

    return 1;
}

/* 8009DAF4-8009DB60       .text check_event_condition__FUlUs */
void check_event_condition(u32, u16) {
    /* Nonmatching */
}

/* 8009DB60-8009DC28       .text check_flontofplayer__FUlss */
void check_flontofplayer(u32, s16, s16) {
    /* Nonmatching */
}

/* 8009DC28-8009DC74       .text distace_weight__Ffsf */
void distace_weight(f32, s16, f32) {
    /* Nonmatching */
}

/* 8009DC74-8009DCD4       .text distace_angle_adjust__Ffsf */
void distace_angle_adjust(f32, s16, f32) {
    /* Nonmatching */
}

/* 8009DCD4-8009DE44       .text check_distace__FP4cXyzsP4cXyzffff */
void check_distace(cXyz*, s16, cXyz*, f32, f32, f32, f32) {
    /* Nonmatching */
}

/* 8009DE44-8009E03C       .text calcWeight__12dAttention_cFiP10fopAc_ac_cfssPUl */
f32 dAttention_c::calcWeight(int, fopAc_ac_c*, f32, s16, s16, u32*) {
    /* Nonmatching */
}

/* 8009E03C-8009E128       .text setLList__12dAttention_cFP10fopAc_ac_cffUl */
void dAttention_c::setLList(fopAc_ac_c* i_actor, f32 weight, f32 distance, u32 type) {
    f32 bestWeight = 0.0f;

    if (weight > 0.0f) {
        int bestIdx;
        if (mLockOnNum < (s32)ARRAY_SIZE(mLockOnList)) {
            bestIdx = mLockOnNum++;
        } else {
            for (int i = (s32)ARRAY_SIZE(mLockOnList); i >= 0; i--) {
                if (weight > bestWeight) {
                    bestIdx = i;
                    bestWeight = weight;
                }
            }
        }

        if (weight < mLockOnList[bestIdx].mWeight) {
            mLockOnList[bestIdx].setActor(i_actor);
            mLockOnList[bestIdx].mWeight = weight;
            mLockOnList[bestIdx].mDistance = distance;
            mLockOnList[bestIdx].mType = type;
        }
    }
}

/* 8009E128-8009E214       .text setAList__12dAttention_cFP10fopAc_ac_cffUl */
void dAttention_c::setAList(fopAc_ac_c* i_actor, f32 weight, f32 distance, u32 type) {
    f32 bestWeight = 0.0f;

    if (weight > 0.0f) {
        int bestIdx;
        if (mActionNum < (s32)ARRAY_SIZE(mActionList)) {
            bestIdx = mActionNum++;
        } else {
            for (int i = (s32)ARRAY_SIZE(mActionList); i >= 0; i--) {
                if (weight > bestWeight) {
                    bestIdx = i;
                    bestWeight = weight;
                }
            }
        }

        if (weight < mActionList[bestIdx].mWeight) {
            mActionList[bestIdx].setActor(i_actor);
            mActionList[bestIdx].mWeight = weight;
            mActionList[bestIdx].mDistance = distance;
            mActionList[bestIdx].mType = type;
        }
    }
}

/* 8009E214-8009E2CC       .text initList__12dAttention_cFUl */
void dAttention_c::initList(u32 flagMask) {
    int i;

    mFlagMask = flagMask;

    for (i = 0; i < (s32)ARRAY_SIZE(mLockOnList); i++) {
        mLockOnList[i].setActor(NULL);
        mLockOnList[i].mWeight = 3.4028235E38f;
    }
    mLockOnOffs = 0;
    mLockOnNum = 0;

    for (i = 0; i < (s32)ARRAY_SIZE(mActionList); i++) {
        mActionList[i].setActor(NULL);
        mActionList[i].mWeight = 3.4028235E38f;
    }
    mActionOffs = 0;
    mActionNum = 0;
}

/* 8009E2CC-8009E2F8       .text select_attention__FP10fopAc_ac_cPv */
int select_attention(fopAc_ac_c* pActor, void* i_attention) {
    dAttention_c * pAttention = (dAttention_c*)i_attention;
    return pAttention->SelectAttention(pActor);
}

/* 8009E2F8-8009E33C       .text makeList__12dAttention_cFv */
s32 dAttention_c::makeList() {
    fopAcIt_Executor((fopAcIt_ExecutorFunc)select_attention, this);
    return mLockOnNum + mActionNum;
}

/* 8009E33C-8009E474       .text SelectAttention__12dAttention_cFP10fopAc_ac_c */
int dAttention_c::SelectAttention(fopAc_ac_c* ac) {
    /* Nonmatching */
    if (ac == mpPlayer || mpPlayer == NULL)
        return 0;

    mFlagMask = ac->mAttentionInfo.mFlags;

    cSGlobe globe1(ac->mAttentionInfo.mPosition - mpPlayer->mAttentionInfo.mPosition);
    cSAngle angle1 = globe1.U() - mpPlayer->shape_angle.y;

    cSGlobe globe2(mpPlayer->mAttentionInfo.mPosition - ac->mAttentionInfo.mPosition);
    cSAngle angle2 = globe2.U() - mpPlayer->shape_angle.y;

    u32 type;
    f32 weight = calcWeight('L', ac, globe1.R(), angle1, angle2, &type);
    setLList(ac, weight, globe1.R(), type);
    weight = calcWeight('A', ac, globe1.R(), angle1, angle2, &type);
    setAList(ac, weight, globe1.R(), type);
    return 0;
}

/* 8009E474-8009E5C4       .text sortList__12dAttention_cFv */
void dAttention_c::sortList() {
    /* Nonmatching */
}

/* 8009E5C4-8009E684       .text stockAttention__12dAttention_cFUl */
void dAttention_c::stockAttention(u32 interactMask) {
    fopAc_ac_c * pTarget = LockonTarget(0);
    initList(interactMask);
    if (makeList())
        sortList();

    if (pTarget != mLockOnList[0].getActor()) {
        if (pTarget != NULL) {
            if (mLockOnList[0].getActor() != NULL)
                mFlags |= 2;
        } else {
            mFlags |= 1;
        }

        mFlags |= 4;
    }

    LockonTarget(0);
}

/* 8009E684-8009E728       .text nextAttention__12dAttention_cFUl */
void dAttention_c::nextAttention(u32 interactMask) {
    fopAc_ac_c * pTarget = fopAcM_SearchByID(mLockOnTargetBsPcID);
    initList(interactMask);
    if (makeList())
        sortList();

    if (pTarget == mLockOnList[0].getActor() && mLockOnNum > 1)
        mLockOnOffs = 1;

    LockonTarget(0);
}

/* 8009E728-8009E764       .text freeAttention__12dAttention_cFv */
s32 dAttention_c::freeAttention() {
    mLockOnOffs = 0;
    mLockOnNum = 0;
    mActionOffs = 0;
    mActionNum = 0;
    initList(0xFFFFFFFF);
    return 0;
}

/* 8009E764-8009E978       .text chaseAttention__12dAttention_cFv */
void dAttention_c::chaseAttention() {
    /* Nonmatching */
}

/* 8009E978-8009EA24       .text EnemyDistance__12dAttention_cFP10fopAc_ac_c */
f32 dAttention_c::EnemyDistance(fopAc_ac_c* actor) {
    /* Nonmatching */
    if (actor == mpPlayer || mpPlayer == NULL)
        return -1.0f;

    if (fopAcM_GetProfName(actor) == PROC_PLAYER)
        return -1.0f;

    if (!(actor->mAttentionInfo.mFlags & 4) && !(actor->mAttentionInfo.mFlags & 0x4000000))
        return -1.0f;

    f32 dist = fopAcM_searchActorDistance(actor, mpPlayer);
    if (dist < (dist_table[actor->mAttentionInfo.mDistances[2]].mDistXZMax + dist_table[actor->mAttentionInfo.mDistances[2]].mDistXZAngleAdjust))
        return dist;
    return -1.0f;
}

/* 8009EA24-8009EAA4       .text sound_attention__FP10fopAc_ac_cPv */
BOOL sound_attention(fopAc_ac_c* actor, void* userWork) {
    dAttention_c* i_this = (dAttention_c*)userWork;
    f32 dist = i_this->EnemyDistance(actor);
    if (dist < 0.0f)
        return FALSE;
    if (dist < i_this->mEnemyDistance) {
        i_this->mEnemyBsPcId = fopAcM_GetID(actor);
        i_this->mEnemyDistance = dist;
    }
    return FALSE;
}

/* 8009EAA4-8009EB38       .text runSoundProc__12dAttention_cFv */
void dAttention_c::runSoundProc() {
    mEnemyBsPcId = -1;
    mEnemyDistance = 10000.0f;
    if (!(mFlags & 0x80000000)) {
        fopAcIt_Executor((fopAcIt_ExecutorFunc)sound_attention, this);
        fopAc_ac_c* actor = fopAcM_SearchByID(mEnemyBsPcId);
        if (actor != NULL) {
            mDoAud_bgmNowBattle(mEnemyDistance * 0.1f);
            mFlags |= 0x100;
        }
    }
}

/* 8009EB38-8009EDB8       .text runDrawProc__12dAttention_cFv */
void dAttention_c::runDrawProc() {
    /* Nonmatching */
}

/* 8009EDB8-8009EDBC       .text runDebugDisp0__12dAttention_cFv */
void dAttention_c::runDebugDisp0() {
}

/* 8009EDBC-8009EDC0       .text runDebugDisp__12dAttention_cFv */
void dAttention_c::runDebugDisp() {
}

/* 8009EDC0-8009EE90       .text judgementButton__12dAttention_cFv */
void dAttention_c::judgementButton() {
    /* Nonmatching */
}

/* 8009EE90-8009EED8       .text judgementTriggerProc__12dAttention_cFv */
void dAttention_c::judgementTriggerProc() {
    /* Nonmatching */
}

/* 8009EED8-8009EF40       .text judgementLostCheck__12dAttention_cFv */
void dAttention_c::judgementLostCheck() {
    /* Nonmatching */
}

/* 8009EF40-8009F0A4       .text judgementStatusSw__12dAttention_cFUl */
void dAttention_c::judgementStatusSw(u32) {
    /* Nonmatching */
}

/* 8009F0A4-8009F1D4       .text judgementStatusHd__12dAttention_cFUl */
void dAttention_c::judgementStatusHd(u32) {
    /* Nonmatching */
}

/* 8009F1D4-8009F460       .text Run__12dAttention_cFUl */
void dAttention_c::Run(u32) {
    /* Nonmatching */
}

/* 8009F460-8009F5FC       .text Draw__12dAttention_cFv */
void dAttention_c::Draw() {
    /* Nonmatching */
}

/* 8009F5FC-8009F6B4       .text setAnm__10dAttDraw_cFiii */
void dAttDraw_c::setAnm(int, int, int) {
    /* Nonmatching */
}

/* 8009F6B4-8009F834       .text draw__10dAttDraw_cFR4cXyzPA4_f */
void dAttDraw_c::draw(cXyz&, f32(*)[4]) {
    /* Nonmatching */
}

/* 8009F834-8009F88C       .text LockonTarget__12dAttention_cFl */
fopAc_ac_c* dAttention_c::LockonTarget(s32 idx) {
    if (idx >= mLockOnNum)
        return NULL;

    s32 listIdx = mLockOnOffs + idx;
    if (listIdx >= mLockOnNum)
        listIdx -= mLockOnNum;

    return mLockOnList[listIdx].getActor();
}

/* 8009F88C-8009F980       .text LockonReleaseDistanse__12dAttention_cFv */
void dAttention_c::LockonReleaseDistanse() {
    /* Nonmatching */
}

/* 8009F980-8009F9B8       .text LockonTargetPId__12dAttention_cFl */
unsigned int dAttention_c::LockonTargetPId(s32 idx) {
    if (idx >= mLockOnNum)
        return NULL;

    s32 listIdx = mLockOnOffs + idx;
    if (listIdx >= mLockOnNum)
        listIdx -= mLockOnNum;

    return mLockOnList[listIdx].getPid();
}

/* 8009F9B8-8009FA10       .text ActionTarget__12dAttention_cFl */
fopAc_ac_c* dAttention_c::ActionTarget(s32 idx) {
    if (idx >= mActionNum)
        return NULL;

    s32 listIdx = mActionOffs + idx;
    if (listIdx >= mActionNum)
        listIdx -= mActionNum;

    return mActionList[listIdx].getActor();
}

/* 8009FA10-8009FA64       .text LockonTruth__12dAttention_cFv */
bool dAttention_c::LockonTruth() {
    return mLockOnState == 1 || (mLockOnState == 2 && LockonTarget(0));
}

/* 8009FA64-8009FA98       .text getActor__10dAttList_cFv */
fopAc_ac_c* dAttList_c::getActor() {
    return fopAcM_SearchByID(mActorID);
}

/* 8009FA98-8009FAB4       .text setActor__10dAttList_cFP10fopAc_ac_c */
void dAttList_c::setActor(fopAc_ac_c* i_actor) {
    mActorID = fpcM_GetID(i_actor);
}

/* 8009FAB4-8009FACC       .text getPId__10dAttHint_cFPv */
u32 dAttHint_c::getPId(void* i_proc) {
    return fpcM_GetID(i_proc);
}

/* 8009FACC-8009FAFC       .text convPId__10dAttHint_cFUi */
fopAc_ac_c* dAttHint_c::convPId(unsigned int i_procID) {
    return fopAcM_SearchByID(i_procID);
}

/* 8009FAFC-8009FB58       .text request__10dAttHint_cFP10fopAc_ac_ci */
int dAttHint_c::request(fopAc_ac_c* i_actor, int priority) {
    if (priority < 0)
        priority = 0x1FF;

    if (priority <= mPriority) {
        mHintActorID = getPId(i_actor);
        mPriority = priority;
    }

    return TRUE;
}

/* 8009FB58-8009FB70       .text init__10dAttHint_cFv */
void dAttHint_c::init() {
    mHintActorID = fpcM_ERROR_PROCESS_ID_e;
    field_0x8 = fpcM_ERROR_PROCESS_ID_e;
    mPriority = 0x200;
}

/* 8009FB70-8009FB8C       .text proc__10dAttHint_cFv */
void dAttHint_c::proc() {
    field_0x8 = mHintActorID;
    mHintActorID = fpcM_ERROR_PROCESS_ID_e;
    mPriority = 0x200;
}

/* 8009FB8C-8009FBBC       .text convPId__11dAttCatch_cFUi */
fopAc_ac_c* dAttCatch_c::convPId(unsigned int i_procID) {
    return fopAcM_SearchByID(i_procID);
}

/* 8009FBBC-8009FBDC       .text init__11dAttCatch_cFv */
void dAttCatch_c::init() {
    field_0xc = 0x56;
    field_0x0 = fpcM_ERROR_PROCESS_ID_e;
    mCatghTargetID = fpcM_ERROR_PROCESS_ID_e;
    field_0x4 = 3;
}

/* 8009FBDC-8009FC08       .text proc__11dAttCatch_cFv */
void dAttCatch_c::proc() {
    mCatghTargetID = field_0x0;
    mChangeItem = field_0xc;
    field_0x0 = fpcM_ERROR_PROCESS_ID_e;
    field_0x4 = 3;
    field_0xc = 0x56;
}

/* 8009FC08-8009FE10       .text request__11dAttCatch_cFP10fopAc_ac_cUcfffsi */
void dAttCatch_c::request(fopAc_ac_c*, u8, f32, f32, f32, s16, int) {
    /* Nonmatching */
}

/* 8009FE10-8009FE40       .text convPId__10dAttLook_cFUi */
fopAc_ac_c* dAttLook_c::convPId(unsigned int i_procID) {
    return fopAcM_SearchByID(i_procID);
}

/* 8009FE40-8009FE58       .text init__10dAttLook_cFv */
void dAttLook_c::init() {
    field_0x0 = -1;
    mLookTargetID = -1;
    field_0x4 = 3;
}

/* 8009FE58-8009FE74       .text proc__10dAttLook_cFv */
void dAttLook_c::proc() {
    mLookTargetID = field_0x0;
    field_0x0 = -1;
    field_0x4 = 3;
}

/* 8009FE74-800A009C       .text request__10dAttLook_cFP10fopAc_ac_cfffsi */
void dAttLook_c::request(fopAc_ac_c*, f32, f32, f32, s16, int) {
    /* Nonmatching */
}

/* 800A009C-800A0270       .text requestF__10dAttLook_cFP10fopAc_ac_csi */
void dAttLook_c::requestF(fopAc_ac_c*, s16, int) {
    /* Nonmatching */
}

