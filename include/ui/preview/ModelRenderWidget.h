#pragma once

#include "model/ModelExporterGltf.h"

#include "QtOpenGLViewer.h"

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

private:
    void loadShaders();
    void loadModel();
    void loadUniforms();

    void updateUniform();
};