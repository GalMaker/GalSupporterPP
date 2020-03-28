#include "scene.h"

void GS_SceneManager::mainLoop()
{
	while (scene_)
	{
		scene_->refresh();
	}
}

void GS_SceneManager::setScene(UPScene newScene)
{
	if (newScene)
	{
		newScene->initLoad();
	}
	scene_ = std::move(newScene);
}
