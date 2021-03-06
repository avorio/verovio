/////////////////////////////////////////////////////////////////////////////
// Name:        keysig.cpp
// Author:      Rodolfo Zitellini
// Created:     10/07/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "keysig.h"

//----------------------------------------------------------------------------

#include <assert.h>
#include <stdlib.h>

namespace vrv {

//----------------------------------------------------------------------------
// KeySig
//----------------------------------------------------------------------------

unsigned char KeySig::flats[] = {PITCHNAME_b, PITCHNAME_e, PITCHNAME_a, PITCHNAME_d, PITCHNAME_g, PITCHNAME_c, PITCHNAME_f};
unsigned char KeySig::sharps[] = {PITCHNAME_f, PITCHNAME_c, PITCHNAME_g, PITCHNAME_d, PITCHNAME_a, PITCHNAME_e, PITCHNAME_b};

int KeySig::octave_map[2][9][7] = {
    {// flats
       //C,  D,  E,  F,  G,  A,  B 
        {01, 01, 01, 00, 00, 00, 00}, // treble
        {00, 00, 00, 00, 00, 00, 00}, // soprano
        {00, 00, 00, 00, 00, -1, -1}, // mezzo
        {00, 00, 00, -1, -1, -1, -1}, // alto
        {00, 00, 00, -1, -1, -1, -1}, // tenor
        {-1, -1, -1, -1, -1, -2, -2}, // ??
        {-1, -1, -1, -2, -2, -2, -2}, // bass
        {-1, -1, -1, -1, -1, -1, -1}, // bariton
        {01, 01, 01, 00, 00, 00, 00}, // french g
    },
    {// sharps
       //C,  D,  E,  F,  G,  A,  B 
        {01, 01, 01, 01, 01, 00, 00}, // treble
        {00, 00, 00, 00, 00, 00, 00}, // soprano
        {00, 00, 00, 00, 00, 00, 00}, // mezzo
        {00, 00, 00, 00, 00, -1, -1}, // alto
        {00, 00, 00, -1, -1, -1, -1}, // tenor
        {-1, -1, -1, -1, -1, -2, -2}, // ??
        {-1, -1, -1, -1, -1, -2, -2}, // bass
        {-1, -1, -1, -1, -1, -1, -1}, // bariton
        {01, 01, 01, 01, 01, 00, 00}, // freench g
    },
};

KeySig::KeySig():
    LayerElement("ksig-")
{
    Reset();
}

KeySig::KeySig(int num_alter, char alter):
    LayerElement("ksig-")
{
    Reset();
    m_num_alter = num_alter;
    m_alteration = alter;
}
    
KeySig::KeySig( KeySigAttr *keySigAttr ):
    LayerElement("ksig-")
{
    Reset();
    char key = keySigAttr->GetKeySig() - KEYSIGNATURE_0;
    /* see data_KEYSIGNATURE order; key will be:
      0 for KEYSIGNATURE_0
     -1 for KEYSIGNATURE_1f
      1 for KEYSIGNATURE_1s
     -2 for KEYSIGNATURE_2f
      2 for KEYSIGNATURE_2s
     etc.
     */
    if ((key > (KEYSIGNATURE_7s - KEYSIGNATURE_0)) || (key < (KEYSIGNATURE_7f - KEYSIGNATURE_0))) {
        // other values are  KEYSIGNATURE_NONE or  KEYSIGNATURE_mixed (unsupported)
        return;
    }
    if (key > 0) {
        m_alteration = ACCID_SHARP;
    }
    else if (key < 0) {
        m_alteration = ACCID_FLAT;
    }
    m_num_alter = abs(key);
}

KeySig::~KeySig()
{
}
    
void KeySig::Reset()
{
    LayerElement::Reset();
    m_num_alter = 0;
    m_alteration = ACCID_NATURAL;
}

unsigned char KeySig::GetAlterationAt(int pos) {
    unsigned char *alteration_set;
    
    if (pos > 6)
        return 0;
    
    if (m_alteration == ACCID_FLAT)
        alteration_set = flats;
    else
        alteration_set = sharps;
    
    return alteration_set[pos];
}

int KeySig::GetOctave(unsigned char pitch, int clefId) {
    int alter_set = 0; // flats
    int key_set = 0;
    
    if (m_alteration == ACCID_SHARP)
        alter_set = 1;
    
    switch (clefId) {
        case G2: key_set = 0; break;
        case G2_8va: key_set = 0; break;
        case G2_8vb: key_set = 3; break;
        case C1: key_set = 1; break;
        case C2: key_set = 2; break;
        case C3: key_set = 3; break;
        case C4: key_set = 4; break;
        case C5: key_set = 5; break;
        case F5: key_set = 5; break;
        case F4: key_set = 6; break;
        case F3: key_set = 7; break;
        case G1: key_set = 8; break;
            
        default: key_set = 0; break;
    }
    
    return octave_map[alter_set][key_set][pitch - 1] + OCTAVE_OFFSET;
}

    
//----------------------------------------------------------------------------
// KeySigAttr
//----------------------------------------------------------------------------

KeySigAttr::KeySigAttr():
    Object(),
    AttKeySigDefaultLog()
{
    Reset();
}


KeySigAttr::~KeySigAttr()
{
}

void KeySigAttr::Reset()
{
    Object::Reset();
    ResetKeySigDefaultLog();
}

} // namespace vrv