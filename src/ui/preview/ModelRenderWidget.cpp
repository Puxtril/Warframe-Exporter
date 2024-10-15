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

    size_t combinedVertSize = (sizeof(float) * 3) + 3;
    std::vector<unsigned char> buf(combinedVertSize * modelInternal.positions.size());

    // Check if model has no/empty vertex colors
    bool fillWithWhite = false;
    if (modelInternal.AO.size() == 0)
        fillWithWhite = true;
    else
    {
        size_t sum = 0;
        for (const uint8_t& x : modelInternal.AO)
            sum += x;
        if (sum == 0)
            fillWithWhite = true;
    }
    
    // Copy Vertex position/color into buffer
    constexpr uint32_t all1 = 0xFFFFFFFF;
    for (size_t i = 0; i < modelInternal.positions.size(); i++)
    {
        size_t bufPtr = i * combinedVertSize;

        memcpy(&buf[0] + bufPtr, &modelInternal.positions[i][0], sizeof(float) * 3);
        bufPtr += sizeof(float) * 3;

        if (fillWithWhite)
            memcpy(&buf[0] + bufPtr, &all1, 3);
        else
        {
            memcpy(&buf[0] + bufPtr++, &modelInternal.AO[i], 1);
            memcpy(&buf[0] + bufPtr++, &modelInternal.AO[i], 1);
            memcpy(&buf[0] + bufPtr++, &modelInternal.AO[i], 1);
        }
    }
    glBufferData(GL_ARRAY_BUFFER, combinedVertSize * modelInternal.positions.size(), buf.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * modelInternal.indices.size(), modelInternal.indices.data(), GL_STATIC_DRAW);

    // vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, combinedVertSize, (void*)0);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, combinedVertSize, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    paintGL();
}

void
ModelRenderWidget::loadShaders()
{
    const char* shaderCode = R"""(#version 420 core
        in vec3 inCol;
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(inCol, 1.0);
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
        layout (location = 1) in vec3 aCol;

        out vec3 inCol;

        layout(binding = 0) uniform MatrixBlock
        {
            mat4 projection;
            mat4 modelview;
        };
        
        void main()
        {
            inCol = aCol;
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

