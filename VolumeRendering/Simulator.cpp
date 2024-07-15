//
//  Simulator.cpp
//  OpenGLTest
//
//  Created by 須之内俊樹 on 2024/05/26.
//
#include "Simulator.hpp"
Simulator::Simulator()
{
//    mesh.input("bun_zipper.obj");
//    inputTXT("resources/output99.txt");
//    inputTXT("resources/output50.txt");
//    for(int k=0;k<TEXDEPTH;++k){
//        for(int j=0;j<TEXHEIGHT;++j){
//            for(int i=0;i<TEXWIDTH;++i){
//                float value;
//                std::cout << rhoTexture[resequence3to1(i, j, k, TEXWIDTH, TEXHEIGHT, TEXDEPTH)] << std::endl;
//            }
//        }
//    }
}
//void Simulator::makeSDF()
//{
//    Eigen::Matrix3X<double> V;
//    Eigen::Matrix3X<int> F;
//    igl::SignedDistanceType type = igl::SIGNED_DISTANCE_TYPE_PSEUDONORMAL;
//    mesh.outputMatrixBaseData(V, F);
    //igl::signed_distance(SDFGridPos, V, F, type, SignedDistanceFunction, AABB_index, SquaredDistanceFunction, presudoNormals);
//}

void Simulator::inputTXT(std::string &InputFileName)
{
    float max = 0;
    rhoTexture = (float*)malloc(sizeof(float) * TEXDEPTH * TEXWIDTH * TEXHEIGHT);
    FILE *ifp = fopen(InputFileName.c_str(),"r");
    for(int k=0;k<TEXDEPTH;++k){
        for(int j=0;j<TEXHEIGHT;++j){
            for(int i=0;i<TEXWIDTH;++i){
                float value;
                fscanf(ifp, "%f", &value);
//                std::cout << value << std::endl;
//                if(value < 10)value = 0;
//                if(max < value)max = value;
                rhoTexture[resequence3to1(i, TEXHEIGHT - j, k, TEXWIDTH, TEXHEIGHT, TEXDEPTH)] = value;
            }
        }
    }
//    std::cout << InputFileName << " max = " << max << std::endl;
    fclose(ifp);
}

//void Simulator::testSDF()
//{
//    Eigen::Matrix3X<double> V;
//    Eigen::Matrix3X<int> F;
//    igl::marching_cubes(SignedDistanceFunction, SDFGridPos, <#const unsigned int nx#>, <#const unsigned int ny#>, <#const unsigned int nz#>, V, F);
//}
