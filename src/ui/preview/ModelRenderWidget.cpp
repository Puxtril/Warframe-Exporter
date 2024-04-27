#include "ui/preview/ModelRenderWidget.h"

ModelRenderWidget::ModelRenderWidget(QWidget *parent)
    : QtOpenGLViewer(parent)
{
    setIs3D(true);
}

void
ModelRenderWidget::initializeGL()
{
    initializeOpenGLFunctions();
    loadShaders();
    loadModel();
    loadUniforms();
}

void
ModelRenderWidget::drawScene()
{
    drawAxes();
    
    glEnable(GL_CULL_FACE);
    glEnable (GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    updateUniform();

    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_glVertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glElementBufferObject);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void
ModelRenderWidget::loadModel(WarframeExporter::Model::ModelBodyInternal& modelInternal)
{
    makeCurrent();

    m_indexCount = modelInternal.indices.size();

    glBindVertexArray(m_glVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBufferObject);

    size_t vertexPosSize = sizeof(float) * 3 * modelInternal.positions.size();
    glBufferData(GL_ARRAY_BUFFER, vertexPosSize, modelInternal.positions.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * modelInternal.indices.size(), modelInternal.indices.data(), GL_STATIC_DRAW);

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    paintGL();
}

void
ModelRenderWidget::loadShaders()
{
    const char* shaderCode = R"""(#version 420 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        })""";

    unsigned int shader;

    shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, shader);
    glLinkProgram(m_shaderProgram);

    glDeleteShader(shader);
    
    const char* vertexShaderCode = R"""(#version 420 core
        layout (location = 0) in vec3 aPos;

        layout(binding = 0) uniform MatrixBlock
        {
            mat4 projection;
            mat4 modelview;
        };
        
        void main()
        {
            gl_Position = projection * modelview * vec4(aPos, 1.0);
        })""";

    unsigned int vertexShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    glAttachShader(m_shaderProgram, vertexShader);
    glLinkProgram(m_shaderProgram);

    glDeleteShader(vertexShader);
}

void
ModelRenderWidget::loadModel()
{
    glGenVertexArrays(1, &m_glVertexArray);
    glGenBuffers(1, &m_glVertexBufferObject);
    glGenBuffers(1, &m_glElementBufferObject);
}

void
ModelRenderWidget::loadUniforms()
{
    glUniformBlockBinding(m_shaderProgram, 0, 0);

    glGenBuffers(1, &m_uniformBufferObject);

    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uniformBufferObject, 0, 2 * sizeof(glm::mat4));
}

void
ModelRenderWidget::updateUniform()
{
    float projection[16];
    float modelview[16];
    glGetFloatv(GL_PROJECTION_MATRIX, projection);
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), projection);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), modelview);
}

