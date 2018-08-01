#include <osg/Notify>
#include <osg/ref_ptr>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osg/Program>
#include <osg/Shader>
#include <osgViewer/Viewer> 
#include <osg/StateSet>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

static const char* vert_source = {
	"$OSG_GLSL_VERSION\n"
	"$OSG_PRECISION_FLOAT\n"
	"\n"
    "uniform mat4 osg_ModelViewProjectionMatrix;\n"
    "uniform mat3 osg_NormalMatrix;\n"
    "in vec4 osg_Vertex;\n"
    "in vec4 osg_MultiTexCoord0;\n"
	"in uint test_attr;\n"
	"out vec2 texCoord;\n"
	"\n"
	"void main(void)\n"
	"{\n"
    "    uint t = test_attr;\n" /* without this warning:  WARNING: Could not find vertex shader attribute 'test_attr' to match BindAttributeLocation request. */
	"    gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;\n"
	"    texCoord = osg_MultiTexCoord0.xy;\n"
	"}\n"
	"\n"
};

static const char* frag_source = {
	"$OSG_GLSL_VERSION\n"
	"uniform sampler2D tex;\n"
	"in vec2 texCoord;\n"
    "out vec4 color;\n"
	"void main(void)\n"
	"{\n"
	"    color = texture(tex, texCoord).rgba;\n"
	"}\n"
};

osg::Image* make_checker_image(unsigned int width, unsigned height)
{
	osg::Image* img = new osg::Image();
	img->allocateImage(width,height,1,GL_RGB, GL_UNSIGNED_BYTE);

	int i, j, c;

	srand(time(NULL));

	osg::Vec3f color(rand() % 128, rand() % 128, rand() % 128);

	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0)));
			unsigned char* pixel = img->data(i,j);
			pixel[0] = c * color.x();
			pixel[1] = c * color.y();
			pixel[2] = c * color.z();
		}
	}

	return img;
}

void create_tex_coords(osg::Vec2Array* uv, unsigned int size)
{
	for (unsigned int i = 0; i < size; ++i) {
		float u = ((float)rand() / (RAND_MAX));
		float v = ((float)rand() / (RAND_MAX));
		uv->push_back(osg::Vec2(u, v));
	}
}

class TexturedCube : public osg::Geometry
{
public:
	TexturedCube(int variant = 1)
	{
		osg::Vec3Array* vert = new osg::Vec3Array();
		setVertexArray(vert);

		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, -1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(-1.0f, 1.0f, 1.0f));
		vert->push_back(osg::Vec3(1.0f, -1.0f, 1.0f));

		osg::Vec2Array* uv = new osg::Vec2Array();
		setTexCoordArray(0, uv);

	    uv->push_back(osg::Vec2(0.000059f, 1.0f-0.000004f));
		uv->push_back(osg::Vec2(0.000103f, 1.0f-0.336048f));
		uv->push_back(osg::Vec2(0.335973f, 1.0f-0.335903f));
		uv->push_back(osg::Vec2(1.000023f, 1.0f-0.000013f));
		uv->push_back(osg::Vec2(0.667979f, 1.0f-0.335851f));
		uv->push_back(osg::Vec2(0.999958f, 1.0f-0.336064f));
		uv->push_back(osg::Vec2(0.667979f, 1.0f-0.335851f));
		uv->push_back(osg::Vec2(0.336024f, 1.0f-0.671877f));
		uv->push_back(osg::Vec2(0.667969f, 1.0f-0.671889f));
		uv->push_back(osg::Vec2(1.000023f, 1.0f-0.000013f));
		uv->push_back(osg::Vec2(0.668104f, 1.0f-0.000013f));
		uv->push_back(osg::Vec2(0.667979f, 1.0f-0.335851f));
		uv->push_back(osg::Vec2(0.000059f, 1.0f-0.000004f));
		uv->push_back(osg::Vec2(0.335973f, 1.0f-0.335903f));
		uv->push_back(osg::Vec2(0.336098f, 1.0f-0.000071f));
		uv->push_back(osg::Vec2(0.667979f, 1.0f-0.335851f));
		uv->push_back(osg::Vec2(0.335973f, 1.0f-0.335903f));
		uv->push_back(osg::Vec2(0.336024f, 1.0f-0.671877f));
		uv->push_back(osg::Vec2(1.000004f, 1.0f-0.671847f));
		uv->push_back(osg::Vec2(0.999958f, 1.0f-0.336064f));
		uv->push_back(osg::Vec2(0.667979f, 1.0f-0.335851f));
		uv->push_back(osg::Vec2(0.668104f, 1.0f-0.000013f));
		uv->push_back(osg::Vec2(0.335973f, 1.0f-0.335903f));
		uv->push_back(osg::Vec2(0.667979f, 1.0f-0.335851f));
		uv->push_back(osg::Vec2(0.335973f, 1.0f-0.335903f));
		uv->push_back(osg::Vec2(0.668104f, 1.0f-0.000013f));
		uv->push_back(osg::Vec2(0.336098f, 1.0f-0.000071f));
		uv->push_back(osg::Vec2(0.000103f, 1.0f-0.336048f));
		uv->push_back(osg::Vec2(0.000004f, 1.0f-0.671870f));
		uv->push_back(osg::Vec2(0.336024f, 1.0f-0.671877f));
		uv->push_back(osg::Vec2(0.000103f, 1.0f-0.336048f));
		uv->push_back(osg::Vec2(0.336024f, 1.0f-0.671877f));
		uv->push_back(osg::Vec2(0.335973f, 1.0f-0.335903f));
		uv->push_back(osg::Vec2(0.667969f, 1.0f-0.671889f));
		uv->push_back(osg::Vec2(1.000004f, 1.0f-0.671847f));
		uv->push_back(osg::Vec2(0.667979f, 1.0f-0.335851f));

        m_attrib_name = "test_attr";
        m_attrib_location = osg::Drawable::COLORS;

		osg::UIntArray* ids = new osg::UIntArray();
		ids->setBinding(osg::Array::BIND_PER_VERTEX);
		ids->setNormalize(false);
        for(int i=0;i<vert->size();++i) {
            ids->push_back(0);
        }
        setVertexAttribArray(m_attrib_location, ids);

		osg::StateSet* ss = getOrCreateStateSet();

		// set up the texture state.
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
		texture->setImage(make_checker_image(128,128));
		ss->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);

		osg::Program* program = new osg::Program;
		program->setName("sample");
        
        program->addBindAttribLocation(m_attrib_name, m_attrib_location);
        program->addShader(new osg::Shader(osg::Shader::VERTEX, vert_source));
        program->addShader(new osg::Shader(osg::Shader::FRAGMENT, frag_source));
        ss->setAttributeAndModes(program, osg::StateAttribute::ON);
        
        addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vert->size()));

	}
    
private:
	std::string m_attrib_name;
	unsigned int m_attrib_location;

};

int main(int argc, char** argv)
{
    //osg::DisplaySettings::instance()->setShaderHint(osg::DisplaySettings::SHADER_NONE);

    osg::ArgumentParser arguments(&argc, argv);
	osg::Geode* root(new osg::Geode);
	root->addDrawable(new TexturedCube());

	osgViewer::Viewer viewer(arguments);
    
#if 1
    // remove unused default shader.
    osg::StateSet* cam_ss = viewer.asView()->getCamera()->getStateSet();
    if(cam_ss) {
        cam_ss->removeAttribute(osg::StateAttribute::PROGRAM);
    }
#endif
	osg::GraphicsContext* gc = viewer.asView()->getCamera()->getGraphicsContext();
	if (gc)
	{
		osg::State* state = gc->getState();

#ifdef OSG_GL3_AVAILABLE
		state->setUseModelViewAndProjectionUniforms(true);
		state->setUseVertexAttributeAliasing(true);
#endif
	}


	viewer.setSceneData(root);
	return viewer.run();
}
