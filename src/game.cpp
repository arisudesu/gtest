#include <glm/gtc/type_ptr.hpp>

#include "include/game.hpp"
#include "include/client.hpp"
#include "include/shader.hpp"
#include "include/textrenderer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <AL/alc.h>
#include <AL/al.h>

using namespace gl;
using namespace std::placeholders;

Client get_client_with_context(IClientEventHandler &e) {
    Client tmp(800, 600, "rvalue test", e);
    glbinding::Binding::initialize();
    return tmp;
}

ALuint loadWavFile(const std::string &filename) {
#pragma pack(1)
    struct RIFF_Header {
        char chunkID[4];
        long chunkSize;//size not including chunkSize or chunkID
        char format[4];
    } riff_header;

    struct WAVE_Format {
        char subChunkID[4];
        long subChunkSize;
        short audioFormat;
        short numChannels;
        long sampleRate;
        long byteRate;
        short blockAlign;
        short bitsPerSample;
    } wave_header;

    struct WAVE_Data {
        char subChunkID[4]; //should contain the word data
        long subChunk2Size; //Stores the size of the data block
    } wav_data;
#pragma pack(0)

    std::ifstream in(filename, std::ios::binary);
    if (in) {
        in.read((char *) &riff_header, sizeof riff_header);
        in.read((char *) &wave_header, sizeof wave_header);
        in.read((char *) &wav_data, sizeof wav_data);

        if (!(
                wav_data.subChunkID[0] == 'd' &&
                wav_data.subChunkID[1] == 'a' &&
                wav_data.subChunkID[2] == 't' &&
                wav_data.subChunkID[3] == 'a'
        )) {
            throw std::runtime_error("not wav data");
        }

        char *data = new char[wav_data.subChunk2Size];
        in.read(data, wav_data.subChunk2Size);

        ALsizei freq = wave_header.sampleRate;
        ALenum format = AL_FORMAT_STEREO16;

        std::cout << wave_header.numChannels << " " << wave_header.bitsPerSample << std::endl;
        if (wave_header.numChannels == 1) {
            if (wave_header.bitsPerSample == 8) {
                format = AL_FORMAT_MONO8;
            } else if (wave_header.bitsPerSample == 16) {
                format = AL_FORMAT_MONO16;
            }
        } else if (wave_header.numChannels == 2) {
            if (wave_header.bitsPerSample == 8) {
                format = AL_FORMAT_STEREO8;
            } else if (wave_header.bitsPerSample == 16) {
                format = AL_FORMAT_STEREO16;
            }
        }

        ALuint buffer;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, data, wav_data.subChunk2Size, freq);

        return buffer;
    }
    return 0;
}

Game::Game() :
        m_client(get_client_with_context(*this)),
        m_menu(std::bind(
                static_cast<void (Game::*)(GuiMenuBase &)>(&Game::OnItemSelect), this, _1)
        ),
        m_exitmenu(std::bind(
                static_cast<void (Game::*)(GuiMenuBase &)>(&Game::OnItemSelect), this, _1)
        ),
        m_pausemenu(std::bind(
                static_cast<void (Game::*)(GuiMenuBase &)>(&Game::OnItemSelect), this, _1
        )),
        m_bDone(false),
        m_activeMenu(&m_menu),
        m_gp(nullptr),
        m_gpPaused(false) {}

int Game::Run() {
    glViewport(0, 0, 800, 600);
    glClearColor(1.0, 1.0, 0.8, 1.0);
    glClearDepth(1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    /*glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);*/

    // TODO: use std::chrono
    double time = glfwGetTime();
    unsigned int framecount = 0;
    float fps = 0;

    static Font sans("data/fonts/menu.ttf", 14);
    static TextRenderer sansRender(sans);

    glEnable(GL_MULTISAMPLE);
    GLuint colorRbo, depthRbo;

    glGenRenderbuffers(1, &colorRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA, 800, 600);
    //std::cout << glGetError() << std::endl;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenRenderbuffers(1, &depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, 800, 600);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    GLuint mainFbo;
    glGenFramebuffers(1, &mainFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, mainFbo);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRbo);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo);

    GLint samples;
    glGetIntegerv(GL_MAX_SAMPLES, &samples);
    //std::cout << samples << " " << glGetError() << std::endl;

    //std::cout << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ALCdevice *dev;
    ALCcontext *alc;

    dev = alcOpenDevice(nullptr);
    if (dev) {
        alc = alcCreateContext(dev, nullptr);

        alcMakeContextCurrent(alc);

        alListener3f(AL_POSITION, 0, 0, 1.0f);
        alListener3f(AL_VELOCITY, 0, 0, 0);
        ALfloat listenerOri[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
        alListenerfv(AL_ORIENTATION, listenerOri);

        std::cout << alGetString(alGetError()) << std::endl;

        ALuint source;
        alGenSources(1, &source);
        alSourcef(source, AL_PITCH, 1);
        alSourcef(source, AL_GAIN, 1);
        alSource3f(source, AL_POSITION, 0, 0, 0);
        alSource3f(source, AL_VELOCITY, 0, 0, 0);
        alSourcei(source, AL_LOOPING, AL_TRUE);
        std::cout << alGetString(alGetError()) << std::endl;

        ALuint buffer = loadWavFile("data/audio/menu.wav");

        std::cout << alGetString(alGetError()) << std::endl;

        alSourcei(source, AL_BUFFER, buffer);
        alSourcePlay(source);
        std::cout << alGetString(alGetError()) << std::endl;
        std::cout << source << " " << buffer << std::endl;
    }

    while (!m_bDone) {
        //ALint state;
        //alGetSourcei(source, AL_SOURCE_STATE, &state);
        //std::cout << state << std::endl;

        m_client.ProcessEvents();

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mainFbo);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_gp) {
            if (!m_gpPaused) {
                m_gp->Update();
            }
            m_gp->Render();
        }

        if (m_activeMenu) {
            m_activeMenu->Render();
        }

        if (glfwGetTime() - time >= 1) {
            fps = framecount / (float) (glfwGetTime() - time);
            time = glfwGetTime();
            framecount = 0;
        }
        framecount++;

        std::ostringstream strfps;
        strfps << "fps:" << fps;
        sansRender.RenderText(strfps.str(), 0, 0);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   // Make sure no FBO is set as the draw framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mainFbo);

        glDrawBuffer(GL_BACK);                       // Set the back buffer as the draw buffer
        glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        //std::cout << glGetError() << std::endl;
        m_client.SwapBuffers();
    }
    return 0;
}

void Game::onWindowClose() {
    m_bDone = true;
}

void Game::onKeyPress(Client::KeyCode key, int /*scancode*/, int /*mods*/) {
    if (m_gp) {
        switch (key) {
            case Client::KEY_ESCAPE:
                if (m_gpPaused) {
                    m_gpPaused = false;
                    m_activeMenu = nullptr;
                } else {
                    m_gpPaused = true;
                    m_activeMenu = &m_pausemenu;
                }
                break;

            default:;
        }
    }

    if (m_activeMenu) {
        switch (key) {
            case Client::KEY_UP:
                m_activeMenu->navigatePrevious();
                break;

            case Client::KEY_DOWN:
                m_activeMenu->navigateNext();
                break;

            case Client::KEY_RETURN:
                m_activeMenu->select();
                break;

            default:;
        }
    }
}

void Game::onCursorMove(float, float) {
}

void Game::OnItemSelect(GuiMenuBase &/*o*/) {
    std::cout << "bound function" << std::endl;
}

void Game::OnItemSelect(GuiMainMenu &/*o*/) {
    std::cout << "bound function 2" << std::endl;
}

void Game::onItemSelect(GuiExitMenu & /*o*/) {
}

void Game::onItemSelect(GuiMainMenu &o) {
    std::cout << "mainmenu" << std::endl;
    switch (o.getSelectedItem()) {
        case 0:
            m_activeMenu = nullptr;
            m_gpPaused = false;
            m_gp = new GamePlay;
            break;

        case 4:
            m_activeMenu = &m_exitmenu;
            break;
    }
}

void Game::onItemSelect(GuiMenuBase &o) {
    std::cout << "generic menu" << std::endl;

    if (&o == &m_exitmenu) {
        switch (o.getSelectedItem()) {
            case 0:
                m_bDone = true;
                break;

            case 1:
                m_activeMenu = &m_menu;
                break;
        }
    }

    if (&o == &m_pausemenu) {
        switch (o.getSelectedItem()) {
            case 0:
                m_gpPaused = false;
                m_activeMenu = nullptr;
                break;

            case 1:
                m_activeMenu = &m_menu;
                delete m_gp;
                break;
        }
    }
}
