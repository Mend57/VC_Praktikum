#pragma once

#include "OpenGLWindow.h"
#include "Transform.h"
#include "Cube.h"
#include <ShaderProgram.h>
#include <memory>
#include <AssetManager.h>

class Scene
{
public:
	Scene(OpenGLWindow* window);
	~Scene();

	bool init();
	void shutdown();
	void render(float dt);
	void update(float dt);
	OpenGLWindow* getWindow();

	void onKey(Key key, Action action, Modifier modifier);
	void onMouseMove(MousePosition mouseposition);
	void onMouseButton(MouseButton button, Action action, Modifier modifier);
	void onMouseScroll(double xscroll, double yscroll);
	void onFrameBufferResize(int width, int height);

private:
	void drawRobotPart(const glm::mat4& parentMatrix, Transform& transform);
	void setRobotInitialScale();
	void setRobotInitialPosition();
	void setupBuffers();

	OpenGLWindow* m_window;
	AssetManager m_assets;
    ShaderProgram* m_shader;
    GLuint vaoID, vboID, iboID;
	float animationTime = 0.0f;

	// Transform cubeTransform;
	Transform robotTransform;
	Transform torsoTransform;
	Transform headTransform;
	Transform leftLegTransform;
	Transform rightLegTransform;
	Transform leftUpperArmTransform;
	Transform leftLowerArmTransform;
	Transform rightUpperArmTransform;
	Transform rightLowerArmTransform;

	Transform initialLeftLegTransform;
	Transform initialRightLegTransform;
	Transform initialLeftUpperArmTransform;
	Transform initialRightUpperArmTransform;
};

