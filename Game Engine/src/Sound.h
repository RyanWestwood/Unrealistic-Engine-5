#pragma once
#include "CrossPlatform.h"
#include "Utils.h"
#include <string>
#include <memory>

namespace UE{

    //class Sound{
    //    public:
    //        void LoadSound(const std::string filename){                
		  //      std::unique_ptr<Mix_Chunk>uniqueMixChunk(Mix_LoadWAV((g_SoundDirectory + filename).c_str()));
		  //      if (uniqueMixChunk == nullptr) std::cout << "Could not load sound!\n";
    //            m_Sound = std::move(uniqueMixChunk);
    //        }

    //        void PlaySound(){
	   //         Mix_PlayChannel(-1, m_Sound.get(), 0);
    //        }
    //    private:
    //        std::unique_ptr<Mix_Chunk> m_Sound;
    //};


    ////TODO: figure out this wizardy to us smart pointers.
    //class Music{
    //    public:
    //        void LoadMusic(const std::string filename){           
    //            m_Music = Mix_LoadMUS((g_SoundDirectory + filename).c_str());
		  //      if (m_Music == nullptr) std::cout << "Could not load sound!\n\t-" << filename << "\n";
    //        }

    //        void PlayMusic(){
	   //         if (Mix_PlayingMusic() == 0) Mix_PlayMusic(m_Music, -1);
    //        }
    //    private:
    //        Mix_Music* m_Music;
    //};
}