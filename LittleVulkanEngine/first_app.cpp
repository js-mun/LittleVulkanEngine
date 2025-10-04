#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "lve_buffer.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

#include <thread>
#include <iostream>

namespace lve {

struct GlobalUbo {
    glm::mat4 projectionView{1.f};
    glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f,
        -3.f, -1.f});
};

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run() {
  LveBuffer globalUboBuffer{
    lveDevice,
    sizeof(GlobalUbo),
    LveSwapChain::MAX_FRAMES_IN_FLIGHT,
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
    64, // fix by magic number
    // lveDevice.properties.limits.nonCoherentAtomSize, // -> fix by chat gpt
    // lveDevice.properties.limits.minUniformBufferOffsetAlignment, // -> Sample code // returned 4 for my pc. expected to return 16~256 
  };

  globalUboBuffer.map();

  SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
  LveCamera camera{};

  using clock = std::chrono::high_resolution_clock;
  auto lastTime = clock::now();
  int frames = 0;

  auto viewerObject = LveGameObject::createGameObject();
  KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();
  while (!lveWindow.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
    std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    currentTime = newTime;

    cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

    float aspect = lveRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

    if (auto commandBuffer = lveRenderer.beginFrame()) {
        int frameIndex = lveRenderer.getFrameIndex();
        FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera};
        
        // update
        GlobalUbo ubo{};
        ubo.projectionView = camera.getProjection() * camera.getView();
        globalUboBuffer.writeToBuffer(&ubo, frameIndex);
        globalUboBuffer.flushIndex(frameIndex);
        
        // render
        lveRenderer.beginSwapChainRenderPass(commandBuffer);
        simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
        lveRenderer.endSwapChainRenderPass(commandBuffer);
        lveRenderer.endFrame();
    }

    frames++;

    auto currentTime = clock::now();
    auto elapsed = std::chrono::duration_cast<
        std::chrono::seconds>(currentTime - lastTime).count();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    if (elapsed >= 1) {
      // std::cout << "FPS: " << frames / elapsed << std::endl;
      frames = 0;
      lastTime = currentTime;
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

void FirstApp::loadGameObjects() {
    std::shared_ptr<LveModel> lveModel = 
        LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");

    auto flatVase = LveGameObject::createGameObject();
    flatVase.model = lveModel;
    flatVase.transform.translation = {-.5f, .5f, 2.5f};
    flatVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.push_back(std::move(flatVase));

    lveModel = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");
    auto smoothVase = LveGameObject::createGameObject();
    smoothVase.model = lveModel;
    smoothVase.transform.translation = {.5f, .5f, 2.5f};
    smoothVase.transform.scale = {3.f, 1.5f, 3.f};
    gameObjects.push_back(std::move(smoothVase));
}

}  // namespace lve
