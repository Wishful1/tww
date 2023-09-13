#ifndef D_CC_D_CC_MASS_S_H
#define D_CC_D_CC_MASS_S_H

#include "SSystem/SComponent/c_cc_d.h"
#include "global.h"

typedef void (*dCcMassS_ObjCallback)(fopAc_ac_c*, cXyz*, u32);

class dCcMassS_Obj {
private:
    /* 0x00 */ cCcD_Obj* mpObj;
    /* 0x04 */ u8 mPriority;
    /* 0x08 */ dCcMassS_ObjCallback mpCallback;
    /* 0x0C */ cCcD_DivideInfo mDivideInfo;
    /* 0x14 vtable */

public:
    dCcMassS_Obj();
    virtual ~dCcMassS_Obj() {}
    void Set(cCcD_Obj* p_obj, u8 priority, dCcMassS_ObjCallback callback);
    void Clear();

    cCcD_Obj* GetObj() { return mpObj; }
    u8 GetPriority() const { return mPriority; }
    dCcMassS_ObjCallback GetCallback() const { return mpCallback; }
    cCcD_DivideInfo& GetDivideInfo() { return mDivideInfo; }
};  // Size = 0x18

STATIC_ASSERT(0x18 == sizeof(dCcMassS_Obj));

class dCcMassS_HitInf {
private:
    /* 0x00 */ cCcD_Obj* mpArea;
    /* 0x04 */ cCcD_Obj* mpAtObj;
    /* 0x08 */ cCcD_Obj* mpCoObj;
    /* 0x0C */ f32 mCoHitLen;

public:
    void ClearPointer();
    void SetAreaHitObj(cCcD_Obj* obj) { mpArea = obj; }
    void SetCoHitObj(cCcD_Obj* obj) { mpCoObj = obj; }
    void SetAtHitObj(cCcD_Obj* obj) { mpAtObj = obj; }
    void SetCoHitLen(f32 len) { mCoHitLen = len; }
    cCcD_Obj* GetAtHitObj() const { return mpAtObj; }
};

class dCcMassS_Mng {
public:
    /* 0x0000 */ cCcD_DivideArea mDivideArea;
    /* 0x0040 */ s32 mMassObjCount;
    /* 0x0044 */ dCcMassS_Obj mMassObjs[5];
    /* 0x00BC */ s32 mMassAreaCount;
    /* 0x00C0 */ dCcMassS_Obj mMassAreas[2];
    /* 0x00F0 */ cCcD_CylAttr mCylAttr;
    /* 0x0128 */ u8 field_0x200;
    /* 0x0129 */ u8 mResultCamBit;
    /* 0x012C */ u32 mFlag;
    /* 0x0130 */ u32 mResultCam;
    /* 0x0134 */ Vec mCamTopPos;
    /* 0x0140 */ f32 mCamTopDist;
    /* 0x0144 */ Vec mCamBottomPos;
    /* 0x0150 */ f32 mCamBottomDist;
    /* 0x0154 */ cCcD_CpsAttr mCpsAttr;
    /* 0x0194 */ cCcD_DivideInfo mDivideInfo;
    /* 0x019C vtable */

public:
    dCcMassS_Mng();
    virtual ~dCcMassS_Mng() {}
    /* 800855E4 */ void Ct();
    /* 80085630 */ void SetAttr(f32 radius, f32 height, u8 param_2, u8 param_3);
    /* 80085690 */ void Prepare();
    /* 800858AC */ u32 Chk(cXyz* p_xyz, fopAc_ac_c** p_actor, dCcMassS_HitInf* p_hitInf);
    /* 80085CF0 */ void Clear();
    /* 80085D98 */ void Set(cCcD_Obj* p_obj, u8 priority);
    /* 80085E6C */ void SetCam(cM3dGCps const& cps);
    /* 80085EB0 */ u8 GetResultCam() const;
    /* 80085EB8 */ void GetCamTopPos(Vec* p_out);
};

STATIC_ASSERT(sizeof(dCcMassS_Mng) == 0x1A0);

#endif /* D_CC_D_CC_MASS_S_H */