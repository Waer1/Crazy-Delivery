#pragma once

#include "../application.hpp"
#include "../systems/sound.hpp"

#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#define radioDelay 1000

namespace our
{
	class RadioSystem {
		Application* app;
		std::string path = "assets/radio";
		
		std::vector<Sound*> radioSongs;
		int currentSong = 0;
		bool canChange = true;

		std::chrono::high_resolution_clock::time_point lastTime;

		std::vector<std::string> split(std::string str, char seperator) {
				std::vector<std::string> strings;
				int i = 0;
				int startIndex = 0, endIndex = 0;
				while (i <= str.length()) {
						if (str[i] == seperator || i == str.length()) {
								endIndex = i;
								std::string subStr = "";
								subStr.append(str, startIndex, endIndex - startIndex);
								strings.push_back(subStr);
								startIndex = endIndex + 1;
						}
						i++;
				}
				return strings;
		}

		void forward() {
			// Handle if the forward button is pressed
			radioSongs[currentSong]->stop();

			if (currentSong == radioSongs.size() - 1) {
				currentSong = radioSongs.size() - 1;
			} else {
				currentSong++;
			}

			radioSongs[currentSong]->play();
		}

		void backward() {
			// Handle if the backward button is pressed
			radioSongs[currentSong]->stop();
			
			if (currentSong == 0) {
				currentSong = 0;
			} else {
				currentSong--;
			}
			
			radioSongs[currentSong]->play();
		}
		
	public:
		void initialize(Application* app) {
			this->app = app;

			// Add off sound
			Sound* offSound = new Sound("assets/radio/off.mp3", true);
			radioSongs.push_back(offSound);

			for (const auto & entry : fs::directory_iterator(path)) {
				std::vector<std::string> splitedPath = split(entry.path().string(), '\\');

				if (splitedPath[splitedPath.size() - 1] != "off.mp3") {
					std::string songPath = path + "/" + splitedPath[splitedPath.size() - 1];

					Sound* song = new Sound(songPath.c_str(), true);
					song->changeVolume(20);
					radioSongs.push_back(song);
				}
			}
		}

		void update() {
			auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
			if (elapsedTime > radioDelay) {
				canChange = true;
			}
			
			// Forward button is pressed
			if (canChange && app->getKeyboard().isPressed(GLFW_KEY_P)) {
				lastTime = std::chrono::high_resolution_clock::now();
				canChange = false;

				forward();
			} else if (canChange && app->getKeyboard().isPressed(GLFW_KEY_O)) {
				lastTime = std::chrono::high_resolution_clock::now();
				canChange = false;

				backward();
			}
		}

		~RadioSystem() {
			for (int i = 0; i < radioSongs.size(); i++) {
				delete radioSongs[i];
			}
		}
	};
}
