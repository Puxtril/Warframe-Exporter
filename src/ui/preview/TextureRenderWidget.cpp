#include "ui/preview/TextureRenderWidget.h"

TextureRenderWidget::TextureRenderWidget(QWidget *parent)
    : QtOpenGLViewer(parent)
{
    setIs3D(false);

    m_textureMap = {
        {WarframeExporter::Texture::TextureCompression::Uncompressed, {GL_RGBA8, GL_RGBA}},
        {WarframeExporter::Texture::TextureCompression::Default, {GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GL_RGB}},
        {WarframeExporter::Texture::TextureCompression::BC1, {GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GL_RGB}},
        {WarframeExporter::Texture::TextureCompression::BC2, {GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_RGBA}},
        {WarframeExporter::Texture::TextureCompression::BC3, {GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_RGBA}},
        {WarframeExporter::Texture::TextureCompression::BC4, {GL_COMPRESSED_RED_RGTC1_EXT, GL_RED}},
        {WarframeExporter::Texture::TextureCompression::BC5, {GL_COMPRESSED_RED_GREEN_RGTC2_EXT, GL_RG}},
        {WarframeExporter::Texture::TextureCompression::BC6, {GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB, GL_RGB}},
        {WarframeExporter::Texture::TextureCompression::BC7, {GL_COMPRESSED_RGBA_BPTC_UNORM, GL_RGBA}}
    };
}

void
TextureRenderWidget::initializeGL()
{
    initializeOpenGLFunctions();
    loadShaders();
    loadSquare();
    loadTexture();
}

void
TextureRenderWidget::drawScene()
{
    glUseProgram(m_shaderProgram);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindVertexArray(m_glVertexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glElementBufferObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void
TextureRenderWidget::resizeGL(int width, int height)
{
    std::tuple<float, float> dims = getMeshCoordsForTexture(width, height, m_texWidth, m_texHeight);

    float data[] =
    {
        std::get<0>(dims), -std::get<1>(dims), 0, 1,
        std::get<0>(dims),  std::get<1>(dims), 0, 0,
        -std::get<0>(dims),  std::get<1>(dims), 1, 0,
        -std::get<0>(dims), -std::get<1>(dims), 1, 1
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, NULL, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, &data[0], GL_DYNAMIC_DRAW);
}

void
TextureRenderWidget::setTexture(WarframeExporter::Texture::TextureInternal& texture)
{
    makeCurrent();
    glBindTexture(GL_TEXTURE_2D, m_texture);

    std::tuple<unsigned int, unsigned int> glFormats = m_textureMap[texture.header.formatEnum];
    size_t mip0Start = texture.body.size() - texture.header.mip0Len;
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, std::get<0>(glFormats), texture.header.width, texture.header.height, 0, texture.header.mip0Len, texture.body.data() + mip0Start);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    m_texWidth = texture.header.width;
    m_texHeight = texture.header.height;
    
    resizeGL(width(), height());
    paintGL();
}

void
TextureRenderWidget::loadTexture()
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
}

void
TextureRenderWidget::loadSquare()
{
    static constexpr float data[] =
    {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    static constexpr unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &m_glVertexArray);
    glGenBuffers(1, &m_glVertexBufferObject);
    glGenBuffers(1, &m_glElementBufferObject);
  
    glBindVertexArray(m_glVertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), &data[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    // vertex texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void
TextureRenderWidget::loadShaders()
{
    const char* shaderCode = R"""(#version 330 core
        out vec4 FragColor;

        in vec2 TexCoord;

        uniform sampler2D ourTexture;

        void main()
        {
            FragColor = texture(ourTexture, TexCoord);
        })""";

    unsigned int shader;

    shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, shader);
    glLinkProgram(m_shaderProgram);

    glDeleteShader(shader);
    
    const char* vertexShaderCode = R"""(#version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aUv;

        out vec2 TexCoord;
        
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
            TexCoord = aUv;
        })""";

    unsigned int vertexShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    glAttachShader(m_shaderProgram, vertexShader);
    glLinkProgram(m_shaderProgram);

    glDeleteShader(vertexShader);
}


std::tuple<float, float>
TextureRenderWidget::getMeshCoordsForTexture(int screenWidth, int screenHeight, int texWidth, int texHeight)
{
    if (texWidth == texHeight)
    {
        if (screenHeight > screenWidth)
            return {1.0, (float)screenWidth / (float)screenHeight};
        return {(float)screenHeight / screenWidth, 1.0f};
    }

    if (texHeight > texWidth)
        return {((float)screenHeight / (float)screenWidth) * ((float)texWidth / (float)texHeight), 1.0};
    return {1.0, ((float)screenWidth / (float)screenHeight) * ((float)texHeight / (float)texWidth)};
}