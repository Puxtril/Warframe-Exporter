#pragma once

#include "texture/TextureExtractor.h"
#include "texture/TextureInfo.h"
#include "QtOpenGLViewer.h"

#include <map>

#include <GL/gl.h>
#include <QtOpenGL/QOpenGLVertexArrayObject>
#include <QtOpenGL/QOpenGLBuffer>
#include <QtOpenGL/QOpenGLShaderProgram>
#include <QtOpenGL/QOpenGLTexture>

class TextureRenderWidget : public QtOpenGLViewer
{
    unsigned int m_glVertexArray;
    unsigned int m_glVertexBufferObject;
    unsigned int m_glElementBufferObject;
    unsigned int m_texture;
    unsigned int m_shaderProgram;
    int m_texWidth;
    int m_texHeight;
    bool m_showAlpha;

    std::map<WarframeExporter::Texture::TextureCompression, std::tuple<unsigned int, unsigned int>> m_textureMap;

public:
    TextureRenderWidget(QWidget *parent = NULL);

    void initializeGL() override;
    void drawScene() override;
    void resizeGL(int width, int height) override;

    void setTexture(WarframeExporter::Texture::TextureInternal& texture);

private:
    void loadTexture();
    void loadSquare();
    void loadShaders();

    static std::tuple<float, float> getMeshCoordsForTexture(int screenWidth, int screenHeight, int texWidth, int texHeight);

public slots:
    void showAlpha(int state);
};