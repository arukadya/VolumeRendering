//
//  ComputeCommand.cpp
//  VolumeRendering
//
//  Created by 須之内俊樹 on 2024/07/15.
//

#include "ComputeCommand.hpp"

unsigned int resequence3to1(unsigned int i,unsigned int j,unsigned int k,unsigned int Ni,unsigned int Nj,unsigned int Nk)
{
    return k * Ni*Nj + j * Ni + i;
}
