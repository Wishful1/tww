#ifndef C_M3D_G_TRI_H_
#define C_M3D_G_TRI_H_

#include "SSystem/SComponent/c_m3d_g_pla.h"
#include "dolphin/types.h"

class cM3dGCyl;

struct cM3dGTriS {
    Vec a;
    Vec b;
    Vec c;
};

class cM3dGTri : public cM3dGPla {
public:
    /* 0x00 */ /* cM3dGPla */
    /* 0x14 */ Vec mA;
    /* 0x20 */ Vec mB;
    /* 0x2C */ Vec mC;

public:
    cM3dGTri() {}
    cM3dGTri(const Vec *pA, const Vec *pB, const Vec *pC);

    bool cross(const cM3dGCyl* cyl, Vec* out) const;

    bool Cross(const cM3dGCps& cps, Vec* out) const { return cM3d_Cross_CpsTri(cps, *this, out); }
    bool Cross(const cM3dGCyl& cyl, Vec* out) const { return cross(&cyl, out); }
    bool Cross(const cM3dGSph& sph, Vec* out) const { return cM3d_Cross_SphTri(&sph, this, out); }
    bool Cross(const cM3dGTri& tri, Vec* out) const { return cM3d_Cross_TriTri(tri, *this, out); }

    void Up(f32 dist) {
        Vec delta;
        PSVECScale(&mNormal, &delta, dist);
        PSVECAdd(&mA, &delta, &mA);
        PSVECAdd(&mB, &delta, &mB);
        PSVECAdd(&mC, &delta, &mC);
        mD -= dist;
    }

    // TODO
    void crossX(const Vec*, f32*) const {}
    void crossY(const Vec*) const {}
    void crossZ(const Vec*, f32*) const {}
    void setBg(const Vec*, const Vec*, const Vec*, const cM3dGPla*) {}
    void setPos(const Vec*, const Vec*, const Vec*) {}
    void setUp() {}
    ~cM3dGTri() {}
};  // Size: 0x38

#endif
