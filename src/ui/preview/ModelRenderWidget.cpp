#include "ui/preview/ModelRenderWidget.h"

ModelRenderWidget::ModelRenderWidget(QWidget *parent)
: QtOpenGLViewer(parent), m_lightDirection(0.55f, 0.55f, 0.55f)
{
    setIs3D(true);

    QWidget* controlPanel = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* lightLayout = new QHBoxLayout();

    m_lightDirectionLabel = new QLabel("Light Direction: (0.55, 0.55, 0.55)", this);

    m_lightXSlider = new QSlider(Qt::Horizontal, this);
    m_lightYSlider = new QSlider(Qt::Horizontal, this);
    m_lightZSlider = new QSlider(Qt::Horizontal, this);

    for (auto slider : {m_lightXSlider, m_lightYSlider, m_lightZSlider}) {
        slider->setRange(-100, 100);
        slider->setValue(100);
        slider->setTickPosition(QSlider::TicksBelow);
        slider->setTickInterval(50);
    }

    lightLayout->addWidget(new QLabel("X:"));
    lightLayout->addWidget(m_lightXSlider);
    lightLayout->addWidget(new QLabel("Y:"));
    lightLayout->addWidget(m_lightYSlider);
    lightLayout->addWidget(new QLabel("Z:"));
    lightLayout->addWidget(m_lightZSlider);

    mainLayout->addWidget(m_lightDirectionLabel);
    mainLayout->addLayout(lightLayout);
    mainLayout->addStretch();

    controlPanel->setLayout(mainLayout);

    connect(m_lightXSlider, &QSlider::valueChanged, this, &ModelRenderWidget::onLightDirectionChanged);
    connect(m_lightYSlider, &QSlider::valueChanged, this, &ModelRenderWidget::onLightDirectionChanged);
    connect(m_lightZSlider, &QSlider::valueChanged, this, &ModelRenderWidget::onLightDirectionChanged);
}

void
ModelRenderWidget::onLightDirectionChanged()
{
    m_lightDirection.x = m_lightXSlider->value() / 100.0f;
    m_lightDirection.y = m_lightYSlider->value() / 100.0f;
    m_lightDirection.z = m_lightZSlider->value() / 100.0f;

    float length = sqrt(m_lightDirection.x * m_lightDirection.x +
    m_lightDirection.y * m_lightDirection.y +
    m_lightDirection.z * m_lightDirection.z);
    if (length > 0.001f) {
        m_lightDirection /= length;
    }

    QString labelText = QString("Light Direction: (%1, %2, %3)")
    .arg(m_lightDirection.x, 0, 'f', 2)
    .arg(m_lightDirection.y, 0, 'f', 2)
    .arg(m_lightDirection.z, 0, 'f', 2);
    m_lightDirectionLabel->setText(labelText);

    update();
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

    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    updateUniform();

    glUseProgram(m_shaderProgram);

    glUniform3f(m_lightDirUniformLocation,
                m_lightDirection.x,
                m_lightDirection.y,
                m_lightDirection.z);

    glBindVertexArray(m_glVertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glElementBufferObject);

    // CLEARLY this isn't correct
    // Since this is just for previewing, I don't care to get this perfect
    // Divide this by 3 and some faces will be missing
    // Pas in `m_indexCount` directly and faces will draw over each other
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount / 2.9), GL_UNSIGNED_SHORT, 0);

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

    size_t combinedVertSize = (sizeof(float) * 3) + 3 + sizeof(uint8_t) * 4;
    std::vector<unsigned char> buf(combinedVertSize * modelInternal.positions.size());

    constexpr uint32_t all1 = 0xFFFFFFFF;
    for (size_t i = 0; i < modelInternal.positions.size(); i++)
    {
        size_t bufPtr = i * combinedVertSize;

        memcpy(&buf[0] + bufPtr, &modelInternal.positions[i][0], sizeof(float) * 3);
        bufPtr += sizeof(float) * 3;

        memcpy(&buf[0] + bufPtr, &all1, 3);
        bufPtr += 3;

        if (i < modelInternal.normals.size()) {
            memcpy(&buf[0] + bufPtr, &modelInternal.normals[i][0], sizeof(uint8_t) * 4);
        } else {
            uint8_t defaultNormal[4] = {128, 128, 255, 0};
            memcpy(&buf[0] + bufPtr, defaultNormal, sizeof(uint8_t) * 4);
        }
    }

    glBufferData(GL_ARRAY_BUFFER, combinedVertSize * modelInternal.positions.size(), buf.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * modelInternal.indices.size(), modelInternal.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, combinedVertSize, (void*)0);
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, combinedVertSize, (void*)(sizeof(float) * 3));
    glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, combinedVertSize, (void*)(sizeof(float) * 3 + 3));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    paintGL();
}

void
ModelRenderWidget::loadShaders()
{
    const char* shaderCode = R"""(#version 420 core
    in vec3 inCol;
    in vec3 Normal;
    out vec4 FragColor;

    uniform vec3 lightDir;

    void main()
    {
        vec3 norm = normalize(Normal);

        float diff = max(dot(norm, lightDir), 0.2);

        vec3 result = diff * inCol;

        FragColor = vec4(result, 1.0);
    })""";

    const char* vertexShaderCode = R"""(#version 420 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aCol;
    layout (location = 2) in uvec4 aNormal;

    out vec3 inCol;
    out vec3 Normal;

    layout(binding = 0) uniform MatrixBlock
    {
        mat4 projection;
        mat4 modelview;
    };

    uniform vec3 lightDir;

    vec3 fixNormal(uvec4 badNormal)
    {
        vec3 N;
        N.x = float(badNormal.x) / 255.0;
        N.y = float(badNormal.y) / 255.0;
        N.z = float(badNormal.z) / 255.0;

        float r0_x = N.z * 255.0;
        float r0_y = N.y * 15.9375;
        float r1_y = fract(r0_y);
        float r2_y = r1_y * 4096.0 + r0_x;
        float r1_z = floor(r0_y);
        float r2_x = N.x * 4080.0 + r1_z;

        float r1_y_temp = 0.000489 * r2_x - 1.0;
        float r1_z_temp = 0.000489 * r2_y - 1.0;

        r1_y_temp = min(1.0, r1_y_temp);
        float r2_x_temp = max(-1.0, r1_y_temp);
        r1_z_temp = min(1.0, r1_z_temp);
        float r2_y_temp = max(-1.0, r1_z_temp);

        float r1_z_abs = 1.0 - abs(r2_x_temp);
        float r2_z = r1_z_abs - abs(r2_y_temp);
        float r1_z_final = r2_z * (-1.0);
        r1_z_final = max(0.0, r1_z_final);

        bool r0_x_bool = (r2_y_temp >= 0.0);
        bool r0_y_bool = (r2_z >= 0.0);

        float r3_x, r3_y;

        if (r0_x_bool) {
            r3_x = r1_z_final;
        } else {
            r3_x = r1_z_final * (-1.0);
        }

        if (r0_y_bool) {
            r3_y = r1_z_final;
        } else {
            r3_y = r1_z_final * (-1.0);
        }

        float r4_x = r3_x + r2_x_temp;
        float r4_y = r3_y + r2_y_temp;
        float r4_z = r2_z;

        float r1_y_length = r4_x * r4_x + r4_y * r4_y + r4_z * r4_z;
        r1_y_length = sqrt(r1_y_length);
        float inv = 1.0 / r1_y_length;

        float r0_x_final = r4_x * inv;
        float r0_y_final = r4_y * inv;
        float r0_z_final = r4_z * inv;

        vec3 result;
        result.x = r0_x_final;
        result.y = r0_z_final;
        result.z = r0_y_final;

        return result;
    }

    void main()
    {
        inCol = aCol;
        vec3 correctNormal = fixNormal(aNormal);
        Normal = correctNormal;
        gl_Position = projection * modelview * vec4(aPos, 1.0);
    })""";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderCode, NULL);
    glCompileShader(fragmentShader);

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    m_lightDirUniformLocation = glGetUniformLocation(m_shaderProgram, "lightDir");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
