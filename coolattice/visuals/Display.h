#pragma once

#include <vector>

#include "../graphical/graphics/Window.h"
#include "../graphical/zzz_tmx/ShaderProgram.h"
#include "../graphical/zzz_tmx/graphics/SimpleRenderer.h"
#include "../graphical/zzz_tmx/graphics/InstanceRenderer.h"
#include "../graphical/zzz_tmx/Camera.h"
#include "shader/LatticeShader.h"

//#include "../System.h"
#include "../graphical/zzz_tmx/ObjectPool.h"
#include "../graphical/zzz_tmx/graphics/Renderable.h"

#include "../system/System.h"
#include "../system/SystemTrajectory.h"

typedef std::vector<System> Trajectory;

namespace visuals {


	class Display
	{
		sparky::graphics::Window m_window;
		tmx::graphics::Camera m_camera;
		tmx::graphics::SimpleRenderer m_renderer;
		//tmx::graphics::InstanceRenderer m_renderer;

		Box* m_box;
		double m_sigma;

		//tmx::graphics::InstanceStaticShader m_shader;
		LatticeShader m_shader;

		SystemTrajectory* m_trajectory;

		tmx::graphics::MTexturedQuad m_activeSite;
		tmx::graphics::MTexturedQuad m_inactiveSite;
		tmx::graphics::MTexturedQuad m_subBox;

		tmx::graphics::MTexturedQuad m_play;
		tmx::graphics::MTexturedQuad m_stop;

		//std::vector<tmx::graphics::Renderable>* m_renderables;
		tmx::memory::ObjectPool<tmx::graphics::Renderable> m_renderablesPool;

		bool m_lightRendering;

	public:
		// TODO: sigma will not be passed. It will be taken from "trajectory"
		Display(int width, int height, double adjustFactor, Box* box, PartSpecs* partSpecs, SystemTrajectory* trajectory, bool lightRendering);
		//~Display();

		void run(int start, int end, int stride, int stepWait);


		void playbackControls();
	private:
		void fillRenderables(int step);
		void fillPlaybackRenderables(int step);

		void addSteps(size_t s);
		void substractSteps(size_t s);
		void moveCamera();

		bool buttonPressed;
		bool pause;
		size_t currentStep;
		size_t totalSteps;
		size_t jumpSteps;

		bool mousePressed;
		bool mousePressedOnce;
		double ix; 
		double iy;
		double fx;
		double fy;

		double current_ix;
		double current_fx;
		double current_iy;
		double current_fy;

	};

}