//
// Generated by dtk
// Translation Unit: J2DPicture.cpp
//

#include "JSystem/J2DGraph/J2DPicture.h"
#include "JSystem/JKernel/JKRFileLoader.h"
#include "JSystem/JSupport/JSURandomInputStream.h"
#include "JSystem/JUtility/JUTResource.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "dolphin/types.h"

/* 802D2D94-802D2E1C       .text __ct__10J2DPictureFv */
J2DPicture::J2DPicture() {
    mpTexture[0] = NULL;
    mNumTexture = 0;
    mpPalette = NULL;
    initinfo();
}

/* 802D2E1C-802D326C       .text __ct__10J2DPictureFP7J2DPaneP20JSURandomInputStream */
J2DPicture::J2DPicture(J2DPane* param_1, JSURandomInputStream* param_2) : mpPalette(NULL) {
    /* Nonmatching */
    J2DPaneHeader header;
    s32 var1 = param_2->getPosition();
    param_2->read(&header, sizeof(J2DPaneHeader));
    mMagic = header.mMagic;
    s32 r31 = var1 + header.mSize;
    makePaneStream(param_1, param_2);
    JUTResReference ref;
    u8 r28 = param_2->readU8();
    ResTIMG* r30 = (ResTIMG*)ref.getResource(param_2, 'TIMG', NULL);
    ResTLUT* r29 = (ResTLUT*)ref.getResource(param_2, 'TLUT', NULL);
    field_0xde = param_2->read8b();
    r28 -= 3;
    if (r28) {
        field_0xdf = param_2->read8b();
        r28--;
    } else {
        field_0xdf = 0;
    }
    if (r28) {
        param_2->read8b();
        r28--;
    }
    mColorBlack = JUtility::TColor(0);
    mColorWhite = JUtility::TColor(-1);
    if (r28) {
        mColorBlack = JUtility::TColor(param_2->readU32());
        r28--;
    }
    if (r28) {
        mColorWhite = JUtility::TColor(param_2->readU32());
        r28--;
    }
    setCornerColor(JUtility::TColor(-1), JUtility::TColor(-1), JUtility::TColor(-1), JUtility::TColor(-1));
    for (int i = 0; r28 && i < 4; i++) {
        mCornerColor[i] = JUtility::TColor(param_2->readU32());
        r28--;
    }
    param_2->seek(r31, JSUStreamSeekFrom_SET);
    mpTexture[0] = NULL;
    mNumTexture = 0;
    mValidTexture = 1;
    if (r30) {
        mpTexture[0] = new JUTTexture(r30, 0);
        mNumTexture++;
    }
    if (r29) {
        mpPalette = new JUTPalette(GX_TLUT0, r29);
        mpTexture[0]->attachPalette(mpPalette);
    }
    setBlendRatio(1.0f, 1.0f, 1.0f, 1.0f);
}

/* 802D326C-802D3320       .text __ct__10J2DPictureFPC7ResTIMG */
J2DPicture::J2DPicture(const ResTIMG* pTimg) {
    mpTexture[0] = NULL;
    mNumTexture = 0;
    if (pTimg) {
        insert(pTimg, mNumTexture, 1.0f);
    }
    mpPalette = NULL;
    initinfo();
}

/* 802D3320-802D33D4       .text __ct__10J2DPictureFPCc */
J2DPicture::J2DPicture(const char* name) {
    mpTexture[0] = NULL;
    mNumTexture = 0;
    if (name) {
        insert(name, mNumTexture, 1.0f);
    }
    mpPalette = NULL;
    initinfo();
}

/* 802D33D4-802D3474       .text __ct__10J2DPictureFUlRCQ29JGeometry8TBox2<f>PC7ResTIMGPC7ResTLUT */
J2DPicture::J2DPicture(u32 param_1, const JGeometry::TBox2<f32>& bounds, const ResTIMG* pTimg, const ResTLUT* pTlut) : J2DPane(param_1, bounds), mpPalette(NULL) {
    private_initiate(pTimg, pTlut);
    initinfo();
}

/* 802D3474-802D3530       .text initiate__10J2DPictureFPC7ResTIMGPC7ResTLUT */
void J2DPicture::initiate(const ResTIMG* pTimg, const ResTLUT* pTlut) {
    private_initiate(pTimg, pTlut);
    if (!mpTexture[0]) {
        return;
    }
    mBounds = JGeometry::TBox2<f32>(0.0f, 0.0f, mpTexture[0]->getWidth(), mpTexture[0]->getHeight());
    calcMtx();
}

/* 802D3530-802D35FC       .text private_initiate__10J2DPictureFPC7ResTIMGPC7ResTLUT */
void J2DPicture::private_initiate(const ResTIMG* pTimg, const ResTLUT* pTlut) {
    mpTexture[0] = NULL;
    mValidTexture = 1;
    mNumTexture = 0;
    if (pTimg) {
        mpTexture[0] = new JUTTexture(pTimg, 0);
        mNumTexture++;
    }
    mpPalette = NULL;
    if (pTlut) {
        mpPalette = new JUTPalette(GX_TLUT0, (ResTLUT*)pTlut);
        mpTexture[0]->attachPalette(mpPalette);
    }
}

/* 802D35FC-802D3774       .text initinfo__10J2DPictureFv */
void J2DPicture::initinfo() {
    mMagic = 'PIC1';
    field_0xde = 0x0f;
    field_0xdf &= 4;
    field_0xdf &= 3;
    setBlendRatio(1.0f, 1.0f, 1.0f, 1.0f);
    mColorBlack = JUtility::TColor(0);
    mColorWhite = JUtility::TColor(-1);
    setCornerColor(JUtility::TColor(-1), JUtility::TColor(-1), JUtility::TColor(-1), JUtility::TColor(-1));
}

/* 802D3774-802D3824       .text __dt__10J2DPictureFv */
J2DPicture::~J2DPicture() {
    for (int i = 0; i < mNumTexture; i++) {
        if (mValidTexture & 1 << i) {
            delete mpTexture[i];
        }
    }
    delete mpPalette;
}

/* 802D3824-802D3A08       .text insert__10J2DPictureFPC7ResTIMGUcf */
bool J2DPicture::insert(const ResTIMG* pTimg, u8 idx, f32 param_3) {
    /* Nonmatching */
    if (!pTimg || mNumTexture >= 4 || idx >= 4 || idx > mNumTexture) {
        return false;
    }
    JUTTexture* texture = new JUTTexture(pTimg, 0);
    for (u8 i = 3; idx < i; i--) {
        mpTexture[i] = mpTexture[i - 1];
        mBlendKonstColorF[i] = mBlendKonstColorF[i - 1];
        mBlendKonstAlphaF[i] = mBlendKonstAlphaF[i - 1];
    }
    s32 mask = (1 << idx) - 1;
    mValidTexture = (mValidTexture & mask) | ((mValidTexture & ~mask) << 1);
    mpTexture[idx] = texture;
    mValidTexture |= (1 << idx);
    mBlendKonstColorF[idx] = param_3;
    mBlendKonstAlphaF[idx] = param_3;
    if (mNumTexture == 0 && mpTexture[0]) {
        mBounds = JGeometry::TBox2<f32>(0.0f, 0.0f, mpTexture[0]->getWidth(), mpTexture[0]->getHeight());
        calcMtx();
    }
    mNumTexture++;
    setBlendKonstColor();
    setBlendKonstAlpha();
    return true;
}

/* 802D3A08-802D3A68       .text insert__10J2DPictureFPCcUcf */
bool J2DPicture::insert(const char* name, u8 idx, f32 param_3) {
    return insert((ResTIMG*)JKRGetNameResource(name, NULL), idx, param_3);
}

/* 802D3A68-802D3B78       .text remove__10J2DPictureFUc */
bool J2DPicture::remove(u8 idx) {
    if (mNumTexture <= idx || mNumTexture == 1) {
        return false;
    }
    if (mValidTexture & 1 << idx) {
        delete mpTexture[idx];
    }
    for (u8 i = idx; i < mNumTexture - 1; i++) {
        mpTexture[i] = mpTexture[i + 1];
        mBlendKonstColorF[i] = mBlendKonstColorF[i + 1];
        mBlendKonstAlphaF[i] = mBlendKonstAlphaF[i + 1];
    }
    mValidTexture = mValidTexture & (1 << idx) - 1 | ((mValidTexture & ~((1 << idx + 1) - 1)) >> 1);
    mNumTexture--;
    setBlendKonstColor();
    setBlendKonstAlpha();
    return true;
}

/* 802D3B78-802D3C34       .text changeTexture__10J2DPictureFPC7ResTIMGUc */
const ResTIMG * J2DPicture::changeTexture(const ResTIMG* timg, u8 idx) {
    if (idx >= mNumTexture)
        return NULL;
    if (getTexture(idx) == NULL || timg == NULL)
        return NULL;
    const ResTIMG * oldImg = getTexture(idx)->getTexInfo();
    getTexture(idx)->storeTIMG(timg, (u8)0);
    return oldImg;
}

/* 802D3C34-802D3C84       .text changeTexture__10J2DPictureFPCcUc */
const ResTIMG * J2DPicture::changeTexture(const char* name, u8 param_2) {
    return changeTexture((ResTIMG*)JKRGetNameResource(name, NULL), param_2);
}

/* 802D3C84-802D3CEC       .text drawSelf__10J2DPictureFff */
void J2DPicture::drawSelf(f32 x, f32 y) {
    Mtx mtx;
    MTXIdentity(mtx);
    drawSelf(x, y, &mtx);
}

/* 802D3CEC-802D3D54       .text drawSelf__10J2DPictureFffPA3_A4_f */
void J2DPicture::drawSelf(f32 x, f32 y, Mtx* mtx) {
    if (!mpTexture[0]) {
        return;
    }
    drawFullSet(mScreenBounds.i.x + x, mScreenBounds.i.y + y, mBounds.getWidth(), mBounds.getHeight(), J2DBinding(field_0xde), J2DMirror(field_0xdf & 3), field_0xdf >> 2 & 1, mtx);
}

/* 802D3D54-802D4074       .text drawFullSet__10J2DPictureFffff10J2DBinding9J2DMirrorbPA3_A4_f */
void J2DPicture::drawFullSet(f32 x, f32 y, f32 width, f32 height, J2DBinding binding, J2DMirror mirror, bool param_7, Mtx* mtx) {
    /* Nonmatching */
}

/* 802D4074-802D4490       .text draw__10J2DPictureFffffbbb */
void J2DPicture::draw(f32 param_1, f32 param_2, f32 param_3, f32 param_4, bool param_5, bool param_6, bool param_7) {
    if (!mVisible) {
        return;
    }
    for (u8 i = 0; i < mNumTexture; i++) {
        if (i < mNumTexture) {
            mpTexture[i]->load((GXTexMapID)i);
        }
    }
    GXSetNumTexGens(mNumTexture);
    mDrawAlpha = mAlpha;
    JUtility::TColor color[4];
    getNewColor(color);
    setTevMode();
    makeMatrix(param_1, param_2);
    GXLoadPosMtxImm(mMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(0.0f, 0.0f, 0.0f);
    GXColor1u32(color[0]);
    if (!param_7) {
        GXTexCoord2s16(param_5 ? 0x8000 : 0, param_6 ? 0x8000 : 0);
    } else {
        GXTexCoord2s16(param_5 ? 0x8000 : 0, param_6 ? 0 : 0x8000);
    }
    GXPosition3f32(param_3, 0.0f, 0.0f);
    GXColor1u32(color[1]);
    if (!param_7) {
        GXTexCoord2s16(param_5 ? 0 : 0x8000, param_6 ? 0x8000 : 0);
    } else {
        GXTexCoord2s16(param_5 ? 0x8000 : 0, param_6 ? 0x8000 : 0);
    }
    GXPosition3f32(param_3, param_4, 0.0f);
    GXColor1u32(color[3]);
    if (!param_7) {
        GXTexCoord2s16(param_5 ? 0 : 0x8000, param_6 ? 0 : 0x8000);
    } else {
        GXTexCoord2s16(param_5 ? 0 : 0x8000, param_6 ? 0x8000 : 0);
    }
    GXPosition3f32(0.0f, param_4, 0.0f);
    GXColor1u32(color[2]);
    if (!param_7) {
        GXTexCoord2s16(param_5 ? 0x8000 : 0, param_6 ? 0 : 0x8000);
    } else {
        GXTexCoord2s16(param_5 ? 0 : 0x8000, param_6 ? 0 : 0x8000);
    }
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_U16, 0x0f);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    Mtx m;
    MTXIdentity(m);
    GXLoadPosMtxImm(m, GX_PNMTX0);
    GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
}

/* 802D4490-802D4874       .text drawOut__10J2DPictureFRCQ29JGeometry8TBox2<f>RCQ29JGeometry8TBox2<f> */
void J2DPicture::drawOut(const JGeometry::TBox2<f32>&, const JGeometry::TBox2<f32>&) {
    /* Nonmatching */
}

/* 802D4874-802D4B3C       .text drawTexCoord__10J2DPictureFffffffffffffPA3_A4_f */
void J2DPicture::drawTexCoord(f32 x, f32 y, f32 width, f32 height, f32 s0, f32 t0, f32 s1, f32 t1, f32 s3, f32 t3, f32 s2, f32 t2, Mtx* mtx) {
    for (u8 i = 0; i < mNumTexture; i++) {
        if (i < mNumTexture) {
            mpTexture[i]->load(GXTexMapID(i));
        }
    }
    f32 x2 = x + width;
    f32 y2 = y + height;
    GXSetNumTexGens(mNumTexture);
    JUtility::TColor colors[4];
    getNewColor(colors);
    setTevMode();
    Mtx stack_108;
    MTXConcat(*mtx, mDrawMtx, stack_108);
    GXLoadPosMtxImm(stack_108, GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x, y, 0.0f);
    GXColor1u32(colors[0]);
    GXTexCoord2f32(s0, t0);
    GXPosition3f32(x2, y, 0.0f);
    GXColor1u32(colors[1]);
    GXTexCoord2f32(s1, t1);
    GXPosition3f32(x2, y2, 0.0f);
    GXColor1u32(colors[3]);
    GXTexCoord2f32(s2, t2);
    GXPosition3f32(x, y2, 0.0f);
    GXColor1u32(colors[2]);
    GXTexCoord2f32(s3, t3);
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0xf);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
}

static inline void GXSetTexCoordGen(GXTexCoordID dst, GXTexGenType type, GXTexGenSrc src, u32 mtx) {
    GXSetTexCoordGen2(dst, type, src, mtx, GX_FALSE, GX_PTIDENTITY);
}

/* 802D4B3C-802D4F44       .text setTevMode__10J2DPictureFv */
void J2DPicture::setTevMode() {
    /* Nonmatching */
    u8 i;
    for (i = 0; i < mNumTexture; i++) {
        GXSetTevOrder(GXTevStageID(i), GXTexCoordID(i), GXTexMapID(i), GX_COLOR_NULL);
    }
    GXSetTevColor(GX_TEVREG2, JUtility::TColor());
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    if (mpTexture[0]->getTransparency()) {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    } else {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    }
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevKColor(GX_KCOLOR0, mBlendKonstColor);
    GXSetTevKColor(GX_KCOLOR2, mBlendKonstAlpha);
    for (i = 1; i < mNumTexture; i++) {
        GXSetTevKColorSel(GXTevStageID(i), GXTevKColorSel(0x20 - i * 4));
        GXSetTevKAlphaSel(GXTevStageID(i), GXTevKAlphaSel(0x22 - i * 4));
        GXSetTevColorIn(GXTevStageID(i), GX_CC_CPREV, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);
        if (mpTexture[i]->getTransparency()) {
            GXSetTevAlphaIn(GXTevStageID(i), GX_CA_APREV, GX_CA_TEXA, GX_CA_KONST, GX_CA_ZERO);
        } else {
            GXSetTevAlphaIn(GXTevStageID(i), GX_CA_APREV, GX_CA_A2, GX_CA_KONST, GX_CA_ZERO);
        }
        GXSetTevColorOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    }
    if (mColorBlack != 0 || mColorWhite != 0xffffffff) {
        GXSetTevOrder(GXTevStageID(i), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColor(GX_TEVREG0, mColorBlack);
        GXSetTevColor(GX_TEVREG1, mColorWhite);
        GXSetTevColorIn(GXTevStageID(i), GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GXTevStageID(i), GX_CA_A0, GX_CA_A1, GX_CA_APREV, GX_CA_ZERO);
        GXSetTevColorOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        i++;
    }
    if (mDrawAlpha != 0xff || mCornerColor[0] != 0xffffffff || mCornerColor[1] != 0xffffffff || mCornerColor[2] != 0xffffffff || mCornerColor[3] != 0xffffffff) {
        GXSetTevOrder(GXTevStageID(i), GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GXTevStageID(i), GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevAlphaIn(GXTevStageID(i), GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevColorOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GXTevStageID(i), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        i++;
    }
    GXSetNumTevStages(u8(i));
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRC_ALPHA, GX_BL_INV_SRC_ALPHA, GX_LO_SET);
    for (i = 0; i < mNumTexture; i++) {
        GXSetTexCoordGen(GXTexCoordID(i), GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    }
    GXSetNumChans(1);
}

/* 802D4F44-802D4F58       .text swap__10J2DPictureFRfRf */
void J2DPicture::swap(f32& f1, f32& f2) {
    f32 tmp = f1;
    f1 = f2;
    f2 = tmp;
}

/* 802D4F58-802D5028       .text setBlendKonstColor__10J2DPictureFv */
void J2DPicture::setBlendKonstColor() {
    int var3 = 0;

    for (u8 i = 1; i < mNumTexture; i++) {
        f32 tmp = 0.0f;
        for (u8 j = 0; j < i; j++) {
            tmp += mBlendKonstColorF[j];
        }
        f32 tmp2 = tmp + mBlendKonstColorF[i];
        if (tmp2 != 0.0f) {
            var3 |= (u8)(255.0f * (1.0f - tmp / tmp2)) << (i - 1) * 8;
        }
    }
    mBlendKonstColor = var3;
}

/* 802D5028-802D50F8       .text setBlendKonstAlpha__10J2DPictureFv */
void J2DPicture::setBlendKonstAlpha() {
    int var3 = 0;

    for (u8 i = 1; i < mNumTexture; i++) {
        f32 tmp = 0.0f;
        for (u8 j = 0; j < i; j++) {
            tmp += mBlendKonstAlphaF[j];
        }
        f32 tmp2 = tmp + mBlendKonstAlphaF[i];
        if (tmp2 != 0.0f) {
            var3 |= (u8)(255.0f * (1.0f - tmp / tmp2)) << (i - 1) * 8;
        }
    }
    mBlendKonstAlpha = var3;
}

/* 802D50F8-802D51D8       .text getNewColor__10J2DPictureFPQ28JUtility6TColor */
void J2DPicture::getNewColor(JUtility::TColor* param_1) {
    param_1[0] = mCornerColor[0];
    param_1[1] = mCornerColor[1];
    param_1[2] = mCornerColor[2];
    param_1[3] = mCornerColor[3];

    if (mDrawAlpha == 0xff) {
        return;
    }
    param_1[0].a = (param_1[0].a * mDrawAlpha) / 0xff;
    param_1[1].a = (param_1[1].a * mDrawAlpha) / 0xff;
    param_1[2].a = (param_1[2].a * mDrawAlpha) / 0xff;
    param_1[3].a = (param_1[3].a * mDrawAlpha) / 0xff;
}
