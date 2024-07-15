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
//#include "ShapeIndex.h"
#include "SolidShapeIndex.h"
#include "SolidShape.h"
//#include "Mesh.hpp"
#include "Simulator.hpp"
#include "SliceRenderer.hpp"
// 六面体の頂点の位置
constexpr Object::Vertex cubeVertex[] =
{
    { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f }, // (0)
    { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.8f }, // (1)
    { -0.5f,  0.5f,  0.5f, 0.0f, 0.8f, 0.0f }, // (2)
    { -0.5f,  0.5f, -0.5f, 0.0f, 0.8f, 0.8f }, // (3)
    {  0.5f,  0.5f, -0.5f, 0.8f, 0.0f, 0.0f }, // (4)
    {  0.5f, -0.5f, -0.5f, 0.8f, 0.0f, 0.8f }, // (5)
    {  0.5f, -0.5f,  0.5f, 0.8f, 0.8f, 0.8f }, // (6)
    {  0.5f,  0.5f,  0.5f, 0.8f, 0.8f, 0.8f } // (7)
};

// 六面体の稜線の両端点のインデックス
constexpr GLuint wireCubeIndex[] =
{
    1, 0, // (a)
    2, 7, // (b)
    3, 0, // (c)
    4, 7, // (d)
    5, 0, // (e)
    6, 7, // (f)
    1, 2, // (g)
    2, 3, // (h)
    3, 4, // (i)
    4, 5, // (j)
    5, 6, // (k)
    6, 1 // (l)
};
// 六面体の面を塗りつぶす三角形の頂点のインデックス
constexpr GLuint solidCubeIndex[] =
{
    0, 1, 2, 3, 4, 5, // 左
    6, 7, 8, 9,10,11,//裏
    12, 13, 14, 15, 16, 17, // 下
    18, 19, 20, 21, 22, 23, // 右
    24, 25, 26, 27, 28, 29, // 上
    30,31,32,33,34,35 //前
};

// 面ごとに色を変えた六面体の頂点属性
constexpr Object::Vertex solidCubeVertex[] =
{
    // 左
    { -0.1f, -0.1f, -0.1f, 0.1f, 0.8f, 0.1f },
    { -0.1f, -0.1f, 0.1f, 0.1f, 0.8f, 0.1f },
    { -0.1f, 0.1f, 0.1f, 0.1f, 0.8f, 0.1f },
    { -0.1f, -0.1f, -0.1f, 0.1f, 0.8f, 0.1f },
    { -0.1f, 0.1f, 0.1f, 0.1f, 0.8f, 0.1f },
    { -0.1f, 0.1f, -0.1f, 0.1f, 0.8f, 0.1f },
    
    // 裏
    { 0.1f, -0.1f, -0.1f, 0.8f, 0.1f, 0.8f },
    { -0.1f, -0.1f, -0.1f, 0.8f, 0.1f, 0.8f },
    { -0.1f, 0.1f, -0.1f, 0.8f, 0.1f, 0.8f },
    { 0.1f, -0.1f, -0.1f, 0.8f, 0.1f, 0.8f },
    { -0.1f, 0.1f, -0.1f, 0.8f, 0.1f, 0.8f },
    { 0.1f, 0.1f, -0.1f, 0.8f, 0.1f, 0.8f },
    
    // 下
    { -0.1f, -0.1f, -0.1f, 0.1f, 0.8f, 0.8f },
    { 0.1f, -0.1f, -0.1f, 0.1f, 0.8f, 0.8f },
    { 0.1f, -0.1f, 0.1f, 0.1f, 0.8f, 0.8f },
    { -0.1f, -0.1f, -0.1f, 0.1f, 0.8f, 0.8f },
    { 0.1f, -0.1f, 0.1f, 0.1f, 0.8f, 0.8f },
    { -0.1f, -0.1f, 0.1f, 0.1f, 0.8f, 0.8f },
    // 右
    { 0.1f, -0.1f, 0.1f, 0.1f, 0.1f, 0.8f },
    { 0.1f, -0.1f, -0.1f, 0.1f, 0.1f, 0.8f },
    { 0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 0.8f },
    { 0.1f, -0.1f, 0.1f, 0.1f, 0.1f, 0.8f },
    { 0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 0.8f },
    { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.8f },
    
    // 上
    { -0.1f, 0.1f, -0.1f, 0.8f, 0.1f, 0.1f },
    { -0.1f, 0.1f, 0.1f, 0.8f, 0.1f, 0.1f },
    { 0.1f, 0.1f, 0.1f, 0.8f, 0.1f, 0.1f },
    { -0.1f, 0.1f, -0.1f, 0.8f, 0.1f, 0.1f },
    { 0.1f, 0.1f, 0.1f, 0.8f, 0.1f, 0.1f },
    { 0.1f, 0.1f, -0.1f, 0.8f, 0.1f, 0.1f },
    // 前
    { -0.1f, -0.1f, 0.1f, 0.8f, 0.8f, 0.1f },
    { 0.1f, -0.1f, 0.1f, 0.8f, 0.8f, 0.1f },
    { 0.1f, 0.1f, 0.1f, 0.8f, 0.8f, 0.1f },
    { -0.1f, -0.1f, 0.1f, 0.8f, 0.8f, 0.1f },
    { 0.1f, 0.1f, 0.1f, 0.8f, 0.8f, 0.1f },
    { -0.1f, 0.1f, 0.1f, 0.8f, 0.8f, 0.1f }
};
int main(int argc, char * argv[])
{
    Simulator simulator;
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
    
//    glEnable(GL_TEXTURE_3D);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //背景カリングを有効にする
//    glFrontFace(GL_CCW);
//    glCullFace(GL_BACK);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_ALPHA_TEST);
    
    //デプスバッファを有効にする
//    glClearDepth(1.0);
//    glDepthFunc(GL_LESS);
//    glEnable(GL_DEPTH_TEST);
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //ビューボートを設定する
    glViewport(100, 50, 300, 300);
    // プログラムオブジェクトを作成する
    const GLuint surfaceProgram(loadProgram("Shader/point.vert", "Shader/point.frag"));
    const GLuint volumeProgram(loadProgram("Shader/volume.vert", "Shader/volume.frag"));
//    const GLuint meshCompute(loadComputeProgram("mesh.comp"));
    //uniform変数の場所を取得する
    const GLint surface_modelviewLoc(glGetUniformLocation(surfaceProgram, "modelview"));
    const GLint surface_projectionLoc(glGetUniformLocation(surfaceProgram, "projection"));
    const GLint surface_light_vecLoc(glGetUniformLocation(surfaceProgram, "light_vec"));
    
    const GLint mtLoc(glGetUniformLocation(volumeProgram, "mt"));
    const GLint mwLoc(glGetUniformLocation(volumeProgram, "mw"));
    const GLint mpLoc(glGetUniformLocation(volumeProgram, "mp"));
    const GLint spacingLoc(glGetUniformLocation(volumeProgram, "spacing"));
    const GLint volume_light_vecLoc(glGetUniformLocation(volumeProgram, "light_vec"));
    const GLint volumeLoc(glGetUniformLocation(volumeProgram, "volume"));
    const GLint thresholdLoc(glGetUniformLocation(volumeProgram, "threshold"));
    //図形データを作成する
//   std::unique_ptr<const Shape>shape(new SolidShapeIndex(3,static_cast<GLuint>(bunny.Vertices.size()) ,bunny.outputVertexData(),3 * static_cast<GLuint>(bunny.Faces.size()),bunny.outputFaceData()));
    
    std::unique_ptr<const Shape>shape_wire(new ShapeIndex(3,8,cubeVertex,24,wireCubeIndex));
    std::unique_ptr<const Shape>shape(new SolidShapeIndex(3,36,solidCubeVertex,36,solidCubeIndex));
    
    //ウィンドウが開いている間繰り返す
    //タイマーを0にセット
    glfwSetTime(0.0);
    //テキストデータのID
    int id = 50;
    while(window)
    {
        std::string inputFileName = "resources/density_txt/output";
        inputFileName += std::to_string(id % 100)+".txt";
        ++id;
        simulator.inputTXT(inputFileName);
        //ウィンドウを消去する
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //デプスバッファを有効にする
        glClearDepth(1.0);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        //背景カリングを有効にする
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        //シェーダプログラムの利用開始
        //描画処理
        glUseProgram(surfaceProgram);
        
        // 拡大縮小の変換行列を求める
        const GLfloat *const size(window.getSize());
        const GLfloat fovy(window.getScale() * 0.01f);
        const GLfloat aspect(size[0] / size[1]);
        const Matrix4x4 projection(Matrix4x4::perspective(fovy, aspect, 1.0f, 10.0f));
        
        // 平行移動の変換行列を求める
        const GLfloat *const position(window.getLocation());
        
        // モデル変換行列を求める
        const GLfloat *const location(window.getLocation());
//        const Matrix4x4 r(Matrix4x4::rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 1.0f, 0.0f));
        Matrix4x4 r(Matrix4x4::rotate(0.0f, 0.0f, 1.0f, 0.0f));
//        Matrix4x4 tr(Matrix4x4::textureRotate(0.0f, 0.0f, 1.0f, 0.0f));
        const Matrix4x4 model(Matrix4x4::translate(location[0], location[1], 0.0f) * r);
        
        Matrix4x4 trans(Matrix4x4::translate(-0.5f, -0.5f, -0.5f));
        //ビュー変換行列を求める
        const Matrix4x4 view(Matrix4x4::lookat(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
        //モデルビュー変換行列を求める
        const Matrix4x4 modelview(view * model);
        
        //レンダラーの設定
        Eigen::Vector3f tgt = {0.0f,0.0f,0.0f};
        sliceRenderer.setViewPoint(3.0f, 4.0f, 5.0f);
        GLfloat smokeColor[3] = {0.0f,0.0f,1.0f};
        sliceRenderer.setSliceDirection(tgt);
        // uniform 変数に値を設定する
        glUniformMatrix4fv(surface_projectionLoc, 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(surface_modelviewLoc, 1, GL_FALSE, modelview.data());
        glUniform4f(surface_light_vecLoc, 3.0f, 4.0f, 5.0f, 0.0f);
        
        //図形を描画する
//        shape->draw();
        shape_wire->draw();

        
        //ボリュームレンダリング
        //ボリュームレンダリングの設定
        //デプステスト無効
//        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glUseProgram(volumeProgram);
        glUniform1f(spacingLoc, 1.0f / static_cast<GLfloat>(SLICENUM - 1));
        glUniformMatrix4fv(mpLoc, 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(mwLoc, 1, GL_FALSE, modelview.data());
//        glUniformMatrix4fv(mtLoc, 1, GL_FALSE, tr.data());
        glUniform4f(volume_light_vecLoc, 3.0f, 4.0f, 5.0f, 0.0f);
        glUniform1f(thresholdLoc, sliceRenderer.getThreshold());
        
        //スライスの図形データを作成
        const GLuint slice(sliceRenderer.makeSlice());
        //ボリュームテクスチャを設定
        
        glEnable(GL_TEXTURE_3D);
        glEnable(GL_BLEND);
        glUniform1f(volumeLoc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, sliceRenderer.makeVolume(simulator.rhoTexture, smokeColor,tgt));
        glBindVertexArray(slice);
        //複製する描画方法．第四引数がインスタンスの数
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, SLICENUM);
        //カラーバッファを入れ替える
        
        glDisable(GL_TEXTURE_3D);
        glDisable(GL_BLEND);
        window.swapBuffers();
    }
    std::cout << "Hello, World!\n";
    return 0;
}
