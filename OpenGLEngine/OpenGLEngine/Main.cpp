// Team Members : Brian Viveiros 101209942, Emmanuel Nofuente-Loblack 101126071

#include "RenderingSystem.h"
#include "RenderingSystemV2.h"
#include "InputEventSystem.h"
#include "FPSControlSystem.h"
#include "FollowCameraSystem.h"
#include "RotateSystem.h"
#include "RotateSystemV2.h"
#include "LifeTimeSystem.h"
#include "FireworksSystem.h"
#include "GravityForceSystem.h"
#include "DragForceSystem.h"
#include "FixedSpringSystem.h"
#include "PairedSpringSystem.h"
#include "ParticleSphereSystem.h"
#include "CableSystem.h"
#include "RodSystem.h"
#include "ParticleContactResolutionSystem.h"
#include "ResetPenetrationDeltaMoveSystem.h"
#include "ForceAccumulatorSystem.h"
#include "ParticleSystem.h"
#include "RigidbodySystem.h"
#include "ForceAndTorqueAccumulatorSystem.h"
#include "DragSystem.h"
#include "AddTorqueFromCameraSystem.h"
#include "AeroControlSystem.h"
#include "AeroSurfaceSystem.h"
#include "ThrusterSystem.h"
#include "DynamicDirectionalLightSystem.h"
#include "DynamicPointLightSystem.h"
#include "DynamicSpotLightSystem.h"
#include <string>
#include <stdlib.h>     
#include <time.h>      

#include "BuoyancyCenterComponent.h"
#include "BuoyancyCenter2Component.h"
#include "NewBuoyancySystem.h"
#include "NewBuoyancy2System.h"


#include "BuoyancyCenter3Component.h"
#include "BuoyancyCenter4Component.h"
#include "NewBuoyancy3System.h"
#include "NewBuoyancy4System.h"

using namespace Reality;

void LoadShaders(ECSWorld& world);
void LoadModels(ECSWorld& world);
void SetupLights(ECSWorld& world);

void MakeABoatSimulator(ECSWorld& world);


int main()
{
	ECSWorld world;

	// Init and Load
	world.data.InitRendering();
	//LoadAssets(world);
	
	world.data.renderUtil->camera.Position = Vector3(0, 0.0f, 50.0f);
	world.data.renderUtil->SetFOV(60);
	// Create entities

	// Make a player controller
	auto e = world.createEntity();
	e.addComponent<FPSControlComponent>();

	SetupLights(world);
	MakeABoatSimulator(world);

	// Create Systems
	world.getSystemManager().addSystem<RenderingSystem>();
	world.getSystemManager().addSystem<RenderingSystemV2>();
	world.getSystemManager().addSystem<InputEventSystem>();
	world.getSystemManager().addSystem<FPSControlSystem>();
	world.getSystemManager().addSystem<FollowCameraSystem>();
	world.getSystemManager().addSystem<RotateSystem>();
	world.getSystemManager().addSystem<RotateSystemV2>();
	world.getSystemManager().addSystem<LifeTimeSystem>();
	world.getSystemManager().addSystem<FireworksSystem>();
	world.getSystemManager().addSystem<GravityForceSystem>();
	world.getSystemManager().addSystem<DragForceSystem>();
	world.getSystemManager().addSystem<FixedSpringSystem>();
	world.getSystemManager().addSystem<PairedSpringSystem>();
	world.getSystemManager().addSystem<ParticleSphereSystem>();
	world.getSystemManager().addSystem<CableSystem>();
	world.getSystemManager().addSystem<RodSystem>();
	world.getSystemManager().addSystem<ParticleContactResolutionSystem>();
	world.getSystemManager().addSystem<ResetPenetrationDeltaMoveSystem>();
	world.getSystemManager().addSystem<ForceAccumulatorSystem>();
	world.getSystemManager().addSystem<ParticleSystem>();
	world.getSystemManager().addSystem<RigidbodySystem>();
	world.getSystemManager().addSystem<ForceAndTorqueAccumulatorSystem>();
	world.getSystemManager().addSystem<DragSystem>();
	world.getSystemManager().addSystem<AddTorqueFromCameraSystem>();
	world.getSystemManager().addSystem<AeroControlSystem>();
	world.getSystemManager().addSystem<AeroSurfaceSystem>();
	world.getSystemManager().addSystem<ThrusterSystem>();
	world.getSystemManager().addSystem<DynamicDirectionalLightSystem>();
	world.getSystemManager().addSystem<DynamicPointLightSystem>();
	world.getSystemManager().addSystem<DynamicSpotLightSystem>();
	world.getSystemManager().addSystem<NewBuoyancySystem>();
	world.getSystemManager().addSystem<NewBuoyancy2System>();
	world.getSystemManager().addSystem<NewBuoyancy3System>();
	world.getSystemManager().addSystem<NewBuoyancy4System>();

	float time = glfwGetTime();
	float stepTime = glfwGetTime();
	float deltaTime = 0;
	float elapsedDeltaTime = 0;
	float logicDelta = 0;
	float debugDelta = 0;

	LoadShaders(world);
	bool shadersLoaded = false;
	bool modelsLoadStarted = false;
	// game loop
	// -----------
	while (!glfwWindowShouldClose(world.data.renderUtil->window->glfwWindow))
	{
		float current = glfwGetTime();
		deltaTime = current - time;
		deltaTime = 1 / 60.0f;
		time = glfwGetTime();

		world.update();

		// Poll OpenGl events
		glfwPollEvents();

		world.data.renderUtil->ClearDisplay(world.data.renderUtil->window->glfwWindow);

		// Load
		if (!shadersLoaded)
		{
			shadersLoaded = world.data.assetLoader->ShadersLoaded();
		}
		if(shadersLoaded && !modelsLoadStarted)
		{
			LoadModels(world);
			modelsLoadStarted = true;
		}
		// Update View
		world.data.renderUtil->UpdateViewMatrix();
		// Process Input
		world.getSystemManager().getSystem<InputEventSystem>().Update(deltaTime);

		// Game Logic Update
		world.getSystemManager().getSystem<FPSControlSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<FollowCameraSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RotateSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RotateSystemV2>().Update(deltaTime);
		world.getSystemManager().getSystem<LifeTimeSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<FireworksSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<ParticleSphereSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<CableSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RodSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<AddTorqueFromCameraSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<AeroControlSystem>().Update(deltaTime);

		// Physics	
		float fixedDeltaTime = 1 / 60.0f;
		// Force Generator
		/// Particle
		world.getSystemManager().getSystem<GravityForceSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<DragForceSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<FixedSpringSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<PairedSpringSystem>().Update(fixedDeltaTime);
		/// Rigidbody
		world.getSystemManager().getSystem<DragSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<AeroSurfaceSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<ThrusterSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<NewBuoyancySystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<NewBuoyancy2System>().Update(fixedDeltaTime);

		world.getSystemManager().getSystem<NewBuoyancy3System>().Update(fixedDeltaTime);

		world.getSystemManager().getSystem<NewBuoyancy4System>().Update(fixedDeltaTime);

		// Force Accumulator
		/// Particle
		world.getSystemManager().getSystem<ForceAccumulatorSystem>().Update(fixedDeltaTime);
		/// Rigidbody
		world.getSystemManager().getSystem<ForceAndTorqueAccumulatorSystem>().Update(fixedDeltaTime);

		// Contact Resolution
		world.getSystemManager().getSystem<ParticleContactResolutionSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<ResetPenetrationDeltaMoveSystem>().Update(fixedDeltaTime);

		// Integrator
		/// Particle
		world.getSystemManager().getSystem<ParticleSystem>().Update(fixedDeltaTime);
		/// Rigidbody
		world.getSystemManager().getSystem<RigidbodySystem>().Update(fixedDeltaTime);

		// Rendering Update

		world.data.renderUtil->DrawCube(Vector3(0, 0, 0), Vector3(0, 0, 0));

		world.getSystemManager().getSystem<DynamicDirectionalLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<DynamicPointLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<DynamicSpotLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RenderingSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RenderingSystemV2>().Update(deltaTime);

		elapsedDeltaTime = glfwGetTime() - time;
		logicDelta = elapsedDeltaTime - world.data.renderUtil->GetRenderDelta();
		stepTime = glfwGetTime();

		// Debug
		if (DEBUG_LOG_LEVEL > 0)
		{
			world.data.renderUtil->RenderText("FPS : " + std::to_string((int)round(1.0f / deltaTime)), 1810.0f, 1060.0f, 0.5f, Color(0, 1, 1, 1));
		}
		if (DEBUG_LOG_LEVEL > 1)
		{
			int logic = (int)round(logicDelta * 100.0f / deltaTime);
			std::string logicString = logic < 10 ? " " + std::to_string(logic) : std::to_string(logic);
			int render = (int)round(world.data.renderUtil->GetRenderDelta() * 100.0f / deltaTime);
			std::string renderString = logic < 10 ? " " + std::to_string(render) : std::to_string(render);
			int debug = (int)round(debugDelta * 100.0f / deltaTime);
			std::string debugString = logic < 10 ? " " + std::to_string(debug) : std::to_string(debug);
			
			world.data.renderUtil->RenderText("Logic : " + logicString + "%" +
				+ " | Rendering : " + renderString + "%" +
				+ " | Debug : " + debugString + "%"
				, 1680.0f, 1040.0f, 0.25f, Color(0, 1, 1, 1));
		}
		if (DEBUG_LOG_LEVEL > 2)
		{
			world.data.renderUtil->RenderText("Draw Calls : " + std::to_string(world.data.renderUtil->GetDrawCalls())
				+ " | Verts : " + std::to_string(world.data.renderUtil->GetVerts())
				+ " | Tris : " + std::to_string(world.data.renderUtil->GetTris())
				+ " | Lines : " + std::to_string(world.data.renderUtil->GetLines())
				, 1610.0f, 1020.0f, 0.25f, Color(0, 1, 1, 1));
		}

		// Update debug delta
		debugDelta = glfwGetTime() - stepTime;
		stepTime = glfwGetTime();

		world.data.renderUtil->SwapBuffers(world.data.renderUtil->window->glfwWindow);

	}


	glfwTerminate();
	return 0;
}

void LoadShaders(ECSWorld& world)
{
	world.data.assetLoader->StartShaderLoading({ {"Shaders/Lighting_Maps.vs", "Shaders/Lighting_Maps.fs"} });
}
void LoadModels(ECSWorld& world)
{
	world.data.assetLoader->StartModelLoading({

	ModelData("Resources/Models/Boats/19287_Bass_boat_v1.obj",
		{{"Blank.mtl"}}),
	ModelData("Resources/Models/water/water.fbx",
		{{"water-texture.jpg"}})
		});
}

void MakeABoatSimulator(ECSWorld& world)
{
	auto water = world.createEntity();
	water.addComponent<ModelComponent>("Resources/Models/water/water.fbx", Vector3(-90, 0, 0), Vector3(0, -10, 0));
	water.addComponent<TransformComponentV2>();

	auto boat = world.createEntity();

	boat.addComponent<RigidbodyComponent>();
	boat.addComponent<BuoyancyCenterComponent>(Vector3(0.0f, -10.0f, 100.0f), 5.0f, 1.0f);
	boat.addComponent<BuoyancyCenter2Component>(Vector3(0.0f, -10.0f, -100.0f), 5.0f, 1.0f);
	boat.addComponent<BuoyancyCenter3Component>(Vector3(80.0f, -10.0f, 0.0f), 5.0f, 1.0f);
	boat.addComponent<BuoyancyCenter4Component>(Vector3(-80.0f, -10.0f, 0.0f), 5.0f, 1.0f);
	boat.addComponent<ForceAndTorqueAccumulatorComponent>();

	boat.addComponent<TransformComponentV2>(Vector3(0, 20, -50), Vector3(0.1f, 0.1f, 0.1f), Vector3(-20, 180, 10));
	boat.addComponent<ModelComponent>("Resources/Models/Boats/19287_Bass_boat_v1.obj", Vector3(-90, 0, 0), Vector3(0, -100, 0));

	boat.addComponent<GravityForceComponent>(1.0f);

	auto engine = world.createEntity();
	engine.addComponent<ThrusterComponent>(boat);

	auto rudder = world.createEntity();
	rudder.addComponent<TransformComponentV2>();
	rudder.addComponent<AeroSurfaceComponent>(boat, Vector3(0, 0, 0), Vector3(0, -200, -100));
	std::vector<int> rudderWingPositiveKeys = { GLFW_KEY_D };
	std::vector<int> rudderWingNegetiveKeys = { GLFW_KEY_A };
	rudder.addComponent<AeroControlComponent>(
		Vector3(-0.4f, 0, 0),
		Vector3(0.4f, 0, 0),
		rudderWingPositiveKeys, rudderWingNegetiveKeys);
	boat.addComponent<FollowCameraComponent>();

	boat.addComponent<DragComponent>();
}

void SetupLights(ECSWorld& world)
{
	auto l = world.createEntity();
	l.addComponent<TransformComponent>(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(90, 0, 0));
	l.addComponent<DynamicDirectionalLightComponent>(Color(1, 1, 1), Color(0.0, 0.1, 0.1), Color(0.0, 0.1, 0.1));

	// Lanterns
	auto pl1 = world.createEntity();
	pl1.addComponent<TransformComponent>(Vector3(22, 14, 48.5f));
	pl1.addComponent<DynamicPointLightComponent>(100.0f, Color(0.1, 0, 0), Color(1.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f));
	auto hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(23, 15, 48.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22, 13.5f, 50.5f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(21, 12.5f, 47.5f));

	auto pl2 = world.createEntity();
	pl2.addComponent<TransformComponent>(Vector3(-14.5f, 14, 49.0f));
	pl2.addComponent<DynamicPointLightComponent>(100.0f, Color(0, 0, 0.1f), Color(0.0f, 0.0f, 1.0f), Color(0.0f, 0.0f, 1.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f + 1, 14 - 1, 49.0f - 1));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 0.5f, 14 + 1, 49.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f, 14 - 1, 49.0f + 1));
	
	auto pl3 = world.createEntity();
	pl3.addComponent<TransformComponent>(Vector3(22, 14, -62.0f));
	pl3.addComponent<DynamicPointLightComponent>(100.0f, Color(0, 0.1f, 0), Color(0.0f, 1.0f, 0.0f), Color(0.0f, 1.0f, 0.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22 - 1, 14 - 1, -62.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22, 14 + 0.5f, -62.0f - 1));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22 + 1, 14, -62.0f + 0.5f));

	auto pl4 = world.createEntity();
	pl4.addComponent<TransformComponent>(Vector3(-14.5f, 14, -61.5f));
	pl4.addComponent<DynamicPointLightComponent>(100.0f, Color(0.1, 0.05, 0), Color(1.0f, 0.55f, 0.0f), Color(1.0f, 0.55f, 0.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 1, 14, -61.5f -1));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 0.25f, 14 - 0.5f, -61.5f + 1));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f + 0.5f, 14+ 1, -61.5f + 1));

	// Spears
	std::vector<Color> cols = { Color(1,0,0), Color(0,1,0), Color(0,0,1), Color(0.7f,0.55f,0) };
	for (int i = 1; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pl1 = world.createEntity();
			pl1.addComponent<TransformComponent>(Vector3((i % 2 == 0 ? 8 : -1), 85, 49.5f - 37 * j), Vector3(1, 1, 1), Vector3(180, 0, 0));
			pl1.addComponent<DynamicSpotLightComponent>(10.0f, 100, Color(0, 0, 0), cols[3 - j], cols[3 - j], 5);
		}
	}
}