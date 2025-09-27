#pragma once

#include "model/ModelExporterGltf.h"

#include "QtOpenGLViewer.h"
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class ModelRenderWidget : public QtOpenGLViewer
{
    unsigned int m_glVertexArray;
    unsigned int m_glVertexBufferObject;
    unsigned int m_glElementBufferObject;
    unsigned int m_shaderProgram;
    unsigned int m_uniformBufferObject;
    size_t m_indexCount;

public:
    ModelRenderWidget(QWidget *parent = NULL);

    void initializeGL() override;
    void drawScene() override;

    void loadModel(WarframeExporter::Model::ModelBodyInternal& modelInternal);
private slots:
    void onLightDirectionChanged();

private:
    void loadShaders();
    void loadModel();
    void loadUniforms();

    void updateUniform();
    QSlider* m_lightXSlider;
    QSlider* m_lightYSlider;
    QSlider* m_lightZSlider;
    QLabel* m_lightDirectionLabel;
    glm::vec3 m_lightDirection;
    GLint m_lightDirUniformLocation;
};
