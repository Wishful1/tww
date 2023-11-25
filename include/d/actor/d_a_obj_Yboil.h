#ifndef D_A_OBJ_YBOIL_H
#define D_A_OBJ_YBOIL_H

#include "f_op/f_op_actor.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"

class daObjYboil_c : public fopAc_ac_c {
public:
    inline s32 _create();
    inline BOOL _delete();
    inline BOOL _draw();
    inline BOOL _execute();
    void prm_get_swSave() const {}
    void set_sea_material(J3DMaterial*) {}

    void CreateHeap();
    void pos_reset(int);
    void CreateInit();
    void set_mtx();

public:
    /* Place member variables here */
};

#endif /* D_A_OBJ_YBOIL_H */