#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderCommand.hpp"
#include "Shape.h"
#include "Window.h"
#include "Matrix4x4.h"
#include "SolidShapeIndex.h"
#include "SolidShape.h"
#include "Simulator.hpp"
#include "FixedObjectRenderer.hpp"
#include "SliceRenderer.hpp"
#include "Eigen/Dense"
int main(int argc, char * argv[])
{
    Simulator simulator;
    FixedObjectRenderer fixedObjectRenderer;
    SliceRenderer sliceRenderer;
//    Mesh bunny = simulator.mesh;
//    bunny.input("bun_zipper_f10000.obj");
//    bunny.input("bun_zipper.obj");
    //GLFWを初期化する
    if(glfwInit() == GL_FALSE){
        std::cerr << "Can't initialize GLFW" << std::endl;
        return 1;
    }
    //プログラム終了時の処理を登録する
    atexit(glfwTerminate);
    //glfwのバージョン等の指定
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //ウィンドウを作成する
    Window window;
    //背景色を指定する
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //ビューボートを設定する
    glViewport(100, 50, 300, 300);
    //タイマーを0にセット
    glfwSetTime(0.0);
    //テキストデータのID
    int id = 0;
//    int id = 0;
    int sumcnt = 0;
    float startTime;
    float sum = 0;
    while(window)
    {
        
        std::string inputFileName = "resources/density_txt/output";
        inputFileName += std::to_string(id % 500)+".txt";
        ++id;
        simulator.inputTXT(inputFileName);
        Eigen::Vector3f viewPoint(4.0f, 4.0f, 4.0f);
//        viewPoint /= 1.732;
        // 拡大縮小の変換行列を求める
        const GLfloat *const size(window.getSize());
        const GLfloat fovy(window.getScale() * 0.01f);
        const GLfloat aspect(size[0] / size[1]);
        Matrix4x4 projection(Matrix4x4::perspective(fovy, aspect, 1.0f, 10.0f));
        // 平行移動の変換行列を求める
        const GLfloat *const position(window.getLocation());
        // モデル変換行列を求める
        const GLfloat *const location(window.getLocation());
//        const Matrix4x4 r(Matrix4x4::rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 1.0f, 0.0f));
        Matrix4x4 r(Matrix4x4::rotate(0.0f, 0.0f, 1.0f, 0.0f));
        const Matrix4x4 model(Matrix4x4::translate(location[0], location[1], 0.0f) * r);
        Matrix4x4 trans(Matrix4x4::translate(-0.5f, -0.5f, -0.5f));
        //ビュー変換行列を求める
        const Matrix4x4 view(Matrix4x4::lookat(viewPoint.x(), viewPoint.y(), viewPoint.z(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
        //モデルビュー変換行列を求める
        Matrix4x4 modelview(view * model);
        //レンダラーの設定
        Eigen::Vector3f tgt = {0.0f,0.0f,0.0f};
        Eigen::Vector3f sliceNormal(0.0f, 0.0f, 1.0f);
        Eigen::Vector3f cross = sliceNormal.cross(viewPoint - tgt);
        double dot = sliceNormal.dot(viewPoint - tgt);
        float sintheta = cross.norm()/((viewPoint - tgt).norm()*sliceNormal.norm());
        float costheta = dot / ((viewPoint - tgt).norm()*sliceNormal.norm());
        sliceRenderer.setViewPoint(viewPoint.x(), viewPoint.y(), viewPoint.z());
        Matrix4x4 sliceRot(
//                           Matrix4x4::rotate(asin(sintheta)
                           Matrix4x4::rotate(acos(costheta)
//                           Matrix4x4::rotate(0.7
                                             ,cross.normalized().x()
                                             ,cross.normalized().y()
                                             ,cross.normalized().z()
                                             )
                           );
        sliceRenderer.setSliceDirection(tgt);
        fixedObjectRenderer.rendering(projection, modelview);
        sliceRenderer.rendering(projection, modelview, sliceRot, simulator.rhoTexture);
        window.swapBuffers();
    }
    return 0;
}
