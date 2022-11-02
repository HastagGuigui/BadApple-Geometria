#define GAME_helpme

#include "geometria.h"
#include <stdio.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct BadApple : public ScriptBehaviour
{
	std::vector<Model*> billys;
    Texture* billydotjpeg;
	json bad_apple;
    int width = 36;
    int height = 28;

    float framerate = 30;
    float ohfuck;
    int frame = 0;

    MiniAudioSource* themusic;

    bool isPlaying = false;

	void OnStart()
	{
        Graphics::MainCamera()->GetTransform().position = Vector3(width/2, height/2, 40);
		std::string big = Files::Read("Game/yoink.json");
		bad_apple = json::parse(big);

        themusic = GetScript<MiniAudioSource>();

        billydotjpeg = new Texture("Game/billy.png");
        TextureManager::UploadToGPU();

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                Model* pixel = new Model(Model::Primitives::SQUARE, Vector3(x,height - (y+1),0), Vector3(0), Vector3(1));
                pixel->texture = billydotjpeg;
                billys.push_back(pixel);
                RendererCore::AddModel(*pixel);
            }
            
        }
        // themusic->Play();
	}

    void OnUpdate()
    {
        if(isPlaying){
            ohfuck += Graphics::DeltaTime()/3;

            if(ohfuck > 1/framerate){
                ohfuck -= 1/framerate;
                frame++;
                DisplayFrame(frame);
            }
        }else{
            if(Input::GetKeyDown(GLFW_KEY_SPACE)){
                isPlaying = true;
            }
        }
    }

    void DisplayFrame(int frame){
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                billys[(y*width) + x]->color = Color(1,1,1,1 - bad_apple[frame][y][x].get<int>());
            }
            
        }
        
    }
};